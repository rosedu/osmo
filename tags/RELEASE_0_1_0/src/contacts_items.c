
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
#include <glib.h>
#include <time.h>
#include <ctype.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "i18n.h"
#include "contacts.h"
#include "contacts_items.h"
#include "tasks.h"
#include "gui.h"
#include "gui_utils.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_utils.h"
#include "calendar_widget.h"
#include "options_prefs.h"
#include "stock_icons.h"


extern GtkWidget        *main_window;
extern GtkWidget        *calendar;
extern gchar            tmpbuf[];
extern GtkTreeModel     *contacts_sort;
extern GtkTreeModel     *contacts_filter;
extern gchar            *contact_fields_tags_name[];
extern GtkListStore     *contacts_group_store;
extern GtkTreeSelection *contacts_list_selection; 
extern GtkListStore     *contacts_list_store;
extern GtkWidget        *contacts_list;
extern gboolean         contacts_filter_disabled;

GtkWidget               *contacts_add_window;
GtkWidget               *contacts_ok_button;
GtkWidget               *add_info_textview;
GtkWidget               *photo_browse_button;
GtkWidget               *birth_day_date_button;
GtkWidget               *name_day_date_button;
GtkWidget               *combobox_group;
GtkWidget               *contact_entries[CONTACTS_NUM_COLUMNS];
GtkWidget               *second_name_label;
GtkWidget               *select_date_window;
GtkWidget               *select_date_calendar;
gboolean                contacts_accept_state, contacts_edit_state;
guint32                 birthday_date, nameday_date;

/*------------------------------------------------------------------------------*/

void
contacts_item_entered_cb (GtkWidget *widget, gpointer data) {

GtkTextBuffer *text_buffer;
GtkTextIter iter_a, iter_b;
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter iter;
gint i;
gboolean free_str;
const gchar *text;

    contacts_filter_disabled = TRUE;

    if (contacts_edit_state == TRUE) {

        gtk_tree_view_get_cursor (GTK_TREE_VIEW (contacts_list), &sort_path, NULL);

        if (sort_path != NULL) {

            filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(contacts_sort), sort_path);

            if (filter_path != NULL) {

                path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(contacts_filter), filter_path);

                if (path != NULL) {
                    gtk_tree_model_get_iter(GTK_TREE_MODEL(contacts_list_store), &iter, path);
                    gtk_list_store_remove(contacts_list_store, &iter);
                    gtk_tree_path_free(path);
                }
                
                gtk_tree_path_free(filter_path);
            }

            gtk_tree_path_free(sort_path);
        }
    }
    
    gtk_list_store_append(contacts_list_store, &iter);

    for(i=0; i<CONTACTS_NUM_COLUMNS; i++) {

        text = NULL;
        free_str = FALSE;

        if (i == COLUMN_BIRTH_DAY_DATE) {
            if (birthday_date) {
                gtk_list_store_set(contacts_list_store, &iter, i, birthday_date, -1);
            }
        } else  if (i == COLUMN_NAME_DAY_DATE) {
            if (nameday_date) {
                gtk_list_store_set(contacts_list_store, &iter, i, nameday_date, -1);
            }
        } else {
        
            if (i == COLUMN_GROUP) {
                text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_group));
                free_str = TRUE;
            } else if (i == COLUMN_INFO) {
                text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(add_info_textview));
                gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &iter_a, 0);
                gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &iter_b, -1);
                text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(text_buffer), &iter_a, &iter_b, TRUE);
                free_str = TRUE;
            } else {
                text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
            }

            if (text != NULL) {
                if (strlen(text)) {
                    gtk_list_store_set(contacts_list_store, &iter, i, text, -1);
                    if (free_str == TRUE) {
                        g_free((gchar *)text);
                    }
                }
            }
        }
    }

    gtk_widget_destroy(contacts_add_window);

    path = gtk_tree_model_get_path (GTK_TREE_MODEL(contacts_list_store), &iter);
    if (path != NULL) {
        gtk_tree_view_set_cursor (GTK_TREE_VIEW (contacts_list), path, NULL, FALSE);
        gtk_tree_path_free(path);
    }

    contacts_filter_disabled = FALSE;
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_FIRST_NAME, config.contacts_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_LAST_NAME, config.contacts_sorting_order);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)contacts_sort, COLUMN_GROUP, config.contacts_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(contacts_filter));
    g_signal_emit_by_name(G_OBJECT(calendar), "day-selected");

    set_export_active();
}

