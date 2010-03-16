
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 Tomasz Maka <pasp@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gui.h"
#include "i18n.h"
#include "utils.h"
#include "utils_gui.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_fullyear.h"
#include "calendar_widget.h"
#include "calendar_jumpto.h"
#include "calendar_calc.h"
#ifdef HAVE_LIBICAL
#include "calendar_ical.h"
#endif  /* HAVE_LIBICAL */
#include "calendar_utils.h"
#include "notes.h"
#include "notes_items.h"
#include "tasks.h"
#include "tasks_items.h"
#include "notes_items.h"
#include "contacts.h"
#include "contacts_items.h"
#include "options_gui.h"
#include "options_prefs.h"
#include "options_gui_general.h"
#include "check_events.h"
#include "stock_icons.h"
#include "config.h"

#include "gui_logo.h"
#include "gui_icon.h"
#include "credits.h"

/*------------------------------------------------------------------------------*/

void
gui_save_data_and_run_command (gchar *command, GUI *appGUI) {
    gui_save_all_data (appGUI);
    utl_run_command (command);
}

/*------------------------------------------------------------------------------*/

void
gui_save_all_data (GUI *appGUI) {

    cal_write_notes (appGUI);
#ifdef TASKS_ENABLED
	store_task_columns_info (appGUI);
    write_tasks_entries (appGUI);
#endif  /* TASKS_ENABLED */
#ifdef HAVE_LIBICAL
    write_ical_entries (appGUI);
    ics_calendar_refresh (appGUI);
#endif  /* HAVE_LIBICAL */
#ifdef CONTACTS_ENABLED
	store_contact_columns_info (appGUI);
    write_contacts_entries (appGUI);
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
	store_note_columns_info (appGUI);
    write_notes_entries (appGUI);
#endif  /* NOTES_ENABLED */
    prefs_write_config (appGUI);

}

/*------------------------------------------------------------------------------*/

void
gui_toggle_window_visibility (GUI *appGUI) {

    if (appGUI->no_tray == FALSE) {

        appGUI->window_visible = !appGUI->window_visible;

        if (appGUI->window_visible == TRUE) {
            gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
            gtk_window_move (GTK_WINDOW (appGUI->main_window), config.window_x, config.window_y);
            gtk_widget_show (appGUI->main_window);
        } else {
            if (appGUI->calendar_only == FALSE) {
                gui_save_all_data (appGUI);
            }
            gtk_widget_hide (appGUI->main_window);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
gui_quit_osmo (gboolean ignore_geometry, GUI *appGUI) {

#ifdef NOTES_ENABLED
    if (appGUI->nte->editor_active == TRUE) {
        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
    }
#endif  /* NOTES_ENABLED */

    if (appGUI->osmo_trayicon != NULL) {
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
    }

    if (appGUI->calendar_only == FALSE) {
        if (appGUI->current_tab == PAGE_CALENDAR 
#ifdef TASKS_ENABLED
            || appGUI->current_tab == PAGE_TASKS 
#endif  /* TASKS ENABLED */
#if defined(NOTES_ENABLED) && defined(CONTACTS_ENABLED)
            || appGUI->current_tab == PAGE_CONTACTS || appGUI->current_tab == PAGE_NOTES) {
#elif CONTACTS_ENABLED
            || appGUI->current_tab == PAGE_CONTACTS) {
#else
            ) {
#endif  /* NOTES_ENABLED && CONTACTS_ENABLED */
            config.latest_tab = appGUI->current_tab;
        }

#ifdef CONTACTS_ENABLED
        config.find_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox));
#endif  /* CONTACTS_ENABLED */

#ifdef TASKS_ENABLED
        config.current_category_in_tasks = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->tsk->cf_combobox));
#endif  /* TASKS_ENABLED */

#ifdef NOTES_ENABLED
        config.current_category_in_notes = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->nte->cf_combobox));
#endif  /* NOTES_ENABLED */

        gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);

        if (ignore_geometry == FALSE) {
            gtk_window_get_size (GTK_WINDOW(appGUI->main_window),
                                &config.window_size_x, &config.window_size_y);
        }
    } 

    if (ignore_geometry == FALSE) {
        gdk_window_get_root_origin (GDK_WINDOW(appGUI->main_window->window),
                            &config.window_x, &config.window_y);
    }

    pango_font_description_free(appGUI->cal->fd_day_name_font);
    pango_font_description_free(appGUI->cal->fd_cal_font);
    pango_font_description_free(appGUI->cal->fd_notes_font);

#ifdef NOTES_ENABLED
    pango_font_description_free(appGUI->nte->fd_notes_font);
#endif  /* NOTES_ENABLED */

    if (appGUI->calendar_only == FALSE) {
        config.lastrun_date = utl_get_current_julian ();
        config.lastrun_time = get_seconds_for_today();
        gui_save_all_data (appGUI);
#ifdef HAVE_LIBNOTIFY
#ifdef TASKS_ENABLED
        free_notifications_list (appGUI);
#endif  /* TASKS_ENABLED */
#endif  /* HAVE_LIBNOTIFY */
        cal_free_notes_list (appGUI);
    }

    utl_gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);
#ifdef CONTACTS_ENABLED
    utl_gui_url_remove_links (&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index);
#endif  /* CONTACTS_ENABLED */

    gtk_main_quit ();
}

/*------------------------------------------------------------------------------*/

void
gui_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
        gui_toggle_window_visibility (appGUI);
    } else {
        gui_quit_osmo(FALSE, appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gint
get_first_active_page (GUI *appGUI) {

    if (!config.hide_calendar) 
        return PAGE_CALENDAR;
#ifdef TASKS_ENABLED
    if (!config.hide_tasks) 
        return PAGE_TASKS;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    if (!config.hide_contacts) 
        return PAGE_CONTACTS;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    if (!config.hide_notes) 
        return PAGE_NOTES;
#endif  /* NOTES_ENABLED */

    return PAGE_OPTIONS;
}

/*------------------------------------------------------------------------------*/

void
set_visible_page (gint page, gboolean dir, GUI *appGUI) {

gboolean flag;
gint n = 1;

    if (dir == FALSE) n = -1;

    page += n;
    flag = TRUE;

    while (flag) {

        flag = FALSE;

        switch (page) {
            case PAGE_CALENDAR:
                if (config.hide_calendar) {
                    page += n;
                    flag = TRUE;
                }
                break;
#ifdef TASKS_ENABLED
            case PAGE_TASKS:
                if (config.hide_tasks) {
                    page += n;
                    flag = TRUE;
                }
                break;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
            case PAGE_CONTACTS:
                if (config.hide_contacts) {
                    page += n;
                    flag = TRUE;
                }
                break;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
            case PAGE_NOTES:
                if (config.hide_notes) {
                    page += n;
                    flag = TRUE;
                }
                break;
#endif  /* NOTES_ENABLED */
        }
    }

    if (dir == FALSE) {
        appGUI->current_tab = (page < 0) ? appGUI->number_of_tabs-1:page;
    } else {
        appGUI->current_tab = (page == appGUI->number_of_tabs) ? get_first_active_page(appGUI):page;
    }
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), appGUI->current_tab);
}

/*------------------------------------------------------------------------------*/

