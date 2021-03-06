
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


#include "calendar_notes.h"
#include "i18n.h"
#include "gui_utils.h"
#include "options_prefs.h"
#include "calendar.h"
#include "stock_icons.h"

/*------------------------------------------------------------------------------*/

gboolean
check_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year)
                return TRUE;
        }

    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gchar *
get_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {

                if (a->note != NULL) {
                    return a->note;
                } else {
                    return NULL;
                }

            }
        }
    }

    return NULL;
}

/*------------------------------------------------------------------------------*/

gchar *
get_color (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {

                if (a->color != NULL) {
                    return a->color;
                } else {
                    return NULL;
                }

            }
        }
    }

    return NULL;
}

/*------------------------------------------------------------------------------*/

void
replace_note_color (gchar *old_color, gchar *new_color, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->color != NULL) {
                if (!strncmp(a->color, old_color, MAXCOLORNAME)) {
                    g_free(a->color);
                    a->color = g_strdup(new_color);
                }
            }

        }
    }
}

/*------------------------------------------------------------------------------*/

void
add_note (guint day, guint month, guint year, gchar *color_str, gchar *text_note, GUI *appGUI) {

struct note *a;

    if (text_note != NULL) {

        a = g_malloc(sizeof(struct note));
        g_assert (a != NULL);

        a->day = day;
        a->month = month;
        a->year = year;

        if (color_str != NULL) {
            a->color = g_strdup(color_str);
        } else {
            a->color = NULL;
        }

        a->note = g_strdup(text_note);

        remove_note (day, month, year, appGUI);     /* FIXME: check STACK here! */

        appGUI->cal->notes_list = g_slist_append (appGUI->cal->notes_list, a);
    }
}

/*------------------------------------------------------------------------------*/

void
remove_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {
                if (a->color != NULL) {
                    g_free(a->color);
                }
                if (a->note != NULL) {
                    g_free(a->note);
                }
                appGUI->cal->notes_list = g_slist_remove (appGUI->cal->notes_list, a);
                break;
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
free_notes_list (GUI *appGUI) {

GSList *node;
struct note *a;
gint i = 0;

    for (node = appGUI->cal->notes_list; node != NULL; node = node->next, i++) {
        a = g_slist_nth_data (appGUI->cal->notes_list, i);
        if (a->color != NULL) {
            g_free(a->color);
        }
        if (a->note != NULL) {
            g_free(a->note);
        }
    }

    if (appGUI->cal->notes_list != NULL) {
        g_slist_free(appGUI->cal->notes_list);
        appGUI->cal->notes_list = NULL;
    }
}

/*------------------------------------------------------------------------------*/

void
read_notes (GUI *appGUI) {

xmlDocPtr doc;
xmlChar *key, *prop;
xmlNodePtr node, cnode, category_node;
GtkTreeIter iter;
GdkPixbuf *image;
guint day, month, year;
gchar *color_str;

    appGUI->cal->notes_list = NULL;

    if (g_file_test (prefs_get_config_filename(CALENDAR_NOTES_FILENAME, appGUI), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(CALENDAR_NOTES_FILENAME, appGUI)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            return;
        }

        if(!xmlStrcmp(node->name, (xmlChar *) CALENDAR_NOTES_NAME)) {

            /* read note */
            node = node->xmlChildrenNode;

            while (node != NULL) {

                if(!xmlStrcmp(node->name, (xmlChar *) CALENDAR_DAY_CATEGORIES_NAME)) {

                    category_node = node->xmlChildrenNode;

                    while (category_node != NULL) {

                        if ((!xmlStrcmp(category_node->name, (const xmlChar *) "name"))) {
                            key = xmlNodeListGetString(doc, category_node->xmlChildrenNode, 1);
                            prop = xmlGetProp(category_node, (const xmlChar *) "color");
                            if (key != NULL) {
                                image = gui_create_color_swatch ((gchar *) prop);
                                gtk_list_store_append(appGUI->opt->calendar_category_store, &iter);
                                gtk_list_store_set(appGUI->opt->calendar_category_store, &iter, 0, image, 
                                                   1, prop, 2, (gchar *) key, -1);
                                g_object_unref (image);
                            }
                            xmlFree(key);
                        }
                        category_node = category_node->next;
                    }
                }

                if(!xmlStrcmp(node->name, (xmlChar *) "note")) {

                    cnode = node->xmlChildrenNode;

                    day = month = year = 0;
                    color_str = NULL;

                    while (cnode != NULL) {

                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "day"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                    day = atoi ((gchar *) key);
                            xmlFree(key);
                        }
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "month"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                    month = atoi ((gchar *) key);
                            xmlFree(key);
                        }
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "year"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                    year = atoi ((gchar *) key);
                            xmlFree(key);
                        }
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "color"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                    color_str = g_strdup((gchar *) key);
                            xmlFree(key);
                        }
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "message"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL) {
                                add_note (day, month, year, color_str, (gchar *) key, appGUI);
                                if (color_str != NULL) {
                                    g_free(color_str);
                                }
                            }
                            xmlFree(key);
                        }

                        cnode = cnode->next;
                    }

                }

                node = node->next;
            }

        }

        xmlFree(node);
        xmlFreeDoc(doc);
    }

}

