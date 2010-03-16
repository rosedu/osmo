
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

extern gboolean         calendar_only;
extern guint            day, month, year;
extern GtkWidget        *notes_button;
extern GtkWidget        *calendar;

extern gboolean         tasks_panel_status;
extern GtkListStore     *tasks_list_store;
extern gint             filter_index;
extern GtkWidget        *cf_combobox;
extern GtkWidget        *tasks_list;
extern GtkWidget        *ct_hide_items_checkbutton;
extern GtkTreeModel     *tasks_filter;
extern GtkTreeSelection *tasks_list_selection; 
extern GtkListStore     *tasks_category_store;

extern GtkWidget        *contacts_find_entry;
extern gboolean         contacts_panel_status;
extern GtkWidget        *contacts_find_combobox;
extern GtkWidget        *contacts_paned;
extern GtkWidget        *contacts_panel_scrolledwindow;
extern gboolean         contacts_filter_disabled;
extern GtkWidget        *contacts_list;
extern GtkListStore     *contacts_list_store;
extern GtkTreeSelection *contacts_list_selection; 
extern GSList           contacts_links_list;
extern gint             contacts_link_index;

extern  const guint8    osmo_stock_about[];
extern  const guint8    osmo_stock_help[];
extern  const guint8    osmo_stock_license[];

GtkWidget               *main_window;
GtkTooltips             *osmo_tooltips; 
GtkWidget               *notebook;
gint                    current_tab;
gint                    number_of_tabs;

PangoFontDescription    *fd_day_name;
PangoFontDescription    *fd_calendar;
PangoFontDescription    *fd_notes;

GtkTextIter             entry_iter;
GtkTextBuffer           *entry_buffer = NULL;

GtkWidget               *about_radiobutton;
GtkWidget               *license_radiobutton;
GtkWidget               *help_radiobutton;
GtkWidget               *scrolled_window;
gint                    about_counter = 0;
GSList                  about_links_list;
gint                    about_link_index;
GtkWidget               *about_textview;

gchar                   tmpbuf[BUFFER_SIZE];
gboolean                all_pages_added;

/*------------------------------------------------------------------------------*/

void
gui_window_close_cb (GtkWidget *widget, gpointer data) {

    gui_url_remove_links (&about_links_list, &about_link_index);
    gui_url_remove_links (&contacts_links_list, &contacts_link_index);

    if (calendar_only == FALSE) {
        if (current_tab == PAGE_CALENDAR || current_tab == PAGE_TASKS || current_tab == PAGE_CONTACTS) {
            config.latest_tab = current_tab;
        }

        config.find_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(contacts_find_combobox));

        gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(notes_button), FALSE);

        gtk_window_get_size (GTK_WINDOW(main_window),
                            &config.window_size_x, &config.window_size_y);
    }

    gdk_window_get_root_origin (main_window->window,
                        &config.window_x, &config.window_y);

    pango_font_description_free(fd_day_name);
    pango_font_description_free(fd_calendar);
    pango_font_description_free(fd_notes);

    if (calendar_only == FALSE) {
        calendar_update_note (day, month, year);
        write_notes ();
        write_tasks_entries();
        write_tasks_entries();
        write_contacts_entries();
    }

    gtk_main_quit ();
}

/*------------------------------------------------------------------------------*/

