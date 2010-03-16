
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
#include <glib/gprintf.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#include "i18n.h"
#include "gui.h"
#include "gui_utils.h"
#include "contacts.h"
#include "contacts_import.h"
#include "calendar_utils.h"
#include "stock_icons.h"
#include "options_prefs.h"


extern gchar            *contact_fields_tags_name[];
extern GtkWidget        *main_window;
extern gchar            tmpbuf[];
extern GtkListStore     *contacts_list_store;
extern GtkWidget        *contacts_list;
extern GtkTreeModel     *contacts_filter;
extern GtkTreeModel     *contacts_sort;
extern GtkListStore     *contacts_group_store;

GtkWidget               *import_window;
GtkWidget               *value_labels[CONTACTS_NUM_COLUMNS];
GtkWidget               *field_type_comboboxes[CONTACTS_NUM_COLUMNS];
GtkWidget               *first_row_as_header_check_button;
GtkWidget               *current_record_spinbutton;
GtkObject               *current_record_spinbutton_adj;
GtkWidget               *n_records_label;

gint                    field_type[CONTACTS_NUM_COLUMNS];
guint                   max_fields;
gchar                   *file_buffer;
guint                   file_length;

/*-------------------------------------------------------------------------------------*/


gboolean
import_contacts_from_csv_file (void) {

GtkWidget *dialog;
gboolean ret = FALSE;
GtkFileFilter *filter_1, *filter_2;

    dialog = gtk_file_chooser_dialog_new(_("Select CSV file"),
                                         GTK_WINDOW(main_window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
                                         NULL);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

    filter_1 = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_1, "*");
    gtk_file_filter_set_name(GTK_FILE_FILTER(filter_1), _("All Files"));
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_1);

    filter_2 = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_2, "*.[cC][sS][vV]");
    gtk_file_filter_set_name(GTK_FILE_FILTER(filter_2), _("CSV (comma-separated values) files (*.csv)"));
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_2);

    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter_2);


    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {

        gtk_widget_hide(dialog);
        while (g_main_context_iteration(NULL, FALSE));

        ret = add_csv_records(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));

    }

    gtk_widget_destroy(dialog);

    return ret;
}

/*------------------------------------------------------------------------------*/

