
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

#include "tasks.h"
#include "i18n.h"
#include "tasks_items.h"
#include "tasks_print.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "calendar_utils.h"
#include "utils.h"
#include "options_prefs.h"
#include "options_gui_tasks.h"
#include "stock_icons.h"

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
get_priority_text (gint index)
{
gchar *priority_table[] = {
    N_("Low"), N_("Medium"), N_("High")
};

	g_return_val_if_fail (index >=0 && index <= 2, NULL);
	return gettext (priority_table[index]);
}

/*------------------------------------------------------------------------------*/

void
search_nearest_day (TASK_ITEM *item, GDate *cdate, gboolean month_mode)
{
gint day, start_day, days_in_month;
gint i, j;

	day = g_date_get_weekday (cdate) - 1;

	if (month_mode == TRUE) {

		days_in_month = utl_get_days_in_month (cdate);
		start_day = g_date_get_day (cdate);

		for (i = 0; i < 7; i++) {
			if (item->repeat_day & (1 << ((day + i) % 7))) break;
		}

		for (j = 0; j < 7; j++) {
			if (item->repeat_day & (1 << ((day + 7 - j) % 7))) break;
		}

		if (start_day + i > days_in_month) i = 7;
		if (start_day - j < 1) j = 7;

		if (i <= j) {
			if (i > 0) g_date_add_days (cdate, i);
		} else {
			if (j > 0) g_date_subtract_days (cdate, j);
		}

	} else {

		for (i = 0; i < 7; i++) {
			if (item->repeat_day & (1 << ((day + i) % 7))) break;
		}
		if (i > 0) g_date_add_days (cdate, i);

	}

}

/*------------------------------------------------------------------------------*/

gint
task_calculate_new_date (TASK_ITEM *item, GUI *appGUI, guint32 *new_date, gint *new_time)
{
GDate *cdate;
guint32 current_julian;
gint current_time;
gint i, cycles;
guint days_in_month;
gboolean repeat_time, repeat_date, set_start_time;

	repeat_time = repeat_date = FALSE;
	set_start_time = TRUE;

	cycles = 0;
	current_julian = utl_get_current_julian ();
	current_time = get_seconds_for_today ();
	*new_date = item->due_date_julian;
	*new_time = item->due_time;

	if (item->repeat_time_start < item->repeat_time_end && item->repeat_time_interval > 0 && item->due_time >= 0)
		repeat_time = TRUE;
	
	if (item->repeat_month_interval > 0 || item->repeat_day_interval > 0)
		repeat_date = TRUE;

	if (repeat_time) {
		for (i = item->repeat_time_start; i <= item->repeat_time_end; i += item->repeat_time_interval) {
			if (i * 60 > item->due_time && i * 60 > current_time) {
				*new_time = i * 60;
				set_start_time = FALSE;
				break;
			}
		}
		if (set_start_time && repeat_date)
			*new_time = item->repeat_time_start * 60;
	}

	if (*new_time < current_time || item->due_time < 0) current_julian++;

	if (item->due_date_julian < current_julian && repeat_date) {
		cdate = g_date_new_julian (item->due_date_julian);
		g_return_val_if_fail (cdate != NULL, 0);
		g_return_val_if_fail (item->repeat_day > 0 && item->repeat_day <= WHOLE_WEEK, 0);

		while (*new_date < current_julian) {
			g_date_add_months (cdate, item->repeat_month_interval);
			g_date_add_days (cdate, item->repeat_day_interval);

			if (item->repeat_month_interval > 0 && item->repeat_day_interval == 0) {
				days_in_month = utl_get_days_in_month (cdate);

				if (item->repeat_start_day > days_in_month) {
					g_date_set_day (cdate, days_in_month);
				} else {
					g_date_set_day (cdate, item->repeat_start_day);
				}

				if (item->repeat_day != WHOLE_WEEK)
					search_nearest_day (item, cdate, TRUE);

			} else if (item->repeat_day != WHOLE_WEEK) {
				search_nearest_day (item, cdate, FALSE);
			}

			*new_date = g_date_get_julian (cdate);
			cycles++;

			if (item->repeat_counter != 0 && cycles >= item->repeat_counter) {
				g_date_free (cdate);
				return cycles;
			}
		}

		if (repeat_time) *new_time = item->repeat_time_start * 60;
		g_date_free (cdate);
	}

	return cycles;
}

