
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
#include <glib/gprintf.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <glib.h>
#include <limits.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "gui.h"
#include "gui_utils.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "options_gui.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"


struct  osmo_prefs              config;

extern  gchar                   tmpbuf[];
extern  GtkWidget               *main_window;
extern  GtkTooltips             *osmo_tooltips;
extern  PangoFontDescription    *fd_day_name;
extern  PangoFontDescription    *fd_calendar;
extern  PangoFontDescription    *fd_notes;

extern  GtkWidget               *calendar;
extern  GtkWidget               *textview;
extern  GtkWidget               *date_label;
extern  GtkToolbar              *calendar_toolbar;

extern  GtkWidget               *tasks_list;
extern  GtkTreeModel            *tasks_filter;
extern  GtkTreeViewColumn       *tasks_columns[];
extern  GtkWidget               *tasks_desc_textview;
extern  GtkWidget               *cf_combobox;
extern  GtkTreeModel            *tasks_sort;
extern  GtkToolbar              *tasks_toolbar;

extern  GtkWidget               *contacts_list;
extern  GtkToolbar              *contacts_toolbar;
extern  GtkTreeModel            *contacts_sort;
extern  GtkTreeModel            *contacts_filter;
extern  GtkTreeViewColumn       *contacts_columns[];
extern  GtkTextTag              *font_tag_object;
extern  GtkTextTag              *contact_name_tag_object;
extern  GtkTextTag              *contact_tags_value_tag_object;
extern  GtkTextTag              *contact_color_tag_object;
extern  GtkTreeSelection        *contacts_list_selection; 
extern  GtkTextTag              *gui_url_tag;

extern  const guint8            osmo_stock_tasks[];
extern  const guint8            osmo_stock_calendar[];
extern  const guint8            osmo_stock_contacts[];
extern  const guint8            osmo_stock_preferences[];

GtkWidget                       *calendar_options_radiobutton;
GtkWidget                       *tasks_options_radiobutton;
GtkWidget                       *contacts_options_radiobutton;
GtkWidget                       *general_options_radiobutton;

GtkWidget                       *show_day_names_checkbutton;
GtkWidget                       *no_month_change_checkbutton;
GtkWidget                       *show_week_numbers_checkbutton;
GtkWidget                       *week_start_monday_checkbutton;
GtkWidget                       *simple_view_in_fy_calendar_checkbutton;
GtkWidget                       *show_notes_in_info_panel_checkbutton;
GtkWidget                       *enable_tooltips_checkbutton;
GtkWidget                       *disable_underline_in_links_checkbutton;
GtkWidget                       *default_stock_icons_checkbutton;
GtkWidget                       *remember_latest_tab_checkbutton;
GtkWidget                       *ct_delete_items_checkbutton;
GtkWidget                       *ct_hide_items_checkbutton;
GtkWidget                       *tasks_enable_rules_hint_checkbutton;
GtkWidget                       *contacts_enable_rules_hint_checkbutton;
GtkWidget                       *contacts_select_first_entry_checkbutton;
GtkWidget                       *contacts_hide_group_column_checkbutton;
GtkWidget                       *vc_due_date_checkbutton;
GtkWidget                       *vc_category_checkbutton;
GtkWidget                       *vc_priority_checkbutton;

GtkWidget                       *day_name_font_entry;
GtkWidget                       *calendar_font_entry;
GtkWidget                       *notes_font_entry;
GtkWidget                       *ti_font_entry;

GtkWidget                       *options_notebook;
GtkWidget                       *calendar_vbox;
GtkWidget                       *tasks_vbox;
GtkWidget                       *contacts_vbox;
GtkWidget                       *general_vbox;

GtkWidget                       *mark_symbol_combobox;
GdkColor                        color;
GtkWidget                       *header_color_picker;
GtkWidget                       *weekend_color_picker;
GtkWidget                       *selection_color_picker;
gboolean                        font_modified;
gint                            options_counter = 0;
GtkWidget                       *tl_start_h_spinbutton;
GtkWidget                       *tl_start_m_spinbutton;
GtkWidget                       *tl_end_h_spinbutton;
GtkWidget                       *tl_end_m_spinbutton;
GtkWidget                       *tl_step_spinbutton;

GtkWidget                       *due_today_color_picker;
GtkWidget                       *due_7days_color_picker;
GtkWidget                       *past_due_color_picker;
GtkWidget                       *tasks_category_entry;
GtkListStore                    *tasks_category_store = NULL;
GtkWidget                       *tasks_category_treeview;
GtkTreeSelection                *tasks_category_select;
GtkWidget                       *tasks_category_add_button;
GtkWidget                       *tasks_category_remove_button;

GtkWidget                       *tasks_sort_order_combobox;
GtkWidget                       *tasks_sort_mode_combobox;

GtkWidget                       *contacts_group_entry;
GtkListStore                    *contacts_group_store = NULL;
GtkWidget                       *contacts_group_treeview;
GtkTreeSelection                *contacts_group_select;
GtkWidget                       *contacts_group_add_button;
GtkWidget                       *contacts_group_remove_button;

GtkWidget                       *contacts_sort_order_combobox;
GtkWidget                       *contacts_sort_mode_combobox;
GtkWidget                       *contacts_photo_size_combobox;
GtkObject                       *name_font_size_spinbutton_adj;
GtkWidget                       *name_font_size_spinbutton;
GtkObject                       *tags_font_size_spinbutton_adj;
GtkWidget                       *tags_font_size_spinbutton;

GtkWidget                       *contact_tag_color_picker;
GtkWidget                       *contact_link_color_picker;

GtkWidget                       *date_format_combobox;
GtkWidget                       *entry_web_browser;
GtkWidget                       *entry_email_client;

gboolean                        callback_active;

/*------------------------------------------------------------------------------*/

void
fill_timeline(void) {

gint i;

    i = config.timeline_start / 60;
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tl_start_h_spinbutton), i);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tl_start_m_spinbutton), config.timeline_start-i*60);
    i = config.timeline_end / 60;
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tl_end_h_spinbutton), i);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tl_end_m_spinbutton), config.timeline_end-i*60);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tl_step_spinbutton), (gdouble)config.timeline_step);
}

/*------------------------------------------------------------------------------*/