void
select_tab (gint tab, GUI *appGUI) {

gint i, n;

    if (tab < get_visible_tabs(appGUI)) {
    
        for (i = n = PAGE_CALENDAR; i < NUMBER_OF_TABS; i++) {

            if (i == PAGE_CALENDAR && !config.hide_calendar) n++;
#ifdef TASKS_ENABLED
            if (i == PAGE_TASKS && !config.hide_tasks) n++;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
            if (i == PAGE_CONTACTS && !config.hide_contacts) n++;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
            if (i == PAGE_NOTES && !config.hide_notes) n++;
#endif  /* NOTES_ENABLED */
            if (i == PAGE_OPTIONS || i == PAGE_ABOUT) n++;

            if (n == tab+1) break;
        }

        if (i != NUMBER_OF_TABS) {
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), i);
        }
    }

}

/*------------------------------------------------------------------------------*/

void
key_counter_add (gint value, GUI *appGUI) {

GtkWidget *dialog = NULL;
gchar tmpbuff[BUFFER_SIZE];

    appGUI->key_counter += value;

    if (appGUI->key_counter == 57) {
	    g_snprintf (tmpbuff, BUFFER_SIZE, "<span size='xx-large'><b>%d times!</b></span>", config.run_counter);

        dialog = gtk_dialog_new_with_buttons ("Counter", GTK_WINDOW(appGUI->main_window),
                                              GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                              OSMO_STOCK_BUTTON_CLOSE, GTK_RESPONSE_NO, NULL);
        utl_gui_fill_iconlabel (dialog, OSMO_STOCK_INFO_HELP, tmpbuff);
        gtk_widget_show (dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

/*------------------------------------------------------------------------------*/

gint 
key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gint page;

    GUI *appGUI = (GUI *)data;

    page = gtk_notebook_get_current_page (GTK_NOTEBOOK(appGUI->notebook));

    /************************************************************************/
    /*** CALENDAR PAGE                                                    ***/
    /************************************************************************/

    if(page == PAGE_CALENDAR) {

        if (!config.day_notes_visible) {

            switch (event->keyval) {
                case GDK_Left:
                    calendar_btn_prev_day(appGUI);
                    return TRUE;
                case GDK_Right:
                    calendar_btn_next_day(appGUI);
                    return TRUE;
                case GDK_Up:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Up */
                        utl_gui_sw_vscrollbar_move_position (appGUI->cal->day_info_scrolledwindow, SW_MOVE_UP);
                        return TRUE;
                    } else {
                        calendar_btn_prev_week(appGUI);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Down:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Up */
                        utl_gui_sw_vscrollbar_move_position (appGUI->cal->day_info_scrolledwindow, SW_MOVE_DOWN);
                        return TRUE;
                    } else {
                        calendar_btn_next_week(appGUI);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Home:
                    calendar_btn_prev_year(appGUI);
                    return TRUE;
                case GDK_End:
                    calendar_btn_next_year(appGUI);
                    return TRUE;
                case GDK_Return:
                    if (appGUI->calendar_only == FALSE) {
                        gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), 
                                                           !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button)));
                    }
                    return TRUE;
                case GDK_space:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Space */
                        config.enable_day_mark = !config.enable_day_mark;
                        cal_refresh_marks (appGUI);
                        cal_set_day_info (appGUI);
                        update_aux_calendars (appGUI);
                        return TRUE;
                    } else {
                        calendar_set_today (appGUI);
                        return TRUE;
                    }
                case GDK_Delete:
                    if (appGUI->calendar_only == FALSE) {
                        calendar_clear_text_cb (NULL, appGUI);
                    }
                    return TRUE;
                case GDK_g:
                    calendar_create_jumpto_window (appGUI);
                    return TRUE;
                case GDK_f:
                    calendar_create_fullyear_window (appGUI);
                    return TRUE;
                case GDK_c:
                    calendar_create_color_selector_window (TRUE, appGUI);
                    key_counter_add (13, appGUI);
                    return TRUE;
                case GDK_d:
                    calendar_create_calc_window (appGUI);
                    return TRUE;
                case GDK_a:
                    gtk_expander_set_expanded (GTK_EXPANDER (appGUI->cal->aux_cal_expander),
                                               !gtk_expander_get_expanded (GTK_EXPANDER (appGUI->cal->aux_cal_expander)));
                    key_counter_add (5, appGUI);
                    return TRUE;
                case GDK_b:
                    if (appGUI->calendar_only == FALSE) {
                        cal_notes_browser (appGUI);
                    }
                    return TRUE;
            }

        }

        switch (event->keyval) {

            case GDK_Escape:
                if (appGUI->calendar_only == FALSE) {
                    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button)) == FALSE) {
                        if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
                            gui_toggle_window_visibility (appGUI);
						}
					} else {
	                    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);
					}
                }
                return TRUE;
            case GDK_Left:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Left */
                    calendar_btn_prev_day(appGUI);
                    return TRUE;
                }
                return FALSE;
            case GDK_Right:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Right */
                    calendar_btn_next_day(appGUI);
                    return TRUE;
                }
                return FALSE;
            case GDK_Up:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Up */
                    calendar_btn_prev_week(appGUI);
                    return TRUE;
                }
                return FALSE;
            case GDK_Down:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Down */
                    calendar_btn_next_week(appGUI);
                    return TRUE;
                }
                return FALSE;
        }

    }

#ifdef TASKS_ENABLED

    /************************************************************************/
    /*** TASKS PAGE                                                        ***/
    /************************************************************************/

    if(page == PAGE_TASKS) {

            switch (event->keyval) {

                case GDK_Escape:
                    if(appGUI->tsk->tasks_panel_status == TRUE) {
                        show_tasks_desc_panel(FALSE, appGUI);
                    } else {
                        if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
                            gui_toggle_window_visibility (appGUI);
						}
					}
                    return TRUE;
                case GDK_Return:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                        if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, NULL, NULL)) {
                            tasks_add_edit_dialog_show (TRUE, 0, get_seconds_for_today(), appGUI);
                            return TRUE;
                        }
                    }
                    if(appGUI->tsk->tasks_panel_status == FALSE) {
                        show_tasks_desc_panel(TRUE, appGUI);
                    }
                    return TRUE;
                case GDK_space:     /* don't use space key for marking task as done */
                    return TRUE;
                case GDK_h:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + h */
                        config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton));
                        config.hide_completed = !config.hide_completed;
                        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton), config.hide_completed);
                        gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
                        update_tasks_number (appGUI);
                    }
                    return TRUE;
                case GDK_Delete:
                    tasks_remove_dialog_show(appGUI->tsk->tasks_list, appGUI->tsk->tasks_list_store, appGUI);
                    return TRUE;
                case GDK_Insert:
                    tasks_add_edit_dialog_show (FALSE, 0, get_seconds_for_today(), appGUI);
                    return TRUE;
                case GDK_Left:
                    if (appGUI->tsk->filter_index > 0) {
                        appGUI->tsk->filter_index--;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->cf_combobox), appGUI->tsk->filter_index);
                    }
                    return TRUE;
                case GDK_Right:
                    if (appGUI->tsk->filter_index < utl_gui_get_combobox_items(GTK_COMBO_BOX(appGUI->tsk->cf_combobox))-1) {
                        appGUI->tsk->filter_index++;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->cf_combobox), appGUI->tsk->filter_index);
                    }
                    return TRUE;
                case GDK_y:
                    key_counter_add (26, appGUI);
                    return TRUE;
            }
    }

