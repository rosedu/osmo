
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
#include "gui.h"
#include "gui_utils.h"
#include "options_gui.h"
#include "options_prefs.h"
#include "tasks.h"
#include "tasks_items.h"
#include "calendar.h"
#include "calendar_utils.h"
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "stock_icons.h"

/*------------------------------------------------------------------------------*/

void
tasks_item_entered_cb (GtkWidget *widget, gpointer data) {

GtkTextBuffer *text_buffer;
GtkTextIter iter_a, iter_b;
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter iter;
guint32 fstartdate;

    GUI *appGUI = (GUI *)data;

    if (appGUI->tsk->tasks_edit_state == TRUE) {

        gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->tsk->tasks_list), &sort_path, NULL);

        if (sort_path != NULL) {

            filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->tsk->tasks_sort), sort_path);

            if (filter_path != NULL) {

                path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter), filter_path);

                if (path != NULL) {
                    gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, path);
                    gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                               COLUMN_START_DATE_JULIAN, &fstartdate, -1);
                    gtk_list_store_remove(appGUI->tsk->tasks_list_store, &iter);
                    gtk_tree_path_free(path);
                }

                gtk_tree_path_free(filter_path);
            }

            gtk_tree_path_free(sort_path);
        }
    }

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->tsk->desc_textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &iter_a, 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &iter_b, -1);

    if(appGUI->tsk->tasks_edit_state == TRUE) {
        add_item_to_list(FALSE,
                         appGUI->tsk->tasks_due_julian_day,
                         fstartdate,
                         gtk_combo_box_get_active_text(GTK_COMBO_BOX(appGUI->tsk->priority_combobox)),
                         gtk_combo_box_get_active_text(GTK_COMBO_BOX(appGUI->tsk->category_combobox)),
                         (gchar *) gtk_entry_get_text(GTK_ENTRY(appGUI->tsk->summary_entry)),
                         gtk_text_buffer_get_text(GTK_TEXT_BUFFER(text_buffer), &iter_a, &iter_b, TRUE), appGUI);

    } else {
        add_item_to_list(FALSE, 
                         appGUI->tsk->tasks_due_julian_day,
                         get_julian_for_today(),
                         gtk_combo_box_get_active_text(GTK_COMBO_BOX(appGUI->tsk->priority_combobox)),
                         gtk_combo_box_get_active_text(GTK_COMBO_BOX(appGUI->tsk->category_combobox)),
                         (gchar *) gtk_entry_get_text(GTK_ENTRY(appGUI->tsk->summary_entry)),
                         gtk_text_buffer_get_text(GTK_TEXT_BUFFER(text_buffer), &iter_a, &iter_b, TRUE), appGUI);
    }

    gtk_widget_destroy(appGUI->tsk->tasks_add_window);
    update_n_items(appGUI);
    tasks_select_first_position_in_list(appGUI);
    g_signal_emit_by_name(G_OBJECT(appGUI->cal->calendar), "day-selected");
}

/*------------------------------------------------------------------------------*/

void
day_selected_cb (GuiCalendar *t_calendar, gpointer user_data) {

guint day, month, year;
gchar *temp;

    GUI *appGUI = (GUI *)user_data;
   
    gui_calendar_get_date (t_calendar, &year, &month, &day);
    gtk_widget_destroy(appGUI->tsk->td_calendar_window);
    
    appGUI->tsk->tasks_due_julian_day = date_to_julian (day, month, year);
    temp = julian_to_str (appGUI->tsk->tasks_due_julian_day, config.date_format);
    gtk_entry_set_text (GTK_ENTRY(appGUI->tsk->due_date_entry), temp);
}

/*------------------------------------------------------------------------------*/

void
close_calendar_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
    gtk_widget_destroy(appGUI->tsk->td_calendar_window);
}

/*------------------------------------------------------------------------------*/

void
button_close_calendar_cb (GtkButton *button, gpointer user_data) {

    close_calendar_cb (GTK_WIDGET(button), NULL, user_data);
}

/*------------------------------------------------------------------------------*/

