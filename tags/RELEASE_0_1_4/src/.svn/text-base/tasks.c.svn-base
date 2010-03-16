
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
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

#include "tasks.h"
#include "gui.h"
#include "gui_utils.h"
#include "i18n.h"
#include "tasks_items.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "time_utils.h"
#include "date_utils.h"
#include "options_prefs.h"
#include "stock_icons.h"

/*------------------------------------------------------------------------------*/

gint
get_combobox_items (GtkComboBox *combo_box) {

    return gtk_tree_model_iter_n_children (gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box)), NULL);
}

/*------------------------------------------------------------------------------*/

void
create_category_combobox (GtkComboBox *combo_box, GtkListStore *store, gboolean none) {

GtkTreeIter iter;
gchar *category;
gint i, n;

    n = get_combobox_items(combo_box);

    gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), -1);

    for (i = n-1; i >= 0; i--) {
        gtk_combo_box_remove_text (GTK_COMBO_BOX (combo_box), i);
    }

    if (none == TRUE) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), _("None"));
    } else {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), _("All items"));
    }

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &category, -1);
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), category);
        g_free(category);
    }

}

/*------------------------------------------------------------------------------*/

gint
get_priority_index (gchar *text) {

gchar *priority_table[] = {
    N_("Low"), N_("Medium"), N_("High")
};

gint i;

    if (text != NULL) {
        for(i=0; i<3; i++) {
            if (!strcmp(gettext(priority_table[i]), text)) 
                break;
        }
    }

    if (i == 3) {
        return -1;
    } else {
        return i;
    }
}

/*------------------------------------------------------------------------------*/

gchar*
get_priority_text (gint index) {

gchar *priority_table[] = {
    N_("Low"), N_("Medium"), N_("High")
};

    if (index >=0 && index <= 2) {
        return gettext(priority_table[index]);
    } else {
        return NULL;
    }
}

/*------------------------------------------------------------------------------*/

void
show_tasks_desc_panel (gboolean enable, GUI *appGUI) {

GtkTreeIter  iter;
GdkRectangle rect, visible_rect;
GtkTreePath  *visible_path;
GtkTreeModel *model;

    if(enable == TRUE) {

        if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, &model, &iter)) {

            gtk_paned_set_position(GTK_PANED(appGUI->tsk->tasks_paned), config.tasks_pane_pos);

            while (g_main_context_iteration(NULL, FALSE));

            visible_path = gtk_tree_model_get_path (model, &iter);

            if (visible_path) {

                gtk_tree_view_get_cell_area (GTK_TREE_VIEW (appGUI->tsk->tasks_list), visible_path, NULL, &rect);
                gtk_tree_view_get_visible_rect (GTK_TREE_VIEW (appGUI->tsk->tasks_list), &visible_rect);

                if (rect.y < visible_rect.y || rect.y > visible_rect.y + visible_rect.height) {
                        gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (appGUI->tsk->tasks_list), visible_path, NULL, TRUE, 0.5, 0.0);
                }

                gtk_tree_path_free(visible_path);
            }

        } else {
            enable = FALSE;
        }

    } else {

        config.tasks_pane_pos = gtk_paned_get_position(GTK_PANED(appGUI->tsk->tasks_paned));
        gtk_paned_set_position(GTK_PANED(appGUI->tsk->tasks_paned), 99999);

    }

    appGUI->tsk->tasks_panel_status = enable;
}

/*------------------------------------------------------------------------------*/

