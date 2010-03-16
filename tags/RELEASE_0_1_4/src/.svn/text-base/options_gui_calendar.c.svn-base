
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
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "options_gui.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"

/*------------------------------------------------------------------------------*/

void
day_name_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.day_name_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.day_name_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->day_name_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(appGUI->fd_day_name);
        appGUI->fd_day_name = pango_font_description_from_string(config.day_name_font);
        gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->date_label), appGUI->fd_day_name);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
calendar_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.calendar_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.calendar_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->calendar_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(appGUI->fd_calendar);
        appGUI->fd_calendar = pango_font_description_from_string(config.calendar_font);
        gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->calendar), appGUI->fd_calendar);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
notes_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.notes_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.notes_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->notes_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(appGUI->fd_notes);
        appGUI->fd_notes = pango_font_description_from_string(config.notes_font);
        gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->calendar_note_textview), appGUI->fd_notes);

    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
calendar_category_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar category_color[MAXCOLORNAME];
GdkPixbuf *image;
gchar *item_color, *item_name;
GdkColor color;

    GUI *appGUI = (GUI *)data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->day_category_color_picker), &color);
    sprintf(category_color, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);

    category_name = gtk_entry_get_text(GTK_ENTRY(appGUI->opt->calendar_category_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, 1, &item_color, 2, &item_name, -1);
            if (!strcmp(category_color, item_color)) {
                if (!strcmp(category_name, item_name)) {
                    g_free(item_name);
                    g_free(item_color);
                    return;
                }
            }
            g_free(item_color);
            g_free(item_name);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, 0);

        image = gui_create_color_swatch (category_color);

        gtk_list_store_append(appGUI->opt->calendar_category_store, &iter);
        gtk_list_store_set(appGUI->opt->calendar_category_store, &iter, 0, image, 1, category_color, 2, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->calendar_category_entry), "");
        gtk_widget_set_sensitive(appGUI->opt->calendar_category_add_button, FALSE);
        g_object_unref (image);

    }
}

/*------------------------------------------------------------------------------*/

void
color_edit_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_destroy(appGUI->opt->color_edit_window);
}

/*------------------------------------------------------------------------------*/

void
button_color_edit_close_cb (GtkWidget *widget, gpointer data) {

    color_edit_close_cb (widget, NULL, data);
}

/*------------------------------------------------------------------------------*/