gint 
key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gint page;

    page = gtk_notebook_get_current_page (GTK_NOTEBOOK(notebook));

    /************************************************************************/
    /*** CALENDAR PAGE                                                    ***/
    /************************************************************************/

    if(page == PAGE_CALENDAR) {

        if (!config.day_notes_visible) {

            switch (event->keyval) {
                case GDK_Left:
                    calendar_btn_prev_day();
                    return TRUE;
                case GDK_Right:
                    calendar_btn_next_day();
                    return TRUE;
                case GDK_Up:
                    calendar_btn_prev_week();
                    return TRUE;
                case GDK_Down:
                    calendar_btn_next_week();
                    return TRUE;
                case GDK_Home:
                    calendar_btn_prev_year();
                    return TRUE;
                case GDK_End:
                    calendar_btn_next_year();
                    return TRUE;
                case GDK_Return:
                    if (calendar_only == FALSE) {
                        gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(notes_button), 
                                                     !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(notes_button)));
                    }
                    return TRUE;
                case GDK_space:
                    set_today (TRUE);
                    return TRUE;
                case GDK_Delete:
                    if (calendar_only == FALSE) {
                        calendar_clear_text_cb (NULL, NULL);
                    }
                    return TRUE;
                case GDK_g:
                    calendar_create_jumpto_window ();
                    return TRUE;
                case GDK_f:
                    calendar_create_fullyear_window ();
                    return TRUE;
            }

        }

        switch (event->keyval) {

            case GDK_Escape:
                if (calendar_only == FALSE) {
                    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(notes_button), FALSE);
                }
                return TRUE;
            case GDK_Left:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Left */
                    calendar_btn_prev_day();
                    return TRUE;
                }
                return FALSE;
            case GDK_Right:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Right */
                    calendar_btn_next_day();
                    return TRUE;
                }
                return FALSE;
            case GDK_Up:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Up */
                    calendar_btn_prev_week();
                    return TRUE;
                }
                return FALSE;
            case GDK_Down:
                if (event->state & GDK_MOD1_MASK) {  /* ALT + Down */
                    calendar_btn_next_week();
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
                    if(tasks_panel_status == TRUE) {
                        show_tasks_desc_panel(FALSE);
                    }
                    return TRUE;
                case GDK_Return:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                        if (gtk_tree_selection_get_selected (tasks_list_selection, NULL, NULL)) {
                            tasks_add_edit_dialog_show (TRUE);
                            return TRUE;
                        }
                    }
                    if(tasks_panel_status == FALSE) {
                        show_tasks_desc_panel(TRUE);
                    }
                    return TRUE;
                case GDK_h:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + h */
                        config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton));
                        config.hide_completed = !config.hide_completed;
                        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton), config.hide_completed);
                        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(tasks_filter));
                    }
                    return TRUE;
                case GDK_Delete:
                    tasks_remove_dialog_show(tasks_list, tasks_list_store);
                    return TRUE;
                case GDK_Insert:
                    tasks_add_edit_dialog_show (FALSE);
                    return TRUE;
                case GDK_space:
                    return TRUE;
                case GDK_Left:
                    if (filter_index > 0) {
                        filter_index--;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(cf_combobox), filter_index);
                    }
                    return TRUE;
                case GDK_Right:
                    if (filter_index < get_combobox_items(GTK_COMBO_BOX(cf_combobox))-1) {
                        filter_index++;
                        gtk_combo_box_set_active(GTK_COMBO_BOX(cf_combobox), filter_index);
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
                    if (gtk_widget_is_focus(contacts_find_entry) == FALSE) {
                        if(contacts_panel_status == TRUE) {
                            show_contacts_desc_panel(FALSE);
                        }
                    } else {
                        if (strlen(gtk_entry_get_text(GTK_ENTRY(contacts_find_entry)))) {
                            gtk_entry_set_text(GTK_ENTRY(contacts_find_entry), "");
                        } 
                    }
                    return TRUE;
                case GDK_Return:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Enter */
                        if (gtk_tree_selection_get_selected (contacts_list_selection, NULL, NULL)) {
                            contacts_add_edit_dialog_show (TRUE);
                            return TRUE;
                        }
                    }
                    if(contacts_panel_status == FALSE) {
                        show_contacts_desc_panel(TRUE);
                        return TRUE;
                    } else if (gtk_widget_is_focus(contacts_find_entry) == FALSE) {
                        gui_scrolled_window_move_position(contacts_panel_scrolledwindow, SW_MOVE_DOWN);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_BackSpace:
                    if(contacts_panel_status == TRUE && gtk_widget_is_focus(contacts_find_entry) == FALSE) {
                        gui_scrolled_window_move_position(contacts_panel_scrolledwindow, SW_MOVE_UP);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Delete:
                    if(gtk_widget_is_focus(contacts_find_entry) == FALSE) {
                        contacts_remove_dialog_show(contacts_list, contacts_list_store);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_Insert:
                    if(gtk_widget_is_focus(contacts_find_entry) == FALSE) {
                        contacts_add_edit_dialog_show (FALSE);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_1:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 1 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(contacts_find_combobox), CONTACTS_FF_FIRST_NAME);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_2:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 2 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(contacts_find_combobox), CONTACTS_FF_LAST_NAME);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_3:
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + 3 */
                        gtk_combo_box_set_active(GTK_COMBO_BOX(contacts_find_combobox), CONTACTS_FF_ALL_FIELDS);
                        return TRUE;
                    }
                    return FALSE;
                case GDK_l:
                    if (event->state & GDK_CONTROL_MASK) {  /* CTRL + l */
                        gtk_widget_grab_focus(GTK_WIDGET(contacts_find_entry));
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
                        options_switch_buttons(FALSE);
                        return TRUE;

                    case GDK_Page_Up:
                        options_switch_buttons(TRUE);
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
                        about_switch_buttons(FALSE);
                        return TRUE;

                    case GDK_Page_Up:
                        about_switch_buttons(TRUE);
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
                    gui_window_close_cb(NULL, NULL);
            }
            return FALSE;    

        case GDK_Page_Up:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Up */
                current_tab--;
                current_tab = current_tab < 0 ? number_of_tabs-1:current_tab;
                gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), current_tab);
                return TRUE;
            } else if (page == PAGE_CALENDAR) {
                if (config.day_notes_visible) {
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + Page_Up */
                        calendar_btn_prev_month();
                        return TRUE;
                    }
                } else {
                    calendar_btn_prev_month();
                    return TRUE;
                }
            }
            return FALSE;

        case GDK_Page_Down:
            if (event->state & GDK_CONTROL_MASK) {  /* CTRL + Page_Down */
                current_tab++;
                current_tab = current_tab == number_of_tabs ? 0:current_tab;
                gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), current_tab);
                return TRUE;
            } else if (page == PAGE_CALENDAR) {
                if (config.day_notes_visible) {
                    if (event->state & GDK_MOD1_MASK) {  /* ALT + Page_Down */
                        calendar_btn_next_month();
                        return TRUE;
                    }
                } else {
                    calendar_btn_next_month();
                    return TRUE;
                }
            }
            return FALSE;
        case GDK_F1:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), PAGE_CALENDAR);
            return TRUE;
        case GDK_F2:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), PAGE_TASKS);
            return TRUE;
        case GDK_F3:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), PAGE_CONTACTS);
            return TRUE;
        case GDK_F4:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), PAGE_OPTIONS);
            return TRUE;
        case GDK_F5:
            gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), PAGE_ABOUT);
            return TRUE;

    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