void
fill_fields (GtkWidget *due_date_entry, GtkWidget *summary_entry, GtkWidget *desc_entry, GUI *appGUI) {

GtkTreeIter iter;
GtkTextIter titer;
GtkTreePath *sort_path, *filter_path, *path;
gchar *fduedate, *fpriority, *fcategory, *fsummary, *fdesc;
GtkTextBuffer *text_buffer;


    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->tsk->tasks_list), &sort_path, NULL);

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(desc_entry));
    gui_clear_text_buffer (text_buffer, &titer);

    if (sort_path != NULL) {

        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->tsk->tasks_sort), sort_path);

        if (filter_path != NULL) {

            path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter), filter_path);

            if (path != NULL) {

                gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, path);

                gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                           COLUMN_DUE_DATE, &fduedate, 
                           COLUMN_DUE_DATE_JULIAN, &appGUI->tsk->tasks_due_julian_day, 
                           COLUMN_PRIORITY, &fpriority,
                           COLUMN_CATEGORY, &fcategory,
                           COLUMN_SUMMARY, &fsummary, 
                           COLUMN_DESCRIPTION, &fdesc, -1);

                gtk_entry_set_text(GTK_ENTRY(due_date_entry), fduedate);
                g_free(fduedate);
                gtk_entry_set_text(GTK_ENTRY(summary_entry), fsummary);
                g_free(fsummary);

                gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->category_combobox), 
                                         gui_list_store_get_text_index (appGUI->opt->tasks_category_store, fcategory));
                g_free(fcategory);
                gtk_combo_box_set_active(GTK_COMBO_BOX(appGUI->tsk->priority_combobox), 
                                         get_priority_index (fpriority));
                g_free(fpriority);

                gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &titer, 0);

                if (fdesc != NULL) {
                    gtk_text_buffer_insert(text_buffer, &titer, fdesc, -1);
                    gtk_text_view_set_buffer(GTK_TEXT_VIEW(desc_entry), text_buffer);
                    g_free(fdesc);
                }

                gtk_tree_path_free(path);

            }

            gtk_tree_path_free(filter_path);
        }

        gtk_tree_path_free(sort_path);
    }
}

/*------------------------------------------------------------------------------*/

void
panel_close_desc_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    show_tasks_desc_panel(FALSE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_item_selected (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;
gchar *text;
GtkTextIter titer;
GtkTextBuffer *text_buffer;
GtkTextChildAnchor *anchor;
GtkWidget *hseparator;
guint32 start_date_julian;
gchar tmpbuf[BUFFER_SIZE];

    GUI *appGUI = (GUI *)data;

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview));
    gui_clear_text_buffer (text_buffer, &titer);

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {

        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), TRUE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), TRUE);

        gtk_tree_model_get (model, &iter, COLUMN_DESCRIPTION, &text,
                            COLUMN_START_DATE_JULIAN, &start_date_julian, -1);  

        sprintf(tmpbuf, "\n%s: %s\n", _("Started"), julian_to_str(start_date_julian, DATE_FULL));

        if (text != NULL) {
            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, text, -1, 
                                                      "info_font", NULL);
            gtk_text_buffer_insert(text_buffer, &titer, "\n", -1);
        }

        if (start_date_julian) {
            gtk_text_buffer_insert(text_buffer, &titer, "\n", -1);
            anchor = gtk_text_buffer_create_child_anchor (text_buffer, &titer);
            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, tmpbuf, -1, 
                                                      "italic", NULL);
            gtk_text_view_set_buffer(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), text_buffer);

            hseparator = gtk_hseparator_new ();
            gtk_widget_show (hseparator);
            gtk_widget_set_size_request (hseparator, 240, -1);
            gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), hseparator, anchor);
        }

        g_free(text);
    } else {
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), FALSE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_add_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    tasks_add_edit_dialog_show (FALSE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_edit_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, NULL, NULL)) {
        tasks_add_edit_dialog_show (TRUE, appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_remove_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    tasks_remove_dialog_show(appGUI->tsk->tasks_list, appGUI->tsk->tasks_list_store, appGUI);
}

/*------------------------------------------------------------------------------*/

void
done_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {

/*GtkTreeModel *model = (GtkTreeModel *)data;*/
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter  iter;
gboolean done_status;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;
    model = GTK_TREE_MODEL(appGUI->tsk->tasks_list_store);

    sort_path = gtk_tree_path_new_from_string (path_str);

    if (sort_path != NULL) {

        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->tsk->tasks_sort), sort_path);

        if (filter_path != NULL) {

            path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter), filter_path);

            if (path != NULL) {
                gtk_tree_model_get_iter (model, &iter, path);   /* get toggled iter */
                gtk_tree_model_get (model, &iter, COLUMN_DONE, &done_status, -1);
                gtk_list_store_set (GTK_LIST_STORE(model), &iter, COLUMN_DONE, !done_status, -1);

                if (done_status == TRUE && config.delete_completed) {
                    gtk_list_store_remove(GTK_LIST_STORE (model), &iter);
                }

                gtk_tree_path_free (path);
            }
            gtk_tree_path_free (filter_path);
        }
        gtk_tree_path_free (sort_path);
    }

    apply_date_colors(appGUI);
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_color(guint32 julian_day, gboolean done) {

static gchar due_date_color[MAXCOLORNAME];
guint32 julian_day_current;
gint32 r;

    strncpy(due_date_color, "black", MAXCOLORNAME-1);

    if (julian_day != 0) {

        julian_day_current = get_julian_for_today();
        r = julian_day - julian_day_current;

        if (r == 0) {

            strncpy(due_date_color, config.due_today_color, MAXCOLORNAME-1);

        } else if (r > 0 && r < 7) {

            strncpy(due_date_color, config.due_7days_color, MAXCOLORNAME-1);

        } else if (r < 0 && done == FALSE) {

            strncpy(due_date_color, config.past_due_color, MAXCOLORNAME-1);
        }
    }

    return due_date_color;
}

