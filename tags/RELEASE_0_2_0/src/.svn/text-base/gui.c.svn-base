
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>
#include <config.h>

#include "i18n.h"
#include "gui.h"
#include "gui_utils.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_fullyear.h"
#include "calendar_widget.h"
#include "calendar_jumpto.h"
#include "calendar_calc.h"
#ifdef HAVE_LIBICAL
#include "calendar_ical.h"
#endif  /* HAVE_LIBICAL */
#include "time_utils.h"
#include "date_utils.h"
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
#include "stock_icons.h"
#include "config.h"

#include "gui_logo.h"
#include "gui_icon.h"

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
                config.lastrun_date = get_julian_for_today();
                config.lastrun_time = get_seconds_for_today();
                write_notes (appGUI);
                write_tasks_entries(appGUI);
#ifdef HAVE_LIBICAL
                write_ical_entries(appGUI);
                ics_calendar_refresh (appGUI);
#endif  /* HAVE_LIBICAL */
                write_contacts_entries(appGUI);
#ifdef HAVE_LIBGRINGOTTS
                write_notes_entries (appGUI);
#endif  /* HAVE_LIBGRINGOTTS */
                prefs_write_config ();
            }
            gtk_widget_hide (appGUI->main_window);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
gui_quit_osmo (GUI *appGUI) {

#ifdef HAVE_LIBGRINGOTTS
    if (appGUI->nte->editor_active == TRUE) {
        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
    }
#endif  /* HAVE_LIBGRINGOTTS */

    if (appGUI->osmo_trayicon != NULL) {
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
    }

    if (appGUI->calendar_only == FALSE) {
        if (appGUI->current_tab == PAGE_CALENDAR || appGUI->current_tab == PAGE_TASKS || 
#ifdef HAVE_LIBGRINGOTTS
            appGUI->current_tab == PAGE_CONTACTS || appGUI->current_tab == PAGE_NOTES) {
#else
            appGUI->current_tab == PAGE_CONTACTS) {
#endif  /* HAVE_LIBGRINGOTTS */
            config.latest_tab = appGUI->current_tab;
        }

        config.find_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox));

        gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);

        gtk_window_get_size (GTK_WINDOW(appGUI->main_window),
                            &config.window_size_x, &config.window_size_y);
    }

    gdk_window_get_root_origin (GDK_WINDOW(appGUI->main_window->window),
                        &config.window_x, &config.window_y);

    pango_font_description_free(appGUI->fd_calendar_day_name);
    pango_font_description_free(appGUI->fd_calendar_cal);
    pango_font_description_free(appGUI->fd_calendar_notes);

    pango_font_description_free(appGUI->fd_notes_editor);

    if (appGUI->calendar_only == FALSE) {
        write_notes (appGUI);
        free_notes_list(appGUI);
        write_tasks_entries(appGUI);
#ifdef HAVE_LIBICAL
        write_ical_entries(appGUI);
#endif  /* HAVE_LIBICAL */
        write_contacts_entries(appGUI);
#ifdef HAVE_LIBGRINGOTTS
        write_notes_entries (appGUI);
#endif  /* HAVE_LIBGRINGOTTS */
    }

    gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);
    gui_url_remove_links (&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index);

    gtk_main_quit ();
}

/*------------------------------------------------------------------------------*/