notebook_sw_cb (GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer user_data) {
 
    current_tab = page_num;

    if(current_tab == PAGE_TASKS) {
        gtk_widget_grab_focus(GTK_WIDGET(tasks_list));
    }

    if(current_tab == PAGE_CONTACTS) {
        gtk_widget_grab_focus(GTK_WIDGET(contacts_find_entry));
    }
}

/*------------------------------------------------------------------------------*/

void gui_create_window(void) {

GdkPixbuf   *icon;

    contacts_filter_disabled = TRUE;
    all_pages_added = FALSE;

    fd_day_name = pango_font_description_from_string(config.day_name_font);
    fd_calendar = pango_font_description_from_string(config.calendar_font);
    fd_notes = pango_font_description_from_string(config.notes_font);

    main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    sprintf(tmpbuf, "OSMO - %s", _("a handy personal organizer"));
    gtk_window_set_title (GTK_WINDOW (main_window), tmpbuf);

    icon = gdk_pixbuf_new_from_inline(-1, osmo_icon, FALSE, NULL);
    gtk_window_set_icon (GTK_WINDOW(main_window), icon);
    g_object_unref(icon);

    gtk_window_move (GTK_WINDOW (main_window), config.window_x, config.window_y);
    if (calendar_only == FALSE) {
        gtk_window_set_default_size (GTK_WINDOW(main_window), config.window_size_x, config.window_size_y);
    } else {
        gtk_window_set_default_size (GTK_WINDOW(main_window), 500, -1);
    }
    gtk_window_set_resizable (GTK_WINDOW (main_window), TRUE);

    g_signal_connect (G_OBJECT (main_window), "delete_event",
                      G_CALLBACK(gui_window_close_cb), NULL);
    g_signal_connect (G_OBJECT (main_window), "key_press_event",
                      G_CALLBACK (key_press_cb), NULL);

    osmo_register_stock_icons();

    osmo_tooltips = gtk_tooltips_new();

    notebook = gtk_notebook_new();
    GTK_WIDGET_UNSET_FLAGS(notebook, GTK_CAN_FOCUS);
    gtk_widget_show (notebook);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_RIGHT);
    gtk_container_add(GTK_CONTAINER(main_window), notebook);
    g_signal_connect(notebook, "switch-page", 
                     G_CALLBACK(notebook_sw_cb), NULL);

    if (calendar_only == FALSE) {
        read_notes ();
    } else {
        gtk_notebook_set_show_tabs (GTK_NOTEBOOK(notebook), FALSE);
    }

    gui_create_calendar(notebook);

    if (calendar_only == FALSE) {
        gui_create_tasks(notebook);
        gui_create_contacts(notebook);
        gui_create_options(notebook);
        gui_create_about(notebook);

        read_tasks_entries ();
        apply_date_colors();

        read_contacts_entries ();
        set_export_active();

        create_category_combobox (GTK_COMBO_BOX (cf_combobox), tasks_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX(cf_combobox), 0);

        contacts_filter_disabled = FALSE;

        number_of_tabs = NUMBER_OF_TABS;

        if(config.remember_latest_tab == TRUE) {
            current_tab = config.latest_tab;
        } else {
            current_tab = PAGE_CALENDAR;
        }
    } else {
        number_of_tabs = 1;
        current_tab = 0;
    }

    gui_url_initialize();

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), current_tab);
    gtk_widget_show(main_window);

    all_pages_added = TRUE;
    set_today (TRUE);
}