/*------------------------------------------------------------------------------*/

void
tasks_repeat_done (GtkTreeIter *iter, TASK_ITEM *item, GUI *appGUI)
{
guint32 new_date;
gint new_time;
gint cycles;

	g_return_if_fail (item->repeat == TRUE);

	cycles = task_calculate_new_date (item, appGUI, &new_date, &new_time);

	if (item->repeat_counter == 0 || item->repeat_counter > cycles) {

		if (item->repeat_counter > cycles) {
			gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
			                    TA_COLUMN_REPEAT_COUNTER, item->repeat_counter - cycles, -1);
		}

		if (new_date == item->due_date_julian && new_time == item->due_time) {

			if (config.delete_completed) {
				gtk_list_store_remove (appGUI->tsk->tasks_list_store, iter);
			} else {
				gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
				                    TA_COLUMN_COLOR, get_date_color (item->due_date_julian, item->due_time, TRUE),
				                    TA_COLUMN_DONE, TRUE, -1);
			}

		} else {
			gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
			                    TA_COLUMN_COLOR, get_date_color (new_date, new_time, FALSE),
			                    TA_COLUMN_DUE_DATE_JULIAN, new_date,
			                    TA_COLUMN_DUE_DATE, get_date_time_full_str (new_date, new_time),
			                    TA_COLUMN_DUE_TIME, new_time,
			                    TA_COLUMN_ACTIVE, TRUE,
			                    TA_COLUMN_ACTIVE_WARNING, TRUE,
			                    TA_COLUMN_DONE, FALSE, -1);
		}

	} else {

		if (config.delete_completed) {
			gtk_list_store_remove (appGUI->tsk->tasks_list_store, iter);
		} else {
			gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
			                    TA_COLUMN_REPEAT_COUNTER, 0,
			                    TA_COLUMN_COLOR, get_date_color (new_date, new_time, TRUE),
			                    TA_COLUMN_DUE_DATE_JULIAN, new_date,
			                    TA_COLUMN_DUE_DATE, get_date_time_full_str (new_date, new_time),
			                    TA_COLUMN_DUE_TIME, new_time,
			                    TA_COLUMN_DONE, TRUE, -1);
		}

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
guint32 start_date_julian, done_date_julian, due_date_julian;
gint due_time;
gchar tmpbuf[BUFFER_SIZE];
gboolean repeat, prev_state, next_state;

    GUI *appGUI = (GUI *)data;

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview));
    gui_clear_text_buffer (text_buffer, &titer);

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {

        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), TRUE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), TRUE);

        gtk_tree_model_get (model, &iter, TA_COLUMN_DESCRIPTION, &text,
                            TA_COLUMN_DUE_DATE_JULIAN, &due_date_julian,
                            TA_COLUMN_START_DATE_JULIAN, &start_date_julian,
                            TA_COLUMN_DONE_DATE_JULIAN, &done_date_julian,
                            TA_COLUMN_DUE_TIME, &due_time,
                            TA_COLUMN_REPEAT, &repeat, -1);

		if (repeat == TRUE) {
			prev_state = FALSE;
			next_state = is_date_in_the_past (due_date_julian, due_time, utl_get_current_julian (), get_seconds_for_today ());
		} else {
			prev_state = next_state = (due_date_julian != 0) ? TRUE : FALSE;
		}

        gtk_widget_set_sensitive (gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/prev_day"), prev_state);
        gtk_widget_set_sensitive (gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/next_day"), next_state);

        g_snprintf (tmpbuf, BUFFER_SIZE, "\n%s: %s\n", 
                    _("Started"), julian_to_str (start_date_julian, DATE_FULL));

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
            if (done_date_julian != 0) {
                g_snprintf (tmpbuf, BUFFER_SIZE, "%s: %s\n", 
                            _("Finished"), julian_to_str (done_date_julian, DATE_FULL));
                gtk_text_buffer_insert_with_tags_by_name (text_buffer, &titer, tmpbuf, -1, 
                                                          "italic", NULL);
            }

            gtk_text_view_set_buffer(GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), text_buffer);

            hseparator = gtk_hseparator_new ();
            gtk_widget_show (hseparator);
            gtk_widget_set_size_request (hseparator, 320, -1);
            gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW(appGUI->tsk->tasks_desc_textview), hseparator, anchor);
        }

        g_free(text);
    } else {
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), FALSE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), FALSE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/prev_day"), FALSE);
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/next_day"), FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_add_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    tasks_add_edit_dialog_show (FALSE, 0, get_seconds_for_today(), appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_edit_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, NULL, NULL)) {
        tasks_add_edit_dialog_show (TRUE, appGUI->tsk->tasks_due_julian_day, appGUI->tsk->tasks_due_time, appGUI);
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
task_modify_due_date (gint value, GUI *appGUI)
{
GtkTreeIter iter, *e_iter;
GtkTreeModel *model;
guint32 due_date;
gint due_time, id;
TASK_ITEM *item;

	if (gtk_tree_selection_get_selected (appGUI->tsk->tasks_list_selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, TA_COLUMN_DUE_DATE_JULIAN, &due_date,
		                    TA_COLUMN_DUE_TIME, &due_time, TA_COLUMN_ID, &id, -1);

		e_iter = task_get_iter (id, appGUI);
		g_return_if_fail (e_iter != NULL);

		item = get_task_item (e_iter, appGUI);
		g_return_if_fail (item != NULL);

		if (item->repeat == TRUE) {
			tasks_repeat_done (e_iter, item, appGUI);
		} else {
			due_date += value;
			gtk_list_store_set (appGUI->tsk->tasks_list_store, e_iter,
			                    TA_COLUMN_COLOR, get_date_color (due_date, due_time, FALSE),
			                    TA_COLUMN_DUE_DATE_JULIAN, due_date,
			                    TA_COLUMN_DUE_DATE, get_date_time_full_str (due_date, due_time), -1);
		}

		g_free (item);
	}

}