void
day_name_font_select_cb (GtkWidget *widget) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.day_name_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.day_name_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(day_name_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(fd_day_name);
        fd_day_name = pango_font_description_from_string(config.day_name_font);
        gtk_widget_modify_font (GTK_WIDGET(date_label), fd_day_name);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
calendar_font_select_cb (GtkWidget *widget) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.calendar_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.calendar_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(calendar_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(fd_calendar);
        fd_calendar = pango_font_description_from_string(config.calendar_font);
        gtk_widget_modify_font (GTK_WIDGET(calendar), fd_calendar);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
notes_font_select_cb (GtkWidget *widget) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.notes_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.notes_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(notes_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(fd_notes);
        fd_notes = pango_font_description_from_string(config.notes_font);
        gtk_widget_modify_font (GTK_WIDGET(textview), fd_notes);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
ti_font_select_cb (GtkWidget *widget) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.task_info_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.task_info_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(ti_font_entry), font_name);
        g_free (font_name);

        /* tasks task info font */
        g_object_set (G_OBJECT (font_tag_object), "font", (gchar *)config.task_info_font, NULL);
    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/


gint 
options_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    if (event->keyval == GDK_Escape) {
            gtk_notebook_set_current_page (GTK_NOTEBOOK (options_notebook), 0);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
options_switch_buttons(gboolean left) {

GtkWidget *buttons_table[] = { 
    calendar_options_radiobutton, 
    tasks_options_radiobutton, 
    contacts_options_radiobutton, 
    general_options_radiobutton
};

    if (left == TRUE && options_counter > 0) {
        --options_counter;
    } else if (left == FALSE && options_counter < 3) {
        options_counter++;
    }

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(buttons_table[options_counter]), TRUE);
    g_signal_emit_by_name(G_OBJECT(buttons_table[options_counter]), "released");

}

/*------------------------------------------------------------------------------*/

void
options_button_released_cb (GtkButton *button, gpointer user_data) {

    gtk_widget_hide (calendar_vbox);
    gtk_widget_hide (tasks_vbox);
    gtk_widget_hide (contacts_vbox);
    gtk_widget_hide (general_vbox);

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(calendar_options_radiobutton)) == TRUE) {

        gtk_widget_show (calendar_vbox);
        options_counter = 0;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(tasks_options_radiobutton)) == TRUE) {

        gtk_widget_show (tasks_vbox);
        options_counter = 1;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(contacts_options_radiobutton)) == TRUE) {

        gtk_widget_show (contacts_vbox);
        options_counter = 2;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(general_options_radiobutton)) == TRUE) {

        gtk_widget_show (general_vbox);
        options_counter = 3;
    }
}

/*------------------------------------------------------------------------------*/

void
hide_delete_items_cb(GtkToggleButton *togglebutton, gpointer user_data) {

    if (user_data == (gpointer *)1 ) {    /* hide toggled */

        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton))) {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ct_delete_items_checkbutton), FALSE);
        } 

    } else {    /* delete toggled */

        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(ct_delete_items_checkbutton))) {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton), FALSE);
        } 
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_category_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    category_name = gtk_entry_get_text(GTK_ENTRY(tasks_category_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(tasks_category_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(tasks_category_store), &iter, 0, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(tasks_category_store), &iter, NULL, 0);

        gtk_list_store_append(tasks_category_store, &iter);
        gtk_list_store_set(tasks_category_store, &iter, 0, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(tasks_category_entry), "");
        gtk_widget_set_sensitive(tasks_category_add_button, FALSE);

        create_category_combobox (GTK_COMBO_BOX (cf_combobox), tasks_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX (cf_combobox), 0);

        apply_date_colors();

        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(tasks_filter));
        update_n_items();
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_category_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (tasks_category_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(tasks_category_store), &iter, path);
        gtk_list_store_remove(tasks_category_store, &iter);
        gtk_tree_path_free(path);
    }
}

/*------------------------------------------------------------------------------*/

gint 
tasks_category_entry_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(tasks_category_entry)))) {
            tasks_category_add_cb (NULL, NULL);
        }
        return TRUE;
    }

    if(strlen(gtk_entry_get_text(GTK_ENTRY(tasks_category_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(tasks_category_add_button, state);

    return FALSE;
}
    
/*------------------------------------------------------------------------------*/

void
contacts_group_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    category_name = gtk_entry_get_text(GTK_ENTRY(contacts_group_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_group_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(contacts_group_store), &iter, 0, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_group_store), &iter, NULL, 0);

        gtk_list_store_append(contacts_group_store, &iter);
        gtk_list_store_set(contacts_group_store, &iter, 0, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(contacts_group_entry), "");
        gtk_widget_set_sensitive(contacts_group_add_button, FALSE);

    }
}

/*------------------------------------------------------------------------------*/

void
contacts_group_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (contacts_group_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(contacts_group_store), &iter, path);
        gtk_list_store_remove(contacts_group_store, &iter);
        gtk_tree_path_free(path);
    }
}

/*------------------------------------------------------------------------------*/

gint 
contacts_group_entry_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(contacts_group_entry)))) {
            contacts_group_add_cb (NULL, NULL);
        }
        return TRUE;
    }

    if(strlen(gtk_entry_get_text(GTK_ENTRY(contacts_group_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(contacts_group_add_button, state);
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
mark_symbol_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.mark_symbol = gtk_combo_box_get_active (GTK_COMBO_BOX(mark_symbol_combobox));
    gui_calendar_set_mark_symbol (GUI_CALENDAR (calendar), get_mark_symbol(config.mark_symbol));

}

/*------------------------------------------------------------------------------*/

void
header_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(header_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.header_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_header_color(GUI_CALENDAR (calendar), config.header_color);

}

/*------------------------------------------------------------------------------*/

void
weekend_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(weekend_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.weekend_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_weekend_color(GUI_CALENDAR (calendar), config.weekend_color);

}

/*------------------------------------------------------------------------------*/

void
selection_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(selection_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.selection_color, tmpbuf, MAXCOLORNAME-1);
    gtk_widget_modify_base(GTK_WIDGET(calendar), GTK_STATE_ACTIVE, &color);

}

/*------------------------------------------------------------------------------*/

void
timeline_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

gint i, j;

    if (!callback_active) return;

    i = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tl_start_h_spinbutton)) * 60 +
        (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tl_start_m_spinbutton));
    j = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tl_end_h_spinbutton)) * 60 +
        (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tl_end_m_spinbutton));

    if (i < j) {
        config.timeline_start = i;
        config.timeline_end = j;                       
        config.timeline_step = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tl_step_spinbutton));
    } else {
        fill_timeline();
    }

}

/*------------------------------------------------------------------------------*/

