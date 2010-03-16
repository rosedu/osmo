
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
#include <stdlib.h>
#include <glib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

#include "i18n.h"
#include "contacts.h"
#include "contacts_items.h"
#include "gui.h"
#include "gui_utils.h"
#include "options_prefs.h"
#include "stock_icons.h"
#include "contacts_import.h"
#include "contacts_export.h"
#include "calendar_utils.h"

/*------------------------------------------------------------------------------*/

void
set_export_active (GUI *appGUI) {

GtkTreeIter iter;
guint n;
gboolean state;

    n = 0;
    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->cnt->contacts_filter), &iter, NULL, n++));
    state = (n == 1) ? FALSE : TRUE;
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/export"), state);
}

/*------------------------------------------------------------------------------*/

void
show_contacts_desc_panel (gboolean enable, GUI *appGUI) {

GtkTreeIter  iter;
GdkRectangle rect, visible_rect;
GtkTreePath  *visible_path;
GtkTreeModel *model;
GtkAdjustment *adj;

    if(enable == TRUE) {

        if (gtk_tree_selection_get_selected (appGUI->cnt->contacts_list_selection, &model, &iter)) {

            gtk_paned_set_position(GTK_PANED(appGUI->cnt->contacts_paned), config.contacts_pane_pos);

            while (g_main_context_iteration(NULL, FALSE));

            visible_path = gtk_tree_model_get_path (model, &iter);

            if (visible_path) {

                gtk_tree_view_get_cell_area (GTK_TREE_VIEW (appGUI->cnt->contacts_list), visible_path, NULL, &rect);
                gtk_tree_view_get_visible_rect (GTK_TREE_VIEW (appGUI->cnt->contacts_list), &visible_rect);

                if (rect.y < visible_rect.y || rect.y > visible_rect.y + visible_rect.height) {
                        gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (appGUI->cnt->contacts_list), visible_path, NULL, TRUE, 0.5, 0.0);
                }

                gtk_tree_path_free(visible_path);
            }

        } else {
            enable = FALSE;
        }

    } else {

        config.contacts_pane_pos = gtk_paned_get_position(GTK_PANED(appGUI->cnt->contacts_paned));
        gtk_paned_set_position(GTK_PANED(appGUI->cnt->contacts_paned), 99999);

        adj = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(appGUI->cnt->contacts_panel_scrolledwindow));
        gtk_adjustment_set_value (adj, 0.0);
        gtk_adjustment_value_changed (adj);

    }

    appGUI->cnt->contacts_panel_status = enable;
}

/*------------------------------------------------------------------------------*/

gboolean
find_combo_box_focus_cb (GtkWidget *widget, GtkDirectionType *arg1, gpointer user_data) {
    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
contacts_panel_close_desc_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    show_contacts_desc_panel(FALSE, appGUI);
}

/*------------------------------------------------------------------------------*/