void
color_edit_action_cb (GtkWidget *widget, gpointer data) {

gchar *old_color, *new_color;
GtkTreeIter iter;
GtkTreeModel *model;
GtkTreeIter p_iter;
GtkTreePath *path;
GdkColor color;
GdkPixbuf *image;
gchar tmpbuf[BUFFER_SIZE];

    GUI *appGUI = (GUI *)data;

    old_color = new_color = NULL;

    gtk_color_button_get_color (GTK_COLOR_BUTTON(appGUI->opt->color_edit_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    new_color = g_strdup(tmpbuf);

    gtk_tree_selection_get_selected (appGUI->opt->calendar_category_select, &model, &iter);
    gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, 1, &old_color, -1);

    if (old_color != NULL && new_color != NULL) {

        gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->calendar_category_treeview), &path, NULL);

        if (path != NULL) {
            gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &p_iter, path);
            gtk_list_store_remove(appGUI->opt->calendar_category_store, &p_iter);
            gtk_tree_path_free(path);
        }

        image = gui_create_color_swatch (new_color);
        gtk_list_store_append(appGUI->opt->calendar_category_store, &iter);
        gtk_list_store_set(appGUI->opt->calendar_category_store, &iter, 0, image, 1, new_color, 
                           2, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->color_edit_name_entry)), -1);
        g_object_unref (image);

        replace_note_color (old_color, new_color, appGUI);
        calendar_refresh_marks (appGUI);
    }

    if (old_color != NULL) {
        g_free(old_color);
    }
    if (new_color != NULL) {
        g_free(new_color);
    }

    color_edit_close_cb (NULL, NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

gint 
color_edit_name_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->color_edit_name_entry)))) {
        gtk_widget_set_sensitive(appGUI->opt->color_edit_ok_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->opt->color_edit_ok_button, FALSE);
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint 
color_edit_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Return:
            color_edit_action_cb (NULL, appGUI);
            return TRUE;
        case GDK_Escape:
            color_edit_close_cb (NULL, NULL, appGUI);
            return TRUE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
gui_color_edit_action (GtkTreeIter *iter, GtkTreeModel *model, GUI *appGUI) {

GtkWidget *vbox1;
GtkWidget *hseparator;
GtkWidget *hbuttonbox;
GtkWidget *cancel_button;
GtkWidget *label;
GtkWidget *frame;
GtkWidget *alignment;
GtkWidget *hbox1;
GtkWidget *hbox2;
GdkColor color;
gchar *color_val, *color_name;
gchar tmpbuf[BUFFER_SIZE];


    appGUI->opt->color_edit_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->color_edit_window), 4);
    gtk_window_set_title (GTK_WINDOW (appGUI->opt->color_edit_window), _("Modify color"));
    gtk_window_set_default_size (GTK_WINDOW(appGUI->opt->color_edit_window), 350, -1);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->opt->color_edit_window), GTK_WINDOW(appGUI->main_window));
    gtk_window_set_position(GTK_WINDOW(appGUI->opt->color_edit_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_modal(GTK_WINDOW(appGUI->opt->color_edit_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->opt->color_edit_window), "delete_event",
                      G_CALLBACK(color_edit_close_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->opt->color_edit_window), "key_press_event",
                      G_CALLBACK (color_edit_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->opt->color_edit_window), vbox1);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox2), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (alignment), hbox1);
    gtk_container_set_border_width (GTK_CONTAINER (hbox1), 4);

    appGUI->opt->color_edit_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->color_edit_picker);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->color_edit_picker, FALSE, FALSE, 0);

    sprintf(tmpbuf, "<b>%s</b>", _("Color"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (hbox2), frame, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);

    appGUI->opt->color_edit_name_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->color_edit_name_entry);
    gtk_container_add (GTK_CONTAINER (alignment), appGUI->opt->color_edit_name_entry);
    g_signal_connect (G_OBJECT (appGUI->opt->color_edit_name_entry), "key_release_event",
                      G_CALLBACK (color_edit_name_key_release_cb), appGUI);

    sprintf(tmpbuf, "<b>%s</b>", _("Name"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 4);

    if (config.default_stock_icons) {
        cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    } else {
        cancel_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CANCEL);
    }
    GTK_WIDGET_UNSET_FLAGS(cancel_button, GTK_CAN_FOCUS);
    gtk_widget_show (cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), cancel_button);
    g_signal_connect (G_OBJECT (cancel_button), "clicked",
                        G_CALLBACK (button_color_edit_close_cb), appGUI);
    
    if (config.default_stock_icons) {
        appGUI->opt->color_edit_ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
    } else {
        appGUI->opt->color_edit_ok_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_OK);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->color_edit_ok_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->color_edit_ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->color_edit_ok_button);
    g_signal_connect (G_OBJECT (appGUI->opt->color_edit_ok_button), "clicked",
                        G_CALLBACK (color_edit_action_cb), appGUI);

    gtk_tree_model_get(GTK_TREE_MODEL(model), iter, 1, &color_val, 2, &color_name, -1);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->color_edit_name_entry), color_name);
    gdk_color_parse(color_val, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->color_edit_picker), &color);
    g_free(color_val);
    g_free(color_name);

    gtk_widget_show (appGUI->opt->color_edit_window);

    gtk_widget_grab_focus(appGUI->opt->color_edit_name_entry);
}

/*------------------------------------------------------------------------------*/

gint
color_edit_list_dbclick_cb (GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

GtkTreeIter iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)func_data;
    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {
        if (gtk_tree_selection_get_selected (appGUI->opt->calendar_category_select, &model, &iter)) {
            gui_color_edit_action (&iter, model, appGUI);
            return TRUE;
        }
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_category_edit_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;
gboolean state;

    GUI *appGUI = (GUI *)data;

    state = gtk_tree_selection_get_selected (appGUI->opt->calendar_category_select, &model, &iter);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_edit_button, state);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_remove_button, state);

    if (state) {
        gui_color_edit_action (&iter, model, appGUI);
    }

}