void
gui_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (config.enable_systray == TRUE && appGUI->no_tray == FALSE && appGUI->calendar_only == FALSE) {
        gui_toggle_window_visibility (appGUI);
    } else {
        gui_quit_osmo(appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gint
get_first_active_page (GUI *appGUI) {

    if (!config.hide_calendar) 
        return PAGE_CALENDAR;
    if (!config.hide_tasks) 
        return PAGE_TASKS;
    if (!config.hide_contacts) 
        return PAGE_CONTACTS;
#ifdef HAVE_LIBGRINGOTTS
    if (!config.hide_notes) 
        return PAGE_NOTES;
#endif  /* HAVE_LIBGRINGOTTS */

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
            case PAGE_TASKS:
                if (config.hide_tasks) {
                    page += n;
                    flag = TRUE;
                }
                break;
            case PAGE_CONTACTS:
                if (config.hide_contacts) {
                    page += n;
                    flag = TRUE;
                }
                break;
#ifdef HAVE_LIBGRINGOTTS
            case PAGE_NOTES:
                if (config.hide_notes) {
                    page += n;
                    flag = TRUE;
                }
                break;
#endif  /* HAVE_LIBGRINGOTTS */
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
            if (i == PAGE_TASKS && !config.hide_tasks) n++;
            if (i == PAGE_CONTACTS && !config.hide_contacts) n++;
#ifdef HAVE_LIBGRINGOTTS
            if (i == PAGE_NOTES && !config.hide_notes) n++;
#endif  /* HAVE_LIBGRINGOTTS */
            if (i == PAGE_OPTIONS || i == PAGE_ABOUT) n++;

            if (n == tab+1) break;
        }

        if (i != NUMBER_OF_TABS) {
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), i);
        }
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
                        gui_scrolled_window_move_position(appGUI->cal->day_info_scrolledwindow, SW_MOVE_UP);
                        return TRUE;
                    } else {
                        calendar_btn_prev_week(appGUI);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Down:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Up */
                        gui_scrolled_window_move_position(appGUI->cal->day_info_scrolledwindow, SW_MOVE_DOWN);
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
                        gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), 
                                                     !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button)));
                    }
                    return TRUE;
                case GDK_space:
                    set_today (TRUE, appGUI);
                    return TRUE;
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
                    return TRUE;
                case GDK_d:
                    calendar_create_calc_window  (appGUI);
                    return TRUE;
                case GDK_a:
                    gtk_expander_set_expanded (GTK_EXPANDER (appGUI->cal->aux_cal_expander),
                                               !gtk_expander_get_expanded (GTK_EXPANDER (appGUI->cal->aux_cal_expander)));
                    return TRUE;
            }

        }

        switch (event->keyval) {

            case GDK_Escape:
                if (appGUI->calendar_only == FALSE) {
                    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);
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

    /************************************************************************/
    /*** TASKS PAGE                                                        ***/
    /************************************************************************/

    if(page == PAGE_TASKS) {

            switch (event->keyval) {

                case GDK_Escape:
                    if(appGUI->tsk->tasks_panel_status == TRUE) {
                        show_tasks_desc_panel(FALSE, appGUI);
                    }
                    return TRUE;
                case GDK_Return:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                        if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, NULL, NULL)) {
                            tasks_add_edit_dialog_show (TRUE, appGUI);
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
                    tasks_add_edit_dialog_show (FALSE, appGUI);
                    return TRUE;
                case GDK_Left:
                    if (appGUI->tsk->filter_index > 0) {
                        appGUI->tsk->filter_index--;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->cf_combobox), appGUI->tsk->filter_index);
                    }
                    return TRUE;
                case GDK_Right:
                    if (appGUI->tsk->filter_index < gui_get_combobox_items(GTK_COMBO_BOX(appGUI->tsk->cf_combobox))-1) {
                        appGUI->tsk->filter_index++;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->cf_combobox), appGUI->tsk->filter_index);
                    }
                    return TRUE;
            }
    }

    /************************************************************************/
    /*** CONTACTS PAGE                                                    ***/
    /************************************************************************/

    if(page == PAGE_CONTACTS) {

            switch (event->keyval) {

                case GDK_Escape:
                    if (gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        if(appGUI->cnt->contacts_panel_status == TRUE) {
                            show_contacts_desc_panel(FALSE, appGUI);
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
                        gui_scrolled_window_move_position(appGUI->cnt->contacts_panel_scrolledwindow, SW_MOVE_DOWN);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_BackSpace:
                    if(appGUI->cnt->contacts_panel_status == TRUE && gtk_widget_is_focus(appGUI->cnt->contacts_find_entry) == FALSE) {
                        gui_scrolled_window_move_position(appGUI->cnt->contacts_panel_scrolledwindow, SW_MOVE_UP);
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
                case GDK_F1:
                    gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_FIRST_NAME);
                    return TRUE;
                case GDK_F2:
                    gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_LAST_NAME);
                    return TRUE;
                case GDK_F3:
                    gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_ALL_FIELDS);
                    return TRUE;
                case GDK_l:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + l */
                        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cnt->contacts_find_entry));
                        return TRUE;
                    }
                    return FALSE;
            }
    }

    /************************************************************************/
    /*** NOTES PAGE                                                       ***/
    /************************************************************************/

#ifdef HAVE_LIBGRINGOTTS
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
                    if (appGUI->nte->filter_index < gui_get_combobox_items(GTK_COMBO_BOX(appGUI->nte->cf_combobox))-1) {
                        appGUI->nte->filter_index++;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->nte->cf_combobox), appGUI->nte->filter_index);
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
#endif  /* HAVE_LIBGRINGOTTS */

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

        case GDK_q:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Q */
                gui_quit_osmo(appGUI);
            }
            return FALSE;

        case GDK_Page_Up:
#ifdef HAVE_LIBGRINGOTTS
            if (appGUI->nte->editor_active == FALSE) {
#endif  /* HAVE_LIBGRINGOTTS */
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
#ifdef HAVE_LIBGRINGOTTS
            }
#endif  /* HAVE_LIBGRINGOTTS */
            return FALSE;

        case GDK_Page_Down:
#ifdef HAVE_LIBGRINGOTTS
            if (appGUI->nte->editor_active == FALSE) {
#endif  /* HAVE_LIBGRINGOTTS */
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
#ifdef HAVE_LIBGRINGOTTS
            }
#endif  /* HAVE_LIBGRINGOTTS */
            return FALSE;
        case GDK_1:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 1 */
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 1 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_calendar_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
                    select_tab (0, appGUI);
                }
                return TRUE;
            }
            return FALSE;
        case GDK_2:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 2 */
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 2 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_tasks_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
                    select_tab (1, appGUI);
                }
                return TRUE;
            }
            return FALSE;
        case GDK_3:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 3 */
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 3 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_contacts_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
                    select_tab (2, appGUI);
                }
                return TRUE;
            }
            return FALSE;
        case GDK_4:
            if ((event->state & GDK_MOD1_MASK)) {  /* ALT + 4 */
#ifdef HAVE_LIBGRINGOTTS
                if (event->state & GDK_CONTROL_MASK) {  /* CTRL + ALT + 4 */
                    g_signal_emit_by_name(G_OBJECT(appGUI->opt->hide_notes_checkbutton), "clicked");
                    select_tab (0, appGUI);
                } else {
#endif  /* HAVE_LIBGRINGOTTS */
                   select_tab (3, appGUI);
#ifdef HAVE_LIBGRINGOTTS
                }
#endif  /* HAVE_LIBGRINGOTTS */
                return TRUE;
            }
            return FALSE;
        case GDK_5:
            if (event->state & GDK_MOD1_MASK) {  /* ALT + 5 */
                select_tab (4, appGUI);
                return TRUE;
            }
            return FALSE;
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

#ifdef HAVE_LIBGRINGOTTS
    if (appGUI->nte->editor_active == TRUE) {
        g_signal_emit_by_name(G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), "clicked");
    }
#endif  /* HAVE_LIBGRINGOTTS */

    appGUI->current_tab = page_num;

    if(appGUI->current_tab == PAGE_TASKS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->tsk->tasks_list));
    }

    if(appGUI->current_tab == PAGE_CONTACTS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cnt->contacts_find_entry));
    }
}

/*------------------------------------------------------------------------------*/