gboolean
contacts_list_filter_cb (GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {

const gchar *text;
gchar *value;
gint i, j, len, text_len;
guint32 date;

    GUI *appGUI = (GUI *)data;

    if (appGUI->cnt->contacts_filter_disabled == TRUE) {
        return TRUE;
    }

    text = gtk_entry_get_text(GTK_ENTRY(appGUI->cnt->contacts_find_entry));

    if (text == NULL) {
        return TRUE;
    }

    text_len = strlen(text);

    if (text_len) {

        if(config.find_mode == CONTACTS_FF_FIRST_NAME) {

            gtk_tree_model_get(model, iter, COLUMN_FIRST_NAME, &value, -1);
            if(value != NULL) {
                if (strlen(value)) {
                    if(g_ascii_strncasecmp(g_utf8_casefold(text, -1), g_utf8_casefold(value, -1), text_len))
                        return FALSE;
                }
            } else {
                return FALSE;
            }

        } else if (config.find_mode == CONTACTS_FF_LAST_NAME) {

            gtk_tree_model_get(model, iter, COLUMN_LAST_NAME, &value, -1);
            if(value != NULL) {
                if(strlen(value)) {
                    if(g_ascii_strncasecmp(g_utf8_casefold(text, -1), g_utf8_casefold(value, -1), text_len))
                        return FALSE;
                }
            } else {
                return FALSE;
            }

        } else if (config.find_mode == CONTACTS_FF_ALL_FIELDS) {

            for(i=0; i < CONTACTS_NUM_COLUMNS; i++) {

                if (i != COLUMN_PHOTO) {
                    if (i == COLUMN_BIRTH_DAY_DATE || i == COLUMN_NAME_DAY_DATE) {
                        gtk_tree_model_get (model, iter, i, &date, -1);
                        if (date == 0) {
                            value = NULL;
                        } else {
                            if (i == COLUMN_BIRTH_DAY_DATE) {
                                value = g_strdup((const gchar *)julian_to_str(date, DATE_FULL));
                            } else {
                                value = g_strdup((const gchar *)julian_to_str(date, DATE_NAME_DAY));
                            }
                        }
                    } else {
                        gtk_tree_model_get(model, iter, i, &value, -1);
                    }

                    if (value != NULL) {

                        len = strlen(value) - text_len;

                        if (len >= 0) {
                            for(j=0; j <= len; j++) {
                                if(!g_ascii_strncasecmp(g_utf8_casefold(text, -1), g_utf8_casefold(value+j, -1), text_len))
                                    return TRUE;
                            }
                        }

                        g_free(value);
                    }
                }
            }

            return FALSE;
        }
    }

    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
contacts_select_first_position_in_list(GUI *appGUI) {

GtkTreeIter     iter;
GtkTreePath     *path;

    /* set cursor at first position */
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter) == TRUE) {
        path = gtk_tree_model_get_path (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter);
        if (path != NULL) {
            gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->cnt->contacts_list), path, NULL, FALSE);
            gtk_tree_path_free(path);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
contacts_item_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;
gchar *text;
GtkTextIter titer;
GtkTextBuffer *text_buffer;
gint i, j;
gchar photo_full_path[PATH_MAX], space_line[PATH_MAX];
char tmpbuf[BUFFER_SIZE];
gboolean activate_home_phone, activate_work_phone, activate_cell_phone;
gboolean activate_email, activate_www;
guint32 date;

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {

        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/edit"), TRUE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/delete"), TRUE);
        set_export_active (appGUI);

        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview));
        gui_clear_text_buffer (text_buffer, &titer);

        if (appGUI->cnt->photo_image != NULL) {      /* remove old photo */
            gtk_widget_destroy(appGUI->cnt->photo_image);
            appGUI->cnt->photo_image = NULL;
        }

        gui_url_remove_links (&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index);

        for(i=0; i < CONTACTS_NUM_COLUMNS; i++) {

            if (i == COLUMN_BIRTH_DAY_DATE || i == COLUMN_NAME_DAY_DATE) {
                gtk_tree_model_get (model, &iter, i, &date, -1);
                if (date == 0) {
                    text = NULL;
                } else {
                    if (i == COLUMN_BIRTH_DAY_DATE) {
                        text = g_strdup((const gchar *)julian_to_str(date, DATE_FULL));
                    } else {
                        text = g_strdup((const gchar *)julian_to_str(date, DATE_NAME_DAY));
                    }
                }
            } else {
                gtk_tree_model_get (model, &iter, i, &text, -1);
            }

                if (i != COLUMN_PHOTO) {

                    if (i == COLUMN_FIRST_NAME || i == COLUMN_SECOND_NAME || i == COLUMN_LAST_NAME) {

                        if (text != NULL) {
                            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, text, -1,
                                                      "big", NULL);
                            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, " ", -1,
                                                      "big", NULL);
                        }

                        if (i == COLUMN_LAST_NAME) {
                            gtk_text_buffer_insert(text_buffer, &titer, "\n", -1);
                        }

                    } else if (text != NULL && i!= COLUMN_GROUP) {

                        gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, "\n", -1, "normal", NULL);

                        activate_home_phone = (i == COLUMN_HOME_PHONE_2 || i == COLUMN_HOME_PHONE_3 || i == COLUMN_HOME_PHONE_4) && strlen(text);
                        activate_work_phone = (i == COLUMN_WORK_PHONE_2 || i == COLUMN_WORK_PHONE_3 || i == COLUMN_WORK_PHONE_4) && strlen(text);
                        activate_cell_phone = (i == COLUMN_WORK_PHONE_2 || i == COLUMN_WORK_PHONE_3 || i == COLUMN_WORK_PHONE_4) && strlen(text);
                        activate_email = (i == COLUMN_EMAIL_2 || i == COLUMN_EMAIL_3 || i == COLUMN_EMAIL_4) && strlen(text);
                        activate_www = (i == COLUMN_WWW_2 || i == COLUMN_WWW_3 || i == COLUMN_WWW_4) && strlen(text);

                        for(j=0;j<PATH_MAX;j++) space_line[j] = ' ';

                        if ((i >= COLUMN_HOME_PHONE_1 && i <= COLUMN_HOME_PHONE_4)  ||
                            (i >= COLUMN_WORK_PHONE_1 && i <= COLUMN_WORK_PHONE_4)  ||
                            (i >= COLUMN_CELL_PHONE_1 && i <= COLUMN_CELL_PHONE_4)  ||
                            (i >= COLUMN_EMAIL_1 && i <= COLUMN_EMAIL_4) ||
                            (i >= COLUMN_WWW_1 && i <= COLUMN_WWW_4)) {

                            if ((activate_home_phone == FALSE && i == COLUMN_HOME_PHONE_1) ||
                               (activate_work_phone == FALSE && i == COLUMN_WORK_PHONE_1)  ||
                               (activate_cell_phone == FALSE && i == COLUMN_CELL_PHONE_1)  ||
                               (activate_email == FALSE && i == COLUMN_EMAIL_1) ||
                               (activate_www == FALSE && i == COLUMN_WWW_1)) {

                                sprintf(tmpbuf, "%s: ", gettext(appGUI->cnt->contact_fields_tags_name[2*i]));
                            } else {
                                sprintf(tmpbuf, " ");
                            }

                        } else {
                            sprintf(tmpbuf, "%s: ", gettext(appGUI->cnt->contact_fields_tags_name[2*i]));
                        }

                        if (i != COLUMN_INFO) {
                            space_line[appGUI->cnt->longest_tag_name - strlen(tmpbuf) + 2] = '\0';
                            strcat(tmpbuf, space_line);
                        }
                        gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, tmpbuf, -1, "tag_name", NULL);

                        if (i == COLUMN_INFO) {
                            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, "\n", -1, "normal", NULL);
                        }

                        if (i == COLUMN_BLOG || (i >= COLUMN_EMAIL_1 && i <= COLUMN_EMAIL_4) || (i >= COLUMN_WWW_1 && i <= COLUMN_WWW_4)) { /* insert link */
                            gui_url_insert_link(&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index, appGUI->cnt->contacts_desc_textview , &titer, config.contact_link_color, config.tags_font_size, text, FALSE, appGUI);
                        } else {
                             gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, text, -1, "normal", NULL);
                        }
                    }
                }

                g_free(text);
        }

        /* insert photo */
        gtk_tree_model_get (model, &iter, COLUMN_PHOTO, &text, -1);

        if (text != NULL) {
            strncpy(photo_full_path, text, PATH_MAX-1);
            insert_photo(photo_full_path, &titer, appGUI);
            g_free(text);
        }

    } else {
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/edit"), FALSE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/delete"), FALSE);
        set_export_active (appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
contacts_desc_scrolled (GtkAdjustment *adj,  gpointer data) {

gint x, y;

    GUI *appGUI = (GUI *)data;

    if (appGUI->cnt->photo_image != NULL) {
        gtk_text_view_buffer_to_window_coords(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), GTK_TEXT_WINDOW_TEXT,
                                              appGUI->cnt->contacts_desc_textview->allocation.width-config.photo_width-5, 5, &x, &y);
        gtk_text_view_move_child(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), appGUI->cnt->photo_image,
                                 appGUI->cnt->contacts_desc_textview->allocation.width-config.photo_width-5, y);
    }
}