gchar *
csv_get_line (guint line) {

gchar *line_buffer;
gint c, i, n;
gboolean line_found = FALSE, quotation_found;

    line_buffer = NULL;

    i = 1;
    n = 0;
    quotation_found = FALSE;

    while (n != file_length) {
        c = file_buffer[n];
        if (i == line) {
            line_found = TRUE;
            break;
        }
        if (c == '"') {
            quotation_found = !quotation_found;
        }
        if (c == '\n' && quotation_found == FALSE) i++;
        n++;
    }

    if (line_found == TRUE) {

        line_buffer = g_malloc0 (MAX_LINE_LENGTH);
        if (line_buffer != NULL) {

            /* get single or multi line */

            quotation_found = FALSE;
            i = 0;
            do {
                c = file_buffer[n];

                if (c == '"') {
                    quotation_found = !quotation_found;
                }

                if (c == '\n' && quotation_found == FALSE) break;

                line_buffer[i++] = c;
                n++;

            } while (n != file_length && i < MAX_LINE_LENGTH);

            line_buffer[i] = 0;
        }
    }

    return line_buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
csv_get_field (gchar *line_buffer, guint field) {

gchar *field_buffer;
gint i, j, current_field;
gboolean quotation_found;


    field_buffer = g_malloc0 (MAX_FIELD_LENGTH);
    if (field_buffer != NULL) {

        quotation_found = FALSE;
        i = j = 0;
        current_field = 1;
        
        while (line_buffer[i]) {

            if (j >= MAX_FIELD_LENGTH-1) break;

            if (line_buffer[i] == '"' && line_buffer[i+1] == '"') {
                field_buffer[j++] = '"';
                i++;
            } else if (line_buffer[i] == '"' && line_buffer[i+1] != '"') {
                quotation_found = !quotation_found;
            } else if (line_buffer[i] == FIELD_SEPARATOR) {
                if (quotation_found == FALSE) {
                    if (current_field == field) break;
                    current_field++;
                    j = 0;
                } else {
                    /* ignore white chars */
                    field_buffer[j++] = line_buffer[i];
                    if (j == 1 && (line_buffer[i] == ' ' || line_buffer[i] == '\t')) --j;
                    if (i > 0 && line_buffer[i] != 0) {
                        if (line_buffer[i] == '"' && line_buffer[i-1] == FIELD_SEPARATOR) --j;
                        if (line_buffer[i] == '"' && line_buffer[i+1] == 0) --j;
                    }
                }
            } else {
                /* ignore white chars */
                field_buffer[j++] = line_buffer[i];
                if (j == 1 && (line_buffer[i] == ' ' || line_buffer[i] == '\t')) --j;
                if (i > 0 && line_buffer[i] != 0) {
                    if (line_buffer[i] == '"' && line_buffer[i-1] == FIELD_SEPARATOR) --j;
                    if (line_buffer[i] == '"' && line_buffer[i+1] == 0) --j;
                }
            }

            i++;
        }

        field_buffer[j] = 0;

        if (current_field != field) {
            g_free(field_buffer);
            field_buffer = NULL;
        }
    }

    return field_buffer;
}

/*------------------------------------------------------------------------------*/

guint
get_number_of_records (void) {

guint lines = 0, i;
gboolean quotation_found = FALSE;

    for (i=0; i != file_length; i++) {
       if (file_buffer[i] == '"') {
            quotation_found = !quotation_found;
       }
       if (file_buffer[i] == '\n' && quotation_found == FALSE) {
           lines++;
       }
    }

    return lines;
}

/*------------------------------------------------------------------------------*/

void
import_window_close_cb (GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(import_window);

    if(file_buffer != NULL) {
        g_free(file_buffer);
        file_buffer = NULL;
        file_length = 0;
    }
}

/*------------------------------------------------------------------------------*/

gint
import_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    switch(event->keyval) {
        case GDK_Escape:
            gtk_widget_destroy(import_window);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
record_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

gint i;
gchar *str, *field_str;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(first_row_as_header_check_button)) == TRUE) {
        str = csv_get_line(gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinbutton))+1);
    } else {
        str = csv_get_line(gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinbutton)));
    }
    if (str != NULL) {
        i = 1;

        while ((field_str = csv_get_field(str, i)) != NULL) {
            gtk_label_set_text (GTK_LABEL(value_labels[i-1]), field_str);
            g_free(field_str);
            i++;
        }

        g_free(str);
    }
}

/*------------------------------------------------------------------------------*/