gboolean            
main_window_resized_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->calendar_only == FALSE) {
        if (appGUI->tsk->tasks_panel_status == FALSE) {
            gtk_paned_set_position(GTK_PANED(appGUI->tsk->tasks_paned), 99999);
        }

        if (appGUI->cnt->contacts_panel_status == FALSE) {
            gtk_paned_set_position(GTK_PANED(appGUI->cnt->contacts_paned), 99999);
        }
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void 
trayicon_clicked_cb (GtkStatusIcon *status_icon, gpointer user_data) {
    GUI *appGUI = (GUI *)user_data;
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
    gui_quit_osmo(appGUI);
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

#ifdef HAVE_LIBGRINGOTTS
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
#endif  /* HAVE_LIBGRINGOTTS */

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
gui_systray_tooltip_update (GUI *appGUI) {

gchar       desc[BUFFER_SIZE], tmpbuf[BUFFER_SIZE];
gboolean    flag = FALSE;

    sprintf(tmpbuf, "%s", julian_to_str(get_julian_for_today(), DATE_FULL));
    desc[0] = '\0';
    strcat(desc, tmpbuf);

    if (check_contacts(get_current_day(), get_current_month(), get_current_year(), appGUI) == TRUE) {
        strcat(desc, "\n");
        flag = TRUE;
        sprintf(tmpbuf, "\n%s", _("Birthday found"));
        strcat(desc, tmpbuf);
    }

    if (check_tasks(get_current_day(), get_current_month(), get_current_year(), appGUI) == TRUE) {
        if (flag == FALSE) {
            strcat(desc, "\n");
            flag = TRUE;
        }
        sprintf(tmpbuf, "\n%s", _("Task found"));
        strcat(desc, tmpbuf);
    }

    if (check_note(get_current_day(), get_current_month()+1, get_current_year(), appGUI) == TRUE) {
        if (flag == FALSE) {
            strcat(desc, "\n");
            flag = TRUE;
        }
        sprintf(tmpbuf, "\n%s", _("Day note available"));
        strcat(desc, tmpbuf);
    }

    gtk_status_icon_set_tooltip (appGUI->osmo_trayicon, desc);
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

    menu_entry = gtk_menu_item_new_with_label(_("Show calendar"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_show_calendar_selected_cb), appGUI);
    gtk_widget_show(menu_entry);

    menu_entry = gtk_menu_item_new_with_label(_("Show tasks"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_show_tasks_selected_cb), appGUI);
    gtk_widget_show(menu_entry);

    menu_entry = gtk_menu_item_new_with_label(_("Show contacts"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_show_contacts_selected_cb), appGUI);
    gtk_widget_show(menu_entry);

#ifdef HAVE_LIBGRINGOTTS
    menu_entry = gtk_menu_item_new_with_label(_("Show notes"));
    gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->trayicon_popup_menu), menu_entry);
    g_signal_connect_swapped(G_OBJECT(menu_entry), "activate", 
                             G_CALLBACK(systray_popup_menu_show_notes_selected_cb), appGUI);
    gtk_widget_show(menu_entry);
#endif  /* HAVE_LIBGRINGOTTS */

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

    if (check_contacts(get_current_day(), get_current_month(), get_current_year(), appGUI) == TRUE) {
        gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_BIRTHDAY);
    }

    if (check_tasks(get_current_day(), get_current_month(), get_current_year(), appGUI) == TRUE) {
        gtk_status_icon_set_from_stock (appGUI->osmo_trayicon, OSMO_STOCK_SYSTRAY_TASK);
    }

    appGUI->window_visible = TRUE;

    gui_systray_tooltip_update (appGUI);

    if (config.enable_systray == TRUE) {
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, TRUE);
        while (g_main_context_iteration(NULL, FALSE));

        if (gtk_status_icon_is_embedded(appGUI->osmo_trayicon) == FALSE) {
            appGUI->no_tray = TRUE;
            gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
            gtk_widget_show(appGUI->main_window);
        } else {
            appGUI->no_tray = FALSE;
            gtk_status_icon_set_visible(appGUI->osmo_trayicon, TRUE);
            if (config.start_minimised_in_systray) {
                gui_toggle_window_visibility (appGUI);
            } else {
                gtk_widget_show(appGUI->main_window);
            }
        }
    } else {
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
        gtk_widget_show(appGUI->main_window);
    }
}

/*------------------------------------------------------------------------------*/