/*------------------------------------------------------------------------------*/

void
write_notes (GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;
xmlDocPtr doc;
xmlNodePtr main_node, node, note_node, dc_node;
xmlAttrPtr attr;
gchar *category, *color_str;
GtkTreeIter iter;
gchar temp[BUFFER_SIZE];


    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    main_node = xmlNewNode(NULL, (const xmlChar *) CALENDAR_NOTES_NAME);
    xmlDocSetRootElement(doc, main_node);

    node = xmlNewChild(main_node, NULL, (const xmlChar *) CALENDAR_DAY_CATEGORIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, 1, &color_str, 2, &category, -1);
        dc_node = xmlNewChild(node, NULL, (const xmlChar *) "name", (xmlChar *) category);
        g_free(category);
        xmlNewProp(dc_node, (const xmlChar *) "color", (xmlChar *) color_str);
        g_free(color_str);
    }

    for (i = 0, lnode = appGUI->cal->notes_list; lnode; lnode = lnode->next, i++) {

        a = g_slist_nth_data (appGUI->cal->notes_list, i);
        note_node = xmlNewChild(main_node, NULL, (const xmlChar *) "note", (xmlChar *) NULL);
        sprintf(temp, "%d", a->day);
        xmlNewChild(note_node, NULL, (const xmlChar *) "day", (xmlChar *) temp);
        sprintf(temp, "%d", a->month);
        xmlNewChild(note_node, NULL, (const xmlChar *) "month", (xmlChar *) temp);
        sprintf(temp, "%d", a->year);
        xmlNewChild(note_node, NULL, (const xmlChar *) "year", (xmlChar *) temp);
        xmlNewChild(note_node, NULL, (const xmlChar *) "color", (xmlChar *) a->color);
        xmlNewChild(note_node, NULL, (const xmlChar *) "message", (xmlChar *) a->note);
    }

    xmlSaveFormatFileEnc(prefs_get_config_filename(CALENDAR_NOTES_FILENAME, appGUI), doc, "utf-8", 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/

void
day_notes_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_destroy (appGUI->cal->day_notes_window);
}

/*------------------------------------------------------------------------------*/

void
button_day_notes_window_close_cb (GtkButton *button, gpointer user_data) {

    day_notes_window_close_cb (GTK_WIDGET(button), NULL, user_data);

}

/*------------------------------------------------------------------------------*/

gint 
day_notes_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Escape:
            gtk_widget_destroy(appGUI->cal->day_notes_window);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
notes_filter_combo_box_focus_cb (GtkWidget *widget, GtkDirectionType *arg1, gpointer user_data) {
    return TRUE;
}