void
calendar_options_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    if (!callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(show_day_names_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_SHOW_DAY_NAMES;
    } else {
        config.display_options &= ~GUI_CALENDAR_SHOW_DAY_NAMES;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(no_month_change_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_NO_MONTH_CHANGE;
    } else {
        config.display_options &= ~GUI_CALENDAR_NO_MONTH_CHANGE;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(show_week_numbers_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_SHOW_WEEK_NUMBERS;
    } else {
        config.display_options &= ~GUI_CALENDAR_SHOW_WEEK_NUMBERS;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(week_start_monday_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_WEEK_START_MONDAY;
    } else {
        config.display_options &= ~GUI_CALENDAR_WEEK_START_MONDAY;
    }

    config.fy_simple_view = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(simple_view_in_fy_calendar_checkbutton));
    config.notes_in_info_panel = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(show_notes_in_info_panel_checkbutton));

    gui_calendar_set_display_options (GUI_CALENDAR (calendar), config.display_options);
    g_signal_emit_by_name(G_OBJECT(calendar), "day-selected");

}

/*------------------------------------------------------------------------------*/

void
due_today_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(due_today_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_today_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
due_7days_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(due_7days_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_7days_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
past_due_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(past_due_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.past_due_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
tasks_enable_rules_hint_checkbutton_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.tasks_rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(tasks_enable_rules_hint_checkbutton));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tasks_list), config.tasks_rules_hint);
}

/*------------------------------------------------------------------------------*/

void
visible_columns_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.visible_due_date_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(vc_due_date_checkbutton));
    gtk_tree_view_column_set_visible (tasks_columns[COLUMN_DUE_DATE], config.visible_due_date_column);
    config.visible_category_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(vc_category_checkbutton));
    gtk_tree_view_column_set_visible (tasks_columns[COLUMN_CATEGORY], config.visible_category_column);
    config.visible_priority_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(vc_priority_checkbutton));
    gtk_tree_view_column_set_visible (tasks_columns[COLUMN_PRIORITY], config.visible_priority_column);

}

/*------------------------------------------------------------------------------*/

void
tasks_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.tasks_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(tasks_sort_order_combobox));
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)tasks_sort, COLUMN_DUE_DATE, config.tasks_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)tasks_sort, COLUMN_PRIORITY, config.tasks_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)tasks_sort, COLUMN_DONE, config.tasks_sorting_order);

}

/*------------------------------------------------------------------------------*/

void
tasks_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.tasks_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(tasks_sort_mode_combobox));
    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(tasks_filter));

}

/*------------------------------------------------------------------------------*/

void
hide_delete_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton));
    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(tasks_filter));
    config.delete_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(ct_delete_items_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
contact_tag_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(contact_tag_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.contact_tag_color, tmpbuf, MAXCOLORNAME-1);
    g_object_set (G_OBJECT (contact_color_tag_object), "foreground-gdk", &color, NULL);

}

/*------------------------------------------------------------------------------*/

void
contact_link_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

    gtk_color_button_get_color(GTK_COLOR_BUTTON(contact_link_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.contact_link_color, tmpbuf, MAXCOLORNAME-1);
    g_object_set (G_OBJECT (gui_url_tag), "foreground-gdk", &color, NULL);
    g_signal_emit_by_name(G_OBJECT(contacts_list_selection), "changed");

}

/*------------------------------------------------------------------------------*/

void
contacts_enable_rules_hint_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.contacts_rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(contacts_enable_rules_hint_checkbutton));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(contacts_list), config.contacts_rules_hint);

}

/*------------------------------------------------------------------------------*/

void
contacts_select_first_entry_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.show_after_search = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(contacts_select_first_entry_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
contacts_hide_group_column_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.hide_group_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(contacts_hide_group_column_checkbutton));
    gtk_tree_view_column_set_visible (contacts_columns[COLUMN_GROUP], !config.hide_group_column);

}

/*------------------------------------------------------------------------------*/

void
default_stock_icons_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(default_stock_icons_checkbutton)) == TRUE) {
        config.default_stock_icons = 1;
    } else {
        config.default_stock_icons = 0;
    }
}

/*------------------------------------------------------------------------------*/

void
disable_underline_in_links_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.disable_underline_links = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(disable_underline_in_links_checkbutton));
    g_signal_emit_by_name(G_OBJECT(contacts_list_selection), "changed");
    about_set_first_page();

}

/*------------------------------------------------------------------------------*/

void
enable_tooltips_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(enable_tooltips_checkbutton)) == TRUE) {
        config.enable_tooltips = 1;
        gtk_tooltips_enable(osmo_tooltips);
    } else {
        config.enable_tooltips = 0;
        gtk_tooltips_disable(osmo_tooltips);
    }

    gtk_toolbar_set_tooltips (calendar_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (tasks_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (contacts_toolbar, config.enable_tooltips);

}

/*------------------------------------------------------------------------------*/

void
remember_latest_tab_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;
 
    config.remember_latest_tab = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(remember_latest_tab_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
helpers_entry_changed_cb (GtkEntry *entry, gpointer  user_data) {

    if (!callback_active) return;

    strncpy(config.web_browser, gtk_entry_get_text(GTK_ENTRY(entry_web_browser)), MAXHELPERCMD-1);
    strncpy(config.email_client, gtk_entry_get_text(GTK_ENTRY(entry_email_client)), MAXHELPERCMD-1);

}

/*------------------------------------------------------------------------------*/

void
name_font_size_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    if (!callback_active) return;

    config.name_font_size = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(name_font_size_spinbutton));

    g_object_set (G_OBJECT (contact_name_tag_object), "size", config.name_font_size * PANGO_SCALE, NULL);

}

/*------------------------------------------------------------------------------*/

void
tags_font_size_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    if (!callback_active) return;

    config.tags_font_size = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(tags_font_size_spinbutton));

    g_object_set (G_OBJECT (contact_color_tag_object), "size", config.tags_font_size * PANGO_SCALE, NULL);
    g_object_set (G_OBJECT (contact_tags_value_tag_object), "size", config.tags_font_size * PANGO_SCALE, NULL);

}

/*------------------------------------------------------------------------------*/

void
contacts_photo_size_changed_cb (GtkComboBox *widget, gpointer user_data) {

gint i;
gint sizes[] = { PHOTO_SMALL, PHOTO_MEDIUM, PHOTO_LARGE };

    i = gtk_combo_box_get_active (GTK_COMBO_BOX(widget));

    config.photo_width = sizes[i % 3];
    g_signal_emit_by_name(contacts_list_selection, "changed");

}

/*------------------------------------------------------------------------------*/

void
contacts_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.contacts_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(contacts_sort_order_combobox));
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_FIRST_NAME, config.contacts_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_LAST_NAME, config.contacts_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_GROUP, config.contacts_sorting_order);

}

/*------------------------------------------------------------------------------*/

