
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

#include "options_gui_notes.h"
#include "utils.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "notes.h"
#include "options_gui.h"
#include "options_gui_calendar.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"

#ifdef HAVE_LIBGRINGOTTS

#include <libgringotts.h>

/*------------------------------------------------------------------------------*/

gint
get_enc_algorithm_value (void) {

gint algorithms_table [8] = {
	GRG_AES, GRG_SERPENT, GRG_TWOFISH, GRG_CAST_256, 
    GRG_SAFERPLUS, GRG_LOKI97, GRG_3DES, GRG_RIJNDAEL_256
};

    return algorithms_table[config.notes_enc_algorithm % 8];
}

gint
get_enc_hashing_value (void) {

gint hashing_table [2] = {
    GRG_SHA1, GRG_RIPEMD_160
};

    return hashing_table[config.notes_enc_hashing % 2];
}

gint
get_comp_algorithm_value (void) {

gint algorithm_table [2] = {
   	GRG_ZLIB, GRG_BZIP
};

    return algorithm_table[config.notes_comp_algorithm % 2];
}

gint
get_comp_ratio_value (void) {

gint ratio_table [4] = {
    GRG_LVL_NONE, GRG_LVL_FAST, GRG_LVL_GOOD, GRG_LVL_BEST
};

    return ratio_table[config.notes_comp_ratio % 4];
}

/*------------------------------------------------------------------------------*/

void
editor_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.notes_editor_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.notes_editor_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->editor_font_entry), font_name);
        g_free (font_name);

        pango_font_description_free(appGUI->nte->fd_notes_font);
        appGUI->nte->fd_notes_font = pango_font_description_from_string(config.notes_editor_font);
        gtk_widget_modify_font (GTK_WIDGET(appGUI->nte->editor_textview), appGUI->nte->fd_notes_font);
    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
notes_enable_rules_hint_checkbutton_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->notes_enable_rules_hint_checkbutton));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->nte->notes_list), config.notes_rules_hint);
}

/*------------------------------------------------------------------------------*/

void
notes_enc_algorithm_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_enc_algorithm = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_enc_algorithm_combobox));
}

/*------------------------------------------------------------------------------*/

void
notes_enc_hashing_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_enc_hashing = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_enc_hashing_combobox));
}

/*------------------------------------------------------------------------------*/

void
notes_comp_algorithm_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_comp_algorithm = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_comp_algorithm_combobox));
}

/*------------------------------------------------------------------------------*/

void
notes_comp_ratio_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_comp_ratio = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_comp_ratio_combobox));
}

/*------------------------------------------------------------------------------*/

void
notes_category_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(appGUI->opt->notes_category_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->opt->notes_category_remove_button, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void
notes_category_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    GUI *appGUI = (GUI *)data;

    category_name = gtk_entry_get_text(GTK_ENTRY(appGUI->opt->notes_category_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->notes_category_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->notes_category_store), &iter, 0, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->notes_category_store), &iter, NULL, 0);

        gtk_list_store_append(appGUI->opt->notes_category_store, &iter);
        gtk_list_store_set(appGUI->opt->notes_category_store, &iter, 0, category_name, -1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->notes_category_entry), "");
        gtk_widget_set_sensitive(appGUI->opt->notes_category_add_button, FALSE);

        gui_create_category_combobox (GTK_COMBO_BOX (appGUI->nte->cf_combobox), appGUI->opt->notes_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->nte->cf_combobox), 0);

    }
}

/*------------------------------------------------------------------------------*/

void
notes_category_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    GUI *appGUI = (GUI *)data;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->notes_category_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->notes_category_store), &iter, path);
        gtk_list_store_remove(appGUI->opt->notes_category_store, &iter);
        gtk_tree_path_free(path);

        gui_create_category_combobox (GTK_COMBO_BOX (appGUI->nte->cf_combobox), appGUI->opt->notes_category_store, FALSE);
        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->nte->cf_combobox), 0);
    }
}

/*------------------------------------------------------------------------------*/