/*------------------------------------------------------------------------------*/

void
apply_date_colors(GUI *appGUI) {

GtkTreeIter iter;
gint i;
gboolean done;
guint32 julian_day;

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, COLUMN_DONE, &done, COLUMN_DUE_DATE_JULIAN, &julian_day, -1);
        gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, COLUMN_COLOR, get_date_color(julian_day, done), -1);
    }
}

/*------------------------------------------------------------------------------*/

void
refresh_due_dates(GUI *appGUI) {

GtkTreeIter iter;
gint i;
guint32 julian_day;

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, COLUMN_DUE_DATE_JULIAN, &julian_day, -1);
        gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, COLUMN_DUE_DATE, julian_to_str(julian_day, config.date_format), -1);
    }

}

/*------------------------------------------------------------------------------*/

void
update_n_items(GUI *appGUI) {

GtkTreeIter iter;
gint i;
gchar tmpbuf[BUFFER_SIZE];

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_filter), &iter, NULL, i++));

    --i;

    if (!i) {
        sprintf (tmpbuf, "<i>%s</i>", _("no entries"));
    } else {
        sprintf (tmpbuf, "<i>%4d %s</i>", i, ngettext ("entry", "entries", i));
    }

    gtk_label_set_markup (GTK_LABEL (appGUI->tsk->n_items_label), tmpbuf);

}

/*------------------------------------------------------------------------------*/

void
add_item_to_list(gboolean done, guint32 due_date_julian, guint32 start_date_julian, gchar *priority, gchar *category, gchar *summary, gchar *desc, GUI *appGUI) {

GtkTreeIter iter;

    gtk_list_store_append(appGUI->tsk->tasks_list_store, &iter);

    if (due_date_julian == 0) {   /* use 'No date' text when due date is 0 */
        gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, COLUMN_DONE, done, COLUMN_DUE_DATE, _("No date"), 
                           COLUMN_START_DATE_JULIAN, start_date_julian, COLUMN_PRIORITY, priority, 
                           COLUMN_CATEGORY, category, COLUMN_SUMMARY, summary, COLUMN_DESCRIPTION, desc, 
                           COLUMN_COLOR, get_date_color(due_date_julian, done), -1);
    } else {
        gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, COLUMN_DONE, done, COLUMN_DUE_DATE, julian_to_str(due_date_julian, config.date_format),
                           COLUMN_DUE_DATE_JULIAN, due_date_julian,
                           COLUMN_START_DATE_JULIAN, start_date_julian, COLUMN_PRIORITY, priority, 
                           COLUMN_CATEGORY, category, COLUMN_SUMMARY, summary, COLUMN_DESCRIPTION, desc,
                           COLUMN_COLOR, get_date_color(due_date_julian, done), -1);
    }
}

/*------------------------------------------------------------------------------*/