void
type_changed_cb (GtkComboBox *widget, gpointer user_data) {

gint i, k;

    k = gtk_combo_box_get_active (GTK_COMBO_BOX(field_type_comboboxes[(gint)user_data]));
    field_type[(gint)user_data] = k;

    for(i=0; i < max_fields; i++) {
        if (i != (gint)user_data) {
            if (k == gtk_combo_box_get_active (GTK_COMBO_BOX(field_type_comboboxes[i]))) {
                gtk_combo_box_set_active (GTK_COMBO_BOX (field_type_comboboxes[i]), 0);
                field_type[i] = 0;
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
start_import_cb (GtkWidget *widget, gpointer data) {

gint i, j, k, n, m;
guint n_records;
gchar *str, *field_str, *item;
GtkTreeIter iter, g_iter;
gboolean g_flag;

    n_records = get_number_of_records ();

    for(i = k = 0; i < max_fields; i++) {
        k += field_type[i];
    }
    if (!k) {
        gui_create_dialog(GTK_MESSAGE_WARNING, _("Nothing to import."), GTK_WINDOW(import_window));
        return;
    }

    /* add records */

    n = (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(first_row_as_header_check_button)) == TRUE) ? 1:0;
        
    for(j=n; j < n_records; j++) {

        str = csv_get_line(j+1);
        if (str != NULL) {
            gtk_list_store_append(contacts_list_store, &iter);

            for(i=0; i < max_fields; i++) {
                if (field_type[i]) {
                    if ((field_str = csv_get_field(str, i+1)) != NULL) {

                        if (field_type[i]-1 == COLUMN_GROUP) {
                            /* set group */
                            if(strlen(field_str)) {
                                m = 0;
                                g_flag = TRUE;
                                while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_group_store), &g_iter, NULL, m++)) {
                                    gtk_tree_model_get(GTK_TREE_MODEL(contacts_group_store), &g_iter, 0, &item, -1);
                                    if (!strcmp(field_str, item)) {
                                        g_free(item);
                                        g_flag = FALSE;
                                        break;
                                    }
                                    g_free(item);
                                }
                                if (g_flag == TRUE) {
                                    gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_group_store), &g_iter, NULL, 0);
                                    gtk_list_store_append(contacts_group_store, &g_iter);
                                    gtk_list_store_set(contacts_group_store, &g_iter, 0, field_str, -1);
                                }
                                gtk_list_store_set(contacts_list_store, &iter, field_type[i]-1, field_str, -1);
                            }
                        } else if (field_type[i]-1 == COLUMN_BIRTH_DAY_DATE) {
                            /* convert birth day date field */
                            if (strlen(field_str)) {
                                gtk_list_store_set(contacts_list_store, &iter, 
                                                   field_type[i]-1, str_to_julian(field_str, DATE_FULL), -1);
                            }
                        } else if (field_type[i]-1 == COLUMN_NAME_DAY_DATE) {
                            /* convert name day date field */
                            if (strlen(field_str)) {                   
                                gtk_list_store_set(contacts_list_store, &iter, 
                                                   field_type[i]-1, str_to_julian(field_str, DATE_NAME_DAY), -1);
                            }
                        } else {
                            /* remaining fields */
                            if (strlen(field_str)) {
                                gtk_list_store_set(contacts_list_store, &iter, field_type[i]-1, field_str, -1);
                            }
                        }
                        g_free(field_str);
                    }
                }
            }
            g_free(str);
        }
    }

    if (n_records == 1) {
        sprintf(tmpbuf, "%s\n", _("One contact added"));
    } else {
        sprintf(tmpbuf, "%d %s\n", n_records-n, _("contacts added"));
    }
    gui_create_dialog(GTK_MESSAGE_INFO, tmpbuf, GTK_WINDOW(import_window));
    import_window_close_cb (NULL, NULL);
}

/*------------------------------------------------------------------------------*/

void
first_row_as_header_cb (GtkToggleButton *button, gpointer user_data) {

gint i, j;
guint n_records;
gchar *str, *field_str;

    n_records = get_number_of_records ();

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(button)) == TRUE) {
        if(n_records > 1) {
            --n_records;
        }
    } 

    GTK_ADJUSTMENT(current_record_spinbutton_adj)->upper = n_records;
    gtk_adjustment_set_value (GTK_ADJUSTMENT(current_record_spinbutton_adj), 1.0);
    sprintf(tmpbuf, "%d", n_records);
    gtk_label_set_text (GTK_LABEL(n_records_label), tmpbuf);

    record_changed_cb (GTK_SPIN_BUTTON (current_record_spinbutton), NULL);

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(button)) == TRUE) {
        str = csv_get_line(1);

        if (str != NULL) {

            for(i=0; i < max_fields; i++) {

                if ((field_str = csv_get_field(str, i+1)) != NULL) {

                    for(j=0; j < CONTACTS_NUM_COLUMNS; j++) {
                        if (j != COLUMN_PHOTO) {
                            if (!strcmp(field_str, contact_fields_tags_name[j*2])) {
                                gtk_combo_box_set_active (GTK_COMBO_BOX (field_type_comboboxes[i]), j+1);
                                field_type[i] = j+1;
                            }
                        }
                    }

                    g_free(field_str);
                }

            }

            g_free(str);
        }
    } else {
        for(i=0; i < max_fields; i++) {
            gtk_combo_box_set_active (GTK_COMBO_BOX (field_type_comboboxes[i]), 0);
            field_type[i] = 0;
        }
    }
}

/*------------------------------------------------------------------------------*/