/*------------------------------------------------------------------------------*/

gchar *
note_reformatting (gchar *note) {

gchar *tmp_note;
gint j, k;

    tmp_note = g_strdup (note);

    j = k = 0;
    while (note[j++] == '\n');
    --j;
    while (note[j] != '\0') {
        if (note[j] == '\n') {
            while (note[j++] == '\n');
            --j;
            if (note[j] != '\0'){
                tmp_note[k++] = '\n';
            }
        } else {
            tmp_note[k++] = note[j++];
        }
    }
    tmp_note[k] = '\0';

    return tmp_note;
}

/*------------------------------------------------------------------------------*/

void
notes_display_items (guint32 julian_start, guint32 julian_end, gchar *search_string, GUI *appGUI) {

GSList          *lnode;
struct note     *day_note;
guint32         julian_day;
GtkTreeIter     iter;
gchar           *tmp_note, *search_key_1, *search_key_2;
gboolean        show_flag, case_state;
gint            i, l, n, m, n_items;
gchar           tmpbuf[BUFFER_SIZE];

    n_items = 0;

    if (appGUI->cal->day_notes_list != NULL && appGUI->cal->day_notes_list_store != NULL) {
    
        gtk_list_store_clear (GTK_LIST_STORE(appGUI->cal->day_notes_list_store));

        if (julian_start+julian_end != 0) {

            for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

                day_note = g_slist_nth_data (appGUI->cal->notes_list, i);
                julian_day = date_to_julian (day_note->day, day_note->month-1, day_note->year);

                tmp_note = note_reformatting (day_note->note);

                if (search_string == NULL) {
                    show_flag = TRUE;
                } else {
                    if (strlen(search_string) != 0) {

                        show_flag = FALSE;

                        l = strlen(search_string);
                        m = strlen(tmp_note) - l;

                        if (m >= 0) {

                            case_state = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->cal->notes_search_checkbutton));

                            if (case_state == FALSE) {
                                search_key_1 = g_utf8_casefold (search_string, l);
                            } else {
                                search_key_1 = g_utf8_collate_key (search_string, l);
                            }

                            for (n = 0; n < m; n++) {

                                if (case_state == FALSE) {
                                    search_key_2 = g_utf8_casefold (tmp_note+n, l);
                                } else {
                                    search_key_2 = g_utf8_collate_key (tmp_note+n, l);
                                }

                                if (g_utf8_collate(search_key_1, search_key_2) == 0) {
                                    show_flag = TRUE;
                                    g_free(search_key_2);
                                    break;
                                }

                                g_free(search_key_2);
                            }

                            g_free(search_key_1);
                        }

                    } else {
                        show_flag = TRUE;
                    }
                }

                if (((julian_start == -1 && julian_end == -1) || 
                    (julian_day >= julian_start && julian_day <= julian_end)) && show_flag) {

                    gtk_list_store_append(appGUI->cal->day_notes_list_store, &iter);
                    gtk_list_store_set(appGUI->cal->day_notes_list_store, &iter, 
                                       DN_COLUMN_DATE, julian_to_str (julian_day, config.date_format),
                                       DN_COLUMN_DATE_JULIAN, julian_day, 
                                       DN_COLUMN_NOTE_LINE, tmp_note, DN_COLUMN_DONE, FALSE, -1);

                    if (julian_day < get_julian_for_today() && gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->cal->past_notes_checkbutton))) {
                        gtk_list_store_set(appGUI->cal->day_notes_list_store, &iter, DN_COLUMN_DONE, TRUE, -1);
                    }

                    n_items++;

                }

                g_free (tmp_note);

            }
        }

        if (!n_items) {
            sprintf (tmpbuf, "<i>%s</i>", _("no entries"));
        } else {
            sprintf (tmpbuf, "<i>%4d %s</i>", n_items, ngettext ("entry", "entries", n_items));
        }

        gtk_label_set_markup (GTK_LABEL (appGUI->cal->n_items_label), tmpbuf);

    }
}