gint
list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {

        if (appGUI->tsk->tasks_panel_status == TRUE) {
            tasks_edit_item_cb (NULL, appGUI);
        } else {
            show_tasks_desc_panel(TRUE, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
tasks_list_filter_cb (GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {

gint done;
gchar *category;

    GUI *appGUI = (GUI *)data;

    if (appGUI->tsk->tasks_filter_disabled == TRUE)
        return TRUE;

    gtk_tree_model_get(model, iter, COLUMN_DONE, &done, COLUMN_CATEGORY, &category, -1);

    if(appGUI->tsk->filter_index) {

        if (gui_list_store_get_text_index (appGUI->opt->tasks_category_store, category) == appGUI->tsk->filter_index) {

            if(config.hide_completed && done == TRUE) {
                return FALSE;
            }
            return TRUE;

        } else {
            return FALSE;
        }

    } else if (config.hide_completed && done == TRUE) {
            return FALSE;
    }

    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
category_filter_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    appGUI->tsk->filter_index = gtk_combo_box_get_active (GTK_COMBO_BOX (widget));

    if (appGUI->tsk->filter_index != -1) {
        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
        update_n_items(appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gint 
custom_tasks_sort_function (GtkTreeModel *model, GtkTreeIter *iter_a, GtkTreeIter *iter_b, gpointer user_data) {

gint done_a, done_b;
gchar *priority_a, *priority_b;
guint32 due_date_a, due_date_b;
gint priority_s, done_s, priority_si, done_si;
guint32 due_date_s, due_date_si;


    if(iter_a == NULL || iter_b == NULL) {
        return 0;
    }

    gtk_tree_model_get(model, iter_a, COLUMN_DONE, &done_a, 
                       COLUMN_DUE_DATE_JULIAN, &due_date_a, COLUMN_PRIORITY, &priority_a, -1);
    gtk_tree_model_get(model, iter_b, COLUMN_DONE, &done_b, 
                       COLUMN_DUE_DATE_JULIAN, &due_date_b, COLUMN_PRIORITY, &priority_b, -1);

    done_s = done_a - done_b;
    done_si = done_b - done_a;
    if (!due_date_a) due_date_a = 1 << 31;
    if (!due_date_b) due_date_b = 1 << 31;
    due_date_s = due_date_a - due_date_b;
    due_date_si = due_date_b - due_date_a;
    priority_s = get_priority_index (priority_a) - get_priority_index (priority_b);
    priority_si = get_priority_index (priority_b) - get_priority_index (priority_a);
    g_free(priority_a);
    g_free(priority_b);

    switch(config.tasks_sorting_mode) {

        /* Done, Due Date, Priority */
        case 0:
            if (done_s !=0)
                return done_si;
            if (due_date_s != 0)
                return due_date_si;
            if (priority_s != 0)
                return priority_s;
            break;

        /* Done, Priority, Due Date */
        case 1:
            if (done_s !=0)
                return done_si;
            if (priority_s != 0)
                return priority_s;
            if (due_date_s != 0)
                return due_date_si;
            break;

        /* Priority, Due Date, Done */
        case 2:
            if (priority_s != 0)
                return priority_s;
            if (due_date_s != 0)
                return due_date_si;
            if (done_s !=0)
                return done_si;
            break;

        /* Priority, Done, Due Date */
        case 3:
            if (priority_s != 0)
                return priority_s;
            if (done_s !=0)
                return done_si;
            if (due_date_s != 0)
                return due_date_si;
            break;

        /* Due Date, Priority, Done */
        case 4:
            if (due_date_s != 0)
                return due_date_si;
            if (priority_s != 0)
                return priority_s;
            if (done_s !=0)
                return done_si;
            break;

        /* Due Date, Done, Priority */
        case 5:
            if (due_date_s != 0)
                return due_date_si;
            if (done_s !=0)
                return done_si;
            if (priority_s != 0)
                return priority_s;
            break;

        default:
            break;
    }

    return 0;
}

/*------------------------------------------------------------------------------*/

void
tasks_select_first_position_in_list(GUI *appGUI) {

GtkTreeIter     iter;
GtkTreePath     *path;

    /* set cursor at first position */
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter) == TRUE) {
        path = gtk_tree_model_get_path (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter);
        if (path != NULL) {
            gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->tsk->tasks_list), path, NULL, TRUE);
            gtk_tree_path_free(path);
        }
    }
}

/*------------------------------------------------------------------------------*/

gboolean
category_combo_box_focus_cb (GtkWidget *widget, GtkDirectionType *arg1, gpointer user_data) {
    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
add_tasks_toolbar_widget (GtkUIManager *tasks_uim_widget, GtkWidget *widget, gpointer user_data) {

GtkWidget *handle_box;

    GUI *appGUI = (GUI *)user_data;

    if (GTK_IS_TOOLBAR (widget)) {

        appGUI->tsk->tasks_toolbar = GTK_TOOLBAR (widget);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (appGUI->tsk->vbox, handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", 
                                  G_CALLBACK (gtk_widget_destroy), handle_box);

    } else {
        gtk_box_pack_start (GTK_BOX(appGUI->tsk->vbox), widget, FALSE, FALSE, 0);
    }

    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

void 
gui_create_tasks(GUI *appGUI) {

GtkWidget           *vbox1;
GtkWidget           *vbox2;
GtkWidget           *table;
GtkWidget           *label;
GtkWidget           *top_scrolledwindow;
GtkWidget           *hseparator;
GtkWidget           *close_button;
GtkCellRenderer     *renderer;
GtkWidget           *top_viewport;
GtkWidget           *bottom_viewport;
GtkTextBuffer       *text_buffer;
GError              *error = NULL;
GtkActionGroup      *action_group = NULL;
gchar tmpbuf[BUFFER_SIZE];

 const gchar *ui_info =
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"add\" action=\"add\" />\n"
"    <toolitem name=\"delete\" action=\"delete\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"edit\" action=\"edit\" />\n"
"  </toolbar>\n";

GtkActionEntry entries[] = {
  { "add", OSMO_STOCK_TASKS_ADD, _("New task"), NULL, _("New task"), NULL },
  { "edit", OSMO_STOCK_TASKS_EDIT, _("Edit task"), NULL, _("Edit task"), NULL },
  { "delete", OSMO_STOCK_TASKS_REMOVE, _("Remove task"), NULL, _("Remove task"), NULL },
};

guint n_entries = G_N_ELEMENTS (entries);

    appGUI->tsk->filter_index = 0;

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    sprintf(tmpbuf, "<b>%s</b>", _("Tasks"));
    gtk_label_set_markup (GTK_LABEL (label), tmpbuf);

    vbox1 = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), vbox1, label);
    appGUI->tsk->vbox = GTK_BOX(vbox1);

    /*-------------------------------------------------------------------------------------*/

    action_group = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group, entries, n_entries, NULL);
    gtk_action_group_set_sensitive(action_group, TRUE);

    appGUI->tsk->tasks_uim_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (appGUI->tsk->tasks_uim_widget, action_group, 0);
    g_signal_connect (appGUI->tsk->tasks_uim_widget, "add_widget", G_CALLBACK (add_tasks_toolbar_widget), appGUI);

    if (!gtk_ui_manager_add_ui_from_string (appGUI->tsk->tasks_uim_widget, ui_info, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (appGUI->tsk->tasks_uim_widget);

    gtk_toolbar_set_style (appGUI->tsk->tasks_toolbar, GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_tooltips (appGUI->tsk->tasks_toolbar, config.enable_tooltips);

    /*-------------------------------------------------------------------------------------*/
    /* assign callbacks */

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/add")), "clicked", 
                      G_CALLBACK(tasks_add_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit")), "clicked", 
                      G_CALLBACK(tasks_edit_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete")), "clicked", 
                      G_CALLBACK(tasks_remove_item_cb), appGUI);

    /*-------------------------------------------------------------------------------------*/

    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), FALSE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 6);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox1), table, FALSE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "<b>%s:</b>", _("Category filter"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    appGUI->tsk->cf_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->tsk->cf_combobox);
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), FALSE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->tsk->cf_combobox, GTK_CAN_FOCUS);
    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->cf_combobox, 1, 2, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (GTK_FILL), 0, 0);
    g_signal_connect(appGUI->tsk->cf_combobox, "changed", 
                     G_CALLBACK(category_filter_cb), appGUI);
    g_signal_connect(G_OBJECT(appGUI->tsk->cf_combobox), "focus", 
                     G_CALLBACK(category_combo_box_focus_cb), NULL);

    appGUI->tsk->n_items_label = gtk_label_new ("");
    gtk_widget_show (appGUI->tsk->n_items_label);
    gtk_widget_set_size_request (appGUI->tsk->n_items_label, 100, -1);
    gtk_table_attach (GTK_TABLE (table), appGUI->tsk->n_items_label, 3, 4, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->tsk->n_items_label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 6);

    /*-------------------------------------------------------------------------------------*/

    appGUI->tsk->tasks_paned = gtk_vpaned_new();
    gtk_widget_show (appGUI->tsk->tasks_paned);
    gtk_paned_set_position(GTK_PANED(appGUI->tsk->tasks_paned), 99999);
    gtk_box_pack_start(GTK_BOX(vbox1), appGUI->tsk->tasks_paned, TRUE, TRUE, 0);

    top_viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (top_viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (top_viewport), GTK_SHADOW_NONE);
    gtk_paned_pack1 (GTK_PANED (appGUI->tsk->tasks_paned), top_viewport, FALSE, TRUE);

    top_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (top_scrolledwindow);
    gtk_container_add (GTK_CONTAINER (top_viewport), top_scrolledwindow);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (top_scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (top_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->tsk->tasks_list_store = gtk_list_store_new(TASKS_NUM_COLUMNS, 
                                         G_TYPE_BOOLEAN,
                                         G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING,
                                         G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    appGUI->tsk->tasks_filter = gtk_tree_model_filter_new(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), NULL);
    gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter), 
                                            (GtkTreeModelFilterVisibleFunc)tasks_list_filter_cb, 
                                            appGUI, NULL);

    appGUI->tsk->tasks_sort = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(appGUI->tsk->tasks_filter));

    appGUI->tsk->tasks_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->tsk->tasks_sort));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->tsk->tasks_list), config.tasks_rules_hint);
    gtk_widget_show (appGUI->tsk->tasks_list);
    GTK_WIDGET_SET_FLAGS (appGUI->tsk->tasks_list, GTK_CAN_DEFAULT);
    gtk_widget_modify_fg(GTK_WIDGET(appGUI->tsk->tasks_list), GTK_STATE_SELECTED,
                         (& GTK_WIDGET(appGUI->tsk->tasks_list)->style->base[GTK_STATE_SELECTED]));
    gtk_widget_modify_fg(GTK_WIDGET(appGUI->tsk->tasks_list), GTK_STATE_NORMAL,
                         (& GTK_WIDGET(appGUI->tsk->tasks_list)->style->bg[GTK_STATE_NORMAL]));

    g_signal_connect(G_OBJECT(appGUI->tsk->tasks_list), "button_press_event",
                     G_CALLBACK(list_dbclick_cb), appGUI);

    appGUI->tsk->tasks_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->tsk->tasks_list));
    g_signal_connect(G_OBJECT(appGUI->tsk->tasks_list_selection), "changed",
                     G_CALLBACK(tasks_item_selected), appGUI);

    /* create columns */

    renderer = gtk_cell_renderer_toggle_new();
    appGUI->tsk->tasks_columns[COLUMN_DONE] = gtk_tree_view_column_new_with_attributes (_("Done"),
                             renderer,
                             "active", COLUMN_DONE,
                             NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_DONE]);

    g_signal_connect (renderer, "toggled", G_CALLBACK (done_toggled), appGUI);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_DUE_DATE] = gtk_tree_view_column_new_with_attributes(_("Due date"),
                              renderer,
                              "text", COLUMN_DUE_DATE,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_DUE_DATE], config.visible_due_date_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_DUE_DATE]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_DUE_DATE_JULIAN] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", COLUMN_DUE_DATE_JULIAN,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_DUE_DATE_JULIAN], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_DUE_DATE_JULIAN]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_START_DATE_JULIAN] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", COLUMN_START_DATE_JULIAN,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_START_DATE_JULIAN], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_START_DATE_JULIAN]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_PRIORITY] = gtk_tree_view_column_new_with_attributes(_("Priority"),
                              renderer,
                              "text", COLUMN_PRIORITY,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_PRIORITY], config.visible_priority_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_PRIORITY]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_CATEGORY] = gtk_tree_view_column_new_with_attributes(_("Category"),
                              renderer,
                              "text", COLUMN_CATEGORY,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_CATEGORY], config.visible_category_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_CATEGORY]);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    gtk_cell_renderer_set_fixed_size(renderer, 0, -1);
    appGUI->tsk->tasks_columns[COLUMN_SUMMARY] = gtk_tree_view_column_new_with_attributes(_("Summary"),
                              renderer,
                              "text", COLUMN_SUMMARY,
                              "strikethrough", COLUMN_DONE,
                              "foreground", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_SUMMARY]);


    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_DESCRIPTION] = gtk_tree_view_column_new_with_attributes(_("Description"),
                              renderer,
                              "text", COLUMN_DESCRIPTION,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_DESCRIPTION], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_DESCRIPTION]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[COLUMN_COLOR] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[COLUMN_COLOR], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[COLUMN_COLOR]);

    /* configure list options */

    gtk_container_add (GTK_CONTAINER (top_scrolledwindow), appGUI->tsk->tasks_list);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW(appGUI->tsk->tasks_list), FALSE);

    /* configure sorting */

    gtk_tree_sortable_set_sort_func((GtkTreeSortable *)appGUI->tsk->tasks_sort, 0, 
                                    (GtkTreeIterCompareFunc)custom_tasks_sort_function, NULL, NULL);

    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->tsk->tasks_sort, COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->tsk->tasks_sort, COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable *)appGUI->tsk->tasks_sort, COLUMN_DONE, config.tasks_sorting_order);

    /*----------------------------------------------------------------------------*/

    bottom_viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (bottom_viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (bottom_viewport), GTK_SHADOW_NONE);
    gtk_paned_pack2 (GTK_PANED (appGUI->tsk->tasks_paned), bottom_viewport, TRUE, TRUE);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 0);
    gtk_container_add (GTK_CONTAINER (bottom_viewport), vbox2);

    appGUI->tsk->panel_hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->tsk->panel_hbox, FALSE, FALSE, 0);
    gtk_widget_show(appGUI->tsk->panel_hbox);

    sprintf(tmpbuf, "%s:", _("Task details"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (appGUI->tsk->panel_hbox), label, FALSE, FALSE, 0);

    if (config.default_stock_icons) {
        close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
    } else {
        close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    gtk_button_set_relief (GTK_BUTTON(close_button), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, close_button, _("Close description panel"), NULL);
    gtk_box_pack_end (GTK_BOX (appGUI->tsk->panel_hbox), close_button, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (close_button), "clicked",
                        G_CALLBACK (panel_close_desc_cb), appGUI);

    appGUI->tsk->panel_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (appGUI->tsk->panel_scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->tsk->panel_scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (appGUI->tsk->panel_scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (appGUI->tsk->panel_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->tsk->tasks_desc_textview = gtk_text_view_new ();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->tsk->tasks_desc_textview), GTK_WRAP_WORD);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), 4);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), 4);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), FALSE);
    gtk_widget_show (appGUI->tsk->tasks_desc_textview);
    gtk_container_add (GTK_CONTAINER (appGUI->tsk->panel_scrolledwindow), appGUI->tsk->tasks_desc_textview);

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview));
    gtk_text_buffer_create_tag (text_buffer, "italic",
                  "style", PANGO_STYLE_ITALIC, NULL);
    appGUI->tsk->font_tag_object = gtk_text_buffer_create_tag (text_buffer, "info_font",
                      "font", (gchar *) config.task_info_font, NULL);

}

/*------------------------------------------------------------------------------*/