#endif /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED

    /************************************************************************/
    /*** CONTACTS PAGE                                                    ***/
    /************************************************************************/

    if(page == PAGE_CONTACTS) {

            switch (event->keyval) {

                case GDK_Escape:
                    if (gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        if(appGUI->cnt->contacts_panel_status == TRUE) {
                            show_contacts_desc_panel(FALSE, appGUI);
                        } else {
							if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
								gui_toggle_window_visibility (appGUI);
							}
						}
                    } else {
                        if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->cnt->contacts_find_entry)))) {
                            gtk_entry_set_text(GTK_ENTRY(appGUI->cnt->contacts_find_entry), "");
                        } 
                    }
                    return TRUE;
                case GDK_Return:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                        if (gtk_tree_selection_get_selected (appGUI->cnt->contacts_list_selection, NULL, NULL)) {
                            contacts_add_edit_dialog_show (TRUE, appGUI);
                            return TRUE;
                        }
                    }
                    if(appGUI->cnt->contacts_panel_status == FALSE) {
                        show_contacts_desc_panel(TRUE, appGUI);
                        return TRUE;
                    } else if (gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        utl_gui_sw_vscrollbar_move_position (appGUI->cnt->contacts_panel_scrolledwindow, SW_MOVE_DOWN);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_BackSpace:
                    if(appGUI->cnt->contacts_panel_status == TRUE && gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        utl_gui_sw_vscrollbar_move_position (appGUI->cnt->contacts_panel_scrolledwindow, SW_MOVE_UP);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Delete:
                    if(gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        contacts_remove_dialog_show(appGUI->cnt->contacts_list, appGUI->cnt->contacts_list_store, appGUI);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Insert:
                    if(gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        contacts_add_edit_dialog_show (FALSE, appGUI);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Down:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Down */
                        if (config.find_mode < CONTACTS_FF_ALL_FIELDS) {
                            config.find_mode++;
                        }
                        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), config.find_mode);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Up:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Up */
                        if (config.find_mode > CONTACTS_FF_FIRST_NAME) {
                            config.find_mode--;
                        }
                        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), config.find_mode);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_l:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + l */
                        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cnt->contacts_find_entry));
                        return TRUE;
                    }
                    return FALSE;
            }
    }

#endif /* CONTACTS_ENABLED */

    /************************************************************************/
    /*** NOTES PAGE                                                       ***/
    /************************************************************************/

#ifdef NOTES_ENABLED
    if(page == PAGE_NOTES) {
        if (appGUI->nte->editor_active == FALSE) {

            /* SELECTOR */
            switch (event->keyval) {

                case GDK_Return:
                    if (gtk_tree_selection_get_selected (appGUI->nte->notes_list_selection, NULL, NULL)) {
                        if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                            notes_edit_dialog_show(appGUI->nte->notes_list, appGUI->nte->notes_filter, appGUI);
                        } else {
                            notes_enter_password (appGUI);
                        }
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Delete:
                    notes_remove_dialog_show (appGUI->nte->notes_list, appGUI->nte->notes_list_store, appGUI);
                    return TRUE;
                case GDK_Insert:
                    notes_add_entry (appGUI);
                    return TRUE;
                case GDK_Left:
                    if (appGUI->nte->filter_index > 0) {
                        appGUI->nte->filter_index--;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->nte->cf_combobox), appGUI->nte->filter_index);
                    }
                    return TRUE;
                case GDK_Right:
                    if (appGUI->nte->filter_index < utl_gui_get_combobox_items(GTK_COMBO_BOX(appGUI->nte->cf_combobox))-1) {
                        appGUI->nte->filter_index++;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->nte->cf_combobox), appGUI->nte->filter_index);
                    }
                    return TRUE;
                case GDK_Escape:
					if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
						gui_toggle_window_visibility (appGUI);
					}
                    return TRUE;
            }

        } else {

            /* EDITOR */
            switch (event->keyval) {

                case GDK_w:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + w */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_Escape:
                    g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
                    return TRUE;

                case GDK_b:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + b */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/bold")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_i:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + i */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/italic")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_m:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + m */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/mark_color")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_u:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + u */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/underline")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_t:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + t */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/strike")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_s:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + s */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_f:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + f */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/find")), "clicked");
                        return TRUE;
                    }
                    return FALSE;

                case GDK_n:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + n */
                        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/clear")), "clicked");
                        return TRUE;
                    }
                    return FALSE;
            }
        }
    }
#endif  /* NOTES_ENABLED */

    /************************************************************************/
    /*** OPTIONS PAGE                                                     ***/
    /************************************************************************/

    if(page == PAGE_OPTIONS) {

            if (!(event->state & GDK_CONTROL_MASK)) {  /* CTRL + Page_Up */
                switch (event->keyval) {

                    case GDK_Page_Down:
                        options_switch_buttons(FALSE, appGUI);
                        return TRUE;

                    case GDK_Page_Up:
                        options_switch_buttons(TRUE, appGUI);
                        return TRUE;

                }
            }
    }

    /************************************************************************/
    /*** ABOUT PAGE                                                       ***/
    /************************************************************************/

    if(page == PAGE_ABOUT) {

            if (!(event->state & GDK_CONTROL_MASK)) {  /* CTRL + Page_Up */
                switch (event->keyval) {

                    case GDK_Page_Down:
                        about_switch_buttons(FALSE, appGUI);
                        return TRUE;

                    case GDK_Page_Up:
                        about_switch_buttons(TRUE, appGUI);
                        return TRUE;

                }
            }
    }

    /************************************************************************/
    /*** GLOBAL SHORTCUTS                                                 ***/
    /************************************************************************/

    switch (event->keyval) {

		case GDK_Escape:
			if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
				gui_toggle_window_visibility (appGUI);
			}
			return FALSE;

        case GDK_q:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Q */
                gui_quit_osmo(FALSE, appGUI);
            }
            return FALSE;

        case GDK_Page_Up:
#ifdef NOTES_ENABLED
            if (appGUI->nte->editor_active == FALSE) {
#endif  /* NOTES_ENABLED */
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Up */
                    set_visible_page(appGUI->current_tab, FALSE, appGUI);
                    return TRUE;
                } else if (page == PAGE_CALENDAR) {
                    if (config.day_notes_visible) {
                        if (event->state & GDK_MOD1_MASK) {  /* ALT + Page_Up */
                            calendar_btn_prev_month(appGUI);
                            return TRUE;
                        }
                    } else {
                        calendar_btn_prev_month(appGUI);
                        return TRUE;
                    }
                }
#ifdef NOTES_ENABLED
            }
#endif  /* NOTES_ENABLED */
            return FALSE;

        case GDK_Page_Down:
#ifdef NOTES_ENABLED
            if (appGUI->nte->editor_active == FALSE) {
#endif  /* NOTES_ENABLED */
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Down */
                    set_visible_page(appGUI->current_tab, TRUE, appGUI);
                    return TRUE;
                } else if (page == PAGE_CALENDAR) {
                    if (config.day_notes_visible) {
                        if (event->state & GDK_MOD1_MASK) {  /* ALT + Page_Down */
                            calendar_btn_next_month(appGUI);
                            return TRUE;
                        }
                    } else {
                        calendar_btn_next_month(appGUI);
                        return TRUE;
                    }
                }
#ifdef NOTES_ENABLED
            }
#endif  /* NOTES_ENABLED */
            return FALSE;
        case GDK_F1:
            select_tab (0, appGUI);
            return TRUE;
        case GDK_1:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 1 */
#if defined(TASKS_ENABLED) || defined(CONTACTS_ENABLED) || defined(NOTES_ENABLED)
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 1 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_calendar_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
#endif
                    select_tab (0, appGUI);
#if defined(TASKS_ENABLED) || defined(CONTACTS_ENABLED) || defined(NOTES_ENABLED)
                }
#endif
                return TRUE;
            }
            return FALSE;
        case GDK_F2:
            select_tab (1, appGUI);
            return TRUE;
        case GDK_2:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 2 */
#ifdef TASKS_ENABLED
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 2 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_tasks_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
#endif  /* TASKS_ENABLED */
                    select_tab (1, appGUI);
#ifdef TASKS_ENABLED
                }
#endif  /* TASKS_ENABLED */
                return TRUE;
            }
            return FALSE;
        case GDK_F3:
            select_tab (2, appGUI);
            return TRUE;
        case GDK_3:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 3 */
#ifdef CONTACTS_ENABLED
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 3 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_contacts_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
#endif  /* CONTACTS_ENABLED */
                    select_tab (2, appGUI);
#ifdef CONTACTS_ENABLED
                }
#endif  /* CONTACTS_ENABLED */
                return TRUE;
            }
            return FALSE;
        case GDK_F4:
            select_tab (3, appGUI);
            return TRUE;
        case GDK_4:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 4 */
#ifdef NOTES_ENABLED
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 4 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_notes_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
#endif  /* NOTES_ENABLED */
                   select_tab (3, appGUI);
#ifdef NOTES_ENABLED
                }
#endif  /* NOTES_ENABLED */
                return TRUE;
            }
            return FALSE;
        case GDK_F5:
            select_tab (4, appGUI);
            return TRUE;
        case GDK_5:
            if (event->state & GDK_MOD1_MASK) {  /* ALT + 5 */
                select_tab (4, appGUI);
                return TRUE;
            }
            return FALSE;
        case GDK_F6:
            select_tab (5, appGUI);
            return TRUE;
        case GDK_6:
            if (event->state & GDK_MOD1_MASK) {  /* ALT + 6 */
                select_tab (5, appGUI);
                return TRUE;
            }
            return FALSE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
notebook_sw_cb (GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer user_data) {
 
    GUI *appGUI = (GUI *)user_data;

#ifdef NOTES_ENABLED
    if (appGUI->nte->editor_active == TRUE) {
        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
    }
#endif  /* NOTES_ENABLED */

    appGUI->current_tab = page_num;

#ifdef TASKS_ENABLED
    if(appGUI->current_tab == PAGE_TASKS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->tsk->tasks_list));
    }
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    if(appGUI->current_tab == PAGE_CONTACTS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cnt->contacts_find_entry));
    }
#endif  /* CONTACTS_ENABLED */
}

/*------------------------------------------------------------------------------*/

gboolean            
main_window_resized_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->calendar_only == FALSE) {

#ifdef TASKS_ENABLED
        if (appGUI->tsk->tasks_panel_status == FALSE) {
            gtk_paned_set_position(GTK_PANED(appGUI->tsk->tasks_paned), 99999);
        }
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
        if (appGUI->cnt->contacts_panel_status == FALSE) {
            gtk_paned_set_position(GTK_PANED(appGUI->cnt->contacts_paned), 99999);
        }
#endif  /* CONTACTS_ENABLED */
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void 
trayicon_clicked_cb (GtkStatusIcon *status_icon, gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
	gtk_status_icon_set_blinking (appGUI->osmo_trayicon, FALSE);
    gui_toggle_window_visibility (appGUI);
}

/*------------------------------------------------------------------------------*/

void 
trayicon_popup_cb (GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_menu_popup(GTK_MENU(appGUI->trayicon_popup_menu), NULL, NULL, NULL, NULL, 
                   button, activate_time);

}

void
systray_popup_menu_quit_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gui_quit_osmo(FALSE, appGUI);
}

void
systray_popup_menu_show_calendar_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gtk_window_deiconify (GTK_WINDOW(appGUI->main_window));
    gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_CALENDAR);
}

#ifdef TASKS_ENABLED
void
systray_popup_menu_show_tasks_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gtk_window_deiconify (GTK_WINDOW(appGUI->main_window));
    gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_TASKS);
}
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
void
systray_popup_menu_show_contacts_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gtk_window_deiconify (GTK_WINDOW(appGUI->main_window));
    gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_CONTACTS);
}
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
void
systray_popup_menu_show_notes_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gtk_window_deiconify (GTK_WINDOW(appGUI->main_window));
    gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_NOTES);
}
#endif  /* NOTES_ENABLED */

void
systray_popup_menu_show_options_selected_cb (gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
    if (appGUI->window_visible == FALSE) {
        gtk_widget_show (appGUI->main_window);
        appGUI->window_visible = TRUE;
    }
    gtk_window_deiconify (GTK_WINDOW(appGUI->main_window));
    gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_OPTIONS);
}

/*------------------------------------------------------------------------------*/

void
gui_systray_tooltip_update (GUI *appGUI)
{
gchar desc[BUFFER_SIZE], tmpbuf[BUFFER_SIZE];
guint32 julian;
gboolean flag;

	flag = FALSE;
	julian = utl_get_current_julian ();

	g_snprintf (tmpbuf, BUFFER_SIZE, "%s", julian_to_str (julian, DATE_FULL));
	desc[0] = '\0';
	g_strlcat (desc, tmpbuf, BUFFER_SIZE);

	if (config.time_format == TIME_24) {
        g_snprintf (tmpbuf, BUFFER_SIZE, ", %s", current_time_to_str(TIME_HH_MM));
    } else {
        g_snprintf (tmpbuf, BUFFER_SIZE, ", %s", current_time_to_str(TIME_HH_MM_AMPM));
    }
	g_strlcat (desc, tmpbuf, BUFFER_SIZE);

	gtk_status_icon_set_tooltip (appGUI->osmo_trayicon, desc);
}

/*------------------------------------------------------------------------------*/

void
gui_systray_update_icon (GUI *appGUI) {

guint32 julian_day;
gboolean flag = FALSE;
        
	if (appGUI->window_visible == TRUE) return;

    julian_day = utl_get_current_julian ();

	if (config.ignore_day_note_events == FALSE) {
		if (check_note (utl_get_current_day (), utl_get_current_month (), utl_get_current_year (), appGUI) == TRUE) {
			gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NOTE);
			flag = TRUE;
		}
	}