/*------------------------------------------------------------------------------*/

void
refresh_notes_list (gint filter_mode, gchar *search_string, GUI *appGUI) {

gint year, month;
guint32 julian_start, julian_end;

    gtk_widget_hide (appGUI->cal->notes_month_spinbutton);
    gtk_widget_hide (appGUI->cal->notes_year_spinbutton);

    if (filter_mode == DN_FILTER_SELECTED_MONTH) {
        gtk_widget_show (appGUI->cal->notes_month_spinbutton);
    } else if (filter_mode == DN_FILTER_SELECTED_YEAR) {
        gtk_widget_show (appGUI->cal->notes_year_spinbutton);
    } else if (filter_mode == DN_FILTER_SELECTED_MONTH_YEAR) {
        gtk_widget_show (appGUI->cal->notes_month_spinbutton);
        gtk_widget_show (appGUI->cal->notes_year_spinbutton);
    } 
    
    julian_start = julian_end = 0;
    month = get_current_month();
    year = get_current_year();

    switch (filter_mode) {
        case DN_FILTER_CURRENT_MONTH:
                julian_start = date_to_julian(1, month, year);
                julian_end = date_to_julian (g_date_get_days_in_month (month + 1, year), month, year);
                break;
        case DN_FILTER_SELECTED_MONTH:
                month = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->notes_month_spinbutton))-1;
                julian_start = date_to_julian(1, month, year);
                julian_end = date_to_julian (g_date_get_days_in_month (month + 1, year), month, year);
                break;
        case DN_FILTER_CURRENT_YEAR:
                julian_start = date_to_julian(1, 0, year);
                julian_end = date_to_julian (g_date_get_days_in_month (12, year), 11, year);
                break;
        case DN_FILTER_SELECTED_YEAR:
                year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->notes_year_spinbutton));
                julian_start = date_to_julian(1, 0, year);
                julian_end = date_to_julian (g_date_get_days_in_month (12, year), 11, year);
                break;
        case DN_FILTER_SELECTED_MONTH_YEAR:
                year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->notes_year_spinbutton));
                month = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->notes_month_spinbutton))-1;
                julian_start = date_to_julian(1, month, year);
                julian_end = date_to_julian (g_date_get_days_in_month (month + 1, year), month, year);
                break;
        case DN_FILTER_ALL_NOTES:
                julian_start = julian_end = -1;
                break;
    }

    notes_display_items (julian_start, julian_end, search_string, appGUI);

    if (appGUI->cal->day_notes_list != NULL) {
        gtk_widget_grab_focus (appGUI->cal->day_notes_list);
    }
}

/*------------------------------------------------------------------------------*/