/*------------------------------------------------------------------------------*/

void
contacts_remove_dialog_show (GtkWidget *list, GtkListStore *list_store) {

GtkWidget *info_dialog, *label;
gint response;
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter iter;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (list), &sort_path, NULL);

    if (sort_path != NULL) {

        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(contacts_sort), sort_path);

        if (filter_path != NULL) {

            path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(contacts_filter), filter_path);

            if (path != NULL) {

                sprintf(tmpbuf, "%s\n\n%s", _("Selected contact will be removed."), _("Are you sure ?"));

                if (config.default_stock_icons) {
                    info_dialog = gtk_message_dialog_new (GTK_WINDOW(main_window), 
                                                          GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, tmpbuf);
                    gtk_window_set_title(GTK_WINDOW(info_dialog), _("Remove contact"));
                } else {
                    info_dialog = gtk_dialog_new_with_buttons (_("Remove contact"), GTK_WINDOW(main_window), 
                                                          GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                          OSMO_STOCK_BUTTON_NO, GTK_RESPONSE_NO, 
                                                          OSMO_STOCK_BUTTON_YES, GTK_RESPONSE_YES, NULL);
                    gtk_window_set_default_size (GTK_WINDOW(info_dialog), 360, 150);
                    label = gtk_label_new (tmpbuf);
                    gtk_widget_show (label);
                    gtk_box_pack_start (GTK_BOX (GTK_DIALOG(info_dialog)->vbox), label, TRUE, TRUE, 0);
                }

                gtk_widget_show (info_dialog);
                response = gtk_dialog_run(GTK_DIALOG(info_dialog));
                gtk_widget_destroy(info_dialog);

                if (response == GTK_RESPONSE_YES) {   
                    gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path);
                    gtk_list_store_remove(list_store, &iter);
                    gtk_tree_path_free(path);
                    contacts_select_first_position_in_list();
                    g_signal_emit_by_name(G_OBJECT(calendar), "day-selected");
                    set_export_active();
                }
            }

            gtk_tree_path_free(filter_path);
        }

        gtk_tree_path_free(sort_path);
    }
}

/*------------------------------------------------------------------------------*/

void
contacts_add_edit_window_close_cb (GtkWidget *widget, gpointer data) {

    gdk_window_get_root_origin (contacts_add_window->window,
                        &config.contacts_add_edit_window_x, &config.contacts_add_edit_window_y);
    gtk_widget_destroy(contacts_add_window);
}

/*------------------------------------------------------------------------------*/

gint 
contacts_add_edit_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    switch(event->keyval) {

        case GDK_Escape:
            contacts_add_edit_window_close_cb (NULL, NULL);
            return TRUE;
    }

    if(strlen(gtk_entry_get_text(GTK_ENTRY(contact_entries[COLUMN_FIRST_NAME]))) || 
       strlen(gtk_entry_get_text(GTK_ENTRY(contact_entries[COLUMN_LAST_NAME])))) {
        contacts_accept_state = TRUE;
    } else {
        contacts_accept_state = FALSE;
    }

    gtk_widget_set_sensitive(contacts_ok_button, contacts_accept_state);

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint 
show_hide_fields_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gint i;
const gchar *text;

    i = (gint) data;

    if (event->keyval == GDK_Return) {

        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));

        if(strlen(text) && i != COLUMN_HOME_PHONE_4 && i != COLUMN_WORK_PHONE_4 &&
           i!= COLUMN_CELL_PHONE_4 && i != COLUMN_EMAIL_4 && i != COLUMN_WWW_4) {
            gtk_widget_show (contact_entries[i+1]);
            gtk_widget_grab_focus (contact_entries[i+1]);
            if (i+1 == COLUMN_SECOND_NAME) {
                gtk_widget_show (second_name_label);
            }
        }

        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean    