/*------------------------------------------------------------------------------*/

void
calendar_category_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    GUI *appGUI = (GUI *)data;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->calendar_category_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, path);
        gtk_list_store_remove(appGUI->opt->calendar_category_store, &iter);
        gtk_tree_path_free(path);
    }
}

/*------------------------------------------------------------------------------*/

gint 
calendar_category_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    GUI *appGUI = (GUI *)data;

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->calendar_category_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->opt->calendar_category_add_button, state);

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->calendar_category_entry)))) {
            calendar_category_add_cb (NULL, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
mark_symbol_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.mark_symbol = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->mark_symbol_combobox));
    gui_calendar_set_mark_symbol (GUI_CALENDAR (appGUI->cal->calendar), get_mark_symbol(config.mark_symbol));
    gui_calendar_set_mark_symbol (GUI_CALENDAR (appGUI->cal->calendar_prev), get_mark_symbol(config.mark_symbol));
    gui_calendar_set_mark_symbol (GUI_CALENDAR (appGUI->cal->calendar_next), get_mark_symbol(config.mark_symbol));

}

/*------------------------------------------------------------------------------*/

void
cursor_thickness_changed_cb (GtkRange *range, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    config.frame_cursor_thickness = (gint) gtk_range_get_value (GTK_RANGE (range));
    gui_calendar_set_frame_cursor_thickness (GUI_CALENDAR (appGUI->cal->calendar), config.frame_cursor_thickness);

}

/*------------------------------------------------------------------------------*/

void
header_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->header_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.header_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar), config.header_color);
    gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar_prev), config.header_color);
    gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar_next), config.header_color);

}

/*------------------------------------------------------------------------------*/

void
weekend_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->weekend_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.weekend_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar), config.weekend_color);
    gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar_prev), config.weekend_color);
    gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar_next), config.weekend_color);

}

/*------------------------------------------------------------------------------*/

void
selection_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->selection_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.selection_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar), config.selection_color);
    gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar_prev), config.selection_color);
    gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar_next), config.selection_color);

}

/*------------------------------------------------------------------------------*/

void
mark_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->mark_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.mark_color, tmpbuf, MAXCOLORNAME-1);
    gui_calendar_set_tc_mark_color (GUI_CALENDAR (appGUI->cal->calendar), config.mark_color);
    gui_calendar_set_tc_mark_color (GUI_CALENDAR (appGUI->cal->calendar_prev), config.mark_color);
    gui_calendar_set_tc_mark_color (GUI_CALENDAR (appGUI->cal->calendar_next), config.mark_color);

}

/*------------------------------------------------------------------------------*/

void
calendar_cursor_settings_enable_disable(GUI *appGUI) {

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_block_cursor_checkbutton)) == TRUE) {
        gtk_widget_hide (appGUI->opt->cft_hscale);
        gtk_widget_hide (appGUI->opt->cft_label_1);
        gtk_widget_hide (appGUI->opt->cft_label_2);
        gtk_widget_hide (appGUI->opt->cft_label_3);
    } else {
        gtk_widget_show (appGUI->opt->cft_hscale);
        gtk_widget_show (appGUI->opt->cft_label_1);
        gtk_widget_show (appGUI->opt->cft_label_2);
        gtk_widget_show (appGUI->opt->cft_label_3);
    }
}

/*------------------------------------------------------------------------------*/