void
contacts_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.contacts_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(contacts_sort_mode_combobox));
    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(contacts_filter));

}

/*------------------------------------------------------------------------------*/

void
date_format_changed_cb (GtkComboBox *widget, gpointer user_data) {

    if (!callback_active) return;

    config.date_format = gtk_combo_box_get_active (GTK_COMBO_BOX(date_format_combobox));
    refresh_due_dates();

}

/*------------------------------------------------------------------------------*/

void 
tasks_category_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;  

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(tasks_category_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(tasks_category_remove_button, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void 
contacts_group_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;  

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(contacts_group_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(contacts_group_remove_button, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void 
gui_create_options(GtkWidget *notebook) {

GtkWidget           *vbox1, *vbox2, *vbox3, *vbox4, *vbox5, *vbox6;
GtkWidget           *vbox7, *vbox8, *vbox9, *vbox10, *vbox11, *vbox12, *vbox13;
GtkWidget           *label;
GtkWidget           *hbox1;
GtkWidget           *frame;
GtkWidget           *table;
GtkWidget           *alignment;
GtkWidget           *day_name_font_button;
GtkWidget           *calendar_font_button;
GtkWidget           *notes_font_button;
GtkWidget           *ti_font_button;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *scrolledwindow;
GtkWidget           *viewport;
GSList              *radiobutton_group = NULL;
GtkObject           *tl_start_h_spinbutton_adj;
GtkObject           *tl_start_m_spinbutton_adj;
GtkObject           *tl_end_h_spinbutton_adj;
GtkObject           *tl_end_m_spinbutton_adj;
GtkObject           *tl_step_spinbutton_adj;
GtkWidget           *timeline_table;
GtkWidget           *tasks_category_table;
GtkWidget           *contacts_group_table;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;



    font_modified = FALSE;
    options_notebook = notebook;

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    gtk_label_set_markup (GTK_LABEL (label), _("Options"));
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox1, label);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    calendar_options_radiobutton = gui_image_label_radio_button(_("Calendar"), osmo_stock_calendar);
    gtk_widget_show (calendar_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (calendar_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(calendar_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (calendar_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (calendar_options_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (calendar_options_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (calendar_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), calendar_options_radiobutton);

    g_signal_connect (G_OBJECT (calendar_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), NULL);

    tasks_options_radiobutton = gui_image_label_radio_button(_("Tasks"), osmo_stock_tasks);
    gtk_widget_show (tasks_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (tasks_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(tasks_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (tasks_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (tasks_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (tasks_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), tasks_options_radiobutton);

    g_signal_connect (G_OBJECT (tasks_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), NULL);

    contacts_options_radiobutton = gui_image_label_radio_button(_("Contacts"), osmo_stock_contacts);
    gtk_widget_show (contacts_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (contacts_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(contacts_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (contacts_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (contacts_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (contacts_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), contacts_options_radiobutton);

    g_signal_connect (G_OBJECT (contacts_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), NULL);

    general_options_radiobutton = gui_image_label_radio_button(_("General"), osmo_stock_preferences);
    gtk_widget_show (general_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (general_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(general_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (general_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (general_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (general_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), general_options_radiobutton);

    g_signal_connect (G_OBJECT (general_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), NULL);

    /*--------------------------------------------------------------------------*/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_NONE);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), viewport);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (viewport), vbox2);

    /*****************************************************************************************************/
    /* Calendar options */
    /*****************************************************************************************************/

    calendar_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), calendar_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (calendar_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Appearance"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox13 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox13);
    gtk_container_add (GTK_CONTAINER (alignment), vbox13);

    table = gtk_table_new (4, 6, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox13), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    mark_symbol_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (mark_symbol_combobox);
    gtk_table_attach (GTK_TABLE (table), mark_symbol_combobox, 1, 3, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (mark_symbol_combobox), "changed",
                      G_CALLBACK(mark_symbol_changed_cb), NULL);
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Middle dot"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Apostrophe"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Asterisk"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Tilde"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Vertical line"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (mark_symbol_combobox), _("Degree sign"));

    gtk_combo_box_set_active (GTK_COMBO_BOX(mark_symbol_combobox), config.mark_symbol);

    hbox1 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (table), hbox1, 1, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    header_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (header_color_picker), "color-set",
                      G_CALLBACK(header_color_changed_cb), NULL);
    gtk_widget_show (header_color_picker);
    gtk_tooltips_set_tip (osmo_tooltips, header_color_picker, _("Header color"), NULL);
    gdk_color_parse(config.header_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (header_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), header_color_picker, FALSE, FALSE, 0);

    weekend_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (weekend_color_picker), "color-set",
                      G_CALLBACK(weekend_color_changed_cb), NULL);
    gtk_widget_show (weekend_color_picker);
    gtk_tooltips_set_tip (osmo_tooltips, weekend_color_picker, _("Weekend days color"), NULL);
    gdk_color_parse(config.weekend_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (weekend_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), weekend_color_picker, FALSE, FALSE, 0);

    selection_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (selection_color_picker), "color-set",
                      G_CALLBACK(selection_color_changed_cb), NULL);
    gtk_widget_show (selection_color_picker);
    gtk_tooltips_set_tip (osmo_tooltips, selection_color_picker, _("Selected day color"), NULL);
    gdk_color_parse(config.selection_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (selection_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), selection_color_picker, FALSE, FALSE, 0);

    day_name_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(day_name_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (day_name_font_entry);
    gtk_table_attach (GTK_TABLE (table), day_name_font_entry, 1, 5, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    calendar_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(calendar_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (calendar_font_entry);
    gtk_table_attach (GTK_TABLE (table), calendar_font_entry, 1, 5, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    notes_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(notes_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (notes_font_entry);
    gtk_table_attach (GTK_TABLE (table), notes_font_entry, 1, 5, 4, 5,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        day_name_font_button = gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        day_name_font_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(day_name_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (day_name_font_button);
    g_signal_connect (G_OBJECT (day_name_font_button), "clicked",
                      G_CALLBACK (day_name_font_select_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), day_name_font_button, 5, 6, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        calendar_font_button = gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        calendar_font_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(calendar_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (calendar_font_button);
    g_signal_connect (G_OBJECT (calendar_font_button), "clicked",
                      G_CALLBACK (calendar_font_select_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), calendar_font_button, 5, 6, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        notes_font_button = gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        notes_font_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(notes_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (notes_font_button);
    g_signal_connect (G_OBJECT (notes_font_button), "clicked",
                      G_CALLBACK (notes_font_select_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), notes_font_button, 5, 6, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    sprintf(tmpbuf, "%s:", _("Colors"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Date font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Calendar font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Note font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Mark symbol:"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (calendar_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Calendar"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox3 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox3);
    gtk_container_add (GTK_CONTAINER (alignment), vbox3);

    table = gtk_table_new (4, 3, FALSE);
    gtk_widget_show (table);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_box_pack_start (GTK_BOX (vbox3), table, FALSE, FALSE, 0);

    week_start_monday_checkbutton = gtk_check_button_new_with_mnemonic (_("Week start on Monday"));
    GTK_WIDGET_UNSET_FLAGS(week_start_monday_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (week_start_monday_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (week_start_monday_checkbutton);
    gtk_table_attach (GTK_TABLE (table), week_start_monday_checkbutton, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    show_day_names_checkbutton = gtk_check_button_new_with_mnemonic (_("Show day names"));
    GTK_WIDGET_UNSET_FLAGS(show_day_names_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (show_day_names_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (show_day_names_checkbutton);
    gtk_table_attach (GTK_TABLE (table), show_day_names_checkbutton, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    no_month_change_checkbutton = gtk_check_button_new_with_mnemonic (_("No month change"));
    GTK_WIDGET_UNSET_FLAGS(no_month_change_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (no_month_change_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (no_month_change_checkbutton);
    gtk_table_attach (GTK_TABLE (table), no_month_change_checkbutton, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    show_week_numbers_checkbutton = gtk_check_button_new_with_mnemonic (_("Show week numbers"));
    GTK_WIDGET_UNSET_FLAGS(show_week_numbers_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (show_week_numbers_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (show_week_numbers_checkbutton);
    gtk_table_attach (GTK_TABLE (table), show_week_numbers_checkbutton, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    simple_view_in_fy_calendar_checkbutton = gtk_check_button_new_with_mnemonic (_("Simple view in full-year calendar"));
    GTK_WIDGET_UNSET_FLAGS(simple_view_in_fy_calendar_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (simple_view_in_fy_calendar_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (simple_view_in_fy_calendar_checkbutton);
    gtk_table_attach (GTK_TABLE (table), simple_view_in_fy_calendar_checkbutton, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    show_notes_in_info_panel_checkbutton = gtk_check_button_new_with_mnemonic (_("Show notes in info panel"));
    GTK_WIDGET_UNSET_FLAGS(show_notes_in_info_panel_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (show_notes_in_info_panel_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), NULL);
    gtk_widget_show (show_notes_in_info_panel_checkbutton);
    gtk_table_attach (GTK_TABLE (table), show_notes_in_info_panel_checkbutton, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (calendar_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Timeline"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    timeline_table = gtk_table_new (1, 9, FALSE);
    gtk_widget_show (timeline_table);
    gtk_container_set_border_width (GTK_CONTAINER (timeline_table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (timeline_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (timeline_table), 4);
    gtk_container_add (GTK_CONTAINER (alignment), timeline_table);

    sprintf(tmpbuf, "%s:", _("Start (hour)"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (timeline_table), label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    tl_start_h_spinbutton_adj = gtk_adjustment_new (1, 0, 24, 1, 10, 10);
    tl_start_h_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tl_start_h_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tl_start_h_spinbutton), "value-changed",
                      G_CALLBACK (timeline_changed_cb), NULL);
    gtk_widget_show (tl_start_h_spinbutton);
    gtk_table_attach (GTK_TABLE (timeline_table), tl_start_h_spinbutton, 1, 2, 0, 1,
                     (GtkAttachOptions) (0),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tl_start_h_spinbutton), TRUE);

    sprintf(tmpbuf, "%s:", _("Step (minutes)"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (timeline_table), label, 0, 1, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    tl_step_spinbutton_adj = gtk_adjustment_new (1, 1, 120, 1, 10, 10);
    tl_step_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tl_step_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tl_step_spinbutton), "value-changed",
                      G_CALLBACK (timeline_changed_cb), NULL);
    gtk_widget_show (tl_step_spinbutton);
    gtk_table_attach (GTK_TABLE (timeline_table), tl_step_spinbutton, 1, 2, 2, 3,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tl_step_spinbutton), TRUE);

    sprintf(tmpbuf, "%s:", _("End (hour)"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (timeline_table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    tl_end_h_spinbutton_adj = gtk_adjustment_new (1, 1, 23, 1, 10, 10);
    tl_end_h_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tl_end_h_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tl_end_h_spinbutton), "value-changed",
                      G_CALLBACK (timeline_changed_cb), NULL);
    gtk_widget_show (tl_end_h_spinbutton);
    gtk_table_attach (GTK_TABLE (timeline_table), tl_end_h_spinbutton, 1, 2, 1, 2,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tl_end_h_spinbutton), TRUE);

    label = gtk_label_new (":");
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (timeline_table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    label = gtk_label_new (":");
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (timeline_table), label, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    tl_start_m_spinbutton_adj = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    tl_start_m_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tl_start_m_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tl_start_m_spinbutton), "value-changed",
                      G_CALLBACK (timeline_changed_cb), NULL);
    gtk_widget_show (tl_start_m_spinbutton);
    gtk_table_attach (GTK_TABLE (timeline_table), tl_start_m_spinbutton, 3, 4, 0, 1,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tl_start_m_spinbutton), TRUE);

    tl_end_m_spinbutton_adj = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    tl_end_m_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tl_end_m_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tl_end_m_spinbutton), "value-changed",
                      G_CALLBACK (timeline_changed_cb), NULL);
    gtk_widget_show (tl_end_m_spinbutton);
    gtk_table_attach (GTK_TABLE (timeline_table), tl_end_m_spinbutton, 3, 4, 1, 2,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tl_end_m_spinbutton), TRUE);


    /*****************************************************************************************************/
    /* TASKS options */
    /*****************************************************************************************************/

    tasks_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), tasks_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Appearance"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox5 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox5);
    gtk_container_add (GTK_CONTAINER (alignment), vbox5);
    
    table = gtk_table_new (3, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox5), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    hbox1 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (table), hbox1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    due_today_color_picker = gtk_color_button_new ();
    gtk_widget_show (due_today_color_picker);
    g_signal_connect (G_OBJECT (due_today_color_picker), "color-set",
                      G_CALLBACK(due_today_color_changed_cb), NULL);
    gtk_tooltips_set_tip (osmo_tooltips, due_today_color_picker, _("Color of items that are due today"), NULL);
    gdk_color_parse(config.due_today_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (due_today_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), due_today_color_picker, FALSE, FALSE, 0);

    due_7days_color_picker = gtk_color_button_new ();
    gtk_widget_show (due_7days_color_picker);
    g_signal_connect (G_OBJECT (due_7days_color_picker), "color-set",
                      G_CALLBACK(due_7days_color_changed_cb), NULL);
    gtk_tooltips_set_tip (osmo_tooltips, due_7days_color_picker, _("Color of items that are due in the next 7 days"), NULL);
    gdk_color_parse(config.due_7days_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (due_7days_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), due_7days_color_picker, FALSE, FALSE, 0);

    past_due_color_picker = gtk_color_button_new ();
    gtk_widget_show (past_due_color_picker);
    g_signal_connect (G_OBJECT (past_due_color_picker), "color-set",
                      G_CALLBACK(past_due_color_changed_cb), NULL);
    gtk_tooltips_set_tip (osmo_tooltips, past_due_color_picker, _("Color of items that are past due"), NULL);
    gdk_color_parse(config.past_due_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (past_due_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), past_due_color_picker, FALSE, FALSE, 0);

    ti_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(ti_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (ti_font_entry);
    gtk_table_attach (GTK_TABLE (table), ti_font_entry, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        ti_font_button = gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        ti_font_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(ti_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (ti_font_button);
    g_signal_connect (G_OBJECT (ti_font_button), "clicked",
                      G_CALLBACK (ti_font_select_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), ti_font_button, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    sprintf(tmpbuf, "%s:", _("Task info font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Colors"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    tasks_enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable rules hint"));
    GTK_WIDGET_UNSET_FLAGS(tasks_enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (tasks_enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (tasks_enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (tasks_enable_rules_hint_checkbutton_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), tasks_enable_rules_hint_checkbutton, 0, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Visible columns"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox12 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox12);
    gtk_container_add (GTK_CONTAINER (alignment), vbox12);

    table = gtk_table_new (1, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox12), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    vc_due_date_checkbutton = gtk_check_button_new_with_mnemonic (_("Due date"));
    GTK_WIDGET_UNSET_FLAGS(vc_due_date_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (vc_due_date_checkbutton);
    g_signal_connect (G_OBJECT (vc_due_date_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), vc_due_date_checkbutton, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    vc_priority_checkbutton = gtk_check_button_new_with_mnemonic (_("Priority"));
    GTK_WIDGET_UNSET_FLAGS(vc_priority_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (vc_priority_checkbutton);
    g_signal_connect (G_OBJECT (vc_priority_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), vc_priority_checkbutton, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    vc_category_checkbutton = gtk_check_button_new_with_mnemonic (_("Category"));
    GTK_WIDGET_UNSET_FLAGS(vc_category_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (vc_category_checkbutton);
    g_signal_connect (G_OBJECT (vc_category_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), vc_category_checkbutton, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Categories"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox6 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox6);
    gtk_container_add (GTK_CONTAINER (alignment), vbox6);

    tasks_category_table = gtk_table_new (4, 3, FALSE);
    gtk_widget_show (tasks_category_table);
    gtk_box_pack_start (GTK_BOX (vbox6), tasks_category_table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (tasks_category_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (tasks_category_table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (tasks_category_table), 4);

    tasks_category_entry = gtk_entry_new ();
    gtk_widget_show (tasks_category_entry);
    gtk_table_attach (GTK_TABLE (tasks_category_table), tasks_category_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (tasks_category_entry), "key_press_event",
                      G_CALLBACK (tasks_category_entry_key_press_cb), NULL);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (tasks_category_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    tasks_category_store = gtk_list_store_new(1, G_TYPE_STRING);

    tasks_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tasks_category_store));
    tasks_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tasks_category_treeview));
    gtk_widget_show (tasks_category_treeview);
    g_signal_connect(G_OBJECT(tasks_category_select), "changed", 
                     G_CALLBACK(tasks_category_selected_cb), NULL);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), tasks_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (tasks_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tasks_category_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (tasks_category_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (tasks_category_treeview), FALSE);
    gtk_widget_set_size_request (tasks_category_treeview, -1, 80);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tasks_category_treeview), column);

    if (config.default_stock_icons) {
        tasks_category_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        tasks_category_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(tasks_category_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (tasks_category_add_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), tasks_category_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(tasks_category_add_button, "clicked", 
                     G_CALLBACK(tasks_category_add_cb), NULL);
    gtk_widget_set_sensitive(tasks_category_add_button, FALSE);

    if (config.default_stock_icons) {
        tasks_category_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {
        tasks_category_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(tasks_category_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (tasks_category_remove_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), tasks_category_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(tasks_category_remove_button, "clicked", 
                     G_CALLBACK(tasks_category_remove_cb), NULL);
    gtk_widget_set_sensitive(tasks_category_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Sorting"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox7 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox7);
    gtk_container_add (GTK_CONTAINER (alignment), vbox7);

    table = gtk_table_new (1, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox7), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Order"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Mode"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    tasks_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (tasks_sort_order_combobox);
    g_signal_connect (G_OBJECT (tasks_sort_order_combobox), "changed",
                      G_CALLBACK(tasks_sort_order_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), tasks_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(tasks_sort_order_combobox), config.tasks_sorting_order);

    tasks_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (tasks_sort_mode_combobox);
    g_signal_connect (G_OBJECT (tasks_sort_mode_combobox), "changed",
                      G_CALLBACK(tasks_sort_mode_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), tasks_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Done, Due Date, Priority"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Done, Priority, Due Date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Priority, Due Date, Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Priority, Done, Due Date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Due Date, Priority, Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (tasks_sort_mode_combobox), _("Due Date, Done, Priority"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(tasks_sort_mode_combobox), config.tasks_sorting_mode);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Tasks options"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox11 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox11);
    gtk_container_add (GTK_CONTAINER (alignment), vbox11);

    ct_hide_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Hide completed tasks"));
    GTK_WIDGET_UNSET_FLAGS(ct_hide_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (ct_hide_items_checkbutton);
    g_signal_connect (G_OBJECT (ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox11), ct_hide_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK(hide_delete_items_cb), (gpointer*) 1);

    ct_delete_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Delete completed tasks without confirmation"));
    GTK_WIDGET_UNSET_FLAGS(ct_delete_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (ct_delete_items_checkbutton);
    g_signal_connect (G_OBJECT (ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox11), ct_delete_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK(hide_delete_items_cb), NULL);


    /*****************************************************************************************************/
    /* CONTACTS options */
    /*****************************************************************************************************/

    contacts_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), contacts_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (contacts_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Appearance"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox8 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox8);
    gtk_container_add (GTK_CONTAINER (alignment), vbox8);

    table = gtk_table_new (4, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox8), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Colors"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    hbox1 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (table), hbox1, 1, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    contact_tag_color_picker = gtk_color_button_new ();
    gtk_widget_show (contact_tag_color_picker);
    g_signal_connect (G_OBJECT (contact_tag_color_picker), "color-set",
                      G_CALLBACK(contact_tag_color_changed_cb), NULL);
    gtk_tooltips_set_tip (osmo_tooltips, contact_tag_color_picker, _("Color of contact tags"), NULL);
    gdk_color_parse(config.contact_tag_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (contact_tag_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), contact_tag_color_picker, FALSE, FALSE, 0);

    contact_link_color_picker = gtk_color_button_new ();
    gtk_widget_show (contact_link_color_picker);
    g_signal_connect (G_OBJECT (contact_link_color_picker), "color-set",
                      G_CALLBACK(contact_link_color_changed_cb), NULL);
    gtk_tooltips_set_tip (osmo_tooltips, contact_link_color_picker, _("Color of links"), NULL);
    gdk_color_parse(config.contact_link_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (contact_link_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), contact_link_color_picker, FALSE, FALSE, 0);

    sprintf(tmpbuf, "%s:", _("Name font size"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    name_font_size_spinbutton_adj = gtk_adjustment_new (6, 6, 100, 1, 10, 10);
    name_font_size_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (name_font_size_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (name_font_size_spinbutton), "value-changed",
                      G_CALLBACK (name_font_size_changed_cb), NULL);
    gtk_widget_show (name_font_size_spinbutton);
    gtk_table_attach (GTK_TABLE (table), name_font_size_spinbutton, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (name_font_size_spinbutton), TRUE);


    sprintf(tmpbuf, "%s:", _("Tags font size"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    tags_font_size_spinbutton_adj = gtk_adjustment_new (6, 6, 100, 1, 10, 10);
    tags_font_size_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (tags_font_size_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (tags_font_size_spinbutton), "value-changed",
                      G_CALLBACK (tags_font_size_changed_cb), NULL);
    gtk_widget_show (tags_font_size_spinbutton);
    gtk_table_attach (GTK_TABLE (table), tags_font_size_spinbutton, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (tags_font_size_spinbutton), TRUE);


    sprintf(tmpbuf, "%s:", _("Photo size"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    hbox1 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (table), hbox1, 1, 4, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    contacts_photo_size_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (contacts_photo_size_combobox);
    g_signal_connect (G_OBJECT (contacts_photo_size_combobox), "changed",
                      G_CALLBACK(contacts_photo_size_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (hbox1), contacts_photo_size_combobox, FALSE, FALSE, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_photo_size_combobox), _("Small"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_photo_size_combobox), _("Medium"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_photo_size_combobox), _("Large"));

    if (config.photo_width == PHOTO_LARGE) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(contacts_photo_size_combobox), 2);
    } else if (config.photo_width == PHOTO_MEDIUM) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(contacts_photo_size_combobox), 1);
    } else {
        gtk_combo_box_set_active (GTK_COMBO_BOX(contacts_photo_size_combobox), 0);
    }
    
    contacts_enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable rules hint"));
    GTK_WIDGET_UNSET_FLAGS(contacts_enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (contacts_enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (contacts_enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (contacts_enable_rules_hint_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox8), contacts_enable_rules_hint_checkbutton, TRUE, TRUE, 4);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (contacts_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Miscellaneous"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox8 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox8);
    gtk_container_add (GTK_CONTAINER (alignment), vbox8);


    contacts_select_first_entry_checkbutton = gtk_check_button_new_with_mnemonic (_("Select and show first item after search"));
    GTK_WIDGET_UNSET_FLAGS(contacts_select_first_entry_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (contacts_select_first_entry_checkbutton);
    g_signal_connect (G_OBJECT (contacts_select_first_entry_checkbutton), "toggled",
                      G_CALLBACK (contacts_select_first_entry_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox8), contacts_select_first_entry_checkbutton, TRUE, TRUE, 4);

    contacts_hide_group_column_checkbutton = gtk_check_button_new_with_mnemonic (_("Hide group column"));
    GTK_WIDGET_UNSET_FLAGS(contacts_hide_group_column_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (contacts_hide_group_column_checkbutton);
    g_signal_connect (G_OBJECT (contacts_hide_group_column_checkbutton), "toggled",
                      G_CALLBACK (contacts_hide_group_column_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox8), contacts_hide_group_column_checkbutton, TRUE, TRUE, 4);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (contacts_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Groups"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox10 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox10);
    gtk_container_add (GTK_CONTAINER (alignment), vbox10);

    contacts_group_table = gtk_table_new (4, 3, FALSE);
    gtk_widget_show (contacts_group_table);
    gtk_box_pack_start (GTK_BOX (vbox10), contacts_group_table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (contacts_group_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (contacts_group_table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (contacts_group_table), 4);

    contacts_group_entry = gtk_entry_new ();
    gtk_widget_show (contacts_group_entry);
    gtk_table_attach (GTK_TABLE (contacts_group_table), contacts_group_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (contacts_group_entry), "key_press_event",
                      G_CALLBACK (contacts_group_entry_key_press_cb), NULL);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (contacts_group_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    contacts_group_store = gtk_list_store_new(1, G_TYPE_STRING);

    contacts_group_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(contacts_group_store));
    contacts_group_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(contacts_group_treeview));
    gtk_widget_show (contacts_group_treeview);
    g_signal_connect(G_OBJECT(contacts_group_select), "changed", 
                     G_CALLBACK(contacts_group_selected_cb), NULL);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), contacts_group_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (contacts_group_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (contacts_group_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (contacts_group_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (contacts_group_treeview), FALSE);
    gtk_widget_set_size_request (contacts_group_treeview, -1, 80);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(contacts_group_treeview), column);

    if (config.default_stock_icons) {
        contacts_group_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        contacts_group_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(contacts_group_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (contacts_group_add_button);
    gtk_table_attach (GTK_TABLE (contacts_group_table), contacts_group_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(contacts_group_add_button, "clicked", 
                     G_CALLBACK(contacts_group_add_cb), NULL);
    gtk_widget_set_sensitive(contacts_group_add_button, FALSE);

    if (config.default_stock_icons) {
        contacts_group_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {     
        contacts_group_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(contacts_group_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (contacts_group_remove_button);
    gtk_table_attach (GTK_TABLE (contacts_group_table), contacts_group_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(contacts_group_remove_button, "clicked", 
                     G_CALLBACK(contacts_group_remove_cb), NULL);
    gtk_widget_set_sensitive(contacts_group_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (contacts_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Sorting"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox9 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox9);
    gtk_container_add (GTK_CONTAINER (alignment), vbox9);

    table = gtk_table_new (1, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox9), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Order"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Mode"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    contacts_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (contacts_sort_order_combobox);
    g_signal_connect (G_OBJECT (contacts_sort_order_combobox), "changed",
                      G_CALLBACK(contacts_sort_order_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), contacts_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(contacts_sort_order_combobox), config.contacts_sorting_order);

    contacts_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (contacts_sort_mode_combobox);
    g_signal_connect (G_OBJECT (contacts_sort_mode_combobox), "changed",
                      G_CALLBACK(contacts_sort_mode_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), contacts_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("Group, First Name, Last Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("Group, Last Name, First Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("Last Name, First Name, Group"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("Last Name, Group, First Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("First Name, Last Name, Group"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (contacts_sort_mode_combobox), _("First Name, Group, Last Name"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(contacts_sort_mode_combobox), config.contacts_sorting_mode);


    /*****************************************************************************************************/
    /* General options */
    /*****************************************************************************************************/

    general_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), general_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Appearance"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox8 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox8);
    gtk_container_add (GTK_CONTAINER (alignment), vbox8);

    default_stock_icons_checkbutton = gtk_check_button_new_with_mnemonic (_("Use default stock icons (needs restart)"));
    GTK_WIDGET_UNSET_FLAGS(default_stock_icons_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (default_stock_icons_checkbutton);
    g_signal_connect (G_OBJECT (default_stock_icons_checkbutton), "toggled",
                      G_CALLBACK (default_stock_icons_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox8), default_stock_icons_checkbutton, FALSE, FALSE, 4);

    disable_underline_in_links_checkbutton = gtk_check_button_new_with_mnemonic (_("Disable underline in links"));
    GTK_WIDGET_UNSET_FLAGS(disable_underline_in_links_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (disable_underline_in_links_checkbutton);
    g_signal_connect (G_OBJECT (disable_underline_in_links_checkbutton), "toggled",
                      G_CALLBACK (disable_underline_in_links_changed_cb), NULL);
    gtk_box_pack_start (GTK_BOX (vbox8), disable_underline_in_links_checkbutton, FALSE, FALSE, 4);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("General"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (alignment), vbox4);

    table = gtk_table_new (3, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox4), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Date format"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    date_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (date_format_combobox);
    g_signal_connect (G_OBJECT (date_format_combobox), "changed",
                      G_CALLBACK(date_format_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), date_format_combobox, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_combo_box_append_text (GTK_COMBO_BOX (date_format_combobox), _("DD-MM-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (date_format_combobox), _("MM-DD-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (date_format_combobox), _("YYYY-MM-DD"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (date_format_combobox), _("YYYY-DD-MM"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(date_format_combobox), config.date_format);


    enable_tooltips_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable tooltips"));
    GTK_WIDGET_UNSET_FLAGS(enable_tooltips_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (enable_tooltips_checkbutton);
    g_signal_connect (G_OBJECT (enable_tooltips_checkbutton), "toggled",
                      G_CALLBACK (enable_tooltips_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), enable_tooltips_checkbutton, 0, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    remember_latest_tab_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember last selected page"));
    GTK_WIDGET_UNSET_FLAGS(remember_latest_tab_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (remember_latest_tab_checkbutton);
    g_signal_connect (G_OBJECT (remember_latest_tab_checkbutton), "toggled",
                      G_CALLBACK (remember_latest_tab_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), remember_latest_tab_checkbutton, 0, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Helpers"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox9 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox9);
    gtk_container_add (GTK_CONTAINER (alignment), vbox9);

    table = gtk_table_new (2, 2, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox9), table, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    label = gtk_label_new (_("Web browser:"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("E-mail client:"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    entry_web_browser = gtk_entry_new ();
    gtk_widget_show (entry_web_browser);
    g_signal_connect (G_OBJECT (entry_web_browser), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), entry_web_browser, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (entry_web_browser), MAXHELPERCMD);

    entry_email_client = gtk_entry_new ();
    gtk_widget_show (entry_email_client);
    g_signal_connect (G_OBJECT (entry_email_client), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), entry_email_client, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (entry_email_client), MAXHELPERCMD);

    /*****************************************************************************************************/

    callback_active = FALSE;

    if(config.display_options & GUI_CALENDAR_SHOW_DAY_NAMES)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(show_day_names_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(no_month_change_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_SHOW_WEEK_NUMBERS)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(show_week_numbers_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_WEEK_START_MONDAY)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(week_start_monday_checkbutton), TRUE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(simple_view_in_fy_calendar_checkbutton), config.fy_simple_view);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(show_notes_in_info_panel_checkbutton), config.notes_in_info_panel);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(default_stock_icons_checkbutton), config.default_stock_icons);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(disable_underline_in_links_checkbutton), config.disable_underline_links);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(enable_tooltips_checkbutton), config.enable_tooltips);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(remember_latest_tab_checkbutton), config.remember_latest_tab);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(tasks_enable_rules_hint_checkbutton), config.tasks_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ct_hide_items_checkbutton), config.hide_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ct_delete_items_checkbutton), config.delete_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(vc_due_date_checkbutton), config.visible_due_date_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(vc_priority_checkbutton), config.visible_priority_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(vc_category_checkbutton), config.visible_category_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(contacts_enable_rules_hint_checkbutton), config.contacts_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(contacts_select_first_entry_checkbutton), config.show_after_search);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(contacts_hide_group_column_checkbutton), config.hide_group_column);
    gtk_tree_view_column_set_visible (contacts_columns[COLUMN_GROUP], !config.hide_group_column);

    gtk_entry_set_text (GTK_ENTRY(day_name_font_entry), config.day_name_font);
    gtk_entry_set_text (GTK_ENTRY(calendar_font_entry), config.calendar_font);
    gtk_entry_set_text (GTK_ENTRY(notes_font_entry), config.notes_font);

    gtk_entry_set_text (GTK_ENTRY(ti_font_entry), config.task_info_font);

    gtk_entry_set_text (GTK_ENTRY(entry_web_browser), config.web_browser);
    gtk_entry_set_text (GTK_ENTRY(entry_email_client), config.email_client);

    gtk_spin_button_set_value (GTK_SPIN_BUTTON(name_font_size_spinbutton), config.name_font_size);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(tags_font_size_spinbutton), config.tags_font_size);

    fill_timeline();

    callback_active = TRUE;

    gtk_widget_show (calendar_vbox);
}


