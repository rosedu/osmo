
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
#include "options_gui_calendar.h"
#include "options_gui_tasks.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"


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
gui_create_contacts_page (GtkWidget *vbox, GUI *appGUI) {

GtkWidget           *vbox8, *vbox9, *vbox10;
GtkWidget           *label;
GtkWidget           *hbox1;
GtkWidget           *frame;
GtkWidget           *table;
GtkWidget           *alignment;
GtkWidget           *scrolledwindow;
GtkWidget           *contacts_group_table;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
gchar tmpbuf[BUFFER_SIZE];
GdkColor            color;


    appGUI->opt->contacts_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), appGUI->opt->contacts_vbox, FALSE, FALSE, 0);

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

}

/*------------------------------------------------------------------------------*/