/*------------------------------------------------------------------------------*/

void
display_license(void) {

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

    gui_clear_text_buffer (entry_buffer, &entry_iter);
    gtk_text_buffer_insert (entry_buffer, &entry_iter, license_text, -1);
    gtk_widget_grab_focus(GTK_WIDGET(scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
display_help(void) {

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

gint i;

    gui_clear_text_buffer (entry_buffer, &entry_iter);

    snprintf(tmpbuf, BUFFER_SIZE-1, "\n%s\n\n", _("OSMO was designed keeping in mind the user convenience, so there are many key shorctuts. Here is the full list:"));
    gtk_text_buffer_insert (entry_buffer, &entry_iter, tmpbuf, -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("General"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(general_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", general_keys_text[i*2], gettext(general_keys_text[i*2+1]));
        gtk_text_buffer_insert (entry_buffer, &entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "\n", -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Calendar"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);
 
    for (i=0; i < G_N_ELEMENTS(calendar_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", calendar_keys_text[i*2], gettext(calendar_keys_text[i*2+1]));
        gtk_text_buffer_insert (entry_buffer, &entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "\n", -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Tasks"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(tasks_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", tasks_keys_text[i*2], gettext(tasks_keys_text[i*2+1]));
        gtk_text_buffer_insert (entry_buffer, &entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "\n", -1);

    snprintf(tmpbuf, BUFFER_SIZE-1, "* %s\n", _("Contacts"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     

    for (i=0; i < G_N_ELEMENTS(contacts_keys_text)/2; i++) {    
        sprintf(tmpbuf, "\t%s - %s\n", contacts_keys_text[i*2], gettext(contacts_keys_text[i*2+1]));
        gtk_text_buffer_insert (entry_buffer, &entry_iter, tmpbuf, -1);
    }
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "\n", -1);

    gtk_widget_grab_focus(GTK_WIDGET(scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
display_about(void) {

    gui_url_remove_links (&about_links_list, &about_link_index);

    gui_clear_text_buffer (entry_buffer, &entry_iter);

    sprintf(tmpbuf, "\n%s\n", _("A handy personal organizer"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "big", "center", NULL);

    sprintf(tmpbuf, "\n(%s %s, %s)\n\n", _("compiled on"), __DATE__, __TIME__);
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "center", "italic", NULL);

    sprintf(tmpbuf, "\n %s: ", _("Homepage"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     

    gui_url_insert_link(&about_links_list, &about_link_index, about_textview, &entry_iter, NULL, 0, "http://clay.ll.pl/osmo");
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "\n\n", -1);

    sprintf(tmpbuf, "\n %s:\n", _("Programming"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "     Tomasz Mąka <", -1);
    gui_url_insert_link(&about_links_list, &about_link_index, about_textview, &entry_iter, NULL, 0, "pasp@users.sourceforge.net");
    gtk_text_buffer_insert (entry_buffer, &entry_iter, ">\n", -1);
    
    sprintf(tmpbuf, "\n %s:\n", _("Graphics"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &entry_iter, tmpbuf, -1, "bold", NULL);     
    gtk_text_buffer_insert (entry_buffer, &entry_iter, "     Maja Kocoń (", -1);
    gui_url_insert_link(&about_links_list, &about_link_index, about_textview, &entry_iter, NULL, 0, "http://ironya.ll.pl");
    gtk_text_buffer_insert (entry_buffer, &entry_iter, ")\n", -1);

    gtk_widget_grab_focus(GTK_WIDGET(scrolled_window));
}

/*------------------------------------------------------------------------------*/

void
about_switch_buttons(gboolean left) {

GtkWidget *buttons_table[] = { 
    about_radiobutton, 
    help_radiobutton,
    license_radiobutton
};

    if (left == TRUE && about_counter > 0) {
        --about_counter;
    } else if (left == FALSE && about_counter < 2) {
        about_counter++;
    }

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(buttons_table[about_counter]), TRUE);
    g_signal_emit_by_name(G_OBJECT(buttons_table[about_counter]), "released");
}

/*------------------------------------------------------------------------------*/

void
about_set_first_page(void) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(about_radiobutton), TRUE);
    g_signal_emit_by_name(G_OBJECT(about_radiobutton), "released");
}

/*------------------------------------------------------------------------------*/

void
button_released_cb (GtkButton *button, gpointer user_data) {

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(about_radiobutton)) == TRUE) {

        display_about();
        about_counter = 0;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(help_radiobutton)) == TRUE) {

        display_help();
        about_counter = 1;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(license_radiobutton)) == TRUE) {

        display_license();
        about_counter = 2;

    }
}

/*------------------------------------------------------------------------------*/

void 
gui_create_about(GtkWidget *notebook) {

GtkWidget *vbox1;
GtkWidget *label;
GtkWidget *hseparator;
GtkWidget *logo_area;
GdkPixbuf *logo;
GtkWidget *viewport;
GtkWidget *hbuttonbox;
GSList    *radiobutton_group = NULL;

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    gtk_label_set_markup (GTK_LABEL (label), _("About"));
 
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox1, label);

    logo_area = gtk_image_new();
    logo = gdk_pixbuf_new_from_inline(-1, osmo_logo, FALSE, NULL);
    gtk_widget_show(logo_area);
    gtk_box_pack_start (GTK_BOX (vbox1), logo_area, FALSE, TRUE, 0);

    sprintf(tmpbuf, "%s %s", _("version"), VERSION);
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

    about_radiobutton = gui_image_label_radio_button(_("About"), osmo_stock_about);
    gtk_widget_show (about_radiobutton);

    gtk_button_set_relief (GTK_BUTTON (about_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(about_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (about_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (about_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (about_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (about_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), about_radiobutton);

    g_signal_connect (G_OBJECT (about_radiobutton), "released",
                      G_CALLBACK(button_released_cb), NULL);

    help_radiobutton = gui_image_label_radio_button(_("Key shortcuts"), osmo_stock_help);
    gtk_widget_show (help_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (help_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(help_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (help_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (help_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (help_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), help_radiobutton);

    g_signal_connect (G_OBJECT (help_radiobutton), "released",
                      G_CALLBACK(button_released_cb), NULL);

    license_radiobutton = gui_image_label_radio_button(_("License"), osmo_stock_license);
    gtk_widget_show (license_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (license_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(license_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (license_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (license_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (license_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), license_radiobutton);

    g_signal_connect (G_OBJECT (license_radiobutton), "released",
                      G_CALLBACK(button_released_cb), NULL);

    /*--------------------------------------------------------------------------*/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 0);
    gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);

    entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &entry_iter, 0);
    gtk_text_buffer_create_tag (entry_buffer, "bold", "weight", PANGO_WEIGHT_ULTRABOLD, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "big", "size", 16 * PANGO_SCALE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "center", "justification", GTK_JUSTIFY_CENTER, NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &entry_iter, 0);

    about_textview = gtk_text_view_new_with_buffer (entry_buffer);
    gtk_container_set_border_width (GTK_CONTAINER (about_textview), 1);
    gtk_text_view_set_editable(GTK_TEXT_VIEW (about_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW (about_textview), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (about_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(about_textview), 2);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(about_textview), 6);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(about_textview), 6);
    gtk_widget_show (about_textview);
    gtk_container_add (GTK_CONTAINER (viewport), about_textview);

    gui_url_setup(&about_links_list, &about_link_index, about_textview);

    display_about();
}

/*------------------------------------------------------------------------------*/