gint 
notes_category_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    GUI *appGUI = (GUI *)data;

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->notes_category_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive (appGUI->opt->notes_category_add_button, state);

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text (GTK_ENTRY(appGUI->opt->notes_category_entry)))) {
            notes_category_add_cb (NULL, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void 
notes_category_cell_edited_cb (GtkCellRendererText *renderer, gchar *path, 
                               gchar *new_text, gpointer user_data) {

GtkTreeIter iter;
GtkTreeModel *model;
  
    GUI *appGUI = (GUI *)user_data;

    if (g_ascii_strcasecmp (new_text, "") != 0) {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(appGUI->opt->notes_category_treeview));
        if (gtk_tree_model_get_iter_from_string (model, &iter, path)) {
            gtk_list_store_set (appGUI->opt->notes_category_store, &iter, 0, new_text, -1);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
notes_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_sort_order_combobox));

    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->nte->notes_sort, 
                                          N_COLUMN_NAME, config.notes_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter));
}

/*------------------------------------------------------------------------------*/

void
notes_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.notes_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->notes_sort_mode_combobox));

    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->nte->notes_sort, 
                                          N_COLUMN_NAME, config.notes_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter));
}

/*------------------------------------------------------------------------------*/

void
remember_notes_category_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    config.remember_category_in_notes = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
}

/*------------------------------------------------------------------------------*/

void
use_system_date_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    config.use_system_date_in_notes = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
}

/*------------------------------------------------------------------------------*/