void 
gui_create_window (GUI *appGUI) {

GdkPixbuf   *icon;
gchar tmpbuf[BUFFER_SIZE];

    appGUI->cnt->contacts_filter_disabled = TRUE;
    appGUI->all_pages_added = FALSE;

    appGUI->fd_calendar_day_name = pango_font_description_from_string(config.day_name_font);
    appGUI->fd_calendar_cal = pango_font_description_from_string(config.calendar_font);
    appGUI->fd_calendar_notes = pango_font_description_from_string(config.notes_font);

    appGUI->fd_notes_editor = pango_font_description_from_string(config.notes_editor_font);

    appGUI->main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    sprintf(tmpbuf, "OSMO - %s", _("a handy personal organizer"));
    gtk_window_set_title (GTK_WINDOW (appGUI->main_window), tmpbuf);

   	gtk_widget_set_events(appGUI->main_window, GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

    icon = gdk_pixbuf_new_from_inline(-1, osmo_icon, FALSE, NULL);
    gtk_window_set_icon (GTK_WINDOW(appGUI->main_window), icon);
    g_object_unref(icon);

    gtk_window_move (GTK_WINDOW (appGUI->main_window), config.window_x, config.window_y);
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

    osmo_register_stock_icons();

    appGUI->osmo_tooltips = gtk_tooltips_new();

    appGUI->notebook = gtk_notebook_new();
    GTK_WIDGET_UNSET_FLAGS(appGUI->notebook, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->notebook);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(appGUI->notebook), config.tabs_position);
    gtk_container_add(GTK_CONTAINER(appGUI->main_window), appGUI->notebook);
    g_signal_connect(G_OBJECT(appGUI->notebook), "switch-page", 
                     G_CALLBACK(notebook_sw_cb), appGUI);

    if (appGUI->calendar_only == TRUE) {
        gtk_notebook_set_show_tabs (GTK_NOTEBOOK(appGUI->notebook), FALSE);
    }

    gui_create_calendar(appGUI->notebook, appGUI);

    if (appGUI->calendar_only == FALSE) {
        gui_create_tasks (appGUI);
        gui_create_contacts (appGUI);
#ifdef HAVE_LIBGRINGOTTS
        gui_create_notes (appGUI);
#endif  /* HAVE_LIBGRINGOTTS */
        gui_create_options (appGUI);
        gui_create_about (appGUI);

        read_notes (appGUI);

#ifdef HAVE_LIBICAL
        if (ics_initialize_timezone () == FALSE) {
            fprintf(stderr, "ERROR: Cannot initialize timezone info. iCal support disabled.\n");
        }
        read_ical_entries (appGUI);
#endif  /* HAVE_LIBICAL */

        read_tasks_entries (appGUI);
        apply_task_attributes (appGUI);

        read_contacts_entries (appGUI);
        set_export_active (appGUI);

#ifdef HAVE_LIBGRINGOTTS
        read_notes_entries (appGUI);

        gui_create_category_combobox (GTK_COMBO_BOX (appGUI->nte->cf_combobox), appGUI->opt->notes_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->nte->cf_combobox), 0);
#endif  /* HAVE_LIBGRINGOTTS */

        gui_create_category_combobox (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), appGUI->opt->tasks_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->tsk->cf_combobox), 0);

        appGUI->cnt->contacts_filter_disabled = FALSE;

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

    gui_url_initialize(appGUI);

    gtk_notebook_set_current_page(GTK_NOTEBOOK(appGUI->notebook), appGUI->current_tab);

    appGUI->all_pages_added = TRUE;
    set_today (TRUE, appGUI);

    update_aux_calendars (appGUI);
    gui_systray_initialize (appGUI);
 
    if (appGUI->calendar_only == TRUE) {
        gtk_widget_show (appGUI->main_window);
    }
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

    gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, license_text, -1);
    gtk_widget_grab_focus(GTK_WIDGET(appGUI->scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
display_help(GUI *appGUI) {

const gchar *general_keys_text[] = {
    "<Ctrl+PageUp>",            N_("switch to previous tab"),
    "<Ctrl+PageDn>",            N_("switch to next tab"),
    "<PageUp/PageDn>",          N_("switch page in options and about tab"),
    "<Ctrl+q>",                 N_("exit")
};

const gchar *calendar_keys_text[] = {
    "<Space>",                  N_("select current date"),
    "<Arrows>",                 N_("change day"),
    "<Alt+Arrows>",             N_("change day while note editing"),
    "<Ctrl+Up/Down>",           N_("scroll the contents in the day info panel"),
    "<PageUp/PageDn>",          N_("change month"),
    "<Home/End>",               N_("change year"),
    "<c>",                      N_("assign background color to day note"),
    "<d>",                      N_("date calculator"),
    "<g>",                      N_("jump to date"),
    "<f>",                      N_("show full-year calendar"),
    "<a>",                      N_("toggle calendars for the previous and next month"),
    "<Delete>",                 N_("remove day note"),
    "<Esc>",                    N_("close day note panel")
};

const gchar *tasks_keys_text[] = {
    "<Alt+a>, <Insert>",        N_("add task"),
    "<Alt+e>, <Ctrl+Enter>",    N_("edit task"),
    "<Alt+r>, <Delete>",        N_("remove task"),
    "<Ctrl+h>",                 N_("toggle hidden tasks"),
    "<Left>, <Right>",          N_("change category filter"),
    "<Esc>",                    N_("close task info panel")
};

const gchar *contacts_keys_text[] = {
    "<Insert>",                 N_("add contact"),
    "<Ctrl+Enter>",             N_("edit contact"),
    "<Delete>",                 N_("remove contact"),
    "<Ctrl+l>",                 N_("activate search field"),
    "<F1/F2/F3>",               N_("change search mode"),
    "<Esc>",                    N_("close contact details panel")
};

#ifdef HAVE_LIBGRINGOTTS

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

#endif  /* HAVE_LIBGRINGOTTS */

gchar tmpbuf[BUFFER_SIZE];
gint i;

    gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);

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

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Tasks"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(tasks_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", tasks_keys_text[i*2], gettext(tasks_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Contacts"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    for (i=0; i < G_N_ELEMENTS(contacts_keys_text)/2; i++) {
        sprintf(tmpbuf, "\t%s - %s\n", contacts_keys_text[i*2], gettext(contacts_keys_text[i*2+1]));
        gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

#ifdef HAVE_LIBGRINGOTTS
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
#endif  /* HAVE_LIBGRINGOTTS */

    gtk_widget_grab_focus(GTK_WIDGET(appGUI->scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
display_about(GUI *appGUI) {

gchar tmpbuf[BUFFER_SIZE];

    gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);

    gui_clear_text_buffer (appGUI->about_entry_buffer, &appGUI->about_entry_iter);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);
    sprintf(tmpbuf, "%s\n", _("A handy personal organizer"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "big", "center", NULL);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "http://clay.ll.pl/osmo", TRUE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, "\n", -1);

    sprintf(tmpbuf, "\n(%s %s, %s)\n\n", _("compiled on"), __DATE__, __TIME__);
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "center", "italic", NULL);

    sprintf(tmpbuf, "\n %s:\n", _("Programming"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     Tomasz Mąka <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "pasp@users.sourceforge.net", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    sprintf(tmpbuf, "\n %s:\n", _("Graphics"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     Maja Kocoń (", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "http://ironya.ll.pl", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ")\n", -1);

    sprintf(tmpbuf, "\n %s:\n", _("Contributors"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     Piotr Mąka <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "silloz@users.sourceforge.net", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    sprintf(tmpbuf, "\n %s:\n", _("Translators"));
    gtk_text_buffer_insert_with_tags_by_name (appGUI->about_entry_buffer, 
                                              &appGUI->about_entry_iter, tmpbuf, -1, "bold", NULL);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [cs]\tDragonlord <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "dragonlord@seznam.cz", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [de]\tMarkus Dahms <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "mad@automagically.de", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [es]\tNacho Alonso González <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "nacho.alonso.gonzalez@gmail.com", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [fr]\tJean-Jacques Moulinier <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "postmaster@moulinier.net", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [lt]\tVaidotas Kazla <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "joshas@gmail.com", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [nl]\tTiger!P <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "tigerp@tigerp.net", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [pl]\tPiotr Mąka <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "silloz@users.sourceforge.net", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, 
                            "     [pt]\tBruno Miguel <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "brunoalexandremiguel@gmail.com", FALSE, appGUI);
    gtk_text_buffer_insert (appGUI->about_entry_buffer, &appGUI->about_entry_iter, ">\n", -1);

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
    sprintf(tmpbuf, "     [+]\t%s (libgringotts)\n", _("Encrypted notes"));
#else
    sprintf(tmpbuf, "     [-]\t%s (libgringotts)\n", _("Encrypted notes"));
#endif  /* HAVE_LIBGRINGOTTS */
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

GtkWidget *vbox1;
GtkWidget *label;
GtkWidget *hseparator;
GtkWidget *logo_area;
GdkPixbuf *logo;
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

#ifndef REV
    sprintf(tmpbuf, "%s %s", _("version"), VERSION);
#else
    sprintf(tmpbuf, "%s %d", _("SVN revision"), REV);
#endif

    label = gtk_label_new(tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (vbox1), label, FALSE, TRUE, 4);

    gtk_image_set_from_pixbuf (GTK_IMAGE (logo_area), logo);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    appGUI->about_radiobutton = gui_stock_label_radio_button(_("About"), 
                                                             OSMO_STOCK_ABOUT, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->about_radiobutton);

    gtk_button_set_relief (GTK_BUTTON (appGUI->about_radiobutton), GTK_RELIEF_NONE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->about_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->about_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->about_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->about_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->about_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->about_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->about_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

    appGUI->help_radiobutton = gui_stock_label_radio_button(_("Key shortcuts"), 
                                                            OSMO_STOCK_HELP, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->help_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->help_radiobutton), GTK_RELIEF_NONE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->help_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->help_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->help_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->help_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->help_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->help_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

    appGUI->license_radiobutton = gui_stock_label_radio_button(_("License"), 
                                                               OSMO_STOCK_LICENSE, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->license_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->license_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->license_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->license_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->license_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->license_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->license_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->license_radiobutton), "released",
                      G_CALLBACK(button_released_cb), appGUI);

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
    gtk_text_view_set_editable(GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->about_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->about_textview), 2);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->about_textview), 6);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->about_textview), 6);
    gtk_widget_show (appGUI->about_textview);
    gtk_container_add (GTK_CONTAINER (viewport), appGUI->about_textview);

    gui_url_setup(&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview, appGUI);

    display_about(appGUI);
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

