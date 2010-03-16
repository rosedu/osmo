
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


struct  osmo_prefs              config;

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
ti_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.task_info_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.task_info_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->ti_font_entry), font_name);
        g_free (font_name);

        /* task info font */
        g_object_set (G_OBJECT (appGUI->tsk->font_tag_object), "font", (gchar *)config.task_info_font, NULL);
    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
options_switch_buttons(gboolean left, GUI *appGUI) {

    if (left == TRUE && appGUI->opt->options_counter > 0) {
        --appGUI->opt->options_counter;
    } else if (left == FALSE && appGUI->opt->options_counter < 3) {
        appGUI->opt->options_counter++;
    }

    if (appGUI->opt->options_counter == 0){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->calendar_options_radiobutton), "released");
    } else if (appGUI->opt->options_counter == 1) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->tasks_options_radiobutton), "released");
    } else if (appGUI->opt->options_counter == 2) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->contacts_options_radiobutton), "released");
    } else {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->general_options_radiobutton), "released");
    }
}

/*------------------------------------------------------------------------------*/

void
options_button_released_cb (GtkButton *button, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_hide (appGUI->opt->calendar_vbox);
    gtk_widget_hide (appGUI->opt->tasks_vbox);
    gtk_widget_hide (appGUI->opt->contacts_vbox);
    gtk_widget_hide (appGUI->opt->general_vbox);

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->calendar_vbox);
        appGUI->opt->options_counter = 0;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->tasks_vbox);
        appGUI->opt->options_counter = 1;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->contacts_vbox);
        appGUI->opt->options_counter = 2;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->general_vbox);
        appGUI->opt->options_counter = 3;
    }
}

/*------------------------------------------------------------------------------*/

void
hide_items_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton), FALSE);
    } 
}

/*------------------------------------------------------------------------------*/

void
delete_items_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton), FALSE);
    } 
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
tasks_category_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    GUI *appGUI = (GUI *)data;

    category_name = gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, 0, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, 0);

        gtk_list_store_append(appGUI->opt->tasks_category_store, &iter);
        gtk_list_store_set(appGUI->opt->tasks_category_store, &iter, 0, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->tasks_category_entry), "");
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, FALSE);

        create_category_combobox (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), appGUI->opt->tasks_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), 0);

        apply_date_colors(appGUI);

        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
        update_n_items(appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_category_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    GUI *appGUI = (GUI *)data;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, path);
        gtk_list_store_remove(appGUI->opt->tasks_category_store, &iter);
        gtk_tree_path_free(path);
    }
}

/*------------------------------------------------------------------------------*/

gint 
tasks_category_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    GUI *appGUI = (GUI *)data;

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, state);

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry)))) {
            tasks_category_add_cb (NULL, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
contacts_group_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    GUI *appGUI = (GUI *)data;

    category_name = gtk_entry_get_text(GTK_ENTRY(appGUI->opt->contacts_group_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->contacts_group_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->contacts_group_store), &iter, 0, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->contacts_group_store), &iter, NULL, 0);

        gtk_list_store_append(appGUI->opt->contacts_group_store, &iter);
        gtk_list_store_set(appGUI->opt->contacts_group_store, &iter, 0, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->contacts_group_entry), "");
        gtk_widget_set_sensitive(appGUI->opt->contacts_group_add_button, FALSE);

    }
}

/*------------------------------------------------------------------------------*/

void
contacts_group_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    GUI *appGUI = (GUI *)data;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->contacts_group_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->contacts_group_store), &iter, path);
        gtk_list_store_remove(appGUI->opt->contacts_group_store, &iter);
        gtk_tree_path_free(path);
    }
}

/*------------------------------------------------------------------------------*/

gint 
contacts_group_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    GUI *appGUI = (GUI *)data;

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->contacts_group_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->opt->contacts_group_add_button, state);

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->contacts_group_entry)))) {
            contacts_group_add_cb (NULL, appGUI);
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

    calendar_cursor_settings_enable_disable(appGUI);

    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar), config.display_options);
    gui_calendar_set_cursor_type (GUI_CALENDAR (appGUI->cal->calendar), config.cursor_type);
    g_signal_emit_by_name(G_OBJECT(appGUI->cal->calendar), "day-selected");

}

/*------------------------------------------------------------------------------*/