void
contacts_desc_size_allocate (GtkWidget *widget, GtkAllocation *allocation, gpointer data) {

    GUI *appGUI = (GUI *)data;
    contacts_desc_scrolled(NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

void
add_contacts_toolbar_widget (GtkUIManager *contacts_uim_widget, GtkWidget *widget, gpointer user_data) {

GtkWidget *handle_box;

    GUI *appGUI = (GUI *)user_data;

    if (GTK_IS_TOOLBAR (widget)) {

        appGUI->cnt->contacts_toolbar = GTK_TOOLBAR (widget);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (appGUI->cnt->vbox, handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", G_CALLBACK (gtk_widget_destroy), handle_box);

    } else {
        gtk_box_pack_start (appGUI->cnt->vbox, widget, FALSE, FALSE, 0);
    }

    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

gboolean
contacts_search_entry_changed_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

GtkTreePath *path;
GtkTreeIter iter;
GtkTextIter titer;
gint i;

    GUI *appGUI = (GUI *)user_data;

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));
    set_export_active (appGUI);

    if(strlen(gtk_entry_get_text (GTK_ENTRY(appGUI->cnt->contacts_find_entry)))) {

        if (config.show_after_search == TRUE) {

            i = 0;
            while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->cnt->contacts_filter), &iter, NULL, i++));

            if (i-1 != 0) {
                show_contacts_desc_panel(TRUE, appGUI);

                path = gtk_tree_path_new_first();
                if (path != NULL) {
                    gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->cnt->contacts_list), path, NULL, FALSE);
                    g_free(path);
                }
            } else {
                gui_clear_text_buffer (gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview)), &titer);
            }
        }
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
contacts_find_type_selected_cb (GtkComboBox *widget, gpointer user_data) {

GtkTreePath *path;

    GUI *appGUI = (GUI *)user_data;

    config.find_mode = gtk_combo_box_get_active (widget);

    if(strlen(gtk_entry_get_text (GTK_ENTRY(appGUI->cnt->contacts_find_entry)))) {
        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));

        path = gtk_tree_path_new_first();
        if (path != NULL) {
            gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->cnt->contacts_list), path, NULL, FALSE);
            g_free(path);
            show_contacts_desc_panel(TRUE, appGUI);
        }
    }
}