void
gui_create_notes_options_page (GtkWidget *vbox, GUI *appGUI) {

GtkWidget           *sorting_vbox, *appearance_vbox, *encryption_vbox;
GtkWidget           *categories_vbox, *notes_opt_vbox;
GtkWidget           *frame;
GtkWidget           *alignment;
GtkWidget           *table;
GtkWidget           *label;
GtkWidget           *editor_font_button;
GtkWidget           *notes_category_table;
GtkWidget           *scrolledwindow;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;

gchar tmpbuf[BUFFER_SIZE];


    appGUI->opt->notes_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), appGUI->opt->notes_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->notes_vbox), frame, FALSE, TRUE, 0);
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

    appearance_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (appearance_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), appearance_vbox);

    table = gtk_table_new (2, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (appearance_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Editor font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->editor_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->editor_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->editor_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->editor_font_entry, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        editor_font_button = gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        editor_font_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(editor_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (editor_font_button);
    g_signal_connect (G_OBJECT (editor_font_button), "clicked",
                      G_CALLBACK (editor_font_select_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), editor_font_button, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->notes_enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Draw rows in alternating colors"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->notes_enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->notes_enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (notes_enable_rules_hint_checkbutton_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_enable_rules_hint_checkbutton, 0, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->notes_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Encryption"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    encryption_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (encryption_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), encryption_vbox);
 
    table = gtk_table_new (1, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (encryption_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Algorithm"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Hashing"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->notes_enc_algorithm_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_enc_algorithm_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_enc_algorithm_combobox), "changed",
                      G_CALLBACK(notes_enc_algorithm_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_enc_algorithm_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "AES");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Serpent");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Twofish");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "CAST 256");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Safer+");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Loki 97");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Triple DES");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_algorithm_combobox), "Rijndael");
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_enc_algorithm_combobox), config.notes_enc_algorithm);

    appGUI->opt->notes_enc_hashing_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_enc_hashing_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_enc_hashing_combobox), "changed",
                      G_CALLBACK(notes_enc_hashing_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_enc_hashing_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_hashing_combobox), "SHA-1");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_enc_hashing_combobox), "RIPEMD-160");
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_enc_hashing_combobox), config.notes_enc_hashing);
 
    sprintf(tmpbuf, "%s:", _("Compression"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Ratio"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->notes_comp_algorithm_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_comp_algorithm_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_comp_algorithm_combobox), "changed",
                      G_CALLBACK(notes_comp_algorithm_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_comp_algorithm_combobox, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_algorithm_combobox), "ZLib");
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_algorithm_combobox), "BZip2");
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_comp_algorithm_combobox), config.notes_comp_algorithm);

    appGUI->opt->notes_comp_ratio_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_comp_ratio_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_comp_ratio_combobox), "changed",
                      G_CALLBACK(notes_comp_ratio_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_comp_ratio_combobox, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_ratio_combobox), _("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_ratio_combobox), _("Fast"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_ratio_combobox), _("Good"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_comp_ratio_combobox), _("Best"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_comp_ratio_combobox), config.notes_comp_ratio);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->notes_vbox), frame, FALSE, TRUE, 0);
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

    categories_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (categories_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), categories_vbox);

    notes_category_table = gtk_table_new (4, 3, FALSE);
    gtk_widget_show (notes_category_table);
    gtk_box_pack_start (GTK_BOX (categories_vbox), notes_category_table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (notes_category_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (notes_category_table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (notes_category_table), 4);

    appGUI->opt->notes_category_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->notes_category_entry);
    gtk_table_attach (GTK_TABLE (notes_category_table), appGUI->opt->notes_category_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_category_entry), "key_release_event",
                      G_CALLBACK (notes_category_entry_key_release_cb), appGUI);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (notes_category_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->opt->notes_category_store = gtk_list_store_new(1, G_TYPE_STRING);

    appGUI->opt->notes_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->opt->notes_category_store));
    appGUI->opt->notes_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(appGUI->opt->notes_category_treeview));
    gtk_widget_show (appGUI->opt->notes_category_treeview);
    g_signal_connect(G_OBJECT(appGUI->opt->notes_category_select), "changed", 
                     G_CALLBACK(notes_category_selected_cb), appGUI);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->opt->notes_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->notes_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (appGUI->opt->notes_category_treeview), FALSE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (appGUI->opt->notes_category_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (appGUI->opt->notes_category_treeview), FALSE);
    gtk_widget_set_size_request (appGUI->opt->notes_category_treeview, -1, 80);

    renderer = gtk_cell_renderer_text_new();
    g_object_set (renderer, "editable", TRUE, "editable-set", TRUE, NULL);
    g_signal_connect (G_OBJECT (renderer), "edited", G_CALLBACK (notes_category_cell_edited_cb), appGUI);
  
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->notes_category_treeview), column);

    if (config.default_stock_icons) {
        appGUI->opt->notes_category_add_button = gui_stock_button(GTK_STOCK_ADD, FALSE);
    } else {
        appGUI->opt->notes_category_add_button = gui_stock_button(OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->notes_category_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->notes_category_add_button);
    gtk_table_attach (GTK_TABLE (notes_category_table), appGUI->opt->notes_category_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->notes_category_add_button, "clicked", 
                     G_CALLBACK(notes_category_add_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->notes_category_add_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->notes_category_remove_button = gui_stock_button(GTK_STOCK_REMOVE, FALSE);
    } else {
        appGUI->opt->notes_category_remove_button = gui_stock_button(OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->notes_category_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->notes_category_remove_button);
    gtk_table_attach (GTK_TABLE (notes_category_table), appGUI->opt->notes_category_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->notes_category_remove_button, "clicked", 
                     G_CALLBACK(notes_category_remove_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->notes_category_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->notes_vbox), frame, FALSE, TRUE, 0);
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

    sorting_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (sorting_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), sorting_vbox);

    table = gtk_table_new (1, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (sorting_vbox), table, FALSE, FALSE, 0);
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

    appGUI->opt->notes_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_sort_order_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_sort_order_combobox), "changed",
                      G_CALLBACK(notes_sort_order_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_sort_order_combobox), config.notes_sorting_order);

    appGUI->opt->notes_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->notes_sort_mode_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->notes_sort_mode_combobox), "changed",
                      G_CALLBACK(notes_sort_mode_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->notes_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    sprintf (tmpbuf, "%s, %s, %s", _("Name"), _("Last changes"), _("Category"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Name"), _("Category"), _("Last changes"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Category"), _("Last changes"), _("Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Category"), _("Name"), _("Last changes"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Last changes"), _("Category"), _("Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Last changes"), _("Name"), _("Category"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->notes_sort_mode_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->notes_sort_mode_combobox), config.notes_sorting_mode);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->notes_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Notes options"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    notes_opt_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (notes_opt_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), notes_opt_vbox);

    appGUI->opt->cn_remember_category_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember the last selected category"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->cn_remember_category_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->cn_remember_category_checkbutton);
    gtk_box_pack_start (GTK_BOX (notes_opt_vbox), appGUI->opt->cn_remember_category_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->cn_remember_category_checkbutton), "toggled",
                      G_CALLBACK (remember_notes_category_cb), appGUI);

    appGUI->opt->cn_use_system_date_checkbutton = gtk_check_button_new_with_mnemonic (_("Use system format for date and time"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->cn_use_system_date_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->cn_use_system_date_checkbutton);
    gtk_box_pack_start (GTK_BOX (notes_opt_vbox), appGUI->opt->cn_use_system_date_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->cn_use_system_date_checkbutton), "toggled",
                      G_CALLBACK (use_system_date_cb), appGUI);
}

/*------------------------------------------------------------------------------*/

#endif  /* HAVE_LIBGRINGOTTS */