#ifdef TASKS_ENABLED
    if (check_tasks(julian_day, FALSE, appGUI) == TRUE) {
        gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_TASK);
		flag = TRUE;
    }
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    if (check_contacts(julian_day, appGUI) == TRUE) {
        gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_BIRTHDAY);
		flag = TRUE;
    }
#endif  /* CONTACTS_ENABLED */

	if (flag == FALSE) {
        gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_NORMAL);
	} else {
		if (config.blink_on_events) {
			gtk_status_icon_set_blinking (appGUI->osmo_trayicon, TRUE);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
gui_systray_initialize (GUI *appGUI) {

GtkWidget   *menu_entry;
GtkWidget   *systray_menu_separator;

    if (appGUI->calendar_only == TRUE) {
        return;
    }

    appGUI->trayicon_popup_menu = gtk_menu_new();

    appGUI->trayicon_menu_calendar_item = gtk_image_menu_item_new_with_label (_("Show calendar"));
    gtk_menu_shell_append (GTK_MENU_SHELL(appGUI->trayicon_popup_menu), appGUI->trayicon_menu_calendar_item);
    g_signal_connect_swapped (G_OBJECT(appGUI->trayicon_menu_calendar_item), "activate", 
                              G_CALLBACK(systray_popup_menu_show_calendar_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (appGUI->trayicon_menu_calendar_item), 
                                   gtk_image_new_from_stock(OSMO_STOCK_SYSTRAY_MENU_CALENDAR, GTK_ICON_SIZE_MENU));
	if (config.hide_calendar == FALSE) {
        gtk_widget_show (appGUI->trayicon_menu_calendar_item);
	}

#ifdef TASKS_ENABLED
    appGUI->trayicon_menu_tasks_item = gtk_image_menu_item_new_with_label (_("Show tasks"));
    gtk_menu_shell_append (GTK_MENU_SHELL(appGUI->trayicon_popup_menu), appGUI->trayicon_menu_tasks_item);
    g_signal_connect_swapped (G_OBJECT(appGUI->trayicon_menu_tasks_item), "activate", 
                              G_CALLBACK(systray_popup_menu_show_tasks_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (appGUI->trayicon_menu_tasks_item), 
                                   gtk_image_new_from_stock(OSMO_STOCK_SYSTRAY_MENU_TASKS, GTK_ICON_SIZE_MENU));
    
	if (config.hide_tasks == FALSE) {
	    gtk_widget_show (appGUI->trayicon_menu_tasks_item);
	}
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    appGUI->trayicon_menu_contacts_item = gtk_image_menu_item_new_with_label (_("Show contacts"));
    gtk_menu_shell_append (GTK_MENU_SHELL(appGUI->trayicon_popup_menu), appGUI->trayicon_menu_contacts_item);
    g_signal_connect_swapped (G_OBJECT(appGUI->trayicon_menu_contacts_item), "activate", 
                              G_CALLBACK(systray_popup_menu_show_contacts_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (appGUI->trayicon_menu_contacts_item), 
                                   gtk_image_new_from_stock(OSMO_STOCK_SYSTRAY_MENU_CONTACTS, GTK_ICON_SIZE_MENU));
	if (config.hide_contacts == FALSE) {
        gtk_widget_show (appGUI->trayicon_menu_contacts_item);
	}
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
    appGUI->trayicon_menu_notes_item = gtk_image_menu_item_new_with_label (_("Show notes"));
    gtk_menu_shell_append (GTK_MENU_SHELL(appGUI->trayicon_popup_menu), appGUI->trayicon_menu_notes_item);
    g_signal_connect_swapped (G_OBJECT(appGUI->trayicon_menu_notes_item), "activate", 
                              G_CALLBACK(systray_popup_menu_show_notes_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (appGUI->trayicon_menu_notes_item), 
                                   gtk_image_new_from_stock(OSMO_STOCK_SYSTRAY_MENU_NOTES, GTK_ICON_SIZE_MENU));
	if (config.hide_notes == FALSE) {
        gtk_widget_show (appGUI->trayicon_menu_notes_item);
	}
#endif  /* NOTES_ENABLED */

    systray_menu_separator = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), systray_menu_separator);
    gtk_widget_show(systray_menu_separator);

	menu_entry = gtk_image_menu_item_new_with_label (_("Show options"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_show_options_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_entry), 
                                   gtk_image_new_from_stock(GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_MENU));
    gtk_widget_show(menu_entry);

    systray_menu_separator = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), systray_menu_separator);
    gtk_widget_show(systray_menu_separator);

	menu_entry = gtk_image_menu_item_new_with_label (_("Quit"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_quit_selected_cb), appGUI);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_entry), 
                                   gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_MENU));
    gtk_widget_show(menu_entry);

    /* create tray icon */

    appGUI->osmo_trayicon = gtk_status_icon_new_from_stock(OSMO_STOCK_SYSTRAY_NORMAL);
    g_signal_connect(G_OBJECT(appGUI->osmo_trayicon), "activate", 
                     G_CALLBACK(trayicon_clicked_cb), appGUI);
    g_signal_connect(G_OBJECT(appGUI->osmo_trayicon), "popup-menu",
                     G_CALLBACK(trayicon_popup_cb), appGUI);

    appGUI->window_visible = TRUE;

    if (config.enable_systray == TRUE) {
        gtk_status_icon_set_visible (appGUI->osmo_trayicon, TRUE);
        while (g_main_context_iteration (NULL, FALSE));

        if (gtk_status_icon_is_embedded (appGUI->osmo_trayicon) == FALSE) {
            appGUI->no_tray = TRUE;
            gtk_status_icon_set_visible (appGUI->osmo_trayicon, FALSE);
            gtk_widget_show(appGUI->main_window);
        } else {
            appGUI->no_tray = FALSE;
            gtk_status_icon_set_visible (appGUI->osmo_trayicon, TRUE);
            if (config.start_minimised_in_systray) {
                gui_toggle_window_visibility (appGUI);
            } else {
                gtk_widget_show (appGUI->main_window);
            }
        }
    } else {
        gtk_status_icon_set_visible (appGUI->osmo_trayicon, FALSE);
        gtk_widget_show (appGUI->main_window);
    }

	gui_systray_update_icon (appGUI);
    gui_systray_tooltip_update (appGUI);
}

/*------------------------------------------------------------------------------*/