/*------------------------------------------------------------------------------*/

gint
custom_contacts_sort_function (GtkTreeModel *model, GtkTreeIter *iter_a, GtkTreeIter *iter_b, gpointer user_data) {

gchar *last_name_a, *last_name_b;
gchar *group_a, *group_b;
gchar *first_name_a, *first_name_b;
gint group_s, first_name_s, last_name_s;

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->cnt->contacts_filter_disabled == TRUE)
        return 0;

    if(iter_a == NULL || iter_b == NULL) {
        return 0;
    }

    gtk_tree_model_get(model, iter_a,
                       COLUMN_GROUP, &group_a, COLUMN_FIRST_NAME, &first_name_a, COLUMN_LAST_NAME, &last_name_a, -1);
    gtk_tree_model_get(model, iter_b,
                       COLUMN_GROUP, &group_b, COLUMN_FIRST_NAME, &first_name_b, COLUMN_LAST_NAME, &last_name_b, -1);

    if (group_a == NULL) {
        group_s = -1;
    } else if (group_b == NULL) {
        group_s = 1;
    } else if (group_a != NULL && group_b != NULL) {
        group_s = g_utf8_collate(group_a, group_b);
    } else {
        group_s = 0;
    }

    if (group_a != NULL) {
        g_free(group_a);
    }
    if (group_b != NULL) {
        g_free(group_b);
    }

    if (first_name_a == NULL) {
        first_name_s = -1;
    } else if (first_name_b == NULL) {
        first_name_s = 1;
    } else if (first_name_a != NULL && first_name_b != NULL) {
        first_name_s = g_utf8_collate(first_name_a, first_name_b);
    } else {
        first_name_s = 0;
    }

    if (first_name_a != NULL) {
        g_free(first_name_a);
    }
    if (first_name_b != NULL) {
        g_free(first_name_b);
    }

    if (last_name_a == NULL) {
        last_name_s = -1;
    } else if (last_name_b == NULL) {
        last_name_s = 1;
    } else if (last_name_a != NULL && last_name_b != NULL) {
        last_name_s = g_utf8_collate(last_name_a, last_name_b);
    } else {
        last_name_s = 0;
    }

    if (last_name_a != NULL) {
        g_free(last_name_a);
    }
    if (last_name_b != NULL) {
        g_free(last_name_b);
    }

    switch(config.contacts_sorting_mode) {

        /* Group, First Name, Last Name */
        case 0:
            if (group_s != 0)
                return group_s;
            if (first_name_s != 0)
                return first_name_s;
            if (last_name_s != 0)
                return last_name_s;
            break;

        /* Group, Last Name, First Name */
        case 1:
            if (group_s != 0)
                return group_s;
            if (last_name_s != 0)
                return last_name_s;
            if (first_name_s != 0)
                return first_name_s;
            break;

        /* Last Name, First Name, Group */
        case 2:
            if (last_name_s != 0)
                return last_name_s;
            if (first_name_s != 0)
                return first_name_s;
            if (group_s != 0)
                return group_s;
            break;

        /* Last Name, Group, First Name */
        case 3:
            if (last_name_s != 0)
                return last_name_s;
            if (group_s != 0)
                return group_s;
            if (first_name_s != 0)
                return first_name_s;
            break;

        /* First Name, Last Name, Group */
        case 4:
            if (first_name_s != 0)
                return first_name_s;
            if (last_name_s != 0)
                return last_name_s;
            if (group_s != 0)
                return group_s;
            break;

        /* First Name, Group, Last Name */
        case 5:
            if (first_name_s != 0)
                return first_name_s;
            if (group_s != 0)
                return group_s;
            if (last_name_s != 0)
                return last_name_s;
            break;

        default:
            break;
    }

    return 0;
}

/*------------------------------------------------------------------------------*/