void
notes_filter_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    config.day_notes_browser_filter = gtk_combo_box_get_active (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox));

    refresh_notes_list (config.day_notes_browser_filter, NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

gint 
search_entry_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (event->keyval == GDK_Return) {
        refresh_notes_list (gtk_combo_box_get_active (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox)), 
                            (gchar *)gtk_entry_get_text(GTK_ENTRY(appGUI->cal->notes_search_entry)), appGUI);
        gtk_widget_grab_focus (appGUI->cal->notes_search_entry);
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint
day_notes_list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

GtkTreeIter iter;
GtkTreeModel *model;
guint32 julian_day;
DATE *dest_date;

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {

        if (gtk_tree_selection_get_selected (appGUI->cal->day_notes_list_selection, &model, &iter)) {

            gtk_tree_model_get (model, &iter, DN_COLUMN_DATE_JULIAN, &julian_day, -1);
 
            dest_date = julian_to_date (julian_day);
            if (dest_date != NULL) {
                jump_to_date (dest_date->day, dest_date->month-1, dest_date->year, appGUI);
                update_aux_calendars (appGUI);
                button_day_notes_window_close_cb (NULL, appGUI);
                g_free(dest_date);
            }
        }

        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
browser_past_notes_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    refresh_notes_list (config.day_notes_browser_filter, NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_notes_browser (GUI *appGUI) {

GtkWidget           *vbox1;
GtkWidget           *hbox1;
GtkWidget           *scrolledwindow;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *close_button;
GtkWidget           *notes_tools_table;
GtkWidget           *label;
GtkObject           *notes_month_spinbutton_adj;
GtkObject           *notes_year_spinbutton_adj;
GtkTreeViewColumn   *column;
GtkCellRenderer     *renderer;
gchar               tmpbuf[BUFFER_SIZE];

    appGUI->cal->day_notes_list = NULL;
    appGUI->cal->day_notes_list_store = NULL;

    appGUI->cal->day_notes_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->cal->day_notes_window), _("Calendar notes"));
    gtk_window_set_position (GTK_WINDOW (appGUI->cal->day_notes_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_default_size (GTK_WINDOW(appGUI->cal->day_notes_window), 570, 680);
    gtk_window_set_modal (GTK_WINDOW (appGUI->cal->day_notes_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cal->day_notes_window), "delete_event",
                      G_CALLBACK(day_notes_window_close_cb), appGUI);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->cal->day_notes_window), GTK_WINDOW(appGUI->main_window));
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cal->day_notes_window), 8);
    g_signal_connect (G_OBJECT (appGUI->cal->day_notes_window), "key_press_event",
                      G_CALLBACK (day_notes_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->cal->day_notes_window), vbox1);

    notes_tools_table = gtk_table_new (6, 3, FALSE);
    gtk_widget_show (notes_tools_table);
    gtk_box_pack_start (GTK_BOX (vbox1), notes_tools_table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (notes_tools_table), 3);
    gtk_table_set_col_spacings (GTK_TABLE (notes_tools_table), 2);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (notes_tools_table), hseparator, 0, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 2);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (notes_tools_table), hseparator, 0, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 2);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (notes_tools_table), hseparator, 0, 3, 5, 6,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 2);

    sprintf(tmpbuf, "<b>%s:</b>", _("Search"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (notes_tools_table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_table_attach (GTK_TABLE (notes_tools_table), hbox1, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    notes_month_spinbutton_adj = gtk_adjustment_new (1, 1, 12, 1, 5, 5);
    appGUI->cal->notes_month_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (notes_month_spinbutton_adj), 1, 0);
    gtk_widget_show (appGUI->cal->notes_month_spinbutton);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->cal->notes_month_spinbutton, TRUE, TRUE, 0);
    g_signal_connect (appGUI->cal->notes_month_spinbutton, "value-changed", 
                      G_CALLBACK (notes_filter_changed_cb), appGUI);

    notes_year_spinbutton_adj = gtk_adjustment_new (2008, 1000, 9999, 1, 10, 10);
    appGUI->cal->notes_year_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (notes_year_spinbutton_adj), 1, 0);
    gtk_widget_show (appGUI->cal->notes_year_spinbutton);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->cal->notes_year_spinbutton, TRUE, TRUE, 0);
    g_signal_connect (appGUI->cal->notes_year_spinbutton, "value-changed", 
                      G_CALLBACK (notes_filter_changed_cb), appGUI);

    sprintf(tmpbuf, "<b>%s:</b>", _("Filter"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (notes_tools_table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->cal->notes_filter_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->cal->notes_filter_combobox);
    g_signal_connect(G_OBJECT(appGUI->cal->notes_filter_combobox), "focus", 
                     G_CALLBACK(notes_filter_combo_box_focus_cb), NULL);
    g_signal_connect(G_OBJECT(appGUI->cal->notes_filter_combobox), "changed", 
                     G_CALLBACK(notes_filter_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (notes_tools_table), appGUI->cal->notes_filter_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("Current month"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("Selected month"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("Current year"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("Selected year"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("Selected month and year"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->notes_filter_combobox), _("All notes"));

    gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->cal->notes_filter_combobox), config.day_notes_browser_filter);

    appGUI->cal->n_items_label = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->n_items_label);
    gtk_widget_set_size_request (appGUI->cal->n_items_label, 100, -1);
    gtk_table_attach (GTK_TABLE (notes_tools_table), appGUI->cal->n_items_label, 2, 3, 4, 5,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->cal->n_items_label), TRUE);

    appGUI->cal->notes_search_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->cal->notes_search_entry);
    gtk_table_attach (GTK_TABLE (notes_tools_table), appGUI->cal->notes_search_entry, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->cal->notes_search_entry), "key_press_event",
                        G_CALLBACK (search_entry_key_press_cb), appGUI);

    appGUI->cal->notes_search_checkbutton = gtk_check_button_new_with_mnemonic (_("Case sensitive"));
    gtk_widget_show (appGUI->cal->notes_search_checkbutton);
    GTK_WIDGET_UNSET_FLAGS(appGUI->cal->notes_search_checkbutton, GTK_CAN_FOCUS);
    gtk_table_attach (GTK_TABLE (notes_tools_table), appGUI->cal->notes_search_checkbutton, 2, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->cal->past_notes_checkbutton = gtk_check_button_new_with_mnemonic (_("Strikethrough past day notes"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->cal->past_notes_checkbutton, GTK_CAN_FOCUS);
    g_signal_connect (G_OBJECT (appGUI->cal->past_notes_checkbutton), "toggled",
                      G_CALLBACK (browser_past_notes_cb), appGUI);
    gtk_widget_show (appGUI->cal->past_notes_checkbutton);
    gtk_table_attach (GTK_TABLE (notes_tools_table), appGUI->cal->past_notes_checkbutton, 0, 2, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 2);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->cal->day_notes_list_store = gtk_list_store_new(DAY_NOTES_NUM_COLUMNS, 
                                                           G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_BOOLEAN);

    appGUI->cal->day_notes_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->cal->day_notes_list_store));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->cal->day_notes_list), TRUE);
    gtk_widget_show (appGUI->cal->day_notes_list);
    GTK_WIDGET_SET_FLAGS (appGUI->cal->day_notes_list, GTK_CAN_DEFAULT);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->cal->day_notes_list);
    g_signal_connect(G_OBJECT(appGUI->cal->day_notes_list), "button_press_event",
                     G_CALLBACK(day_notes_list_dbclick_cb), appGUI);

    appGUI->cal->day_notes_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->cal->day_notes_list));

    /* create columns */

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Date"), renderer, 
                                                      "text", DN_COLUMN_DATE, 
                                                      "strikethrough", DN_COLUMN_DONE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->day_notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Julian", renderer, 
                                                      "text", DN_COLUMN_DATE_JULIAN, 
                                                      "strikethrough", DN_COLUMN_DONE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->day_notes_list), column);
    gtk_tree_view_column_set_sort_column_id (column, DN_COLUMN_DATE_JULIAN);

    g_signal_emit_by_name(column, "clicked");

    if (config.ascending_sorting_in_day_notes_browser == FALSE) {
        g_signal_emit_by_name(column, "clicked");
    }

    renderer = gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    column = gtk_tree_view_column_new_with_attributes(_("Note"), renderer, 
                                                      "text", DN_COLUMN_NOTE_LINE, 
                                                      "strikethrough", DN_COLUMN_DONE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->day_notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, 
                                                      "text", DN_COLUMN_DONE, 
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->day_notes_list), column);

    refresh_notes_list (config.day_notes_browser_filter, NULL, appGUI);
   
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
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    g_signal_connect(close_button, "clicked", G_CALLBACK(button_day_notes_window_close_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), close_button);

    gtk_widget_show(appGUI->cal->day_notes_window);
    gtk_widget_grab_focus (appGUI->cal->day_notes_list);

}

/*------------------------------------------------------------------------------*/