gboolean
add_csv_records (gchar *filename) {

GtkWidget *vbox1;
GtkWidget *vbox2;
GtkWidget *hbox1;
GtkWidget *label;
GtkWidget *max_fields_label;
GtkWidget *fields_scrolledwindow;
GtkWidget *fields_table;
GtkWidget *hseparator;
GtkWidget *hbuttonbox;
GtkWidget *cancel_button;
GtkWidget *import_button;

gchar *str, *field_str;
guint record, fields, n_records, i, j;
gint win_xpos, win_ypos;

    if (g_access (filename, R_OK) != -1) {

        file_buffer = NULL;
        file_length = 0;

        if (g_file_get_contents (filename, &file_buffer, &file_length, NULL) == TRUE) {

            /* get maximum number of fields */

            n_records = get_number_of_records ();

            if (n_records) {

                record = 1;
                max_fields = 0;

                for (record = 1; record <= n_records; record++) {

                    str = csv_get_line(record);

                    if (str != NULL) {
                        fields = 0;

                        while ((field_str = csv_get_field(str, fields+1)) != NULL) {
                            g_free(field_str);
                            fields++;
                        }

                        g_free(str);

                        if (fields > max_fields) {
                            max_fields = fields;
                        }
                    }
                }

                if (max_fields > CONTACTS_NUM_COLUMNS) {
                    max_fields = CONTACTS_NUM_COLUMNS;
                }

                /* create gui */

                import_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
                gtk_window_set_title (GTK_WINDOW (import_window), _("Import contacts"));
                gtk_container_set_border_width (GTK_CONTAINER (import_window), 6);

                gtk_window_set_default_size (GTK_WINDOW(import_window), IMPORT_WINDOW_SIZE_X, IMPORT_WINDOW_SIZE_Y);
                gtk_window_set_position(GTK_WINDOW(import_window), GTK_WIN_POS_CENTER_ON_PARENT);
                gtk_window_set_transient_for(GTK_WINDOW(import_window), GTK_WINDOW(main_window));
                gtk_window_set_modal(GTK_WINDOW(import_window), TRUE);

                g_signal_connect (G_OBJECT (import_window), "key_press_event", G_CALLBACK (import_key_press_cb), NULL);

                g_signal_connect (G_OBJECT (import_window), "delete_event", G_CALLBACK(import_window_close_cb), NULL);

                vbox1 = gtk_vbox_new (FALSE, 0);
                gtk_widget_show (vbox1);
                gtk_container_add (GTK_CONTAINER (import_window), vbox1);

                vbox2 = gtk_vbox_new (FALSE, 0);
                gtk_widget_show (vbox2);
                gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

                hbox1 = gtk_hbox_new (FALSE, 0);
                gtk_widget_show (hbox1);
                gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, TRUE, 4);

                sprintf(tmpbuf, "%d", n_records);
                n_records_label = gtk_label_new (tmpbuf);
                gtk_widget_show (n_records_label);
                gtk_box_pack_end (GTK_BOX (hbox1), n_records_label, FALSE, FALSE, 0);
                gtk_misc_set_padding (GTK_MISC (n_records_label), 6, 0);

                sprintf(tmpbuf, "<b>%s</b>", _("of"));
                label = gtk_label_new (tmpbuf);
                gtk_widget_show (label);
                gtk_box_pack_end (GTK_BOX (hbox1), label, FALSE, FALSE, 0);
                gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
                gtk_misc_set_padding (GTK_MISC (label), 6, 0);

                current_record_spinbutton_adj = gtk_adjustment_new (1, 1, n_records, 1, 10, 10);
                current_record_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (current_record_spinbutton_adj), 1, 0);
                gtk_widget_show (current_record_spinbutton);
                g_signal_connect (G_OBJECT (current_record_spinbutton), "value-changed", 
                                  G_CALLBACK(record_changed_cb), NULL);
                gtk_box_pack_end (GTK_BOX (hbox1), current_record_spinbutton, FALSE, FALSE, 0);

                sprintf(tmpbuf, "<b>%s:</b>", _("Record"));
                label = gtk_label_new (tmpbuf);
                gtk_widget_show (label);
                gtk_box_pack_end (GTK_BOX (hbox1), label, FALSE, FALSE, 0);
                gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
                gtk_misc_set_padding (GTK_MISC (label), 6, 0);

                sprintf(tmpbuf, "<b>%s:</b>", _("Number fields per record"));
                label = gtk_label_new (tmpbuf);
                gtk_widget_show (label);
                gtk_box_pack_start (GTK_BOX (hbox1), label, FALSE, FALSE, 0);
                gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
                gtk_misc_set_padding (GTK_MISC (label), 6, 0);

                sprintf(tmpbuf, "%d", max_fields);
                max_fields_label = gtk_label_new (tmpbuf);
                gtk_widget_show (max_fields_label);
                gtk_box_pack_start (GTK_BOX (hbox1), max_fields_label, FALSE, FALSE, 0);
                gtk_misc_set_padding (GTK_MISC (max_fields_label), 6, 0);

                hseparator = gtk_hseparator_new ();
                gtk_widget_show (hseparator);
                gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, TRUE, 8);

                fields_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
                gtk_widget_show (fields_scrolledwindow);
                gtk_box_pack_start (GTK_BOX (vbox2), fields_scrolledwindow, TRUE, TRUE, 0);
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (fields_scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

                fields_table = gtk_table_new (max_fields, 4, FALSE);
                gtk_widget_show (fields_table);
                gtk_container_set_border_width (GTK_CONTAINER (fields_table), 4);
                gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (fields_scrolledwindow), fields_table);
                gtk_table_set_row_spacings (GTK_TABLE (fields_table), 4);
                gtk_table_set_col_spacings (GTK_TABLE (fields_table), 4);

                sprintf(tmpbuf, "%s", _("Use first record as header"));
                first_row_as_header_check_button = gtk_check_button_new_with_mnemonic (tmpbuf);
                gtk_widget_show (first_row_as_header_check_button);
                gtk_box_pack_start (GTK_BOX (vbox1), first_row_as_header_check_button, FALSE, TRUE, 4);
                GTK_WIDGET_UNSET_FLAGS(first_row_as_header_check_button, GTK_CAN_FOCUS);
                g_signal_connect (G_OBJECT (first_row_as_header_check_button), "toggled",
                                  G_CALLBACK (first_row_as_header_cb), NULL);

                hseparator = gtk_hseparator_new ();
                gtk_widget_show (hseparator);
                gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

                hbuttonbox = gtk_hbutton_box_new ();
                gtk_widget_show (hbuttonbox);
                gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
                gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
                gtk_box_set_spacing (GTK_BOX (hbuttonbox), 8);

                if (config.default_stock_icons) {
                    cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
                } else {
                    cancel_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CANCEL);
                }
                gtk_widget_show (cancel_button);
                gtk_container_add (GTK_CONTAINER (hbuttonbox), cancel_button);
                GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);
                g_signal_connect(cancel_button, "clicked", G_CALLBACK(import_window_close_cb), NULL);

                import_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CONTACTS_IMPORT);
                gtk_widget_show (import_button);
                gtk_container_add (GTK_CONTAINER (hbuttonbox), import_button);
                GTK_WIDGET_SET_FLAGS (import_button, GTK_CAN_DEFAULT);
                g_signal_connect(import_button, "clicked", G_CALLBACK(start_import_cb), NULL);

                gtk_window_get_position (GTK_WINDOW(import_window), &win_xpos, &win_ypos);
                gtk_window_move (GTK_WINDOW (import_window), win_xpos-IMPORT_WINDOW_SIZE_X/2, win_ypos-10);
                gtk_widget_show(import_window);

                gtk_widget_set_sensitive (fields_table, FALSE);
                gtk_widget_set_sensitive (current_record_spinbutton, FALSE);
                gtk_widget_set_sensitive (first_row_as_header_check_button, FALSE);
                gtk_widget_set_sensitive (cancel_button, FALSE);
                gtk_widget_set_sensitive (import_button, FALSE);

                while (g_main_context_iteration(NULL, FALSE));

                str = csv_get_line(1);

                for(i=0; i < max_fields; i++) {

                    sprintf(tmpbuf, "<b>%s:</b>", _("Field type"));
                    label = gtk_label_new (tmpbuf);
                    gtk_widget_show (label);
                    gtk_table_attach (GTK_TABLE (fields_table), label, 0, 1, i, i+1,
                                      (GtkAttachOptions) (GTK_FILL),
                                      (GtkAttachOptions) (0), 0, 0);
                    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
                    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
                    gtk_misc_set_padding (GTK_MISC (label), 8, 0);

                    sprintf(tmpbuf, "<b>%s:</b>", _("Value"));
                    label = gtk_label_new (tmpbuf);
                    gtk_widget_show (label);
                    gtk_table_attach (GTK_TABLE (fields_table), label, 2, 3, i, i+1,
                                      (GtkAttachOptions) (GTK_FILL),
                                      (GtkAttachOptions) (0), 0, 0);
                    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
                    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
                    gtk_misc_set_padding (GTK_MISC (label), 8, 0);

                    value_labels[i] = gtk_label_new ("");
                    gtk_widget_show (value_labels[i]);
                    gtk_table_attach (GTK_TABLE (fields_table), value_labels[i], 3, 4, i, i+1,
                                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                                      (GtkAttachOptions) (0), 0, 0);
                    gtk_widget_set_size_request (value_labels[i], 50, -1);
                    gtk_misc_set_alignment (GTK_MISC (value_labels[i]), 0, 0.5);
                    gtk_misc_set_padding (GTK_MISC (value_labels[i]), 8, 0);

                    field_type_comboboxes[i] = gtk_combo_box_new_text ();
                    gtk_widget_show (field_type_comboboxes[i]);
                    gtk_table_attach (GTK_TABLE (fields_table), field_type_comboboxes[i], 1, 2, i, i+1,
                                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                                      (GtkAttachOptions) (GTK_FILL), 0, 0);
                    gtk_container_set_border_width (GTK_CONTAINER (field_type_comboboxes[i]), 4);
                    sprintf(tmpbuf, "[%s]", _("None"));
                    gtk_combo_box_append_text (GTK_COMBO_BOX (field_type_comboboxes[i]), tmpbuf);

                    for(j = 0; j < CONTACTS_NUM_COLUMNS; j++) {
                        gtk_combo_box_append_text (GTK_COMBO_BOX (field_type_comboboxes[i]), 
                                                   contact_fields_tags_name[j*2]);
                    }

                    gtk_combo_box_set_active (GTK_COMBO_BOX (field_type_comboboxes[i]), 0);
                    field_type[i] = 0;
                    gtk_combo_box_set_wrap_width (GTK_COMBO_BOX (field_type_comboboxes[i]), 4);

                    g_signal_connect (G_OBJECT (field_type_comboboxes[i]), "changed", 
                                      G_CALLBACK(type_changed_cb), (gpointer)i);
        
                    if (str != NULL) {
                        if ((field_str = csv_get_field(str, i+1)) != NULL) {
                            gtk_label_set_text (GTK_LABEL(value_labels[i]), field_str);
                            g_free(field_str);
                        }
                    }

                    while (g_main_context_iteration(NULL, FALSE));
                }

                if (str != NULL) {
                    g_free(str);
                }

                gtk_widget_set_sensitive (fields_table, TRUE);
                gtk_widget_set_sensitive (current_record_spinbutton, TRUE);
                gtk_widget_set_sensitive (first_row_as_header_check_button, TRUE);
                gtk_widget_set_sensitive (cancel_button, TRUE);
                gtk_widget_set_sensitive (import_button, TRUE);

            } else {
                gui_create_dialog(GTK_MESSAGE_ERROR, _("No records found in selected file."), GTK_WINDOW(main_window));
                return FALSE;
            }

        } else {
            gui_create_dialog(GTK_MESSAGE_ERROR, _("Cannot read file."), GTK_WINDOW(main_window));
            return FALSE;
        }
    } else {
        gui_create_dialog(GTK_MESSAGE_ERROR, _("Cannot open file."), GTK_WINDOW(main_window));
        return FALSE;
    }

    return TRUE;
}

/*------------------------------------------------------------------------------*/