void
contacts_add_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    contacts_add_edit_dialog_show (FALSE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
contacts_edit_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (appGUI->cnt->contacts_list_selection, NULL, NULL)) {
        GUI *appGUI = (GUI *)data;
        contacts_add_edit_dialog_show (TRUE, appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
contacts_remove_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    contacts_remove_dialog_show(appGUI->cnt->contacts_list, appGUI->cnt->contacts_list_store, appGUI);
}

/*------------------------------------------------------------------------------*/

void
contacts_export_items_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    contacts_create_export_window(appGUI);
}

/*------------------------------------------------------------------------------*/

void
contacts_import_items_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    import_contacts_from_csv_file(appGUI);
}

/*------------------------------------------------------------------------------*/

gint
contacts_list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {

        if (appGUI->cnt->contacts_panel_status == TRUE) {
            contacts_edit_item_cb (NULL, appGUI);
        } else {
            show_contacts_desc_panel(TRUE, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
gui_create_contacts(GUI *appGUI) {

GtkWidget       *vbox1;
GtkWidget       *vbox2;
GtkWidget       *hbox2;
GtkWidget       *hseparator;
GtkWidget       *label;
GtkWidget       *top_scrolledwindow;
GtkWidget       *top_viewport;
GtkWidget       *bottom_viewport;
GtkWidget       *close_button;
GtkCellRenderer *renderer[CONTACTS_NUM_COLUMNS];
GType           contact_columns_types[CONTACTS_NUM_COLUMNS];
GtkTextBuffer   *text_buffer;
gint            i;
GError          *error = NULL;
GtkActionGroup  *action_group = NULL;
GtkAdjustment   *adjustment;
char tmpbuf[BUFFER_SIZE];

static gchar *contact_fields_tags_name[] = {
    N_("Group"), "group", N_("First name"), "first_name", N_("Second name"), "second_name", 
    N_("Last name"), "last_name", 
    N_("Nickname"), "nickname", N_("Birthday date"), "birthday_date", N_("Name day date"), "name_day_date",

    /*--------------------------------------------------*/
    N_("Home address"), "home_address", N_("Home postcode"), "home_postcode", N_("Home city"), 
    "home_city", N_("Home state"), "home_state", N_("Home country"), "home_country",
    /*--------------------------------------------------*/

    N_("Organization"), "organization", N_("Department"), "department",

    /*--------------------------------------------------*/
    N_("Work address"), "work_address", N_("Work postcode"), "work_postcode", N_("Work city"), 
    "work_city", N_("Work state"), "work_state", N_("Work country"), "work_country",
    /*--------------------------------------------------*/

    N_("Fax"), "work_fax",

    /*--------------------------------------------------*/
    N_("Home phone"), "home_phone_1", N_("Home phone 2"), "home_phone_2",
    N_("Home phone 3"), "home_phone_3", N_("Home phone 4"), "home_phone_4",
    N_("Work phone"), "work_phone_1", N_("Work phone 2"), "work_phone_2",
    N_("Work phone 3"), "work_phone_3", N_("Work phone 4"), "work_phone_4",
    N_("Cell phone"), "cell_phone_1", N_("Cell phone 2"), "cell_phone_2",
    N_("Cell phone 3"), "cell_phone_3", N_("Cell phone 4"), "cell_phone_4",
    N_("E-Mail"), "email_1", N_("E-Mail 2"), "email_2", N_("E-Mail 3"), "email_3", 
    N_("E-Mail 4"), "email_4", N_("WWW"), "www_1", N_("WWW 2"), "www_2", N_("WWW 3"), "www_3", 
    N_("WWW 4"), "www_4",
    /*--------------------------------------------------*/

    N_("IM Gadu-Gadu"), "im_gg", N_("IM Yahoo"), "im_yahoo", N_("IM MSN"), "im_msn",
    N_("IM ICQ"), "im_icq", N_("IM AOL"), "im_aol",
    N_("IM Jabber"), "im_jabber", N_("IM Skype"), "im_skype", N_("IM Tlen"), "im_tlen",
    N_("Blog"), "blog", N_("Photo"), "photo_path", N_("Additional info"), "additional_info"
};

const gchar *ui_info =
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"add\" action=\"add\" />\n"
"    <toolitem name=\"delete\" action=\"delete\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"edit\" action=\"edit\" />\n"
"    <separator name=\"sep2\" />\n"
"    <toolitem name=\"import\" action=\"import\" />\n"
"    <toolitem name=\"export\" action=\"export\" />\n"
"  </toolbar>\n";

GtkActionEntry entries[] = {
    { "add", OSMO_STOCK_CONTACTS_ADD, _("New contact"), NULL, _("New contact"), NULL },
    { "edit", OSMO_STOCK_CONTACTS_EDIT, _("Edit contact"), NULL, _("Edit contact"), NULL },
    { "delete", OSMO_STOCK_CONTACTS_REMOVE, _("Remove contact"), NULL, _("Remove contact"), NULL },
    { "import", OSMO_STOCK_CONTACTS_IMPORT, _("Import contacts"), NULL, _("Import contacts"), NULL },
    { "export", OSMO_STOCK_CONTACTS_EXPORT, _("Export contacts"), NULL, _("Export contacts"), NULL },
};

guint n_entries = G_N_ELEMENTS (entries);

    appGUI->cnt->contact_fields_tags_name = contact_fields_tags_name;
    appGUI->cnt->longest_tag_name = 0;

    for(i=0; i < CONTACTS_NUM_COLUMNS; i++) {
        if (appGUI->cnt->longest_tag_name < strlen(gettext(contact_fields_tags_name[i*2]))) {
            appGUI->cnt->longest_tag_name = strlen(gettext(contact_fields_tags_name[i*2]));
        }
    }

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    sprintf(tmpbuf, "<b>%s</b>", _("Contacts"));
    gtk_label_set_markup (GTK_LABEL (label), tmpbuf);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), vbox1, label);
    appGUI->cnt->vbox = GTK_BOX(vbox1);

    /*-------------------------------------------------------------------------------------*/

    action_group = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group, entries, n_entries, NULL);
    gtk_action_group_set_sensitive(action_group, TRUE);

    appGUI->cnt->contacts_uim_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (appGUI->cnt->contacts_uim_widget, action_group, 0);
    g_signal_connect (appGUI->cnt->contacts_uim_widget, "add_widget", G_CALLBACK (add_contacts_toolbar_widget), appGUI);

    if (!gtk_ui_manager_add_ui_from_string (appGUI->cnt->contacts_uim_widget, ui_info, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (appGUI->cnt->contacts_uim_widget);

    gtk_toolbar_set_style (appGUI->cnt->contacts_toolbar, GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_tooltips (appGUI->cnt->contacts_toolbar, config.enable_tooltips);

    /*-------------------------------------------------------------------------------------*/
    /* assign callbacks */

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/add")), "clicked", 
                      G_CALLBACK(contacts_add_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/edit")), "clicked", 
                      G_CALLBACK(contacts_edit_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/delete")), "clicked", 
                      G_CALLBACK(contacts_remove_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/export")), "clicked", 
                      G_CALLBACK(contacts_export_items_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/import")), "clicked", 
                      G_CALLBACK(contacts_import_items_cb), appGUI);

    /*-------------------------------------------------------------------------------------*/

    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/edit"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->cnt->contacts_uim_widget, "/toolbar/delete"), FALSE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, FALSE, 6);

    sprintf(tmpbuf, "<b>%s:</b>", _("Search"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_label_set_use_markup (GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX (vbox1), label, FALSE, FALSE, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, TRUE, 0);

    appGUI->cnt->contacts_find_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(appGUI->cnt->contacts_find_entry), 128);
    gtk_widget_show (appGUI->cnt->contacts_find_entry);
    g_signal_connect (G_OBJECT(appGUI->cnt->contacts_find_entry), "key_release_event",
                        G_CALLBACK(contacts_search_entry_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (hbox2), appGUI->cnt->contacts_find_entry, TRUE, TRUE, 0);

    appGUI->cnt->contacts_find_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->cnt->contacts_find_combobox);
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox2), appGUI->cnt->contacts_find_combobox, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (appGUI->cnt->contacts_find_combobox), "changed",
                      G_CALLBACK (contacts_find_type_selected_cb), appGUI);
    g_signal_connect(G_OBJECT(appGUI->cnt->contacts_find_combobox), "focus",
                     G_CALLBACK(find_combo_box_focus_cb), appGUI);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), _("First Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), _("Last Name"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), _("All fields"));

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, FALSE, 6);

/******************************************************************************************/

    appGUI->cnt->contacts_paned = gtk_vpaned_new();
    gtk_widget_show (appGUI->cnt->contacts_paned);
    gtk_paned_set_position(GTK_PANED(appGUI->cnt->contacts_paned), 99999);
    gtk_box_pack_start(GTK_BOX(vbox1), appGUI->cnt->contacts_paned, TRUE, TRUE, 0);

    top_viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (top_viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (top_viewport), GTK_SHADOW_NONE);
    gtk_paned_pack1 (GTK_PANED (appGUI->cnt->contacts_paned), top_viewport, FALSE, TRUE);

    top_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (top_scrolledwindow);
    gtk_container_add (GTK_CONTAINER (top_viewport), top_scrolledwindow);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (top_scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (top_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    for(i=0; i< CONTACTS_NUM_COLUMNS; i++) {
        if (i == COLUMN_BIRTH_DAY_DATE || i == COLUMN_NAME_DAY_DATE) {
            contact_columns_types[i] = G_TYPE_UINT;
        } else {
            contact_columns_types[i] = G_TYPE_STRING;
        }
    }

    appGUI->cnt->contacts_list_store = gtk_list_store_newv(CONTACTS_NUM_COLUMNS, &contact_columns_types[0]);

    appGUI->cnt->contacts_filter = gtk_tree_model_filter_new(GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), NULL);
    gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter),
                                            (GtkTreeModelFilterVisibleFunc)contacts_list_filter_cb,
                                            appGUI, NULL);

    appGUI->cnt->contacts_sort = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(appGUI->cnt->contacts_filter));

    appGUI->cnt->contacts_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->cnt->contacts_sort));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->cnt->contacts_list), config.contacts_rules_hint);
    gtk_widget_show (appGUI->cnt->contacts_list);
    GTK_WIDGET_SET_FLAGS (appGUI->cnt->contacts_list, GTK_CAN_DEFAULT);
    gtk_widget_modify_fg(GTK_WIDGET(appGUI->cnt->contacts_list), GTK_STATE_SELECTED,
                         (& GTK_WIDGET(appGUI->cnt->contacts_list)->style->base[GTK_STATE_SELECTED]));

    g_signal_connect(G_OBJECT(appGUI->cnt->contacts_list), "button_press_event",
                     G_CALLBACK(contacts_list_dbclick_cb), appGUI);

    appGUI->cnt->contacts_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->cnt->contacts_list));
    g_signal_connect(G_OBJECT(appGUI->cnt->contacts_list_selection), "changed",
                     G_CALLBACK(contacts_item_selected_cb), appGUI);

    /* columns setup */

    for (i = COLUMN_GROUP; i < CONTACTS_NUM_COLUMNS; i++) {
        renderer[i] = gtk_cell_renderer_text_new();
        appGUI->cnt->contacts_columns[i] = gtk_tree_view_column_new_with_attributes(gettext(contact_fields_tags_name[i*2]),
                                                              renderer[i], "text", i, NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->cnt->contacts_list), appGUI->cnt->contacts_columns[i]);

        if(i != COLUMN_FIRST_NAME && i != COLUMN_LAST_NAME && i != COLUMN_GROUP) {
            gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[i], FALSE);
        } else {
            if (config.hide_group_column == TRUE && i == COLUMN_GROUP) {
                gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[i], FALSE);
            }
            gtk_tree_view_column_set_expand (appGUI->cnt->contacts_columns[i], TRUE);
        }
    }

    gtk_container_add (GTK_CONTAINER (top_scrolledwindow), appGUI->cnt->contacts_list);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW(appGUI->cnt->contacts_list), FALSE);

    /* configure sorting */

    gtk_tree_sortable_set_sort_func((GtkTreeSortable *)appGUI->cnt->contacts_sort, 0,
                                    (GtkTreeIterCompareFunc)custom_contacts_sort_function, appGUI, NULL);

    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->cnt->contacts_sort, COLUMN_FIRST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->cnt->contacts_sort, COLUMN_LAST_NAME, config.contacts_sorting_order);
    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->cnt->contacts_sort, COLUMN_GROUP, config.contacts_sorting_order);

    bottom_viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (bottom_viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (bottom_viewport), GTK_SHADOW_NONE);
    gtk_paned_pack2 (GTK_PANED (appGUI->cnt->contacts_paned), bottom_viewport, TRUE, TRUE);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 0);
    gtk_container_add (GTK_CONTAINER (bottom_viewport), vbox2);

    appGUI->cnt->panel_hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cnt->panel_hbox, FALSE, FALSE, 0);
    gtk_widget_show(appGUI->cnt->panel_hbox);

    sprintf(tmpbuf, "%s:", _("Contact details"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (appGUI->cnt->panel_hbox), label, FALSE, FALSE, 0);

    if (config.default_stock_icons) {
        close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
    } else {
        close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    gtk_button_set_relief (GTK_BUTTON(close_button), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, close_button, _("Close contact panel"), NULL);
    gtk_box_pack_end (GTK_BOX (appGUI->cnt->panel_hbox), close_button, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (close_button), "clicked",
                        G_CALLBACK (contacts_panel_close_desc_cb), appGUI);

    appGUI->cnt->contacts_panel_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (appGUI->cnt->contacts_panel_scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cnt->contacts_panel_scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (appGUI->cnt->contacts_panel_scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (appGUI->cnt->contacts_panel_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->cnt->contacts_desc_textview = gtk_text_view_new ();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->cnt->contacts_desc_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), 4);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), 4);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), FALSE);
    gtk_widget_show (appGUI->cnt->contacts_desc_textview);
    gtk_container_add (GTK_CONTAINER (appGUI->cnt->contacts_panel_scrolledwindow), appGUI->cnt->contacts_desc_textview);

    gui_url_setup(&appGUI->cnt->contacts_links_list, &appGUI->cnt->contacts_link_index, appGUI->cnt->contacts_desc_textview, appGUI);

    adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(appGUI->cnt->contacts_panel_scrolledwindow));
    g_signal_connect(G_OBJECT(adjustment), "value-changed", G_CALLBACK(contacts_desc_scrolled), appGUI);
    g_signal_connect(G_OBJECT(appGUI->cnt->contacts_desc_textview), "size_allocate", 
                     G_CALLBACK(contacts_desc_size_allocate), appGUI);

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview));

    appGUI->cnt->contact_name_tag_object = gtk_text_buffer_create_tag (text_buffer, "big",
                                                          "size", config.name_font_size * PANGO_SCALE, NULL);
    appGUI->cnt->contact_tags_value_tag_object = gtk_text_buffer_create_tag (text_buffer, "normal",
                                                                "size", config.tags_font_size * PANGO_SCALE, NULL);
    appGUI->cnt->contact_color_tag_object = gtk_text_buffer_create_tag (text_buffer, "tag_name",
                               "foreground", config.contact_tag_color, "font", "Monospace",
                               "size", config.tags_font_size * PANGO_SCALE, "weight", PANGO_WEIGHT_SEMIBOLD, NULL);

    gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->cnt->contacts_find_combobox), config.find_mode);

    appGUI->cnt->contacts_filter_disabled = FALSE;
    gtk_tree_sortable_sort_column_changed((GtkTreeSortable *)appGUI->cnt->contacts_sort);
    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->cnt->contacts_filter));
}