gboolean
gui_create_window (GUI *appGUI) {

GdkPixbuf   *icon;

#ifdef CONTACTS_ENABLED
    appGUI->cnt->contacts_filter_disabled = TRUE;
#endif  /* CONTACTS_ENABLED */
    appGUI->all_pages_added = FALSE;

    appGUI->cal->fd_day_name_font = pango_font_description_from_string(config.day_name_font);
    appGUI->cal->fd_cal_font = pango_font_description_from_string(config.calendar_font);
    appGUI->cal->fd_notes_font = pango_font_description_from_string(config.notes_font);

#ifdef NOTES_ENABLED
    appGUI->nte->fd_notes_font = pango_font_description_from_string(config.notes_editor_font);
#endif  /* NOTES_ENABLED */

    appGUI->main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->main_window), _(APPLICATION_NAME));

   	gtk_widget_set_events(appGUI->main_window, GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

    icon = gdk_pixbuf_new_from_inline(-1, osmo_icon, FALSE, NULL);
    gtk_window_set_icon (GTK_WINDOW(appGUI->main_window), icon);
    g_object_unref(icon);

    gtk_window_move (GTK_WINDOW (appGUI->main_window), config.window_x, config.window_y);

    gtk_widget_realize (appGUI->main_window);

    if (appGUI->calendar_only == FALSE) {
        gtk_window_set_default_size (GTK_WINDOW(appGUI->main_window), config.window_size_x, config.window_size_y);
    } else {
        gtk_window_set_default_size (GTK_WINDOW(appGUI->main_window), 500, -1);
    }
    gtk_window_set_resizable (GTK_WINDOW (appGUI->main_window), TRUE);

    g_signal_connect (G_OBJECT (appGUI->main_window), "delete-event",
                      G_CALLBACK(gui_window_close_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->main_window), "key_press_event",
                      G_CALLBACK (key_press_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->main_window), "configure_event",
                      G_CALLBACK (main_window_resized_cb), appGUI);

    osmo_register_stock_icons ();
    utl_gui_url_initialize (appGUI);

    appGUI->notebook = gtk_notebook_new ();
    GTK_WIDGET_UNSET_FLAGS (appGUI->notebook, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->notebook);
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK(appGUI->notebook), config.tabs_position);
    gtk_container_add (GTK_CONTAINER(appGUI->main_window), appGUI->notebook);
    g_signal_connect (G_OBJECT(appGUI->notebook), "switch-page", 
                      G_CALLBACK(notebook_sw_cb), appGUI);

    gtk_notebook_set_scrollable (GTK_NOTEBOOK(appGUI->notebook), appGUI->tiny_gui);

    if (appGUI->calendar_only == TRUE) {
        gtk_notebook_set_show_tabs (GTK_NOTEBOOK(appGUI->notebook), FALSE);
    }

    gui_create_calendar(appGUI->notebook, appGUI);

    if (appGUI->calendar_only == FALSE) {

#ifdef TASKS_ENABLED
        gui_create_tasks (appGUI);
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
        gui_create_contacts (appGUI);
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
        gui_create_notes (appGUI);
#endif  /* NOTES_ENABLED */

        gui_create_options (appGUI);
        gui_create_about (appGUI);

        cal_read_notes (appGUI);

#ifdef HAVE_LIBICAL
        ics_initialize_timezone ();
        read_ical_entries (appGUI);
#endif  /* HAVE_LIBICAL */

#ifdef TASKS_ENABLED
        read_tasks_entries (appGUI);
        apply_task_attributes (appGUI);
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
        read_contacts_entries (appGUI);
        set_export_active (appGUI);
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
        read_notes_entries (appGUI);

        utl_gui_create_category_combobox (GTK_COMBO_BOX (appGUI->nte->cf_combobox), 
										  appGUI->opt->notes_category_store, FALSE);

        if (config.remember_category_in_notes == TRUE) {
            gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->nte->cf_combobox), config.current_category_in_notes);
        } else {
            gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->nte->cf_combobox), 0);
        }
#endif  /* NOTES_ENABLED */

#ifdef TASKS_ENABLED
        refresh_tasks (appGUI);
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
        appGUI->cnt->contacts_filter_disabled = FALSE;
#endif  /* CONTACTS_ENABLED */

        appGUI->number_of_tabs = NUMBER_OF_TABS;

        if(config.remember_latest_tab == TRUE) {
            appGUI->current_tab = config.latest_tab;
        } else {
            appGUI->current_tab = PAGE_CALENDAR;
        }
    } else {
        appGUI->number_of_tabs = 1;
        appGUI->current_tab = 0;
    }

    appGUI->all_pages_added = TRUE;
    calendar_set_today (appGUI);

    update_aux_calendars (appGUI);

    if (appGUI->check_events == TRUE) {
        return create_event_checker_window (appGUI);
    } else {
        gui_systray_initialize (appGUI);
    }
 
    if (appGUI->calendar_only == TRUE) {
        gtk_widget_show (appGUI->main_window);
    }

    gtk_notebook_set_current_page(GTK_NOTEBOOK(appGUI->notebook), appGUI->current_tab);

    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