/*------------------------------------------------------------------------------*/

void
tasks_change_due_date_to_prev_day_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    task_modify_due_date (-1, appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_change_due_date_to_next_day_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    task_modify_due_date (1, appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_print_visible_items_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    tasks_print (appGUI);
}

/*------------------------------------------------------------------------------*/

void
done_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer user_data)
{
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeIter  iter;
gboolean done_status;
guint32 done_date;
GtkTreeModel *model;

    GUI *appGUI = (GUI *) user_data;
    model = GTK_TREE_MODEL (appGUI->tsk->tasks_list_store);

    sort_path = gtk_tree_path_new_from_string (path_str);

    if (sort_path != NULL) {
        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (appGUI->tsk->tasks_sort), sort_path);

        if (filter_path != NULL) {
            path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (appGUI->tsk->tasks_filter), filter_path);

            if (path != NULL) {
                gtk_tree_model_get_iter (model, &iter, path);   /* get toggled iter */
                gtk_tree_model_get (model, &iter, TA_COLUMN_DONE, &done_status, -1);

                if (done_status == FALSE) {
                    done_date = utl_get_current_julian ();
                } else {
                    done_date = 0;
                }

                gtk_list_store_set (GTK_LIST_STORE (model), &iter, 
                                    TA_COLUMN_DONE, !done_status, 
                                    TA_COLUMN_DONE_DATE_JULIAN, done_date, -1);

                if (done_status == FALSE && config.delete_completed) {
                    gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
                }

                gtk_tree_path_free (path);
            }

            gtk_tree_path_free (filter_path);
        }

        gtk_tree_path_free (sort_path);
    }

    apply_task_attributes (appGUI);
    cal_set_day_info (appGUI);
}

/*------------------------------------------------------------------------------*/