group_combo_box_focus_cb (GtkWidget *widget, GtkDirectionType *arg1, gpointer user_data) {
    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
group_combobox_changed_cb (GtkComboBox *widget, gpointer user_data) {
    gtk_widget_grab_focus (contact_entries[COLUMN_FIRST_NAME]);
}

/*------------------------------------------------------------------------------*/

void
select_date_close_cb (GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(select_date_window);
}

/*------------------------------------------------------------------------------*/

void
select_date_selected_cb (GuiCalendar *s_calendar, gpointer user_data) {

guint day, month, year;
gchar *temp;
guint32 julian_day;

    gui_calendar_get_date (s_calendar, &year, &month, &day);
    gtk_widget_destroy(select_date_window);
    
    julian_day = date_to_julian (day, month, year);

    if (user_data == (gpointer)COLUMN_BIRTH_DAY_DATE) {
        temp = julian_to_str (julian_day, DATE_FULL);
        gtk_entry_set_text (GTK_ENTRY(contact_entries[COLUMN_BIRTH_DAY_DATE]), temp);
        birthday_date = julian_day;
    } else if (user_data == (gpointer)COLUMN_NAME_DAY_DATE) {
        temp = julian_to_str (julian_day, DATE_NAME_DAY);
        gtk_entry_set_text (GTK_ENTRY(contact_entries[COLUMN_NAME_DAY_DATE]), temp);
        nameday_date = julian_day;
    }   
}

/*------------------------------------------------------------------------------*/

void
select_date_nodate_cb (GuiCalendar *s_calendar, gpointer user_data) {

    gtk_widget_destroy(select_date_window);

    if (user_data == (gpointer)COLUMN_BIRTH_DAY_DATE) {
        gtk_entry_set_text (GTK_ENTRY(contact_entries[COLUMN_BIRTH_DAY_DATE]), "");
        birthday_date = 0;
    } else if (user_data == (gpointer)COLUMN_NAME_DAY_DATE) {
        gtk_entry_set_text (GTK_ENTRY(contact_entries[COLUMN_NAME_DAY_DATE]), "");
        nameday_date = 0;
    }   

}

/*------------------------------------------------------------------------------*/

void
date_select_cb (GtkWidget *widget, gpointer user_data) {

GDate *cdate = NULL;
GtkWidget *vbox1;
GtkWidget *hbox1;
GtkWidget *alignment;
GtkWidget *nodate_button;
GtkWidget *close_button;

    select_date_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(select_date_window), GTK_WIN_POS_MOUSE);
   	gtk_window_set_modal(GTK_WINDOW(select_date_window), TRUE);
   	gtk_window_set_transient_for(GTK_WINDOW(select_date_window), GTK_WINDOW(contacts_add_window));
    gtk_window_set_decorated(GTK_WINDOW(select_date_window), FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (select_date_window), vbox1);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
	gtk_box_pack_start (GTK_BOX (vbox1), alignment, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);

    select_date_calendar = gui_calendar_new();
    gtk_widget_show(select_date_calendar);
  	g_signal_connect (GTK_OBJECT (select_date_calendar), "day_selected_double_click",
                      G_CALLBACK (select_date_selected_cb), user_data);
    gtk_container_add (GTK_CONTAINER (alignment), select_date_calendar);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
	gtk_box_pack_start (GTK_BOX (vbox1), alignment, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);

    hbox1 = gtk_hbox_new (FALSE, 2);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (alignment), hbox1);

    nodate_button = gtk_button_new_with_mnemonic (_("No date"));
    gtk_widget_show (nodate_button);
    gtk_box_pack_start (GTK_BOX (hbox1), nodate_button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (nodate_button), "clicked",
                        G_CALLBACK (select_date_nodate_cb), user_data);

    if (config.default_stock_icons) {
        close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
    } else {
        close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    gtk_button_set_relief (GTK_BUTTON(close_button), GTK_RELIEF_NONE);
    gtk_widget_show (close_button);
    gtk_box_pack_end (GTK_BOX (hbox1), close_button, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (close_button), "clicked",
                        G_CALLBACK (select_date_close_cb), NULL);

    gtk_widget_show (select_date_window);

    gui_calendar_set_display_options (GUI_CALENDAR (select_date_calendar), 
                                      GUI_CALENDAR_SHOW_HEADING | GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY);

    if (user_data == (gpointer)COLUMN_BIRTH_DAY_DATE) {
        if (g_date_valid_julian(birthday_date)) {
            cdate = g_date_new_julian (birthday_date);
            if (cdate != NULL) {
                gui_calendar_select_month(GUI_CALENDAR(select_date_calendar), g_date_get_month(cdate)-1, g_date_get_year(cdate));
                gui_calendar_select_day(GUI_CALENDAR(select_date_calendar), g_date_get_day(cdate));
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
photo_file_select_cb (GtkWidget *widget, gpointer user_data) {

GtkWidget *dialog;

        dialog = gtk_file_chooser_dialog_new(_("Select photo"), 
                                             GTK_WINDOW(main_window), 
                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 
                                             GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
                                             NULL);

        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
        gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);
        gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            gtk_entry_set_text (GTK_ENTRY(contact_entries[COLUMN_PHOTO]), gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
        }

        gtk_widget_destroy(dialog);
}

/*------------------------------------------------------------------------------*/

void
show_hide_multiple_fields(void) {

gint i;
const gchar *text;

    for(i=COLUMN_HOME_PHONE_2; i<COLUMN_WORK_PHONE_1; i++) {
        gtk_widget_hide (contact_entries[i]);
        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
        if (!strlen(text)) break;
        gtk_widget_show (contact_entries[i]);
    }

    for(i=COLUMN_WORK_PHONE_2; i<COLUMN_CELL_PHONE_1; i++) {
        gtk_widget_hide (contact_entries[i]);
        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
        if (!strlen(text)) break;
        gtk_widget_show (contact_entries[i]);
    }

    for(i=COLUMN_CELL_PHONE_2; i<COLUMN_EMAIL_1; i++) {
        gtk_widget_hide (contact_entries[i]);
        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
        if (!strlen(text)) break;
        gtk_widget_show (contact_entries[i]);
    }

    for(i=COLUMN_EMAIL_2; i<COLUMN_WWW_1; i++) {
        gtk_widget_hide (contact_entries[i]);
        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
        if (!strlen(text)) break;
        gtk_widget_show (contact_entries[i]);
    }

    for(i=COLUMN_WWW_2; i<COLUMN_IM_GG; i++) {
        gtk_widget_hide (contact_entries[i]);
        text = gtk_entry_get_text (GTK_ENTRY(contact_entries[i]));
        if (!strlen(text)) break;
        gtk_widget_show (contact_entries[i]);
    }

}

/*------------------------------------------------------------------------------*/

void
contacts_add_edit_dialog_show (gboolean contacts_edit_mode) {

GtkWidget       *vbox1;
GtkWidget       *vbox2;
GtkWidget       *hseparator;
GtkWidget       *hbuttonbox;
GtkWidget       *table;
GtkWidget       *cancel_button;
GtkWidget       *contacts_add_scrolledwindow;
GtkWidget       *viewport;
GtkWidget       *label;
GtkWidget       *scrolledwindow;
GtkTreeIter     iter;
GtkTreeModel    *model;  
GtkTextIter     titer;
GtkTextBuffer   *text_buffer;
gchar           *text;
gint            pos, i;
guint32         date;


	contacts_add_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (contacts_add_window), 6);

    if(contacts_edit_mode == TRUE) {
        gtk_window_set_title (GTK_WINDOW (contacts_add_window), _("Edit contact"));
    } else {
        gtk_window_set_title (GTK_WINDOW (contacts_add_window), _("Add contact"));
    }

    birthday_date = nameday_date = 0;

    gtk_window_set_default_size (GTK_WINDOW(contacts_add_window), CONTACTS_ADD_EDIT_WINDOW_SIZE_X, CONTACTS_ADD_EDIT_WINDOW_SIZE_Y);
	gtk_window_set_transient_for(GTK_WINDOW(contacts_add_window), GTK_WINDOW(main_window));
	gtk_window_set_modal(GTK_WINDOW(contacts_add_window), TRUE);
    gtk_window_move (GTK_WINDOW (contacts_add_window), config.contacts_add_edit_window_x, config.contacts_add_edit_window_y);

    g_signal_connect (G_OBJECT (contacts_add_window), "key_release_event",
					  G_CALLBACK (contacts_add_edit_key_release_cb), NULL);

    g_signal_connect (G_OBJECT (contacts_add_window), "delete_event",
                      G_CALLBACK(contacts_add_edit_window_close_cb), NULL);

    vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (contacts_add_window), vbox1);

    vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox2);
	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

    /*-------------------------------------------------------------------------*/

    contacts_add_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (contacts_add_scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox2), contacts_add_scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (contacts_add_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_add (GTK_CONTAINER (contacts_add_scrolledwindow), viewport);

    table = gtk_table_new (55, 4, FALSE);
    gtk_widget_show (table);
    gtk_container_add (GTK_CONTAINER (viewport), table);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    /*** PERSONAL ***/

    pos = 0;

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Personal"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    sprintf(tmpbuf, "%s:", _("Group"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 4, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    combobox_group = gtk_combo_box_new_text ();
    gtk_widget_show (combobox_group);
    g_signal_connect(G_OBJECT(combobox_group), "focus", 
                     G_CALLBACK(group_combo_box_focus_cb), NULL);    
      
    gtk_table_attach (GTK_TABLE (table), combobox_group, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    create_category_combobox (GTK_COMBO_BOX(combobox_group), contacts_group_store, TRUE);

    if (contacts_edit_mode == FALSE) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_group), 0);
    } 

    pos++;

    for(i=COLUMN_FIRST_NAME; i<COLUMN_BIRTH_DAY_DATE; i++) {

        sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[i*2]));
        label = gtk_label_new (tmpbuf);
        if (i != COLUMN_SECOND_NAME) {
            gtk_widget_show (label);
        } else {
            second_name_label = label;
        }
        gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 4, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

        contact_entries[i] = gtk_entry_new ();
        if (i != COLUMN_SECOND_NAME) {
            gtk_widget_show (contact_entries[i]);
        }
        if (i == COLUMN_FIRST_NAME) {
            g_signal_connect (G_OBJECT (contact_entries[i]), "key_press_event",
                              G_CALLBACK (show_hide_fields_key_press_cb), GINT_TO_POINTER(i));
        }
        gtk_table_attach (GTK_TABLE (table), contact_entries[i], 2, 4, pos, pos+1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        pos++;
    }

    /* BIRTHDAY DATE */

    sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[COLUMN_BIRTH_DAY_DATE*2]));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 4, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    contact_entries[COLUMN_BIRTH_DAY_DATE] = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(contact_entries[COLUMN_BIRTH_DAY_DATE], GTK_CAN_FOCUS);
    gtk_editable_set_editable (GTK_EDITABLE(contact_entries[COLUMN_BIRTH_DAY_DATE]), FALSE);
    gtk_widget_show (contact_entries[COLUMN_BIRTH_DAY_DATE]);
    gtk_table_attach (GTK_TABLE (table), contact_entries[COLUMN_BIRTH_DAY_DATE], 2, 3, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    birth_day_date_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_DATE, FALSE);
    GTK_WIDGET_UNSET_FLAGS(birth_day_date_button, GTK_CAN_FOCUS);
    gtk_widget_show (birth_day_date_button);
    g_signal_connect (G_OBJECT (birth_day_date_button), "clicked",
                      G_CALLBACK (date_select_cb), (gpointer) COLUMN_BIRTH_DAY_DATE);
    gtk_table_attach (GTK_TABLE (table), birth_day_date_button, 3, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    pos++;

    /* NAMEDAY DATE */

    sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[COLUMN_NAME_DAY_DATE*2]));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 4, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    contact_entries[COLUMN_NAME_DAY_DATE] = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(contact_entries[COLUMN_NAME_DAY_DATE], GTK_CAN_FOCUS);
    gtk_editable_set_editable (GTK_EDITABLE(contact_entries[COLUMN_NAME_DAY_DATE]), FALSE);
    gtk_widget_show (contact_entries[COLUMN_NAME_DAY_DATE]);
    gtk_table_attach (GTK_TABLE (table), contact_entries[COLUMN_NAME_DAY_DATE], 2, 3, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    name_day_date_button = gui_stock_button (OSMO_STOCK_BUTTON_SELECT_DATE, FALSE);
    GTK_WIDGET_UNSET_FLAGS(name_day_date_button, GTK_CAN_FOCUS);
    gtk_widget_show (name_day_date_button);
    g_signal_connect (G_OBJECT (name_day_date_button), "clicked",
                      G_CALLBACK (date_select_cb), (gpointer) COLUMN_NAME_DAY_DATE);
    gtk_table_attach (GTK_TABLE (table), name_day_date_button, 3, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    pos++;

    /* PHOTO */

    sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[COLUMN_PHOTO*2]));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 4, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    contact_entries[COLUMN_PHOTO] = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(contact_entries[COLUMN_PHOTO], GTK_CAN_FOCUS);
    gtk_editable_set_editable (GTK_EDITABLE(contact_entries[COLUMN_PHOTO]), FALSE);
    gtk_widget_show (contact_entries[COLUMN_PHOTO]);
    gtk_table_attach (GTK_TABLE (table), contact_entries[COLUMN_PHOTO], 2, 3, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        photo_browse_button = gui_stock_button (GTK_STOCK_DIRECTORY, FALSE);
    } else {
        photo_browse_button = gui_stock_button (OSMO_STOCK_BUTTON_OPEN, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(photo_browse_button, GTK_CAN_FOCUS);
    gtk_widget_show (photo_browse_button);
    g_signal_connect (G_OBJECT (photo_browse_button), "clicked",
                      G_CALLBACK (photo_file_select_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), photo_browse_button, 3, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    pos++;

    /*** HOME ***/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Home"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    for(i=COLUMN_HOME_ADDRESS; i<COLUMN_WORK_ORGANIZATION; i++) {

        sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[i*2]));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 4, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

        contact_entries[i] = gtk_entry_new ();
        gtk_widget_show (contact_entries[i]);
        gtk_table_attach (GTK_TABLE (table), contact_entries[i], 2, 4, pos, pos+1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        pos++;
    }

    /*** WORK ***/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Work"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    for(i=COLUMN_WORK_ORGANIZATION; i<COLUMN_HOME_PHONE_1; i++) {

        sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[i*2]));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 4, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

        contact_entries[i] = gtk_entry_new ();
        gtk_widget_show (contact_entries[i]);
        gtk_table_attach (GTK_TABLE (table), contact_entries[i], 2, 4, pos, pos+1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        pos++;
    }

    /*** PHONES ***/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Phones"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    for(i=COLUMN_HOME_PHONE_1; i<COLUMN_EMAIL_1; i++) {

        sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[i*2]));
        label = gtk_label_new (tmpbuf);
        gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 4, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

        contact_entries[i] = gtk_entry_new ();
        gtk_table_attach (GTK_TABLE (table), contact_entries[i], 2, 4, pos, pos+1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        g_signal_connect (G_OBJECT (contact_entries[i]), "key_press_event",
                          G_CALLBACK (show_hide_fields_key_press_cb), GINT_TO_POINTER(i));

        if(i == COLUMN_HOME_PHONE_1 || i == COLUMN_WORK_PHONE_1 || i == COLUMN_CELL_PHONE_1) {
            gtk_widget_show (label);
            gtk_widget_show (contact_entries[i]);
        }

        pos++;
    }

    /*** INTERNET ***/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Internet"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    for(i=COLUMN_EMAIL_1; i<COLUMN_PHOTO; i++) {

        sprintf(tmpbuf, "%s:", gettext(contact_fields_tags_name[i*2]));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 2, pos, pos+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 4, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

        contact_entries[i] = gtk_entry_new ();
        gtk_widget_show (contact_entries[i]);
        gtk_table_attach (GTK_TABLE (table), contact_entries[i], 2, 4, pos, pos+1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        g_signal_connect (G_OBJECT (contact_entries[i]), "key_press_event",
                          G_CALLBACK (show_hide_fields_key_press_cb), GINT_TO_POINTER(i));

        if((i > COLUMN_EMAIL_1  && i < COLUMN_WWW_1) || (i > COLUMN_WWW_1  && i < COLUMN_IM_GG)) {
            gtk_widget_hide (label);
            gtk_widget_hide (contact_entries[i]);
        }

        pos++;
    }

    /*** Additional info ***/	
    
    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 0, 1, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_size_request (hseparator, 20, -1);

    sprintf(tmpbuf, "<b>%s</b>", _("Additional info"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, pos, pos+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 5, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (table), hseparator, 2, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    pos++;

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);
    gtk_table_attach (GTK_TABLE (table), scrolledwindow, 0, 4, pos, pos+1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    add_info_textview = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (scrolledwindow), add_info_textview);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (add_info_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(add_info_textview), 4);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(add_info_textview), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(add_info_textview), 4);
    gtk_widget_set_size_request (add_info_textview, -1, 350);
	gtk_widget_show (add_info_textview);

    /*-------------------------------------------------------------------------*/

	hseparator = gtk_hseparator_new ();
	gtk_widget_show (hseparator);
	gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

	hbuttonbox = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox);
	gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox), 2);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
	gtk_box_set_spacing (GTK_BOX (hbuttonbox), 16);

    if (config.default_stock_icons) {
	    cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    } else {
	    cancel_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CANCEL);
    }
	gtk_widget_show (cancel_button);
	gtk_container_add (GTK_CONTAINER (hbuttonbox), cancel_button);
	g_signal_connect(cancel_button, "clicked", 
                     G_CALLBACK(contacts_add_edit_window_close_cb), NULL);

    if (config.default_stock_icons) {
	    contacts_ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
    } else {
	    contacts_ok_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_OK);
    }
	gtk_widget_show (contacts_ok_button);
	gtk_container_add (GTK_CONTAINER (hbuttonbox), contacts_ok_button);
	g_signal_connect(contacts_ok_button, "clicked", 
                     G_CALLBACK(contacts_item_entered_cb), NULL);

    contacts_edit_state = contacts_edit_mode;
    contacts_accept_state = FALSE;

    if (contacts_edit_mode == TRUE) {
        contacts_accept_state = TRUE;

        if (gtk_tree_selection_get_selected (contacts_list_selection, &model, &iter)) {


            for(i=0; i<CONTACTS_NUM_COLUMNS; i++) {

                if (i == COLUMN_BIRTH_DAY_DATE || i == COLUMN_NAME_DAY_DATE) {
                    gtk_tree_model_get (model, &iter, i, &date, -1);  
                    if (date == 0) {
                        text = NULL;
                    } else {
                        if (i == COLUMN_BIRTH_DAY_DATE) {
                            birthday_date = date;
                            text = g_strdup((const gchar *)julian_to_str(date, DATE_FULL));
                        } else {
                            nameday_date = date;
                            text = g_strdup((const gchar *)julian_to_str(date, DATE_NAME_DAY));
                        }
                    }
                } else {
                    gtk_tree_model_get (model, &iter, i, &text, -1);  
                }

                if (text != NULL) {
                    if (i == COLUMN_GROUP) {
                        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_group),
                                                 gui_list_store_get_text_index (contacts_group_store, text));
                    } else if (i == COLUMN_INFO) {
                        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(add_info_textview));
                        gui_clear_text_buffer (text_buffer, &titer);
                        gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &titer, 0);
                        gtk_text_buffer_insert(text_buffer, &titer, text, -1);
                        gtk_text_view_set_buffer(GTK_TEXT_VIEW(add_info_textview), text_buffer);
                    } else {
                        gtk_entry_set_text (GTK_ENTRY(contact_entries[i]), text);
                    }
                    g_free(text);
                }

            }

            show_hide_multiple_fields();
        }
    }

    gtk_widget_set_sensitive(contacts_ok_button, contacts_accept_state);

    g_signal_connect(G_OBJECT(combobox_group), "changed", 
                     G_CALLBACK(group_combobox_changed_cb), NULL);

    gtk_widget_show(contacts_add_window);
    gtk_widget_grab_focus (contact_entries[COLUMN_FIRST_NAME]);
}