display_license(GUI *appGUI) {

const gchar license_text[] = {
  "\nThis program is free software; you can redistribute it and/or modify "
  "it under the terms of the GNU General Public License as published by "
  "the Free Software Foundation; either version 2 of the License, or "
  "(at your option) any later version."
  "\n\n"
  "This program is distributed in the hope that it will be useful, "
  "but WITHOUT ANY WARRANTY; without even the implied warranty of "
  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
  "GNU General Public License for more details."
  "\n\n"
  "You should have received a copy of the GNU General Public License "
  "along with this program; if not, write to the Free Software "
  "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA."
};

    utl_gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, license_text, -1);
    gtk_widget_grab_focus (GTK_WIDGET(appGUI->scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
display_help(GUI *appGUI) {

const gchar *general_keys_text[] = {
    "<Ctrl+PageUp>",            N_("switch to previous tab"),
    "<Ctrl+PageDn>",            N_("switch to next tab"),
    "<Alt+1...6>, F1-F6",       N_("switch to selected page"),
    "<PageUp/PageDn>",          N_("switch page in options and about tab"),
    "<Ctrl+q>",                 N_("exit")
};

const gchar *calendar_keys_text[] = {
    "<Space>",                  N_("select current date"),
    "<Ctrl+Space>",             N_("toggle personal data visibility"),
    "<Arrows>",                 N_("change day"),
    "<Alt+Arrows>",             N_("change day while note editing"),
    "<Ctrl+Up/Down>",           N_("scroll the contents in the day info panel"),
    "<PageUp/PageDn>",          N_("change month"),
    "<Home/End>",               N_("change year"),
    "<a>",                      N_("toggle calendars for the previous and next month"),
    "<b>",                      N_("day notes browser"),
    "<c>",                      N_("assign background color to day note"),
    "<d>",                      N_("date calculator"),
    "<f>",                      N_("show full-year calendar"),
    "<g>",                      N_("jump to date"),
    "<Delete>",                 N_("remove day note"),
    "<Esc>",                    N_("close day note panel")
};

#ifdef TASKS_ENABLED
const gchar *tasks_keys_text[] = {
    "<Alt+a>, <Insert>",        N_("add task"),
    "<Alt+e>, <Ctrl+Enter>",    N_("edit task"),
    "<Alt+r>, <Delete>",        N_("remove task"),
    "<Ctrl+h>",                 N_("toggle hidden tasks"),
    "<Left>, <Right>",          N_("change category filter"),
    "<Esc>",                    N_("close task info panel")
};
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
const gchar *contacts_keys_text[] = {
    "<Insert>",                 N_("add contact"),
    "<Ctrl+Enter>",             N_("edit contact"),
    "<Delete>",                 N_("remove contact"),
    "<Ctrl+l>",                 N_("activate search field"),
    "<Ctrl+Up/Down>",           N_("change search mode"),
    "<Esc>",                    N_("close contact details panel")
};
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
const gchar *notes_selector_keys_text[] = {
    "<Enter>",                  N_("open note"),
    "<Insert>",                 N_("add note"),
    "<Delete>",                 N_("remove note"),
    "<Ctrl+Enter>",             N_("edit note name and category"),
    "<Left>, <Right>",          N_("change category filter")
};

const gchar *notes_editor_keys_text[] = {
    "<Ctrl+w>",                 N_("close note editor"),
    "<Ctrl+s>",                 N_("save note"),
    "<Ctrl+f>",                 N_("find text"),
    "<Ctrl+b>",                 N_("mark selection in bold"),
    "<Ctrl+i>",                 N_("mark selection in italic"),
    "<Ctrl+m>",                 N_("mark selection in background color"),
    "<Ctrl+u>",                 N_("underline selection"),
    "<Ctrl+t>",                 N_("strikethrough selection"),
    "<Ctrl+n>",                 N_("clear selection attributes")
};
#endif  /* NOTES_ENABLED */

gchar tmpbuf[BUFFER_SIZE];
gint i;

    utl_gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);

    snprintf(tmpbuf, BUFFER_SIZE-1, "\n%s\n\n", _("OSMO was designed keeping in mind the user convenience, so there are many key shorctuts. Here is the full list:"));
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("General"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(general_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", general_keys_text[i*2], gettext(general_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Calendar"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);
 
    for (i=0; i < G_N_ELEMENTS(calendar_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", calendar_keys_text[i*2], gettext(calendar_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

#ifdef TASKS_ENABLED
    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Tasks"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(tasks_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", tasks_keys_text[i*2], gettext(tasks_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Contacts"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    for (i=0; i < G_N_ELEMENTS(contacts_keys_text)/2; i++) {
        sprintf(tmpbuf, "\t%s - %s\n", contacts_keys_text[i*2], gettext(contacts_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Notes"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    snprintf(tmpbuf, BUFFER_SIZE-1, "\t%s:\n", _("Selector"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    for (i=0; i < G_N_ELEMENTS(notes_selector_keys_text)/2; i++) {
        sprintf(tmpbuf, "\t%s - %s\n", notes_selector_keys_text[i*2], gettext(notes_selector_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }

    snprintf(tmpbuf, BUFFER_SIZE-1, "\t%s:\n", _("Editor"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    for (i=0; i < G_N_ELEMENTS(notes_editor_keys_text)/2; i++) {
        sprintf(tmpbuf, "\t%s - %s\n", notes_editor_keys_text[i*2], gettext(notes_editor_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);
#endif  /* NOTES_ENABLED */

    gtk_widget_grab_focus(GTK_WIDGET(appGUI->scrolled_window));
}

/*------------------------------------------------------------------------------*/

static void 
add_credits_section (GUI *appGUI, const gchar *section_title, 
					 OsmoCreditEntry *credits, guint32 n_credits)
{

gint32 i;
gchar *s;
static gboolean init = TRUE;

	if(init) {
		init = FALSE;
		gtk_text_buffer_create_tag (appGUI->about_entry_buffer, "fixed", "family", "monospace", NULL);
	}

	s = g_strdup_printf ("\n %s:\n", section_title);
	gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
											  &appGUI->about_entry_iter, s, -1, "bold", NULL);
	g_free (s);

	for(i = 0; i < n_credits; i ++) {

		if(credits[i].tag) {
			s = g_strdup_printf ("  [%s]", credits[i].tag);
		} else {
			s = g_strdup("      ");
		}

		gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer,
												  &appGUI->about_entry_iter, s, -1, "fixed", NULL);
		g_free (s);

		s = g_strdup_printf ("\t%s <", credits[i].name);
		gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, s, -1);
		g_free (s);

		utl_gui_url_insert_link (&appGUI->about_links_list,
							     &appGUI->about_link_index, appGUI->about_textview,
							     &appGUI->about_entry_iter, NULL, NULL, credits[i].email, FALSE,	appGUI);

		gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);
	}
}

/*------------------------------------------------------------------------------*/

void
display_about(GUI *appGUI) {
gchar tmpbuf[BUFFER_SIZE];

    utl_gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);

    utl_gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);
    sprintf(tmpbuf, "%s\n", _("A handy personal organizer"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "big", "center", NULL);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    utl_gui_url_insert_link (&appGUI->about_links_list, &appGUI->about_link_index, 
							 appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
							 NULL, "http://clay.ll.pl/osmo", TRUE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    sprintf(tmpbuf, "\n(%s %s, %s)\n\n", _("compiled on"), __DATE__, __TIME__);
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "center", "italic", NULL);

	add_credits_section (appGUI, _("Programming"), credits_programming, 
						 G_N_ELEMENTS(credits_programming));
	add_credits_section (appGUI, _("Graphics"), credits_graphics, 
						 G_N_ELEMENTS(credits_graphics));
	add_credits_section (appGUI, _("Contributors"), credits_contributors, 
						 G_N_ELEMENTS(credits_contributors));
	add_credits_section (appGUI, _("Translators"), credits_translators, 
						 G_N_ELEMENTS(credits_translators));

    /* Available modules */

    sprintf(tmpbuf, "\n %s:\n", _("Available modules"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    sprintf(tmpbuf, "     [+]\t%s\n", _("Calendar"));
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef TASKS_ENABLED
    sprintf(tmpbuf, "     [+]\t%s\n", _("Tasks"));
#else
    sprintf(tmpbuf, "     [-]\t%s\n", _("Tasks"));
#endif  /* TASKS_ENABLED */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef CONTACTS_ENABLED
    sprintf(tmpbuf, "     [+]\t%s\n", _("Contacts"));
#else
    sprintf(tmpbuf, "     [-]\t%s\n", _("Contacts"));
#endif  /* CONTACTS_ENABLED */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef NOTES_ENABLED
    sprintf(tmpbuf, "     [+]\t%s\n", _("Notes"));
#else
    sprintf(tmpbuf, "     [-]\t%s\n", _("Notes"));
#endif  /* NOTES_ENABLED */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);


    /* Compiled-in features */

    sprintf(tmpbuf, "\n %s:\n", _("Compiled-in features"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

#ifdef HAVE_LIBICAL
    sprintf(tmpbuf, "     [+]\t%s (libical)\n", _("iCalendar support"));
#else
    sprintf(tmpbuf, "     [-]\t%s (libical)\n", _("iCalendar support"));
#endif  /* HAVE_LIBICAL */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef HAVE_LIBGRINGOTTS
    sprintf(tmpbuf, "     [+]\t%s (libgringotts)\n", _("Encrypted notes support"));
#else
    sprintf(tmpbuf, "     [-]\t%s (libgringotts)\n", _("Encrypted notes support"));
#endif  /* HAVE_LIBGRINGOTTS */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef HAVE_LIBSYNCML
    sprintf(tmpbuf, "     [+]\t%s (libsyncml)\n", _("SyncML enabled device support"));
#else
    sprintf(tmpbuf, "     [-]\t%s (libsyncml)\n", _("SyncML enabled device support"));
#endif  /* HAVE_LIBSYNCML */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef PRINTING_SUPPORT
    sprintf(tmpbuf, "     [+]\t%s\n", _("Printing support"));
#else
    sprintf(tmpbuf, "     [-]\t%s\n", _("Printing support"));
#endif  /* PRINTING_SUPPORT */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

#ifdef HAVE_GTKSPELL
    sprintf(tmpbuf, "     [+]\t%s (gtkspell)\n", _("Spell checker support"));
#else
    sprintf(tmpbuf, "     [-]\t%s (gtkspell)\n", _("Spell checker support"));
#endif  /* HAVE_GTKSPELL */
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);

    gtk_widget_grab_focus(GTK_WIDGET(appGUI->scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
about_switch_buttons(gboolean left, GUI *appGUI) {

    if (left == TRUE && appGUI->about_counter > 0) {
        --appGUI->about_counter;
    } else if (left == FALSE && appGUI->about_counter < 2) {
        appGUI->about_counter++;
    }

    if (appGUI->about_counter == 0){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->about_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->about_radiobutton), "released");
    } else if (appGUI->about_counter == 1) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->help_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->help_radiobutton), "released");
    } else {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->license_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->license_radiobutton), "released");
    }
}

/*------------------------------------------------------------------------------*/

void
about_set_first_page(GUI *appGUI) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->about_radiobutton), TRUE);
    g_signal_emit_by_name(G_OBJECT(appGUI->about_radiobutton), "released");
}

/*------------------------------------------------------------------------------*/

void
button_released_cb (GtkButton *button, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->about_radiobutton)) == TRUE) {

        display_about(appGUI);
        appGUI->about_counter = 0;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->help_radiobutton)) == TRUE) {

        display_help(appGUI);
        appGUI->about_counter = 1;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->license_radiobutton)) == TRUE) {

        display_license(appGUI);
        appGUI->about_counter = 2;

    }
}