void
calendar_options_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_day_names_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_SHOW_DAY_NAMES;
    } else {
        config.display_options &= ~GUI_CALENDAR_SHOW_DAY_NAMES;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->no_month_change_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_NO_MONTH_CHANGE;
    } else {
        config.display_options &= ~GUI_CALENDAR_NO_MONTH_CHANGE;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_week_numbers_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_SHOW_WEEK_NUMBERS;
    } else {
        config.display_options &= ~GUI_CALENDAR_SHOW_WEEK_NUMBERS;
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->week_start_monday_checkbutton)) == TRUE) {
        config.display_options |= GUI_CALENDAR_WEEK_START_MONDAY;
    } else {
        config.display_options &= ~GUI_CALENDAR_WEEK_START_MONDAY;
    }

    config.fy_simple_view = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->simple_view_in_fy_calendar_checkbutton));
    config.notes_in_info_panel = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_notes_in_info_panel_checkbutton));
    config.cursor_type = !gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_block_cursor_checkbutton));

    config.enable_auxilary_calendars = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_auxilary_calendars_checkbutton));

    if (config.enable_auxilary_calendars == TRUE) {
        gtk_widget_show (appGUI->cal->aux_cal_expander);
    } else {
        gtk_widget_hide (appGUI->cal->aux_cal_expander);
    }

    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar_prev), 
                                      (config.display_options & (GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY)) | GUI_CALENDAR_NO_MONTH_CHANGE);
    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar_next), 
                                      (config.display_options & (GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY)) | GUI_CALENDAR_NO_MONTH_CHANGE);

    calendar_cursor_settings_enable_disable(appGUI);

    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar), config.display_options);
    gui_calendar_set_cursor_type (GUI_CALENDAR (appGUI->cal->calendar), config.cursor_type);
    g_signal_emit_by_name(G_OBJECT(appGUI->cal->calendar), "day-selected");

}

/*------------------------------------------------------------------------------*/

void
calendar_category_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;
gboolean state;

    GUI *appGUI = (GUI *)data;

    state = gtk_tree_selection_get_selected (selection, &model, &iter);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_edit_button, state);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_remove_button, state);
}

/*------------------------------------------------------------------------------*/