/*------------------------------------------------------------------------------*/

void
read_contacts_entries (void) {

xmlDocPtr doc;
xmlChar *key;
xmlNodePtr node, cnode, main_node;
xmlNodePtr group_node;
GtkTreeIter iter;
gint j;
guint32 date;


    if (g_file_test (prefs_get_config_filename(CONTACTS_ENTRIES_FILENAME), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(CONTACTS_ENTRIES_FILENAME)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            xmlFreeDoc(doc);
            return;
        }

        if (xmlStrcmp(node->name, (const xmlChar *) CONTACTS_NAME)) {
            xmlFreeDoc(doc);
            return;
        }

        main_node = node->xmlChildrenNode;

        while (main_node != NULL) {

            if(!xmlStrcmp(main_node->name, (xmlChar *) CONTACTS_GROUP_ENTRIES_NAME)) {

                group_node = main_node->xmlChildrenNode;

                while (group_node != NULL) {

                    if ((!xmlStrcmp(group_node->name, (const xmlChar *) "name"))) {
                        key = xmlNodeListGetString(doc, group_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            gtk_list_store_append(contacts_group_store, &iter);
                            gtk_list_store_set(contacts_group_store, &iter, 0, (gchar *) key, -1);
                        }
                        xmlFree(key);
                    }

                    group_node = group_node->next;
                }
            }

            /*---------------------------------------------------------------------------------------*/

            if(!xmlStrcmp(main_node->name, (xmlChar *) CONTACTS_ENTRIES_NAME)) {

                /* read note */
                node = main_node->xmlChildrenNode;

                while (node != NULL) {

                    if(!xmlStrcmp(node->name, (xmlChar *) "record")) {

                        cnode = node->xmlChildrenNode;

                        gtk_list_store_append(contacts_list_store, &iter);

                        while (cnode != NULL) {

                            for (j = COLUMN_GROUP; j < CONTACTS_NUM_COLUMNS; j++) {

                                if ((!xmlStrcmp(cnode->name, (const xmlChar *) contact_fields_tags_name[j*2+1]))) {

                                    if (j == COLUMN_BIRTH_DAY_DATE || j == COLUMN_NAME_DAY_DATE) {
                                        key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                        if (key != NULL) {
                                            sscanf((gchar *) key, "%d", &date);
                                            gtk_list_store_set(contacts_list_store, &iter, j, date, -1);
                                            xmlFree(key);
                                        }
                                    } else {
                                        key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                        if (key != NULL) {
                                            gtk_list_store_set(contacts_list_store, &iter, j, key, -1);
                                            xmlFree(key);
                                        }
                                    }
                                }
                            }

                            cnode = cnode->next;
                        }

                    }

                    node = node->next;
                }

            }

            /*---------------------------------------------------------------------------------------*/

            main_node = main_node->next;
        }

        xmlFree(node);
        xmlFreeDoc(doc);
    }

}