void
set_today_calendar_cb (GtkWidget *widget, gpointer data) {

struct tm   *timer;
time_t      tmm;

    GUI *appGUI = (GUI *)data;

    tmm = time(NULL); 
    timer = localtime(&tmm);

    gui_calendar_select_month (GUI_CALENDAR(appGUI->tsk->td_calendar), timer->tm_mon, timer->tm_year + 1900);
    gui_calendar_select_day (GUI_CALENDAR(appGUI->tsk->td_calendar), timer->tm_mday);
    day_selected_cb (GUI_CALENDAR(appGUI->tsk->td_calendar), appGUI);
}

/*------------------------------------------------------------------------------*/

void
nodate_calendar_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    gtk_entry_set_text (GTK_ENTRY(appGUI->tsk->due_date_entry), _("No date"));
    appGUI->tsk->tasks_due_julian_day = 0;
    close_calendar_cb (widget, NULL, data);
}

/*------------------------------------------------------------------------------*/

gint 
sd_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (event->keyval == GDK_Escape) {
        close_calendar_cb (NULL, NULL, appGUI);
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
select_date_cb (GtkWidget *widget, gpointer data) {

GtkWidget *vbox1;
GtkWidget *hbox1;
GtkWidget *alignment;
GtkWidget *today_button;
GtkWidget *nodate_button;
GtkWidget *close_button;

    GUI *appGUI = (GUI *)data;

    appGUI->tsk->td_calendar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(appGUI->tsk->td_calendar_window), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal(GTK_WINDOW(appGUI->tsk->td_calendar_window), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->tsk->td_calendar_window), GTK_WINDOW(appGUI->tsk->tasks_add_window));
    gtk_window_set_decorated(GTK_WINDOW(appGUI->tsk->td_calendar_window), FALSE);
    g_signal_connect (G_OBJECT (appGUI->tsk->td_calendar_window), "key_press_event",
                      G_CALLBACK (sd_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->tsk->td_calendar_window), vbox1);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_box_pack_start (GTK_BOX (vbox1), alignment, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);

    appGUI->tsk->td_calendar = gui_calendar_new();
    gui_calendar_set_cursor_type(GUI_CALENDAR(appGUI->tsk->td_calendar), CURSOR_BLOCK);
    gtk_widget_show(appGUI->tsk->td_calendar);
    g_signal_connect (GTK_OBJECT (appGUI->tsk->td_calendar), "day_selected_double_click",
                      G_CALLBACK (day_selected_cb), appGUI);
    gtk_container_add (GTK_CONTAINER (alignment), appGUI->tsk->td_calendar);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_box_pack_start (GTK_BOX (vbox1), alignment, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);

    hbox1 = gtk_hbox_new (FALSE, 2);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (alignment), hbox1);

    today_button = gtk_button_new_with_mnemonic (_("Today"));
    gtk_widget_show (today_button);
    gtk_box_pack_start (GTK_BOX (hbox1), today_button, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (today_button), "clicked",
                        G_CALLBACK (set_today_calendar_cb), appGUI);

    nodate_button = gtk_button_new_with_mnemonic (_("No date"));
    gtk_widget_show (nodate_button);
    gtk_box_pack_start (GTK_BOX (hbox1), nodate_button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (nodate_button), "clicked",
                        G_CALLBACK (nodate_calendar_cb), appGUI);

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
                        G_CALLBACK (button_close_calendar_cb), appGUI);

    gtk_widget_show (appGUI->tsk->td_calendar_window);

    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->tsk->td_calendar), 
                                      GUI_CALENDAR_SHOW_HEADING | GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY);
}

/*------------------------------------------------------------------------------*/

void
tasks_add_edit_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
    gtk_widget_destroy(appGUI->tsk->tasks_add_window);
}

/*------------------------------------------------------------------------------*/

void
button_tasks_add_edit_window_close_cb (GtkButton *button, gpointer user_data) {

    tasks_add_edit_window_close_cb (GTK_WIDGET(button), NULL, user_data);
}

/*------------------------------------------------------------------------------*/