gchar *
get_date_color (guint32 julian_day, gint time, gboolean done)
{
static gchar due_date_color[MAXCOLORNAME];
gint current_time;
gint32 r;

	current_time = get_seconds_for_today ();
	g_strlcpy (due_date_color, "black", MAXCOLORNAME);

	if (julian_day != 0 && done == FALSE) {

		r = julian_day - utl_get_current_julian ();

		if (r == 0) {
			if (time >= 0 && current_time > time) {
				g_strlcpy (due_date_color, config.past_due_color, MAXCOLORNAME);
			}
			else {
				g_strlcpy (due_date_color, config.due_today_color, MAXCOLORNAME);
			}
		} else if (r > 0 && r < 7) {
			g_strlcpy (due_date_color, config.due_7days_color, MAXCOLORNAME);
		} else if (r < 0) {
			g_strlcpy (due_date_color, config.past_due_color, MAXCOLORNAME);
		}
	}

	return due_date_color;
}

/*------------------------------------------------------------------------------*/

void
apply_task_attributes (GUI *appGUI) {

GtkTreeIter iter;
gint i;
gboolean done;
gchar *priority;
guint32 julian_day;
gint time;

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                           TA_COLUMN_DONE, &done, TA_COLUMN_DUE_DATE_JULIAN, &julian_day, 
                           TA_COLUMN_DUE_TIME, &time, TA_COLUMN_PRIORITY, &priority, -1);

        if (get_priority_index (priority) == 2 && config.tasks_high_in_bold == TRUE) {    /* high priority ? */
            gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, 
                               TA_COLUMN_COLOR, get_date_color (julian_day, time, done), 
                               TA_COLUMN_BOLD, PANGO_WEIGHT_BOLD,-1);
        } else {
            gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, 
                               TA_COLUMN_COLOR, get_date_color (julian_day, time, done), 
                               TA_COLUMN_BOLD, PANGO_WEIGHT_NORMAL,-1);
        }

        g_free (priority);
    }
}

/*------------------------------------------------------------------------------*/

void
refresh_tasks (GUI *appGUI) {

GtkTreeIter iter;
gint i, n, due_time;
guint32 julian_day;
gchar *category;
gboolean done, tasks_state;

    n = gui_get_combobox_items(GTK_COMBO_BOX (appGUI->tsk->cf_combobox));

    for (i = n-1; i >= 0; i--) {
        gtk_combo_box_remove_text (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), i);
    }
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), _("All items"));

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, i++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, 
                           TC_COLUMN_NAME, &category, TC_COLUMN_TASKS, &tasks_state, -1);
        if (tasks_state == TRUE) {
            gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), category);
        }
        g_free(category);
    }

    if (config.remember_category_in_tasks == TRUE) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->tsk->cf_combobox), config.current_category_in_tasks);
    } else {
        gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->tsk->cf_combobox), 0);
    }

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

        gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                            TA_COLUMN_DONE, &done,
                            TA_COLUMN_DUE_DATE_JULIAN, &julian_day,
                            TA_COLUMN_DUE_TIME, &due_time,
                            -1);

        gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter, 
                            TA_COLUMN_DUE_DATE, get_date_time_full_str (julian_day, due_time), 
                            TA_COLUMN_COLOR, get_date_color (julian_day, due_time, done),
                            -1);
    }

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
}

/*------------------------------------------------------------------------------*/

void
update_tasks_number (GUI *appGUI)
{
GtkTreeIter iter;
gint i;
gchar tmpbuf[BUFFER_SIZE];

	i = 0;

	while (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (appGUI->tsk->tasks_filter), &iter, NULL, i++));

	i--;

	if (i > 0) {
		g_snprintf (tmpbuf, BUFFER_SIZE, "<i>%4d %s</i>", i, ngettext ("entry", "entries", i));
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/print"), TRUE);
	} else {
		g_snprintf (tmpbuf, BUFFER_SIZE, "<i>%s</i>", _("no entries"));
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/print"), FALSE);
	}

	gtk_label_set_markup (GTK_LABEL (appGUI->tsk->n_items_label), tmpbuf);

}

/*------------------------------------------------------------------------------*/