/*------------------------------------------------------------------------------*/

void
write_contacts_entries (void) {

gint i, j;
xmlDocPtr doc;
xmlNodePtr main_node, node, note_node;
xmlAttrPtr attr;
GtkTreeIter iter;
gchar *item;
guint32 date;
gchar temp[BUFFER_SIZE];

    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    main_node = xmlNewNode(NULL, (const xmlChar *) CONTACTS_NAME);
    xmlDocSetRootElement(doc, main_node);

    node = xmlNewChild(main_node, NULL, (const xmlChar *) CONTACTS_GROUP_ENTRIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_group_store), &iter, NULL, i++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(contacts_group_store), &iter, 0, &item, -1);
        xmlNewChild(node, NULL, (const xmlChar *) "name", (xmlChar *) item);
        g_free(item);
    }

    node = xmlNewChild(main_node, NULL, (const xmlChar *) CONTACTS_ENTRIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(contacts_list_store), &iter, NULL, i++)) {

        note_node = xmlNewChild(node, NULL, (const xmlChar *) "record", (xmlChar *) NULL);

        for (j = COLUMN_GROUP; j < CONTACTS_NUM_COLUMNS; j++) {

            if (j == COLUMN_BIRTH_DAY_DATE || j == COLUMN_NAME_DAY_DATE) {

                gtk_tree_model_get(GTK_TREE_MODEL(contacts_list_store), &iter, j, &date, -1);
                sprintf(temp, "%d", date);
                xmlNewChild(note_node, NULL, (const xmlChar *) contact_fields_tags_name[j*2+1], (xmlChar *) temp);

            } else {

                gtk_tree_model_get(GTK_TREE_MODEL(contacts_list_store), &iter, j, &item, -1);

                if (item != NULL) {
                    if (strlen(item)) {
                        xmlNewChild(note_node, NULL, (const xmlChar *) contact_fields_tags_name[j*2+1], 
                                    (xmlChar *) item);
                    }
                }
            }
        }
    }

    xmlSaveFormatFileEnc(prefs_get_config_filename(CONTACTS_ENTRIES_FILENAME), doc, "utf-8", 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*/