gint 
tasks_add_edit_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Escape:
            gtk_widget_destroy(appGUI->tsk->tasks_add_window);
            return TRUE;
    }

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->tsk->summary_entry)))) {
        appGUI->tsk->tasks_accept_state = TRUE;
    } else {
        appGUI->tsk->tasks_accept_state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->tsk->tasks_ok_button, appGUI->tsk->tasks_accept_state);

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
tasks_remove_dialog_show (GtkWidget *list, GtkListStore *list_store, GUI *appGUI) {

gint response;
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter iter;
gchar tmpbuf[BUFFER_SIZE];

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (list), &sort_path, NULL);

    if (sort_path != NULL) {

        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->tsk->tasks_sort), sort_path);

        if (filter_path != NULL) {

            path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter), filter_path);

            if (path != NULL) {

                sprintf (tmpbuf, "%s\n\n%s", _("Selected task will be removed."), _("Are you sure?"));

                response = gui_create_dialog(GTK_MESSAGE_QUESTION, tmpbuf, GTK_WINDOW(appGUI->main_window));

                if (response == GTK_RESPONSE_YES) {   
                    gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path);
                    gtk_list_store_remove(list_store, &iter);
                    gtk_tree_path_free(path);
                    tasks_select_first_position_in_list(appGUI);
                    update_n_items(appGUI);
                    g_signal_emit_by_name(G_OBJECT(appGUI->cal->calendar), "day-selected");
                }
            }

            gtk_tree_path_free(filter_path);
        }

        gtk_tree_path_free(sort_path);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_add_edit_dialog_show (gboolean tasks_edit_mode, GUI *appGUI) {

GtkWidget *vbox1;
GtkWidget *vbox2;
GtkWidget *label;
GtkWidget *select_date_button;
GtkWidget *scrolledwindow;
GtkWidget *hseparator;
GtkWidget *hbuttonbox;
GtkWidget *table;
GtkWidget *cancel_button;
gint win_xpos, win_ypos;
gchar tmpbuf[BUFFER_SIZE];


    appGUI->tsk->tasks_add_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->tsk->tasks_add_window), 6);

    if(tasks_edit_mode == TRUE) {
        gtk_window_set_title (GTK_WINDOW (appGUI->tsk->tasks_add_window), _("Edit task"));
    } else {
        gtk_window_set_title (GTK_WINDOW (appGUI->tsk->tasks_add_window), _("Add task"));
    }

    gtk_window_set_default_size (GTK_WINDOW(appGUI->tsk->tasks_add_window), TASKS_ADD_EDIT_WINDOW_SIZE_X, TASKS_ADD_EDIT_WINDOW_SIZE_Y);
    gtk_window_set_position(GTK_WINDOW(appGUI->tsk->tasks_add_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->tsk->tasks_add_window), GTK_WINDOW(appGUI->main_window));
    gtk_window_set_modal(GTK_WINDOW(appGUI->tsk->tasks_add_window), TRUE);

    g_signal_connect (G_OBJECT (appGUI->tsk->tasks_add_window), "key_press_event",
                      G_CALLBACK (tasks_add_edit_key_press_cb), appGUI);

    g_signal_connect (G_OBJECT (appGUI->tsk->tasks_add_window), "delete_event",
                      G_CALLBACK(tasks_add_edit_window_close_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->tsk->tasks_add_window), vbox1);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

    table = gtk_table_new (4, 4, FALSE);
    gtk_widget_show (table);
    gtk_table_set_row_spacings (GTK_TABLE (table), 6);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    gtk_box_pack_start (GTK_BOX (vbox2), table, FALSE, FALSE, 4);

    sprintf(tmpbuf, "<b>%s:</b>", _("Summary"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "<b>%s:</b>", _("Due date"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->tsk->summary_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->tsk->summary_entry);

    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->summary_entry, 1, 4, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);

    appGUI->tsk->due_date_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->tsk->due_date_entry);
    GTK_WIDGET_UNSET_FLAGS(appGUI->tsk->due_date_entry, GTK_CAN_FOCUS);
    gtk_editable_set_editable (GTK_EDITABLE (appGUI->tsk->due_date_entry), FALSE);

    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->due_date_entry, 1, 2, 1, 2,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);

    select_date_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_SELECT_DATE);
    gtk_widget_show (select_date_button);
    GTK_WIDGET_UNSET_FLAGS(select_date_button, GTK_CAN_FOCUS);
    g_signal_connect(select_date_button, "clicked", 
                     G_CALLBACK(select_date_cb), appGUI);

    gtk_table_attach (GTK_TABLE (table), select_date_button, 2, 4, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);

    sprintf(tmpbuf, "<b>%s:</b>", _("Category"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->tsk->category_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->tsk->category_combobox);
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->tsk->category_combobox), FALSE);

    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->category_combobox, 1, 2, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    create_category_combobox (GTK_COMBO_BOX (appGUI->tsk->category_combobox), appGUI->opt->tasks_category_store, TRUE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->category_combobox), 
            gui_list_store_get_text_index (appGUI->opt->tasks_category_store, gtk_combo_box_get_active_text(GTK_COMBO_BOX(appGUI->tsk->cf_combobox))));

    sprintf(tmpbuf, "<b>%s:</b>", _("Priority"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->tsk->priority_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->tsk->priority_combobox);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->tsk->priority_combobox), _("Low"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->tsk->priority_combobox), _("Medium"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->tsk->priority_combobox), _("High"));
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->tsk->priority_combobox), FALSE);

    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->priority_combobox, 3, 4, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);

    gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->tsk->priority_combobox), 1);

    sprintf(tmpbuf, "<b>%s:</b>", _("Description"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->tsk->desc_textview = gtk_text_view_new ();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->tsk->desc_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->tsk->desc_textview), 4);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->tsk->desc_textview), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->tsk->desc_textview), 4);
    gtk_widget_show (appGUI->tsk->desc_textview);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->tsk->desc_textview);

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
                     G_CALLBACK(button_tasks_add_edit_window_close_cb), appGUI);

    if (config.default_stock_icons) {
        appGUI->tsk->tasks_ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
    } else {
        appGUI->tsk->tasks_ok_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_OK);
    }
    gtk_widget_show (appGUI->tsk->tasks_ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->tsk->tasks_ok_button);
    g_signal_connect(appGUI->tsk->tasks_ok_button, "clicked", 
                     G_CALLBACK(tasks_item_entered_cb), appGUI);

    appGUI->tsk->tasks_edit_state = tasks_edit_mode;
    appGUI->tsk->tasks_accept_state = FALSE;
    gtk_entry_set_text (GTK_ENTRY(appGUI->tsk->due_date_entry), _("No date"));

    if (tasks_edit_mode == TRUE) {
        appGUI->tsk->tasks_accept_state = TRUE;
        fill_fields (appGUI->tsk->due_date_entry, appGUI->tsk->summary_entry, appGUI->tsk->desc_textview, appGUI);
    }

    gtk_widget_set_sensitive(appGUI->tsk->tasks_ok_button, appGUI->tsk->tasks_accept_state);

    gtk_window_get_position (GTK_WINDOW(appGUI->tsk->tasks_add_window), &win_xpos, &win_ypos);
    gtk_window_move (GTK_WINDOW (appGUI->tsk->tasks_add_window), win_xpos-TASKS_ADD_EDIT_WINDOW_SIZE_X/2, win_ypos-10);
    gtk_widget_show(appGUI->tsk->tasks_add_window);
}