void
gui_create_calendar_page (GtkWidget *vbox, GUI *appGUI) {

GtkWidget           *vbox3, *vbox6, *vbox13;
GtkWidget           *label;
GtkWidget           *hbox1;
GtkWidget           *frame;
GtkWidget           *table;
GtkWidget           *alignment;
GtkWidget           *day_name_font_button;
GtkWidget           *calendar_font_button;
GtkWidget           *notes_font_button;
GtkWidget           *scrolledwindow;
GtkWidget           *calendar_category_table;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
GtkObject           *cft_hscale_adj;
gchar tmpbuf[BUFFER_SIZE];
GdkColor            color;


    appGUI->opt->calendar_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), appGUI->opt->calendar_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->calendar_vbox), frame, FALSE, TRUE, 0);
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

    table = gtk_table_new (8, 6, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox13), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    appGUI->opt->mark_symbol_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->mark_symbol_combobox);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->mark_symbol_combobox, 1, 3, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->mark_symbol_combobox), "changed",
                      G_CALLBACK(mark_symbol_changed_cb), appGUI);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Middle dot"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Apostrophe"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Asterisk"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Tilde"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Vertical line"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->mark_symbol_combobox), _("Degree sign"));

    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->mark_symbol_combobox), config.mark_symbol);

    hbox1 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (table), hbox1, 1, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->header_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (appGUI->opt->header_color_picker), "color-set",
                      G_CALLBACK(header_color_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->header_color_picker);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->header_color_picker, _("Header color"), NULL);
    gdk_color_parse(config.header_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->header_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->header_color_picker, FALSE, FALSE, 0);

    appGUI->opt->weekend_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (appGUI->opt->weekend_color_picker), "color-set",
                      G_CALLBACK(weekend_color_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->weekend_color_picker);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->weekend_color_picker, _("Weekend days color"), NULL);
    gdk_color_parse(config.weekend_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->weekend_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->weekend_color_picker, FALSE, FALSE, 0);

    appGUI->opt->selection_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (appGUI->opt->selection_color_picker), "color-set",
                      G_CALLBACK(selection_color_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->selection_color_picker);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->selection_color_picker, _("Cursor color"), NULL);
    gdk_color_parse(config.selection_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->selection_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->selection_color_picker, FALSE, FALSE, 0);

    appGUI->opt->mark_color_picker = gtk_color_button_new ();
    g_signal_connect (G_OBJECT (appGUI->opt->mark_color_picker), "color-set",
                      G_CALLBACK(mark_color_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->mark_color_picker);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->mark_color_picker, _("Marked day color"), NULL);
    gdk_color_parse(config.mark_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->mark_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->mark_color_picker, FALSE, FALSE, 0);

    appGUI->opt->day_name_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->day_name_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->day_name_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->day_name_font_entry, 1, 5, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->calendar_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->calendar_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->calendar_font_entry, 1, 5, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->notes_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->notes_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->notes_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_font_entry, 1, 5, 4, 5,
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
                      G_CALLBACK (day_name_font_select_cb), appGUI);
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
                      G_CALLBACK (calendar_font_select_cb), appGUI);
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
                      G_CALLBACK (notes_font_select_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), notes_font_button, 5, 6, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    sprintf (tmpbuf, "%s:", _("Colors"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("Date font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("Calendar font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("Note font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("Mark symbol"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);


    appGUI->opt->enable_block_cursor_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable block cursor"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_block_cursor_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_block_cursor_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->enable_block_cursor_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_block_cursor_checkbutton, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    sprintf (tmpbuf, "%s:", _("Cursor thickness"));
    appGUI->opt->cft_label_1 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->cft_label_1);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->cft_label_1, 0, 1, 6, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->cft_label_1), 0, 0.5);

    sprintf(tmpbuf, "<u>%s</u>", _("Thin"));
    appGUI->opt->cft_label_2 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->cft_label_2);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->opt->cft_label_2), TRUE);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->cft_label_2, 1, 5, 6, 7,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->cft_label_2), 0, 1.0);

    sprintf(tmpbuf, "<u>%s</u>", _("Thick"));
    appGUI->opt->cft_label_3 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->cft_label_3);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->opt->cft_label_3), TRUE);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->cft_label_3, 1, 5, 6, 7,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->cft_label_3), 1.0, 1.0);

    cft_hscale_adj = gtk_adjustment_new (1, 1, 6, 1, 1, 1);
    appGUI->opt->cft_hscale = gtk_hscale_new (GTK_ADJUSTMENT (cft_hscale_adj));
    g_signal_connect (G_OBJECT (appGUI->opt->cft_hscale), "value-changed",
                      G_CALLBACK (cursor_thickness_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->cft_hscale);
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->cft_hscale, GTK_CAN_FOCUS);
    gtk_scale_set_draw_value (GTK_SCALE (appGUI->opt->cft_hscale), FALSE);
    gtk_range_set_update_policy (GTK_RANGE (appGUI->opt->cft_hscale), GTK_UPDATE_DISCONTINUOUS);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->cft_hscale, 1, 5, 7, 8,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->calendar_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Day categories"));
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

    calendar_category_table = gtk_table_new (4, 4, FALSE);
    gtk_widget_show (calendar_category_table);
    gtk_box_pack_start (GTK_BOX (vbox6), calendar_category_table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (calendar_category_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (calendar_category_table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (calendar_category_table), 4);

    appGUI->opt->day_category_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->day_category_color_picker);
    gdk_color_parse("#a1aaaf", &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->day_category_color_picker), &color);
    gtk_table_attach (GTK_TABLE (calendar_category_table), appGUI->opt->day_category_color_picker, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->calendar_category_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->calendar_category_entry);
    gtk_table_attach (GTK_TABLE (calendar_category_table), appGUI->opt->calendar_category_entry, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->calendar_category_entry), "key_release_event",
                      G_CALLBACK (calendar_category_entry_key_release_cb), appGUI);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (calendar_category_table), scrolledwindow, 0, 5, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->opt->calendar_category_store = gtk_list_store_new(3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING);

    appGUI->opt->calendar_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->opt->calendar_category_store));
    g_signal_connect(G_OBJECT(appGUI->opt->calendar_category_treeview), "button_press_event",
                     G_CALLBACK(color_edit_list_dbclick_cb), appGUI);
    appGUI->opt->calendar_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(appGUI->opt->calendar_category_treeview));
    gtk_widget_show (appGUI->opt->calendar_category_treeview);
    g_signal_connect(G_OBJECT(appGUI->opt->calendar_category_select), "changed", 
                     G_CALLBACK(calendar_category_selected_cb), appGUI);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->opt->calendar_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->calendar_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (appGUI->opt->calendar_category_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (appGUI->opt->calendar_category_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (appGUI->opt->calendar_category_treeview), FALSE);
    gtk_widget_set_size_request (appGUI->opt->calendar_category_treeview, -1, 80);

    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "pixbuf", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->calendar_category_treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->calendar_category_treeview), column);
    gtk_tree_view_column_set_visible (column, FALSE);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->calendar_category_treeview), column);

    if (config.default_stock_icons) {
        appGUI->opt->calendar_category_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        appGUI->opt->calendar_category_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_category_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->calendar_category_add_button);
    gtk_table_attach (GTK_TABLE (calendar_category_table), appGUI->opt->calendar_category_add_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->calendar_category_add_button, "clicked", 
                     G_CALLBACK(calendar_category_add_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_add_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->calendar_category_edit_button = gui_stock_button(GTK_STOCK_EDIT, FALSE);
    } else {
        appGUI->opt->calendar_category_edit_button = gui_stock_button(OSMO_STOCK_BUTTON_EDIT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_category_edit_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->calendar_category_edit_button);
    gtk_table_attach (GTK_TABLE (calendar_category_table), appGUI->opt->calendar_category_edit_button, 3, 4, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->calendar_category_edit_button, "clicked", 
                     G_CALLBACK(calendar_category_edit_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_edit_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->calendar_category_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {
        appGUI->opt->calendar_category_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_category_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->calendar_category_remove_button);
    gtk_table_attach (GTK_TABLE (calendar_category_table), appGUI->opt->calendar_category_remove_button, 4, 5, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->calendar_category_remove_button, "clicked", 
                     G_CALLBACK(calendar_category_remove_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->calendar_category_remove_button, FALSE);



    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->calendar_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->week_start_monday_checkbutton = gtk_check_button_new_with_mnemonic (_("Week start on Monday"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->week_start_monday_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->week_start_monday_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->week_start_monday_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->week_start_monday_checkbutton, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->show_day_names_checkbutton = gtk_check_button_new_with_mnemonic (_("Show day names"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->show_day_names_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->show_day_names_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->show_day_names_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->show_day_names_checkbutton, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->no_month_change_checkbutton = gtk_check_button_new_with_mnemonic (_("No month change"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->no_month_change_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->no_month_change_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->no_month_change_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->no_month_change_checkbutton, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->show_week_numbers_checkbutton = gtk_check_button_new_with_mnemonic (_("Show week numbers"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->show_week_numbers_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->show_week_numbers_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->show_week_numbers_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->show_week_numbers_checkbutton, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->simple_view_in_fy_calendar_checkbutton = gtk_check_button_new_with_mnemonic (_("Simple view in full-year calendar"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->simple_view_in_fy_calendar_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->simple_view_in_fy_calendar_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->simple_view_in_fy_calendar_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->simple_view_in_fy_calendar_checkbutton, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->show_notes_in_info_panel_checkbutton = gtk_check_button_new_with_mnemonic (_("Show notes in info panel"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->show_notes_in_info_panel_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->show_notes_in_info_panel_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->show_notes_in_info_panel_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->show_notes_in_info_panel_checkbutton, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->enable_auxilary_calendars_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable auxilary calendars"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_auxilary_calendars_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_auxilary_calendars_checkbutton), "toggled",
                      G_CALLBACK (calendar_options_cb), appGUI);
    gtk_widget_show (appGUI->opt->enable_auxilary_calendars_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_auxilary_calendars_checkbutton, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

}

/*------------------------------------------------------------------------------*/