void
add_item_to_list (TASK_ITEM *item, GUI *appGUI) {

GtkTreeIter iter;
GdkPixbuf *image;
GtkWidget *helper;
const gchar *stock_id;

	gtk_list_store_append (appGUI->tsk->tasks_list_store, &iter);

	gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter, TA_COLUMN_DONE, item->done,
	                    TA_COLUMN_DUE_DATE_JULIAN, item->due_date_julian,
	                    TA_COLUMN_START_DATE_JULIAN, item->start_date_julian,
	                    TA_COLUMN_DONE_DATE_JULIAN, item->done_date_julian,
	                    TA_COLUMN_DUE_TIME, item->due_time,
	                    TA_COLUMN_PRIORITY, item->priority, TA_COLUMN_CATEGORY, item->category,
	                    TA_COLUMN_SUMMARY, item->summary, TA_COLUMN_DESCRIPTION, item->desc,
	                    TA_COLUMN_COLOR, get_date_color (item->due_date_julian, item->due_time, item->done),
	                    TA_COLUMN_ACTIVE, item->active, TA_COLUMN_OFFLINE_IGNORE, item->offline_ignore,
	                    TA_COLUMN_ACTIVE_WARNING, item->active_warning,
	                    TA_COLUMN_REPEAT, item->repeat, TA_COLUMN_REPEAT_DAY, item->repeat_day,
	                    TA_COLUMN_REPEAT_MONTH_INTERVAL, item->repeat_month_interval,
	                    TA_COLUMN_REPEAT_DAY_INTERVAL, item->repeat_day_interval,
	                    TA_COLUMN_REPEAT_START_DAY, item->repeat_start_day,
	                    TA_COLUMN_REPEAT_TIME_START, item->repeat_time_start,
	                    TA_COLUMN_REPEAT_TIME_END, item->repeat_time_end,
	                    TA_COLUMN_REPEAT_TIME_INTERVAL, item->repeat_time_interval,
	                    TA_COLUMN_REPEAT_COUNTER, item->repeat_counter, TA_COLUMN_ALARM_COMMAND, item->alarm_command,
	                    TA_COLUMN_WARNING_DAYS, item->warning_days, TA_COLUMN_WARNING_TIME, item->warning_time,
	                    TA_COLUMN_ID, appGUI->tsk->id_counter++,
	                    -1);

    helper = gtk_image_new ();
    stock_id = (item->repeat == TRUE) ? OSMO_STOCK_TASKS_TYPE_RECURRENT : OSMO_STOCK_TASKS_TYPE_NORMAL;
    image = gtk_widget_render_icon (helper, stock_id, GTK_ICON_SIZE_MENU, NULL);
    gtk_list_store_set(appGUI->tsk->tasks_list_store, &iter, TA_COLUMN_TYPE, image,-1);
    g_object_unref (image);

	if (item->due_date_julian == 0) {   /* use 'No date' text when due date is 0 */
		gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter, TA_COLUMN_DUE_DATE, _("No date"), -1);
	} else {
		gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter,
		                    TA_COLUMN_DUE_DATE, get_date_time_full_str (item->due_date_julian, item->due_time));
	}
}

/*------------------------------------------------------------------------------*/