/*------------------------------------------------------------------------------*/

void
read_tasks_entries (GUI *appGUI) {

xmlDocPtr doc;
xmlChar *key;
xmlNodePtr node, cnode, category_node, main_node;
GtkTreeIter iter;
gboolean done;
gchar priority[BUFFER_SIZE], category[BUFFER_SIZE];
gchar summary[MAX_SUMMARY_SIZE];
guint32 due_date_julian, start_date_julian, priority_n;


    if (g_file_test (prefs_get_config_filename(TASKS_ENTRIES_FILENAME), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(TASKS_ENTRIES_FILENAME)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            xmlFreeDoc(doc);
            return;
        }

        if (xmlStrcmp(node->name, (const xmlChar *) TASKS_NAME)) {
            xmlFreeDoc(doc);
            return;
        }

        main_node = node->xmlChildrenNode;

        while (main_node != NULL) {

            if(!xmlStrcmp(main_node->name, (xmlChar *) TASKS_CATEGORY_ENTRIES_NAME)) {

                /* read note */
                category_node = main_node->xmlChildrenNode;

                while (category_node != NULL) {

                    if ((!xmlStrcmp(category_node->name, (const xmlChar *) "name"))) {
                        key = xmlNodeListGetString(doc, category_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            gtk_list_store_append(appGUI->opt->tasks_category_store, &iter);
                            gtk_list_store_set(appGUI->opt->tasks_category_store, &iter, 0, (gchar *) key, -1);
                        }
                        xmlFree(key);
                    }

                    category_node = category_node->next;
                }
            }

            /*---------------------------------------------------------------------------------------*/

            if(!xmlStrcmp(main_node->name, (xmlChar *) TASKS_ENTRIES_NAME)) {

                /* read note */
                node = main_node->xmlChildrenNode;

                while (node != NULL) {

                    if(!xmlStrcmp(node->name, (xmlChar *) "entry")) {

                        cnode = node->xmlChildrenNode;

                        done = FALSE;
                        due_date_julian = start_date_julian = 0;

                        while (cnode != NULL) {

                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "status"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    sscanf((gchar *) key, "%d", &done);
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "due_date"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    sscanf((gchar *) key, "%d", &due_date_julian);
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "start_date"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    sscanf((gchar *) key, "%d", &start_date_julian);
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "priority"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    strncpy (priority, (gchar *) key, BUFFER_SIZE-1);
                                    if (get_priority_index(gettext(priority)) == -1) {
                                        sscanf((gchar *) key, "%d", &priority_n);
                                        strncpy (priority, get_priority_text(priority_n), BUFFER_SIZE-1);
                                    }
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "category"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    strncpy (category, (gchar *) key, BUFFER_SIZE-1);
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "summary"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                                if (key != NULL) {
                                    strncpy (summary, (gchar *) key, MAX_SUMMARY_SIZE-1);
                                    xmlFree(key);
                                }
                            }
                            if ((!xmlStrcmp(cnode->name, (const xmlChar *) "description"))) {
                                key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);

                                add_item_to_list(done, due_date_julian, start_date_julian, priority, 
                                                     category, summary, (gchar *) key, appGUI);
                                if (key != NULL) {
                                    xmlFree(key);
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
write_tasks_entries (GUI *appGUI) {

gint i;
xmlDocPtr doc;
xmlNodePtr main_node, node, note_node;
xmlAttrPtr attr;
gchar temp[BUFFER_SIZE];
GtkTreeIter iter;
gboolean *done;
gchar *priority, *category, *summary, *desc;
guint32 due_date_julian, start_date_julian;


    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    main_node = xmlNewNode(NULL, (const xmlChar *) TASKS_NAME);
    xmlDocSetRootElement(doc, main_node);

    node = xmlNewChild(main_node, NULL, (const xmlChar *) TASKS_CATEGORY_ENTRIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, 0, &category, -1);
        xmlNewChild(node, NULL, (const xmlChar *) "name", (xmlChar *) category);
        g_free(category);
    }

    node = xmlNewChild(main_node, NULL, (const xmlChar *) TASKS_ENTRIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                           COLUMN_DONE, &done,
                           COLUMN_DUE_DATE_JULIAN, &due_date_julian,
                           COLUMN_START_DATE_JULIAN, &start_date_julian,
                           COLUMN_PRIORITY, &priority,
                           COLUMN_CATEGORY, &category,
                           COLUMN_SUMMARY, &summary,
                           COLUMN_DESCRIPTION, &desc, -1);

        note_node = xmlNewChild(node, NULL, (const xmlChar *) "entry", (xmlChar *) NULL);
        sprintf(temp, "%d", (gint) done);
        xmlNewChild(note_node, NULL, (const xmlChar *) "status", (xmlChar *) temp);
        sprintf(temp, "%d", (guint32) due_date_julian);
        xmlNewChild(note_node, NULL, (const xmlChar *) "due_date", (xmlChar *) temp);
        sprintf(temp, "%d", (guint32) start_date_julian);
        xmlNewChild(note_node, NULL, (const xmlChar *) "start_date", (xmlChar *) temp);
        sprintf(temp, "%d", get_priority_index(gettext(priority)));
        xmlNewChild(note_node, NULL, (const xmlChar *) "priority", (xmlChar *) temp);
        xmlNewChild(note_node, NULL, (const xmlChar *) "category", (xmlChar *) category);
        xmlNewChild(note_node, NULL, (const xmlChar *) "summary", (xmlChar *) summary);
        xmlNewChild(note_node, NULL, (const xmlChar *) "description", (xmlChar *) desc);
    }

    xmlSaveFormatFileEnc(prefs_get_config_filename(TASKS_ENTRIES_FILENAME), doc, "utf-8", 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/


