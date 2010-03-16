
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

#include "i18n.h"
#include "gui.h"
#include "gui_utils.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_fullyear.h"
#include "calendar_widget.h"
#include "calendar_jumpto.h"
#include "calendar_calc.h"
#include "tasks.h"
#include "tasks_items.h"
#include "contacts.h"
#include "contacts_items.h"
#include "options_gui.h"
#include "options_prefs.h"
#include "stock_icons.h"
#include "config.h"

#include "gui_logo.h"
#include "gui_icon.h"

/*------------------------------------------------------------------------------*/

void
gui_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->calendar_only == FALSE) {
        if (appGUI->current_tab == PAGE_CALENDAR || 
            appGUI->current_tab == PAGE_TASKS || appGUI->current_tab == PAGE_CONTACTS) {
            config.latest_tab = appGUI->current_tab;
        }

        config.find_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox));

        gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);

        gtk_window_get_size (GTK_WINDOW(widget),
                            &config.window_size_x, &config.window_size_y);
    }

    gdk_window_get_root_origin (GDK_WINDOW(widget->window),
                        &config.window_x, &config.window_y);

    pango_font_description_free(appGUI->fd_day_name);
    pango_font_description_free(appGUI->fd_calendar);
    pango_font_description_free(appGUI->fd_notes);

    if (appGUI->calendar_only == FALSE) {
        write_notes (appGUI);
        write_tasks_entries(appGUI);
        write_contacts_entries(appGUI);
    }

    gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);
    gui_url_remove_links (&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index);

    gtk_main_quit ();
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
                    calendar_btn_prev_week(appGUI);
                    return TRUE;
                case GDK_Down:
                    calendar_btn_next_week(appGUI);
                    return TRUE;
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
                case GDK_h:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + h */
                        config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton));
                        config.hide_completed = !config.hide_completed;
                        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton), config.hide_completed);
                        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
                        update_n_items(appGUI);
                    }
                    return TRUE;
                case GDK_Delete:
                    tasks_remove_dialog_show(appGUI->tsk->tasks_list, appGUI->tsk->tasks_list_store, appGUI);
                    return TRUE;
                case GDK_Insert:
                    tasks_add_edit_dialog_show (FALSE, appGUI);
                    return TRUE;
                case GDK_space:
                    return TRUE;
                case GDK_Left:
                    if (appGUI->tsk->filter_index > 0) {
                        appGUI->tsk->filter_index--;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->cf_combobox), appGUI->tsk->filter_index);
                    }
                    return TRUE;
                case GDK_Right:
                    if (appGUI->tsk->filter_index < get_combobox_items(GTK_COMBO_BOX(appGUI->tsk->cf_combobox))-1) {
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
                case GDK_1:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 1 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_FIRST_NAME);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_2:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 2 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_LAST_NAME);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_3:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 3 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cnt->contacts_find_combobox), CONTACTS_FF_ALL_FIELDS);
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
                    gui_window_close_cb(appGUI->main_window, NULL, appGUI);
            }
            return FALSE;

        case GDK_Page_Up:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Up */
                appGUI->current_tab--;
                appGUI->current_tab = appGUI->current_tab < 0 ? appGUI->number_of_tabs-1:appGUI->current_tab;
                gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), appGUI->current_tab);
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
            return FALSE;

        case GDK_Page_Down:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Down */
                appGUI->current_tab++;
                appGUI->current_tab = appGUI->current_tab == appGUI->number_of_tabs ? 0:appGUI->current_tab;
                gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), appGUI->current_tab);
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
            return FALSE;
        case GDK_F1:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_CALENDAR);
            return TRUE;
        case GDK_F2:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_TASKS);
            return TRUE;
        case GDK_F3:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_CONTACTS);
            return TRUE;
        case GDK_F4:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_OPTIONS);
            return TRUE;
        case GDK_F5:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_ABOUT);
            return TRUE;

    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
notebook_sw_cb (GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer user_data) {
 
    GUI *appGUI = (GUI *)user_data;

    appGUI->current_tab = page_num;

    if(appGUI->current_tab == PAGE_TASKS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->tsk->tasks_list));
    }

    if(appGUI->current_tab == PAGE_CONTACTS) {
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cnt->contacts_find_entry));
    }
}

/*------------------------------------------------------------------------------*/

void gui_create_window(GUI *appGUI) {

GdkPixbuf   *icon;
gchar tmpbuf[BUFFER_SIZE];

    appGUI->cnt->contacts_filter_disabled = TRUE;
    appGUI->all_pages_added = FALSE;

    appGUI->fd_day_name = pango_font_description_from_string(config.day_name_font);
    appGUI->fd_calendar = pango_font_description_from_string(config.calendar_font);
    appGUI->fd_notes = pango_font_description_from_string(config.notes_font);

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

    osmo_register_stock_icons();

    appGUI->osmo_tooltips = gtk_tooltips_new();

    appGUI->notebook = gtk_notebook_new();
    GTK_WIDGET_UNSET_FLAGS(appGUI->notebook, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->notebook);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(appGUI->notebook), GTK_POS_RIGHT);
    gtk_container_add(GTK_CONTAINER(appGUI->main_window), appGUI->notebook);
    g_signal_connect(G_OBJECT(appGUI->notebook), "switch-page", 
                     G_CALLBACK(notebook_sw_cb), appGUI);

    if (appGUI->calendar_only == TRUE) {
        gtk_notebook_set_show_tabs (GTK_NOTEBOOK(appGUI->notebook), FALSE);
    }

    gui_create_calendar(appGUI->notebook, appGUI);

    if (appGUI->calendar_only == FALSE) {
        gui_create_tasks(appGUI);
        gui_create_contacts(appGUI);
        gui_create_options(appGUI);
        gui_create_about(appGUI);

        read_notes (appGUI);

        read_tasks_entries (appGUI);
        apply_date_colors(appGUI);

        read_contacts_entries (appGUI);
        set_export_active(appGUI);

        create_category_combobox (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), appGUI->opt->tasks_category_store, FALSE);
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
    gtk_widget_show(appGUI->main_window);

    appGUI->all_pages_added = TRUE;
    set_today (TRUE, appGUI);
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
    "<F1>",                     N_("switch to calendar tab"),
    "<F2>",                     N_("switch to tasks tab"),
    "<F3>",                     N_("switch to options tab"),
    "<F4>",                     N_("switch to about tab"),
    "<Ctrl+PageUp>",            N_("switch to previous tab"),
    "<Ctrl+PageDn>",            N_("switch to next tab"),
    "<PageUp/PageDn>",          N_("switch page in options and about tab"),
    "<Ctrl+q>",                 N_("exit")
};

const gchar *calendar_keys_text[] = {
    "<Space>",                  N_("select current date"),
    "<Arrows>",                 N_("change day"),
    "<Alt+Arrows>",             N_("change day while note editing"),
    "<PageUp/PageDn>",          N_("change month"),
    "<Home/End>",               N_("change year"),
    "<c>",                      N_("assign background color to day note"),
    "<d>",                      N_("date calculator"),
    "<g>",                      N_("jump to date"),
    "<f>",                      N_("show full-year calendar"),
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
    "<Alt+1/2/3>",              N_("change search mode"),
    "<Esc>",                    N_("close contact details panel")
};

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
                            "     [fr]\tJean-Jacques Moulinier <", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, 
                        appGUI->about_textview, &appGUI->about_entry_iter, NULL, 
                        0, "postmaster@moulinier.net", FALSE, appGUI);
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

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    gtk_label_set_markup (GTK_LABEL (label), _("About"));

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), vbox1, label);

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