void
due_today_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->due_today_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_today_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
due_7days_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->due_7days_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_7days_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
past_due_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->past_due_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.past_due_color, tmpbuf, MAXCOLORNAME-1);

}

/*------------------------------------------------------------------------------*/

void
tasks_enable_rules_hint_checkbutton_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tasks_rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_enable_rules_hint_checkbutton));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->tsk->tasks_list), config.tasks_rules_hint);
}

/*------------------------------------------------------------------------------*/

void
visible_columns_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.visible_due_date_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_due_date_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_DUE_DATE], config.visible_due_date_column);
    config.visible_category_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_category_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_CATEGORY], config.visible_category_column);
    config.visible_priority_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_priority_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_PRIORITY], config.visible_priority_column);

}

/*------------------------------------------------------------------------------*/

void
tasks_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tasks_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_order_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_DONE, config.tasks_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
}

/*------------------------------------------------------------------------------*/

void
tasks_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tasks_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_mode_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          COLUMN_DONE, config.tasks_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));

}

/*------------------------------------------------------------------------------*/

void
hide_delete_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton));
    gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (appGUI->tsk->tasks_filter));
    config.delete_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
contact_tag_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->contact_tag_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.contact_tag_color, tmpbuf, MAXCOLORNAME-1);
    g_object_set (G_OBJECT (appGUI->cnt->contact_color_tag_object), "foreground-gdk", &color, NULL);

}

/*------------------------------------------------------------------------------*/

void
contact_link_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->contact_link_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.contact_link_color, tmpbuf, MAXCOLORNAME-1);
    g_object_set (G_OBJECT (appGUI->gui_url_tag), "foreground-gdk", &color, NULL);
    g_signal_emit_by_name(G_OBJECT(appGUI->cnt->contacts_list_selection), "changed");

}

/*------------------------------------------------------------------------------*/

void
contacts_enable_rules_hint_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.contacts_rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_enable_rules_hint_checkbutton));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->cnt->contacts_list), config.contacts_rules_hint);

}

/*------------------------------------------------------------------------------*/

void
contacts_select_first_entry_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.show_after_search = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_select_first_entry_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
contacts_hide_group_column_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.hide_group_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_hide_group_column_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[COLUMN_GROUP], !config.hide_group_column);

}

/*------------------------------------------------------------------------------*/

void
default_stock_icons_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->default_stock_icons_checkbutton)) == TRUE) {
        config.default_stock_icons = 1;
    } else {
        config.default_stock_icons = 0;
    }
}

/*------------------------------------------------------------------------------*/

void
disable_underline_in_links_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.disable_underline_links = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->disable_underline_in_links_checkbutton));
    g_signal_emit_by_name(G_OBJECT(appGUI->cnt->contacts_list_selection), "changed");
    about_set_first_page(appGUI);

}

/*------------------------------------------------------------------------------*/

void
enable_tooltips_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton)) == TRUE) {
        config.enable_tooltips = 1;
        gtk_tooltips_enable(appGUI->osmo_tooltips);
    } else {
        config.enable_tooltips = 0;
        gtk_tooltips_disable(appGUI->osmo_tooltips);
    }

    gtk_toolbar_set_tooltips (appGUI->cal->calendar_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (appGUI->tsk->tasks_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (appGUI->cnt->contacts_toolbar, config.enable_tooltips);

}

/*------------------------------------------------------------------------------*/

void
remember_latest_tab_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;
 
    config.remember_latest_tab = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->remember_latest_tab_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
helpers_entry_changed_cb (GtkEntry *entry, gpointer  user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    strncpy(config.web_browser, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_web_browser)), MAXHELPERCMD-1);
    strncpy(config.email_client, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_email_client)), MAXHELPERCMD-1);

}

/*------------------------------------------------------------------------------*/

void
name_font_size_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.name_font_size = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(appGUI->opt->name_font_size_spinbutton));

    g_object_set (G_OBJECT (appGUI->cnt->contact_name_tag_object), "size", config.name_font_size * PANGO_SCALE, NULL);

}

/*------------------------------------------------------------------------------*/

void
tags_font_size_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tags_font_size = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(appGUI->opt->tags_font_size_spinbutton));

    g_object_set (G_OBJECT (appGUI->cnt->contact_color_tag_object), "size", config.tags_font_size * PANGO_SCALE, NULL);
    g_object_set (G_OBJECT (appGUI->cnt->contact_tags_value_tag_object), "size", config.tags_font_size * PANGO_SCALE, NULL);

}