/*------------------------------------------------------------------------------*/

void
insert_photo(gchar *photo_filename, GtkTextIter *iter, GUI *appGUI) {

GtkTextBuffer * buffer;
GdkPixbuf *pixbuf;
guchar *pixels, *p;
gint i, d_photo_width, d_photo_height;
gint rowstride, channels, bc1, bc2, bc3, bc4;


        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview));

        pixbuf = NULL;

        d_photo_width = config.photo_width;

        pixbuf = gdk_pixbuf_new_from_file_at_size (photo_filename, d_photo_width, -1, NULL);

        if (pixbuf != NULL) {

                d_photo_width = gdk_pixbuf_get_width(pixbuf);
                d_photo_height = gdk_pixbuf_get_height(pixbuf);
                channels = gdk_pixbuf_get_n_channels (pixbuf);
                rowstride = gdk_pixbuf_get_rowstride (pixbuf);
                pixels = gdk_pixbuf_get_pixels (pixbuf);

                bc1 = bc3 = 0;
                bc2 = bc4 = 255;

                /* horizontal lines */

                for(i=0; i < d_photo_width; i++) {
                        p = pixels + i * channels;
                        p[0] = p[1] = p[2] = bc1;
                        p = pixels + (d_photo_height-1) * rowstride + i * channels;
                        p[0] = p[1] = p[2] = bc2;
                }

                /* vertical lines */

                for(i=0; i < d_photo_height; i++) {
                        p = pixels + i * rowstride;
                        p[0] = p[1] = p[2] = bc3;
                        p = pixels + i * rowstride + (d_photo_width-1) * channels;
                        p[0] = p[1] = p[2] = bc4;
                }

                /* insert image */

                appGUI->cnt->photo_image = gtk_image_new_from_pixbuf(pixbuf);

                if (appGUI->cnt->photo_image != NULL) {
                    gtk_text_view_add_child_in_window(GTK_TEXT_VIEW(appGUI->cnt->contacts_desc_textview), appGUI->cnt->photo_image, GTK_TEXT_WINDOW_TEXT, 
                                                      appGUI->cnt->contacts_desc_textview->allocation.width-d_photo_width-5, 5);
                    gtk_widget_show_all(appGUI->cnt->contacts_desc_textview);
                }

                g_object_unref (pixbuf);
        }

}

/*------------------------------------------------------------------------------*/

