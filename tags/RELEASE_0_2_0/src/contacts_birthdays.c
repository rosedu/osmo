
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
#include <time.h>
#include <ctype.h>

#include "i18n.h"
#include "calendar.h"
#include "gui.h"
#include "contacts.h"
#include "calendar_notes.h"
#include "options_prefs.h"
#include "stock_icons.h"
#include "time_utils.h"
#include "gui_utils.h"
#include "date_utils.h"
#include "contacts_birthdays.h"


/*------------------------------------------------------------------------------*/

void
birthdays_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_destroy (appGUI->cnt->birthdays_window);
}

/*------------------------------------------------------------------------------*/

void
button_birthdays_window_close_cb (GtkButton *button, gpointer user_data) {

    birthdays_window_close_cb (GTK_WIDGET(button), NULL, user_data);

}

/*------------------------------------------------------------------------------*/

gint 
birthdays_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Escape:
            gtk_widget_destroy(appGUI->cnt->birthdays_window);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint
birthdays_list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

GtkTreeIter iter;
GtkTreeModel *model;
GtkTreePath *sort_path, *filter_path, *path;
gint id, id_c;

    GUI *appGUI = (GUI *)func_data;


    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {

        if (gtk_tree_selection_get_selected (appGUI->cnt->birthdays_list_selection, &model, &iter)) {

            if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->cnt->contacts_find_entry)))) {
                gtk_entry_set_text(GTK_ENTRY(appGUI->cnt->contacts_find_entry), "");
                gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));
            } 

            gtk_tree_model_get (model, &iter, B_COLUMN_ID, &id, -1);
            gtk_widget_destroy(appGUI->cnt->birthdays_window);

            if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter) == TRUE) {

                sort_path = gtk_tree_model_get_path (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter);

                while (sort_path != NULL) {

                    gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->cnt->contacts_list), sort_path, NULL, FALSE);

                    filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->cnt->contacts_sort), sort_path);

                    if (filter_path != NULL) {

                        path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter), filter_path);

                        if (path != NULL) {
                            gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, path);
                            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, COLUMN_ID, &id_c, -1);
                            if (id == id_c) {
                                gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->cnt->contacts_list), sort_path, NULL, FALSE);
                                show_contacts_desc_panel (TRUE, appGUI);
                                gtk_tree_path_free(path);
                                gtk_tree_path_free(filter_path);
                                break;
                            }
                            gtk_tree_path_free(path);
                        }

                        gtk_tree_path_free(filter_path);
                    }

                    gtk_tree_path_next (sort_path);
                    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, sort_path) == FALSE) break;
                }

                gtk_tree_path_free(sort_path);
 
            }

        }
    
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
contacts_create_birthdays_window (GUI *appGUI) {

GtkWidget           *vbox1;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *close_button;
GtkTreeViewColumn   *column;
GtkCellRenderer     *renderer;
GtkWidget           *scrolledwindow;
gint i, n, id;
guint32 date, age;
gchar *text, buffer[BUFFER_SIZE];
GtkTreeIter iter, n_iter;
GDate *cdate_birthday = NULL, *cdate_current = NULL;
guint b_day, b_month, b_year;
guint c_day, c_month, c_year;
gboolean flag;

    i = n = 0;
    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, NULL, i++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, COLUMN_BIRTH_DAY_DATE, &date, -1);
        if (date) n++;
    }

    if (n == 0) {
        gui_create_dialog(GTK_MESSAGE_INFO, _("No birthdays defined"), GTK_WINDOW(appGUI->main_window));
        return;
    }

    appGUI->cnt->birthdays_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->cnt->birthdays_window), _("Birthdays list"));
    gtk_window_set_position (GTK_WINDOW (appGUI->cnt->birthdays_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_default_size (GTK_WINDOW(appGUI->cnt->birthdays_window), 450, 600);
    gtk_window_set_modal (GTK_WINDOW (appGUI->cnt->birthdays_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cnt->birthdays_window), "delete_event",
                      G_CALLBACK(birthdays_window_close_cb), appGUI);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->cnt->birthdays_window), GTK_WINDOW(appGUI->main_window));
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cnt->birthdays_window), 8);
    g_signal_connect (G_OBJECT (appGUI->cnt->birthdays_window), "key_press_event",
                      G_CALLBACK (birthdays_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->cnt->birthdays_window), vbox1);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->cnt->birthdays_list_store = gtk_list_store_new(BIRTHDAYS_NUM_COLUMNS, 
                                                           G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    appGUI->cnt->birthdays_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->cnt->birthdays_list_store));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->cnt->birthdays_list), FALSE);
    gtk_widget_show (appGUI->cnt->birthdays_list);
    GTK_WIDGET_SET_FLAGS (appGUI->cnt->birthdays_list, GTK_CAN_DEFAULT);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->cnt->birthdays_list);

    appGUI->cnt->birthdays_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->cnt->birthdays_list));

    g_signal_connect(G_OBJECT(appGUI->cnt->birthdays_list), "button_press_event",
                     G_CALLBACK(birthdays_list_dbclick_cb), appGUI);

    /* create columns */

    renderer = gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    column = gtk_tree_view_column_new_with_attributes(_("Name"), renderer, "text", B_COLUMN_NAME, NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cnt->birthdays_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Days to birthday"), renderer, "text", B_COLUMN_DAYS, NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cnt->birthdays_list), column);
    gtk_tree_view_column_set_sort_column_id (column, B_COLUMN_DAYS);
    g_signal_emit_by_name(column, "clicked");

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Age"), renderer, "text", B_COLUMN_AGE, NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cnt->birthdays_list), column);
    gtk_tree_view_column_set_sort_column_id (column, B_COLUMN_AGE);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", B_COLUMN_ID, NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cnt->birthdays_list), column);

    gtk_tree_view_set_enable_search (GTK_TREE_VIEW(appGUI->cnt->birthdays_list), FALSE);

    i = 0;

    cdate_current = g_date_new_julian (get_julian_for_today());
    c_day = c_month = c_year = 0;

    if (cdate_current != NULL) {
        if (g_date_valid (cdate_current)) {
            c_day = g_date_get_day(cdate_current);
            c_month = g_date_get_month(cdate_current);
            c_year = g_date_get_year(cdate_current);
        }
        g_date_free(cdate_current);
    }

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, COLUMN_BIRTH_DAY_DATE, &date, -1);

        if (date != 0) {

            flag = FALSE;

            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), 
                               &iter, COLUMN_FIRST_NAME, &text, -1);

            if (text != NULL) {
                strcpy(buffer, text);
                g_free(text);
                flag = TRUE;
            }

            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), 
                               &iter, COLUMN_LAST_NAME, &text, -1);

            if (text != NULL) {
                if (flag == TRUE) {
                    strcat(buffer, " ");
                    strcat(buffer, text);
                } else {
                    strcpy(buffer, text);
                }
                g_free(text);
            }

            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), 
                               &iter, COLUMN_ID, &id, -1);

            gtk_list_store_append(appGUI->cnt->birthdays_list_store, &n_iter);
            gtk_list_store_set(appGUI->cnt->birthdays_list_store, &n_iter, 
                               B_COLUMN_NAME, buffer, B_COLUMN_ID, id, -1);

            /* calculate age */

            cdate_birthday = g_date_new_julian (date);

            if (cdate_birthday != NULL) {
                if (g_date_valid (cdate_birthday)) {
                    age = get_current_year () - g_date_get_year (cdate_birthday);
                    if ((g_date_get_month (cdate_birthday) < get_current_month () + 1) || 
                       (g_date_get_month (cdate_birthday) == get_current_month () + 1 &&
                        g_date_get_day (cdate_birthday) < get_current_day ())) {
                        age++;
                    } 
                    gtk_list_store_set (appGUI->cnt->birthdays_list_store, &n_iter, B_COLUMN_AGE, age, -1);
                }
                g_date_free(cdate_birthday);
            }

            /* calculate days to birthday */

            cdate_birthday = g_date_new_julian (date);
            b_day = b_month = b_year = 0;

            if (cdate_birthday != NULL) {
                if (g_date_valid (cdate_birthday)) {
                    b_day = g_date_get_day(cdate_birthday);
                    b_month = g_date_get_month(cdate_birthday);
                    b_year = g_date_get_year(cdate_birthday);
                }
                g_date_free(cdate_birthday);
            }

            b_year = c_year;
            if ((b_month < c_month) || (b_month == c_month && b_day < c_day)) {
                b_year++;
            }

            date = date_to_julian(b_day, b_month-1, b_year);
            date -= get_julian_for_today();

            gtk_list_store_set(appGUI->cnt->birthdays_list_store, &n_iter, B_COLUMN_DAYS, date, -1);


        }

    }

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 4);

    if (config.default_stock_icons) {
        close_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    } else {
        close_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CLOSE);
    }
    gtk_widget_show (close_button);
    g_signal_connect(close_button, "clicked", G_CALLBACK(button_birthdays_window_close_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), close_button);

    gtk_widget_show(appGUI->cnt->birthdays_window);
    gtk_widget_grab_focus (close_button);
}

/*------------------------------------------------------------------------------*/