/*------------------------------------------------------------------------------*/

void
contacts_photo_size_changed_cb (GtkComboBox *widget, gpointer user_data) {

gint i;
gint sizes[] = { PHOTO_SMALL, PHOTO_MEDIUM, PHOTO_LARGE };

    GUI *appGUI = (GUI *)user_data;

    i = gtk_combo_box_get_active (GTK_COMBO_BOX(widget));

    config.photo_width = sizes[i % 3];
    g_signal_emit_by_name(appGUI->cnt->contacts_list_selection, "changed");

}

/*------------------------------------------------------------------------------*/

void
contacts_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.contacts_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->contacts_sort_order_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                         COLUMN_FIRST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                         COLUMN_LAST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                         COLUMN_GROUP, config.contacts_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));
}

/*------------------------------------------------------------------------------*/

void
contacts_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.contacts_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->contacts_sort_mode_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                          COLUMN_FIRST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                          COLUMN_LAST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->cnt->contacts_sort, 
                                          COLUMN_GROUP, config.contacts_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));
}

/*------------------------------------------------------------------------------*/

void
date_format_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.date_format = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->date_format_combobox));
    refresh_due_dates(appGUI);

}

/*------------------------------------------------------------------------------*/

void
tasks_category_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, FALSE);
    }
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
contacts_group_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(appGUI->opt->contacts_group_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->opt->contacts_group_remove_button, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void
gui_create_options(GUI *appGUI) {

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
GtkWidget           *calendar_category_table;
GtkWidget           *tasks_category_table;
GtkWidget           *contacts_group_table;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
GtkObject           *cft_hscale_adj;
gchar tmpbuf[BUFFER_SIZE];
GdkColor            color;

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    gtk_label_set_markup (GTK_LABEL (label), _("Options"));
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), vbox1, label);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    appGUI->opt->calendar_options_radiobutton = gui_stock_label_radio_button(_("Calendar"), 
                                                             OSMO_STOCK_CALENDAR, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->calendar_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->calendar_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->calendar_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->calendar_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->tasks_options_radiobutton = gui_stock_label_radio_button(_("Tasks"), 
                                                             OSMO_STOCK_TASKS, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->tasks_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->tasks_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->tasks_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->tasks_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->tasks_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->contacts_options_radiobutton = gui_stock_label_radio_button(_("Contacts"), 
                                                             OSMO_STOCK_CONTACTS, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->contacts_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->contacts_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->contacts_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->contacts_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->contacts_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->general_options_radiobutton = gui_stock_label_radio_button(_("General"), 
                                                             OSMO_STOCK_PREFERENCES, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->general_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->general_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->general_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->general_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->general_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->general_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

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

    appGUI->opt->calendar_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->opt->calendar_vbox, FALSE, FALSE, 0);

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


    /*****************************************************************************************************/
    /* TASKS options */
    /*****************************************************************************************************/

    appGUI->opt->tasks_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->opt->tasks_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->due_today_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->due_today_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->due_today_color_picker), "color-set",
                      G_CALLBACK(due_today_color_changed_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->due_today_color_picker, _("Color of items that are due today"), NULL);
    gdk_color_parse(config.due_today_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->due_today_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->due_today_color_picker, FALSE, FALSE, 0);

    appGUI->opt->due_7days_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->due_7days_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->due_7days_color_picker), "color-set",
                      G_CALLBACK(due_7days_color_changed_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->due_7days_color_picker, _("Color of items that are due in the next 7 days"), NULL);
    gdk_color_parse(config.due_7days_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->due_7days_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->due_7days_color_picker, FALSE, FALSE, 0);

    appGUI->opt->past_due_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->past_due_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->past_due_color_picker), "color-set",
                      G_CALLBACK(past_due_color_changed_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->past_due_color_picker, _("Color of items that are past due"), NULL);
    gdk_color_parse(config.past_due_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->past_due_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->past_due_color_picker, FALSE, FALSE, 0);

    appGUI->opt->ti_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ti_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ti_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->ti_font_entry, 1, 2, 1, 2,
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
                      G_CALLBACK (ti_font_select_cb), appGUI);
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

    appGUI->opt->tasks_enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Draw rows in alternating colors"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->tasks_enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (tasks_enable_rules_hint_checkbutton_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tasks_enable_rules_hint_checkbutton, 0, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->vc_due_date_checkbutton = gtk_check_button_new_with_mnemonic (_("Due date"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_due_date_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_due_date_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_due_date_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_due_date_checkbutton, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->vc_priority_checkbutton = gtk_check_button_new_with_mnemonic (_("Priority"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_priority_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_priority_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_priority_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_priority_checkbutton, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->vc_category_checkbutton = gtk_check_button_new_with_mnemonic (_("Category"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_category_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_category_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_category_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_category_checkbutton, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->tasks_category_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->tasks_category_entry);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_category_entry), "key_release_event",
                      G_CALLBACK (tasks_category_entry_key_release_cb), appGUI);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (tasks_category_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->opt->tasks_category_store = gtk_list_store_new(1, G_TYPE_STRING);

    appGUI->opt->tasks_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->opt->tasks_category_store));
    appGUI->opt->tasks_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview));
    gtk_widget_show (appGUI->opt->tasks_category_treeview);
    g_signal_connect(G_OBJECT(appGUI->opt->tasks_category_select), "changed", 
                     G_CALLBACK(tasks_category_selected_cb), appGUI);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->opt->tasks_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->tasks_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), FALSE);
    gtk_widget_set_size_request (appGUI->opt->tasks_category_treeview, -1, 80);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview), column);

    if (config.default_stock_icons) {
        appGUI->opt->tasks_category_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        appGUI->opt->tasks_category_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_category_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->tasks_category_add_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->tasks_category_add_button, "clicked", 
                     G_CALLBACK(tasks_category_add_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->tasks_category_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {
        appGUI->opt->tasks_category_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_category_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->tasks_category_remove_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->tasks_category_remove_button, "clicked", 
                     G_CALLBACK(tasks_category_remove_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->tasks_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->tasks_sort_order_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_sort_order_combobox), "changed",
                      G_CALLBACK(tasks_sort_order_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tasks_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_order_combobox), config.tasks_sorting_order);

    appGUI->opt->tasks_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->tasks_sort_mode_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_sort_mode_combobox), "changed",
                      G_CALLBACK(tasks_sort_mode_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tasks_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    sprintf (tmpbuf, "%s, %s, %s", _("Done"), _("Due date"), _("Priority"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Done"), _("Priority"), _("Due date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Priority"), _("Due date"), _("Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Priority"), _("Done"), _("Due date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Due date"), _("Priority"), _("Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Due date"), _("Done"), _("Priority"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_mode_combobox), config.tasks_sorting_mode);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->ct_hide_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Hide completed tasks"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_hide_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_hide_items_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox11), appGUI->opt->ct_hide_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK (hide_items_cb), appGUI);

    appGUI->opt->ct_delete_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Delete completed tasks without confirmation"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_delete_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_delete_items_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox11), appGUI->opt->ct_delete_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK (delete_items_cb), appGUI);


    /*****************************************************************************************************/
    /* CONTACTS options */
    /*****************************************************************************************************/

    appGUI->opt->contacts_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->opt->contacts_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->contacts_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->contact_tag_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->contact_tag_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->contact_tag_color_picker), "color-set",
                      G_CALLBACK(contact_tag_color_changed_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->contact_tag_color_picker, _("Color of contact tags"), NULL);
    gdk_color_parse(config.contact_tag_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->contact_tag_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->contact_tag_color_picker, FALSE, FALSE, 0);

    appGUI->opt->contact_link_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->contact_link_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->contact_link_color_picker), "color-set",
                      G_CALLBACK(contact_link_color_changed_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->opt->contact_link_color_picker, _("Color of links"), NULL);
    gdk_color_parse(config.contact_link_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->contact_link_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->contact_link_color_picker, FALSE, FALSE, 0);

    sprintf(tmpbuf, "%s:", _("Name font size"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->name_font_size_spinbutton_adj = gtk_adjustment_new (6, 6, 100, 1, 10, 10);
    appGUI->opt->name_font_size_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (appGUI->opt->name_font_size_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->name_font_size_spinbutton), "value-changed",
                      G_CALLBACK (name_font_size_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->name_font_size_spinbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->name_font_size_spinbutton, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (appGUI->opt->name_font_size_spinbutton), TRUE);


    sprintf(tmpbuf, "%s:", _("Tags font size"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->tags_font_size_spinbutton_adj = gtk_adjustment_new (6, 6, 100, 1, 10, 10);
    appGUI->opt->tags_font_size_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (appGUI->opt->tags_font_size_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->tags_font_size_spinbutton), "value-changed",
                      G_CALLBACK (tags_font_size_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->tags_font_size_spinbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tags_font_size_spinbutton, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (appGUI->opt->tags_font_size_spinbutton), TRUE);


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

    appGUI->opt->contacts_photo_size_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->contacts_photo_size_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_photo_size_combobox), "changed",
                      G_CALLBACK(contacts_photo_size_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->opt->contacts_photo_size_combobox, FALSE, FALSE, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_photo_size_combobox), _("Small"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_photo_size_combobox), _("Medium"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_photo_size_combobox), _("Large"));

    if (config.photo_width == PHOTO_LARGE) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->contacts_photo_size_combobox), 2);
    } else if (config.photo_width == PHOTO_MEDIUM) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->contacts_photo_size_combobox), 1);
    } else {
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->contacts_photo_size_combobox), 0);
    }

    appGUI->opt->contacts_enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Draw rows in alternating colors"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->contacts_enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (contacts_enable_rules_hint_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->contacts_enable_rules_hint_checkbutton, TRUE, TRUE, 4);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->contacts_vbox), frame, FALSE, TRUE, 0);
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


    appGUI->opt->contacts_select_first_entry_checkbutton = gtk_check_button_new_with_mnemonic (_("Select and show first item after search"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_select_first_entry_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->contacts_select_first_entry_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_select_first_entry_checkbutton), "toggled",
                      G_CALLBACK (contacts_select_first_entry_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->contacts_select_first_entry_checkbutton, TRUE, TRUE, 4);

    appGUI->opt->contacts_hide_group_column_checkbutton = gtk_check_button_new_with_mnemonic (_("Hide group column"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_hide_group_column_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->contacts_hide_group_column_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_hide_group_column_checkbutton), "toggled",
                      G_CALLBACK (contacts_hide_group_column_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->contacts_hide_group_column_checkbutton, TRUE, TRUE, 4);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->contacts_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->contacts_group_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->contacts_group_entry);
    gtk_table_attach (GTK_TABLE (contacts_group_table), appGUI->opt->contacts_group_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_group_entry), "key_release_event",
                      G_CALLBACK (contacts_group_entry_key_release_cb), appGUI);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (contacts_group_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->opt->contacts_group_store = gtk_list_store_new(1, G_TYPE_STRING);

    appGUI->opt->contacts_group_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->opt->contacts_group_store));
    appGUI->opt->contacts_group_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(appGUI->opt->contacts_group_treeview));
    gtk_widget_show (appGUI->opt->contacts_group_treeview);
    g_signal_connect(G_OBJECT(appGUI->opt->contacts_group_select), "changed", 
                     G_CALLBACK(contacts_group_selected_cb), appGUI);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->opt->contacts_group_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->contacts_group_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (appGUI->opt->contacts_group_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (appGUI->opt->contacts_group_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (appGUI->opt->contacts_group_treeview), FALSE);
    gtk_widget_set_size_request (appGUI->opt->contacts_group_treeview, -1, 80);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->contacts_group_treeview), column);

    if (config.default_stock_icons) {
        appGUI->opt->contacts_group_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        appGUI->opt->contacts_group_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_group_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->contacts_group_add_button);
    gtk_table_attach (GTK_TABLE (contacts_group_table), appGUI->opt->contacts_group_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->contacts_group_add_button, "clicked", 
                     G_CALLBACK(contacts_group_add_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->contacts_group_add_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->contacts_group_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {
        appGUI->opt->contacts_group_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_group_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->contacts_group_remove_button);
    gtk_table_attach (GTK_TABLE (contacts_group_table), appGUI->opt->contacts_group_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->contacts_group_remove_button, "clicked", 
                     G_CALLBACK(contacts_group_remove_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->contacts_group_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->contacts_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->contacts_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->contacts_sort_order_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_sort_order_combobox), "changed",
                      G_CALLBACK(contacts_sort_order_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->contacts_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->contacts_sort_order_combobox), config.contacts_sorting_order);

    appGUI->opt->contacts_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->contacts_sort_mode_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->contacts_sort_mode_combobox), "changed",
                      G_CALLBACK(contacts_sort_mode_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->contacts_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    sprintf (tmpbuf, "%s, %s, %s", _("Group"), _("First Name"), _("Last Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Group"), _("Last Name"), _("First Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Last Name"), _("First Name"), _("Group"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Last Name"), _("Group"), _("First Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("First Name"), _("Last Name"), _("Group"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("First Name"), _("Group"), _("Last Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->contacts_sort_mode_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->contacts_sort_mode_combobox), config.contacts_sorting_mode);


    /*****************************************************************************************************/
    /* General options */
    /*****************************************************************************************************/

    appGUI->opt->general_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->opt->general_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->default_stock_icons_checkbutton = gtk_check_button_new_with_mnemonic (_("Use default stock icons (needs restart)"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->default_stock_icons_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->default_stock_icons_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->default_stock_icons_checkbutton), "toggled",
                      G_CALLBACK (default_stock_icons_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->default_stock_icons_checkbutton, FALSE, FALSE, 4);

    appGUI->opt->disable_underline_in_links_checkbutton = gtk_check_button_new_with_mnemonic (_("Disable underline in links"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->disable_underline_in_links_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->disable_underline_in_links_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->disable_underline_in_links_checkbutton), "toggled",
                      G_CALLBACK (disable_underline_in_links_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->disable_underline_in_links_checkbutton, FALSE, FALSE, 4);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
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

    appGUI->opt->date_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->date_format_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->date_format_combobox), "changed",
                      G_CALLBACK(date_format_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->date_format_combobox, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("DD-MM-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("MM-DD-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-MM-DD"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-DD-MM"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->date_format_combobox), config.date_format);

    appGUI->opt->enable_tooltips_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable tooltips"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_tooltips_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->enable_tooltips_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_tooltips_checkbutton), "toggled",
                      G_CALLBACK (enable_tooltips_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_tooltips_checkbutton, 0, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->remember_latest_tab_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember last selected page"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->remember_latest_tab_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->remember_latest_tab_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->remember_latest_tab_checkbutton), "toggled",
                      G_CALLBACK (remember_latest_tab_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->remember_latest_tab_checkbutton, 0, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
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

    sprintf (tmpbuf, "%s:", _("Web browser"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("E-mail client"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->entry_web_browser = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->entry_web_browser);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_web_browser), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_web_browser, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_web_browser), MAXHELPERCMD);

    appGUI->opt->entry_email_client = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->entry_email_client);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_email_client), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_email_client, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_email_client), MAXHELPERCMD);

    /*****************************************************************************************************/

    appGUI->opt->callback_active = FALSE;

    if(config.display_options & GUI_CALENDAR_SHOW_DAY_NAMES)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_day_names_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->no_month_change_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_SHOW_WEEK_NUMBERS)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_week_numbers_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_WEEK_START_MONDAY)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->week_start_monday_checkbutton), TRUE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->simple_view_in_fy_calendar_checkbutton), config.fy_simple_view);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_notes_in_info_panel_checkbutton), config.notes_in_info_panel);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_block_cursor_checkbutton), !config.cursor_type);

    calendar_cursor_settings_enable_disable(appGUI);

    gtk_range_set_value (GTK_RANGE (appGUI->opt->cft_hscale), config.frame_cursor_thickness);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->default_stock_icons_checkbutton), config.default_stock_icons);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->disable_underline_in_links_checkbutton), config.disable_underline_links);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton), config.enable_tooltips);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->remember_latest_tab_checkbutton), config.remember_latest_tab);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_enable_rules_hint_checkbutton), config.tasks_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton), config.hide_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_delete_items_checkbutton), config.delete_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_due_date_checkbutton), config.visible_due_date_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_priority_checkbutton), config.visible_priority_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_category_checkbutton), config.visible_category_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_enable_rules_hint_checkbutton), config.contacts_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_select_first_entry_checkbutton), config.show_after_search);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_hide_group_column_checkbutton), config.hide_group_column);
    gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[COLUMN_GROUP], !config.hide_group_column);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->day_name_font_entry), config.day_name_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->calendar_font_entry), config.calendar_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->notes_font_entry), config.notes_font);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->ti_font_entry), config.task_info_font);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_web_browser), config.web_browser);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_email_client), config.email_client);

    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->name_font_size_spinbutton), config.name_font_size);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->tags_font_size_spinbutton), config.tags_font_size);

    appGUI->opt->callback_active = TRUE;

    gtk_widget_show (appGUI->opt->calendar_vbox);
}

/*------------------------------------------------------------------------------*/