/*------------------------------------------------------------------------------*/

void 
gui_create_about(GUI *appGUI) {

GtkWidget *vbox1, *hbox;
GtkWidget *label;
GtkWidget *hseparator;
GtkWidget *logo_area;
GdkPixbuf *logo, *logo_scaled = NULL;
GtkWidget *viewport;
GtkWidget *hbuttonbox;
GSList    *radiobutton_group = NULL;
char tmpbuf[BUFFER_SIZE];

    appGUI->about_counter = 0;

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gui_add_to_notebook (vbox1, _("About"), appGUI);

    appGUI->about_vbox = GTK_BOX(vbox1);

    logo_area = gtk_image_new();
    logo = gdk_pixbuf_new_from_inline(-1, osmo_logo, FALSE, NULL);
    gtk_widget_show(logo_area);
    gtk_box_pack_start (GTK_BOX (vbox1), logo_area, FALSE, TRUE, 0);

    if (appGUI->tiny_gui == TRUE) {
        logo_scaled = gdk_pixbuf_scale_simple(logo,
                                              gdk_pixbuf_get_width(logo) / 2, 
                                              gdk_pixbuf_get_height(logo) / 2, 
                                              GDK_INTERP_HYPER);
    }

#ifndef REV
    sprintf(tmpbuf, "%s %s", _("version"), VERSION);
#else
    sprintf(tmpbuf, "%s %d", _("SVN revision"), REV);
#endif

    label = gtk_label_new(tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (vbox1), label, FALSE, TRUE, 4);

    if (logo_scaled != NULL) {
        gtk_image_set_from_pixbuf (GTK_IMAGE (logo_area), logo_scaled);
    } else {
        gtk_image_set_from_pixbuf (GTK_IMAGE (logo_area), logo);
    }

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    if (appGUI->tiny_gui == TRUE) {
        hbox = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox);
        gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, TRUE, 0);
    } else {
        hbuttonbox = gtk_hbutton_box_new ();
        gtk_widget_show (hbuttonbox);
        gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
        gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    }

    if (appGUI->tiny_gui == TRUE) {
        appGUI->about_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_ABOUT, 
																	  GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->about_radiobutton = utl_gui_stock_label_radio_button (_("About"), OSMO_STOCK_ABOUT, 
																	  GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->about_radiobutton);
    }
    gtk_widget_show (appGUI->about_radiobutton);

    gtk_button_set_relief (GTK_BUTTON (appGUI->about_radiobutton), GTK_RELIEF_NONE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->about_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->about_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->about_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->about_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->about_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->about_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

    if (appGUI->tiny_gui == TRUE) {
        appGUI->help_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_HELP, 
																	 GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->help_radiobutton = utl_gui_stock_label_radio_button (_("Key shortcuts"), OSMO_STOCK_HELP, 
																	 GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->help_radiobutton);
    }
    gtk_widget_show (appGUI->help_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->help_radiobutton), GTK_RELIEF_NONE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->help_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->help_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->help_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->help_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->help_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

    if (appGUI->tiny_gui == TRUE) {
        appGUI->license_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_LICENSE, 
																		GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->license_radiobutton = utl_gui_stock_label_radio_button (_("License"), OSMO_STOCK_LICENSE, 
																		GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->license_radiobutton);
    }
    gtk_widget_show (appGUI->license_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->license_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->license_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->license_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->license_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->license_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->license_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

    if (appGUI->tiny_gui == TRUE) {
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->license_radiobutton, FALSE, TRUE, 0);
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->help_radiobutton, FALSE, TRUE, 0);
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->about_radiobutton, FALSE, TRUE, 0);
    }

    /*--------------------------------------------------------------------------*/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    appGUI->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox1), appGUI->scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (appGUI->scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (appGUI->scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 0);
    gtk_container_add (GTK_CONTAINER (appGUI->scrolled_window), viewport);

    appGUI->about_entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 0);
    gtk_text_buffer_create_tag (appGUI->about_entry_buffer, "bold", "weight", PANGO_WEIGHT_ULTRABOLD, NULL);
    gtk_text_buffer_create_tag (appGUI->about_entry_buffer, "big", "size", 16 * PANGO_SCALE, NULL);
    gtk_text_buffer_create_tag (appGUI->about_entry_buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (appGUI->about_entry_buffer, "center", "justification", GTK_JUSTIFY_CENTER, NULL);
    gtk_text_buffer_get_iter_at_offset (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 0);

    appGUI->about_textview = gtk_text_view_new_with_buffer (appGUI->about_entry_buffer);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->about_textview), 1);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->about_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW(appGUI->about_textview), 2);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(appGUI->about_textview), 6);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW(appGUI->about_textview), 6);
    gtk_widget_show (appGUI->about_textview);
    gtk_container_add (GTK_CONTAINER (viewport), appGUI->about_textview);

    utl_gui_url_setup (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview, appGUI);

    display_about (appGUI);
}

/*------------------------------------------------------------------------------*/

void
gui_add_to_notebook (GtkWidget *widget, gchar *text, GUI *appGUI) {

GtkWidget *label;

    label = gtk_label_new(NULL);

    if (config.tabs_position == GTK_POS_LEFT) {
        gtk_label_set_angle (GTK_LABEL(label), 90.0);
    } else if (config.tabs_position == GTK_POS_RIGHT) {
        gtk_label_set_angle (GTK_LABEL(label), -90.0);
    }
    gtk_label_set_markup (GTK_LABEL (label), text);

    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), widget, label);

}

/*------------------------------------------------------------------------------*/