gint
list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {

        if (appGUI->tsk->tasks_panel_status == TRUE) {
            if (config.add_edit == FALSE) {
                tasks_edit_item_cb (NULL, appGUI);
            } else {
                tasks_add_edit_dialog_show (FALSE, 0, get_seconds_for_today(), appGUI);
            }
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

gint done, idx;
gchar *category, *category_cf;

    GUI *appGUI = (GUI *)data;

    if (appGUI->tsk->tasks_filter_disabled == TRUE)
        return TRUE;

    gtk_tree_model_get(model, iter, TA_COLUMN_DONE, &done, TA_COLUMN_CATEGORY, &category, -1);
    
    if (tasks_category_get_state (category, STATE_TASKS, appGUI) == FALSE) {
        g_free (category);
        return FALSE;
    }

    category_cf = gtk_combo_box_get_active_text (GTK_COMBO_BOX (appGUI->tsk->cf_combobox));
    idx = gui_list_store_get_text_index (appGUI->opt->tasks_category_store, category_cf);
    g_free (category_cf);

    if(idx) {

        if (gui_list_store_get_text_index (appGUI->opt->tasks_category_store, category) == idx) {

            if(config.hide_completed && done == TRUE) {
                g_free (category);
                return FALSE;
            }
            g_free (category);
            return TRUE;

        } else {
            g_free (category);
            return FALSE;
        }

    } else if (config.hide_completed && done == TRUE) {
            g_free (category);
            return FALSE;
    }

    g_free (category);
    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
category_filter_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    appGUI->tsk->filter_index = gtk_combo_box_get_active (GTK_COMBO_BOX (widget));

    if (appGUI->tsk->filter_index != -1) {
        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
        update_tasks_number (appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gint 
custom_tasks_sort_function (GtkTreeModel *model, GtkTreeIter *iter_a, GtkTreeIter *iter_b, gpointer user_data) {

gint done_a, done_b, done_s;
gchar *priority_a, *priority_b;
gint priority_s;
guint32 due_date_a, due_date_b, due_date_s;
gint due_time_a, due_time_b, due_time_s;


    if(iter_a == NULL || iter_b == NULL) {
        return 0;
    }

    gtk_tree_model_get (model, iter_a, TA_COLUMN_DONE, &done_a, TA_COLUMN_DUE_TIME, &due_time_a, 
                        TA_COLUMN_DUE_DATE_JULIAN, &due_date_a, TA_COLUMN_PRIORITY, &priority_a, -1);
    gtk_tree_model_get (model, iter_b, TA_COLUMN_DONE, &done_b, TA_COLUMN_DUE_TIME, &due_time_b,
                        TA_COLUMN_DUE_DATE_JULIAN, &due_date_b, TA_COLUMN_PRIORITY, &priority_b, -1);

    done_s = done_b - done_a;
    if (!due_date_a) due_date_a = 1 << 31;
    if (!due_date_b) due_date_b = 1 << 31;
    due_date_s = due_date_b - due_date_a;
    due_time_s = due_time_b - due_time_a;
    priority_s = get_priority_index (priority_a) - get_priority_index (priority_b);
    g_free (priority_a);
    g_free (priority_b);

    switch(config.tasks_sorting_mode) {

        /* Done, Due Date, Priority */
        case 0:
            if (done_s !=0)
                return done_s;
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            if (priority_s != 0)
                return priority_s;
            break;

        /* Done, Priority, Due Date */
        case 1:
            if (done_s !=0)
                return done_s;
            if (priority_s != 0)
                return priority_s;
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            break;

        /* Priority, Due Date, Done */
        case 2:
            if (priority_s != 0)
                return priority_s;
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            if (done_s !=0)
                return done_s;
            break;

        /* Priority, Done, Due Date */
        case 3:
            if (priority_s != 0)
                return priority_s;
            if (done_s !=0)
                return done_s;
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            break;

        /* Due Date, Priority, Done */
        case 4:
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            if (priority_s != 0)
                return priority_s;
            if (done_s !=0)
                return done_s;
            break;

        /* Due Date, Done, Priority */
        case 5:
            if (due_date_s == 0 && due_time_s != 0)
                return due_time_s;
            if (due_date_s != 0)
                return due_date_s;
            if (done_s !=0)
                return done_s;
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
tasks_selection_activate (gboolean active, GUI *appGUI) {
    if (active == TRUE) {
        g_signal_connect (G_OBJECT(appGUI->tsk->tasks_list_selection), "changed",
                          G_CALLBACK(tasks_item_selected), appGUI);
    } else {
        g_signal_handlers_disconnect_by_func (G_OBJECT (appGUI->tsk->tasks_list_selection),
                                              G_CALLBACK (tasks_item_selected), appGUI);
    }
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
"    <toolitem name=\"edit\" action=\"edit\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"delete\" action=\"delete\" />\n"
"    <separator name=\"sep2\" />\n"
"    <toolitem name=\"prev_day\" action=\"prev_day\" />\n"
"    <toolitem name=\"next_day\" action=\"next_day\" />\n"
"    <separator name=\"sep3\" />\n"
"    <toolitem name=\"print\" action=\"print\" />\n"
"  </toolbar>\n";

GtkActionEntry entries[] = {
  { "add", OSMO_STOCK_TASKS_ADD, _("New task"), NULL, _("New task"), NULL },
  { "edit", OSMO_STOCK_TASKS_EDIT, _("Edit task"), NULL, _("Edit task"), NULL },
  { "delete", OSMO_STOCK_TASKS_REMOVE, _("Remove task"), NULL, _("Remove task"), NULL },
  { "prev_day", OSMO_STOCK_TASKS_PREV_DAY, _("Change due date to previous day"), NULL, _("Change due date to previous day"), NULL },
  { "next_day", OSMO_STOCK_TASKS_NEXT_DAY, _("Change due date to next day"), NULL, _("Change due date to next day"), NULL },
  { "print", OSMO_STOCK_PRINT, _("Print visible tasks list"), NULL, _("Print visible tasks list"), NULL },
};

guint n_entries = G_N_ELEMENTS (entries);

    appGUI->tsk->filter_index = 0;

    vbox1 = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    sprintf(tmpbuf, "<b>%s</b>", _("Tasks"));
    gui_add_to_notebook (vbox1, tmpbuf, appGUI);

    appGUI->tsk->vbox = GTK_BOX(vbox1);

    if (config.hide_tasks == TRUE) {
        gtk_widget_hide(GTK_WIDGET(appGUI->tsk->vbox));
    }

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
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/prev_day")), "clicked", 
                      G_CALLBACK(tasks_change_due_date_to_prev_day_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/next_day")), "clicked", 
                      G_CALLBACK(tasks_change_due_date_to_next_day_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/print")), "clicked", 
                      G_CALLBACK(tasks_print_visible_items_cb), appGUI);

    /*-------------------------------------------------------------------------------------*/

    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/edit"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/delete"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/prev_day"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/next_day"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->tsk->tasks_uim_widget, "/toolbar/print"), FALSE);

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
    if (appGUI->tiny_gui == TRUE) {
        gtk_table_attach (GTK_TABLE (table), appGUI->tsk->cf_combobox, 1, 2, 0, 1,
                         (GtkAttachOptions) (0),
                         (GtkAttachOptions) (0), 0, 0);
    } else {
        gtk_table_attach (GTK_TABLE (table), appGUI->tsk->cf_combobox, 1, 2, 0, 1,
                         (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                         (GtkAttachOptions) (GTK_FILL), 0, 0);
    }
    g_signal_connect(G_OBJECT(appGUI->tsk->cf_combobox), "changed", 
                     G_CALLBACK(category_filter_cb), appGUI);
    g_signal_connect(G_OBJECT(appGUI->tsk->cf_combobox), "focus", 
                     G_CALLBACK(category_combo_box_focus_cb), NULL);

    appGUI->tsk->n_items_label = gtk_label_new ("");
    gtk_widget_show (appGUI->tsk->n_items_label);
    if (appGUI->tiny_gui == FALSE) {
        gtk_widget_set_size_request (appGUI->tsk->n_items_label, 100, -1);
    }
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

	appGUI->tsk->tasks_list_store = gtk_list_store_new (TASKS_NUM_COLUMNS,
	                                    G_TYPE_BOOLEAN, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_INT,
	                                    G_TYPE_UINT, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
	                                    G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_BOOLEAN,
	                                    G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_INT,
	                                    G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
	                                    G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

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
    tasks_selection_activate (TRUE, appGUI);

    /* create columns */

    renderer = gtk_cell_renderer_toggle_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DONE] = gtk_tree_view_column_new_with_attributes (_("Done"),
                             renderer,
                             "active", TA_COLUMN_DONE,
                             NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DONE]);

    g_signal_connect (renderer, "toggled", G_CALLBACK (done_toggled), appGUI);

    renderer = gtk_cell_renderer_pixbuf_new();  /* icon */
    appGUI->tsk->tasks_columns[TA_COLUMN_TYPE]  = gtk_tree_view_column_new_with_attributes(_("Type"), 
                             renderer, 
                             "pixbuf", TA_COLUMN_TYPE, 
                             NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_TYPE]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE] = gtk_tree_view_column_new_with_attributes(_("Due date"),
                              renderer,
                              "text", TA_COLUMN_DUE_DATE,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE], config.visible_due_date_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE_JULIAN] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_DUE_DATE_JULIAN,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE_JULIAN], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE_JULIAN]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DUE_TIME] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_DUE_TIME,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DUE_TIME], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DUE_TIME]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_START_DATE_JULIAN] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_START_DATE_JULIAN,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_START_DATE_JULIAN], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_START_DATE_JULIAN]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DONE_DATE_JULIAN] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_DONE_DATE_JULIAN,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DONE_DATE_JULIAN], FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DONE_DATE_JULIAN]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_PRIORITY] = gtk_tree_view_column_new_with_attributes(_("Priority"),
                              renderer,
                              "text", TA_COLUMN_PRIORITY,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_PRIORITY], config.visible_priority_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_PRIORITY]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_CATEGORY] = gtk_tree_view_column_new_with_attributes(_("Category"),
                              renderer,
                              "text", TA_COLUMN_CATEGORY,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_CATEGORY], config.visible_category_column);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_CATEGORY]);

    renderer = gtk_cell_renderer_text_new();
    g_object_set (G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    gtk_cell_renderer_set_fixed_size (renderer, 0, -1);
    appGUI->tsk->tasks_columns[TA_COLUMN_SUMMARY] = gtk_tree_view_column_new_with_attributes(_("Summary"),
                              renderer,
                              "text", TA_COLUMN_SUMMARY,
                              "strikethrough", TA_COLUMN_DONE,
                              "foreground", TA_COLUMN_COLOR,
                              "weight", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_SUMMARY]);


    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_DESCRIPTION] = gtk_tree_view_column_new_with_attributes(_("Description"),
                              renderer,
                              "text", TA_COLUMN_DESCRIPTION,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DESCRIPTION], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_DESCRIPTION]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_COLOR] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_COLOR,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_COLOR], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_COLOR]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_BOLD] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_BOLD,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_BOLD], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_BOLD]);

    renderer = gtk_cell_renderer_toggle_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_ACTIVE] = gtk_tree_view_column_new_with_attributes (NULL,
                             renderer,
                             "active", TA_COLUMN_ACTIVE,
                             NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_ACTIVE], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_ACTIVE]);

    renderer = gtk_cell_renderer_toggle_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_OFFLINE_IGNORE] = gtk_tree_view_column_new_with_attributes (NULL,
                             renderer,
                             "active", TA_COLUMN_OFFLINE_IGNORE,
                             NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_OFFLINE_IGNORE], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_OFFLINE_IGNORE]);

    renderer = gtk_cell_renderer_toggle_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT] = gtk_tree_view_column_new_with_attributes (NULL,
                             renderer,
                             "active", TA_COLUMN_REPEAT,
                             NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_DAY,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_MONTH_INTERVAL] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_MONTH_INTERVAL,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_MONTH_INTERVAL], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_MONTH_INTERVAL]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY_INTERVAL] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_DAY_INTERVAL,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY_INTERVAL], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_DAY_INTERVAL]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_START_DAY] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_START_DAY,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_START_DAY], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_START_DAY]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_START] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_TIME_START,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_START], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_START]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_END] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_TIME_END,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_END], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_END]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_INTERVAL] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_TIME_INTERVAL,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_INTERVAL], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_TIME_INTERVAL]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_COUNTER] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_REPEAT_COUNTER,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_COUNTER], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_REPEAT_COUNTER]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_ALARM_COMMAND] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_ALARM_COMMAND,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_ALARM_COMMAND], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_ALARM_COMMAND]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_DAYS] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_WARNING_DAYS,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_DAYS], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_DAYS]);

	renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_TIME] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_WARNING_TIME,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_TIME], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_WARNING_TIME]);

    renderer = gtk_cell_renderer_text_new();
    appGUI->tsk->tasks_columns[TA_COLUMN_ID] = gtk_tree_view_column_new_with_attributes(NULL,
                              renderer,
                              "text", TA_COLUMN_ID,
                              NULL);
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_ID], FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->tsk->tasks_list), appGUI->tsk->tasks_columns[TA_COLUMN_ID]);

    /* configure list options */

    gtk_container_add (GTK_CONTAINER (top_scrolledwindow), appGUI->tsk->tasks_list);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW(appGUI->tsk->tasks_list), FALSE);

    /* configure sorting */

    gtk_tree_sortable_set_sort_func ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 0, 
                                     (GtkTreeIterCompareFunc)custom_tasks_sort_function, NULL, NULL);

    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DONE, config.tasks_sorting_order);

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


