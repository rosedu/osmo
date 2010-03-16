
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

#include "check_events.h"
#include "i18n.h"
#include "calendar.h"
#include "gui.h"
#include "utils.h"
#include "calendar_notes.h"
#include "options_prefs.h"
#include "options_gui_tasks.h"
#include "calendar_utils.h"
#include "stock_icons.h"
#include "tasks_items.h"

/*------------------------------------------------------------------------------*/

void
free_notifications_list (GUI *appGUI)
{
GSList *node;
TASK_NTF *a;
guint i;

	for (i = 0, node = appGUI->tsk->notifications; node != NULL; node = node->next, i++) {
		a = g_slist_nth_data (appGUI->tsk->notifications, i);
		notify_notification_close (a->notify, NULL);
		if (a->item != NULL) {
			g_free (a->item);
		}
	}

	if (appGUI->tsk->notifications != NULL) {
		g_slist_free (appGUI->tsk->notifications);
		appGUI->tsk->notifications = NULL;
	}
}

/*------------------------------------------------------------------------------*/

void
tsk_show_info_dialog (GUI *appGUI)
{
gchar tmpbuf[BUFFER_SIZE];

	g_snprintf (tmpbuf, BUFFER_SIZE, "%s\n%s", _("Cannot perform selected operation."),
	            _("Task has been modified or removed."));
	gui_create_dialog (GTK_MESSAGE_INFO, tmpbuf, GTK_WINDOW (appGUI->main_window));
}

/*------------------------------------------------------------------------------*/

void
tsk_done_cb (NotifyNotification *n, const char *action, gpointer user_data)
{
GtkTreeIter *iter;
GSList *tnode;
TASK_NTF *a;
guint i;

	GUI *appGUI = (GUI *) user_data;

	for (i = 0, tnode = appGUI->tsk->notifications; tnode != NULL; tnode = tnode->next, i++) {
		a = g_slist_nth_data (appGUI->tsk->notifications, i);

		if (a->notify == n) {
			iter = task_get_iter (a->id, appGUI);

			if (iter != NULL) {

				if (a->item->repeat == TRUE) {
					tasks_repeat_done (iter, a->item, appGUI);
				} else if (config.delete_completed) {
					gtk_list_store_remove (appGUI->tsk->tasks_list_store, iter);
				} else {
					gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
					                    TA_COLUMN_COLOR, get_date_color (a->item->due_date_julian, a->item->due_time, TRUE),
					                    TA_COLUMN_DONE, TRUE, -1);
				}

			} else {
				tsk_show_info_dialog (appGUI);
			}

			break;
		}

	}

	notify_notification_close (a->notify, NULL);
	if (a->item != NULL) {
		g_free (a->item);
	}
	appGUI->tsk->notifications = g_slist_remove (appGUI->tsk->notifications, a);

}

/*------------------------------------------------------------------------------*/

void
tsk_postpone_notify_cb (NotifyNotification *n, const char *action, gpointer user_data)
{
GSList *tnode;
TASK_NTF *a;
guint i;

	GUI *appGUI = (GUI *) user_data;

	for (i = 0, tnode = appGUI->tsk->notifications; tnode != NULL; tnode = tnode->next, i++) {
		a = g_slist_nth_data (appGUI->tsk->notifications, i);

		if (a->notify == n) {
			a->time = get_seconds_for_today () + config.postpone_time * 60;
			a->date = utl_get_current_julian ();

			if (a->time >= 24 * 3600) {
				a->time -= 24 * 3600;
				a->date++;
			}

		}

	}

	notify_notification_close (a->notify, NULL);
}

/*------------------------------------------------------------------------------*/

void
tsk_show_task_cb (NotifyNotification *n, const char *action, gpointer user_data)
{
GtkTreeIter iter;
GSList *tnode;
GtkTreePath *sort_path, *filter_path, *path;
GtkTreeModel *model;
TASK_NTF *a;
guint i, id;

	GUI *appGUI = (GUI *) user_data;
	model = GTK_TREE_MODEL (appGUI->tsk->tasks_list_store);

	/* deiconify main window */
	gtk_window_deiconify (GTK_WINDOW (appGUI->main_window));

	/* select task tab */
	if (config.hide_tasks == FALSE) {
		gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_TASKS);
	}

	/* select task on list */
	for (i = 0, tnode = appGUI->tsk->notifications; tnode != NULL; tnode = tnode->next, i++) {
		a = g_slist_nth_data (appGUI->tsk->notifications, i);

		if (a->notify == n) {
			tasks_selection_activate (FALSE, appGUI);

			if (gtk_tree_model_get_iter_first (model, &iter) == TRUE) {
				sort_path = gtk_tree_model_get_path (model, &iter);

				while (sort_path != NULL) {
					gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->tsk->tasks_list), sort_path, NULL, FALSE);
					filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (appGUI->tsk->tasks_sort), sort_path);

					if (filter_path != NULL) {
						path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (appGUI->tsk->tasks_filter), filter_path);

						if (path != NULL) {
							gtk_tree_model_get_iter (model, &iter, path);
							gtk_tree_model_get (model, &iter, TA_COLUMN_ID, &id, -1);

							if (a->id == id) {
								tasks_selection_activate (TRUE, appGUI);
								gtk_tree_view_set_cursor (GTK_TREE_VIEW (appGUI->tsk->tasks_list), sort_path, NULL, FALSE);
								gtk_tree_path_free (path);
								gtk_tree_path_free (filter_path);
								g_signal_emit_by_name (G_OBJECT (appGUI->tsk->tasks_list_selection), "changed");
								break;
							}

							gtk_tree_path_free (path);
						}

						gtk_tree_path_free (filter_path);
					}

					gtk_tree_path_next (sort_path);
					if (gtk_tree_model_get_iter (model, &iter, sort_path) == FALSE) break;
				}

				gtk_tree_path_free (sort_path);
			}
		}
	}

	notify_notification_clear_actions (a->notify);
	if (config.postpone_time > 0) {
		notify_notification_add_action (a->notify, "postpone", _("Remind me later"),
		                                (NotifyActionCallback) tsk_postpone_notify_cb, appGUI, NULL);
	}
	notify_notification_add_action (a->notify, "done", _("Done"), (NotifyActionCallback) tsk_done_cb, appGUI, NULL);
    notify_notification_show (a->notify, NULL);
}

/*------------------------------------------------------------------------------*/
void
subtract_from_date (guint32 date, gint time, gint days, gint seconds, guint32 *new_date, gint *new_time)
{
	*new_date = date - days;

	if (time >= 0) {
		*new_time = time - seconds;

		if (*new_time < 0) {
			*new_time = (*new_time) + 24 * 3600;
			*new_date = (*new_date) - 1;
		}
	} else {
		*new_time = -1;
	}
}

/*------------------------------------------------------------------------------*/

gboolean
tsk_show_warning_notification (TASK_ITEM *item, GtkTreeIter *iter, GUI *appGUI)
{
guint32 current_date, warning_date;
gint current_time, warning_time;

	if (item->active_warning == TRUE && (item->warning_days > 0 || item->warning_time > 0)) {

		current_date = utl_get_current_julian ();
		current_time = get_seconds_for_today ();
		subtract_from_date (item->due_date_julian, item->due_time,
		                    item->warning_days, item->warning_time, &warning_date, &warning_time);

		if (warning_date < current_date + (warning_time < current_time) ? 1 : 0) {

			if (item->repeat == TRUE && item->offline_ignore == TRUE) {
				if (warning_date < appGUI->run_date + (warning_time < appGUI->run_time) ? 1 : 0) {

					gtk_list_store_set (appGUI->tsk->tasks_list_store, iter,
					                    TA_COLUMN_ACTIVE_WARNING, FALSE, -1);
					return FALSE;
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
tsk_show_task_notification (TASK_ITEM *item, GtkTreeIter *iter, GUI *appGUI)
{

	if (is_date_in_the_past (item->due_date_julian, item->due_time, utl_get_current_julian (), get_seconds_for_today ())) {

		if (item->repeat == TRUE && item->offline_ignore == TRUE) {
			if (is_date_in_the_past (item->due_date_julian, item->due_time, appGUI->run_date, appGUI->run_time)) {
				tasks_repeat_done (iter, item, appGUI);
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------*/

void
show_postponed_notification (GUI *appGUI)
{
gchar tmpbuf[BUFFER_SIZE];
gchar tmpbuf2[BUFFER_SIZE];
guint32 current_date;
gint current_time;
GSList *tnode;
TASK_NTF *a;
gint i;

	current_date = utl_get_current_julian ();
	current_time = get_seconds_for_today ();

	for (i = 0, tnode = appGUI->tsk->notifications; tnode != NULL; tnode = tnode->next, i++) {
		a = g_slist_nth_data (appGUI->tsk->notifications, i);

		if (a->date > 0 && a->time >= 0) {
			if (is_date_in_the_past (a->date, a->time, current_date, current_time)) {
				a->date = 0;
				a->time = -1;
				g_snprintf (tmpbuf2, BUFFER_SIZE, "<i>%s</i>", get_date_time_full_str (a->item->due_date_julian, a->item->due_time));
				g_strlcpy (tmpbuf, tmpbuf2, BUFFER_SIZE);
				if (a->item->desc != NULL) {
					if (strlen (a->item->desc)) {
						g_snprintf (tmpbuf, BUFFER_SIZE, "%s\n%.100s", tmpbuf2, a->item->desc);
					}
				}
				g_snprintf (tmpbuf2, BUFFER_SIZE, "%s (%s)", a->item->summary, _("postponed"));
				notify_notification_update (a->notify, tmpbuf2, tmpbuf, GTK_STOCK_DIALOG_WARNING);

                if (strlen (config.global_notification_command)) {
                    gui_save_data_and_run_command (config.global_notification_command, appGUI);
                }

				notify_notification_show (a->notify, NULL);
			}
		}

	}

}

/*------------------------------------------------------------------------------*/

gboolean
notify_tasks (GUI *appGUI)
{
gchar tmpbuf[BUFFER_SIZE];
gchar tmpbuf2[BUFFER_SIZE];
guint32 current_date;
gint current_time;

GtkTreeIter iter;
TASK_ITEM *item;
TASK_NTF *a;
gint i;

	show_postponed_notification (appGUI);

	current_date = utl_get_current_julian ();
	current_time = get_seconds_for_today ();

	i = 0;

	while (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (appGUI->tsk->tasks_list_store), &iter, NULL, i++)) {

		item = get_task_item (&iter, appGUI);
		if (item != NULL) {

			if (item->active == TRUE && item->done == FALSE) {

				if (tsk_show_task_notification (item, &iter, appGUI) == TRUE) {

					if (item->alarm_command != NULL) {
						if (strlen (item->alarm_command)) {
							gui_save_data_and_run_command (item->alarm_command, appGUI);
						}
					}

					g_snprintf (tmpbuf2, BUFFER_SIZE, "<i>%s</i>", get_date_time_full_str (item->due_date_julian, item->due_time));
					g_strlcpy (tmpbuf, tmpbuf2, BUFFER_SIZE);
					if (item->desc != NULL) {
						if (strlen (item->desc)) {
						    g_snprintf (tmpbuf, BUFFER_SIZE, "%s\n%.100s", tmpbuf2, item->desc);
						}
					}

					a = g_malloc (sizeof (TASK_NTF));
					g_assert (a != NULL);

					gtk_tree_model_get (GTK_TREE_MODEL (appGUI->tsk->tasks_list_store), &iter, TA_COLUMN_ID, &(a->id), -1);
					a->item = get_task_item (&iter, appGUI);
					a->time = -1;
					a->date = 0;
					a->notify = notify_notification_new (item->summary, tmpbuf, GTK_STOCK_DIALOG_WARNING, NULL);

					notify_notification_set_timeout (a->notify, NOTIFY_EXPIRES_NEVER);
					switch (get_priority_index (item->priority)) {
						case 0: notify_notification_set_urgency (a->notify, NOTIFY_URGENCY_LOW); break;
						case 1: notify_notification_set_urgency (a->notify, NOTIFY_URGENCY_NORMAL); break;
						case 2: notify_notification_set_urgency (a->notify, NOTIFY_URGENCY_CRITICAL); break;
					}

					if (config.postpone_time > 0) {
						notify_notification_add_action (a->notify, "postpone", _("Remind me later"),
						                                (NotifyActionCallback) tsk_postpone_notify_cb, appGUI, NULL);
					}

					if (tasks_category_get_state (item->category, STATE_TASKS, appGUI) == TRUE) {
						notify_notification_add_action (a->notify, "show_task", _("Show task"),
						                                (NotifyActionCallback) tsk_show_task_cb, appGUI, NULL);
					}

					notify_notification_add_action (a->notify, "done", _("Done"),
					                                (NotifyActionCallback) tsk_done_cb, appGUI, NULL);

					if (gtk_status_icon_get_visible (appGUI->osmo_trayicon)) {
						notify_notification_attach_to_status_icon (a->notify, appGUI->osmo_trayicon);
					}

                    if (strlen (config.global_notification_command)) {
                        gui_save_data_and_run_command (config.global_notification_command, appGUI);
                    }

					if (!notify_notification_show (a->notify, NULL)) {
						g_warning ("Failed to send notification");
						return FALSE;
					}

					appGUI->tsk->notifications = g_slist_append (appGUI->tsk->notifications, a);

					gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter,
					                    TA_COLUMN_ACTIVE, FALSE,
					                    TA_COLUMN_ACTIVE_WARNING, FALSE, -1);

				} else if (tsk_show_warning_notification (item, &iter, appGUI)) {

					g_snprintf (tmpbuf2, BUFFER_SIZE, "<b>%s</b>\n<i>%s</i>",
					            item->summary, get_date_time_full_str (item->due_date_julian, item->due_time));
					g_strlcpy (tmpbuf, tmpbuf2, BUFFER_SIZE);
					if (item->desc != NULL) {
						if (strlen (item->desc)) {
						    g_snprintf (tmpbuf, BUFFER_SIZE, "%s\n%.100s", tmpbuf2, item->desc);
						}
					}

					a = g_malloc (sizeof (TASK_NTF));
					g_assert (a != NULL);

					gtk_tree_model_get (GTK_TREE_MODEL (appGUI->tsk->tasks_list_store), &iter, TA_COLUMN_ID, &(a->id), -1);
					a->item = get_task_item (&iter, appGUI);
					a->time = -1;
					a->date = 0;
					a->notify = notify_notification_new (_("Alarm warning!"), tmpbuf, GTK_STOCK_DIALOG_INFO, NULL);

					notify_notification_set_timeout (a->notify, NOTIFY_EXPIRES_NEVER);
					notify_notification_set_urgency (a->notify, NOTIFY_URGENCY_NORMAL);
					notify_notification_add_action (a->notify, "show_task", _("Show task"),
					                                (NotifyActionCallback)tsk_show_task_cb, appGUI, NULL);

					if (gtk_status_icon_get_visible (appGUI->osmo_trayicon)) {
						notify_notification_attach_to_status_icon (a->notify, appGUI->osmo_trayicon);
					}

                    if (strlen (config.global_notification_command)) {
                        gui_save_data_and_run_command (config.global_notification_command, appGUI);
                    }

					if (!notify_notification_show (a->notify, NULL)) {
						g_warning ("Failed to send notification");
						return FALSE;
					}

					appGUI->tsk->notifications = g_slist_append (appGUI->tsk->notifications, a);

					gtk_list_store_set (appGUI->tsk->tasks_list_store, &iter,
					                    TA_COLUMN_ACTIVE_WARNING, FALSE, -1);
				}

			}
			task_item_free (item);
		}
	}

	return TRUE;
}

/*------------------------------------------------------------------------------*/

gboolean
time_handler (GUI *appGUI)
{
static gint counter = 0;

	update_clock (appGUI);

	if (counter == 0) {
		/* check tasks */
		notify_tasks (appGUI);

		/* check contacts */
		/* check notes */
	}
	counter = (counter + 1) % 60;

	return TRUE;
}

/*------------------------------------------------------------------------------*/

gboolean
check_tasks_contacts (guint32 julian_day, GUI *appGUI) {

    if (appGUI->calendar_only == TRUE || appGUI->all_pages_added == FALSE) {
        return FALSE;
    }

    /* check contacts */
    if (check_contacts (julian_day, appGUI) == TRUE) {
        return TRUE;
    }

    /* check tasks */
    return check_tasks (julian_day, TRUE, appGUI);
}

/*------------------------------------------------------------------------------*/

gboolean
check_tasks (guint32 julian_day, gboolean calendar, GUI *appGUI) {

GtkTreePath *path;
GtkTreeIter iter;
gchar *category;
guint32 date;

    if (appGUI->tsk->tasks_list_store != NULL) {
        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                                TA_COLUMN_DUE_DATE_JULIAN, &date, TA_COLUMN_CATEGORY, &category, -1);

            if (calendar == TRUE) {
                if (date == julian_day && tasks_category_get_state (category, STATE_CALENDAR, appGUI) == TRUE) {
                    return TRUE;
                }
            } else if (date == julian_day) {
                return TRUE;
            }

            g_free (category);
            gtk_tree_path_next (path);
        }
        gtk_tree_path_free (path);

    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
check_contacts (guint32 julian_day, GUI *appGUI) {

GtkTreePath *path;
GtkTreeIter iter;
GDate *new_cdate;
guint32 date, age;


    if (appGUI->cnt->contacts_list_store != NULL) {

        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, COLUMN_BIRTH_DAY_DATE, &date, -1);
            if (date != 0) {

                new_cdate = g_date_new_julian (date);

                if (new_cdate != NULL) {

                    age = g_date_get_year (appGUI->cal->date) - g_date_get_year (new_cdate);

                    if (age >= 0) {
						if (g_date_valid_dmy (g_date_get_day (new_cdate), g_date_get_month (new_cdate),
						                      g_date_get_year (appGUI->cal->date)) == FALSE) {
							g_date_subtract_days (new_cdate, 1);
						}

                        g_date_set_year (new_cdate, julian_to_year(julian_day));
                        date = g_date_get_julian (new_cdate);

                        if (date != 0 && julian_day == date) {
                            return TRUE;
                        }
                    }

                    g_date_free(new_cdate);
                }
            }
            gtk_tree_path_next(path);
        }
        gtk_tree_path_free(path);
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
button_event_checker_window_delete_cb (GtkButton *button, gpointer user_data) {}

void
button_event_checker_window_close_cb (GtkButton *button, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_destroy (appGUI->event_checker_window);
    gui_quit_osmo (TRUE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
button_event_checker_show_osmo_window_cb (GtkButton *button, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    appGUI->check_events = FALSE;
    gtk_widget_destroy (appGUI->event_checker_window);

    gui_systray_initialize (appGUI);
    gtk_widget_show (appGUI->main_window);
    gtk_window_move (GTK_WINDOW (appGUI->main_window), config.window_x, config.window_y);
}

/*------------------------------------------------------------------------------*/

void
event_checker_select_item (GUI *appGUI)
{
GtkTreeIter iter;
GtkTreeModel *model;
guint32 julian_day;
GDate *date;

	date = g_date_new ();
	g_return_if_fail (date != NULL);

	if (gtk_tree_selection_get_selected (appGUI->event_checker_list_selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, CE_COLUMN_DATE_JULIAN, &julian_day, -1);

		if (g_date_valid_julian (julian_day) == TRUE) {
			g_date_set_julian (date, julian_day);
			cal_jump_to_date (date, appGUI);
			update_aux_calendars (appGUI);
			gtk_notebook_set_current_page (GTK_NOTEBOOK (appGUI->notebook), PAGE_CALENDAR);
			button_event_checker_show_osmo_window_cb (NULL, appGUI);
		}
	}

	g_date_free (date);
}

/*------------------------------------------------------------------------------*/

gint
event_checker_list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {
        event_checker_select_item (appGUI);
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint 
event_checker_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Return:
            event_checker_select_item (appGUI);
            return TRUE;
        case GDK_Escape:
            button_event_checker_window_close_cb (NULL, appGUI);
            return TRUE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
create_event_checker_window (GUI *appGUI) {

GtkWidget           *scrolledwindow;
GtkWidget           *vbox1;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *close_button;
GtkWidget           *show_osmo_button;
GtkTreeViewColumn   *column, *julian_day_column;
GtkCellRenderer     *renderer;
GtkTreeIter         iter;
GtkTreePath         *path;
guint32             i, j, start_date, current_date, julian_day, date, age;
gboolean            day_notes_flag, tasks_flag, birthdays_flag;
GDate               *new_cdate;
GSList              *lnode;
struct note         *day_note;
gchar               *tmp_note, *summary, *first_name, *last_name, *category;
gchar               tmpbuf[BUFFER_SIZE], template[BUFFER_SIZE], date_str[BUFFER_SIZE];

    start_date = config.lastrun_date;           /* - */
    current_date = utl_get_current_julian () + appGUI->check_ndays_events;     /* + */

    /* check day notes */

    day_notes_flag = FALSE;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            day_note = g_slist_nth_data (appGUI->cal->notes_list, i);
            julian_day = utl_dmy_to_julian (day_note->day, day_note->month, day_note->year);

            j = start_date;

            while (j <= current_date) {
                if (j == julian_day) {
                    day_notes_flag = TRUE;
                    break;
                }
                j++;
            };

            if (day_notes_flag == TRUE) break;
        }
    }

    /* check tasks */

    tasks_flag = FALSE;

    if (appGUI->tsk->tasks_list_store != NULL) {
        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                                TA_COLUMN_DUE_DATE_JULIAN, &julian_day, TA_COLUMN_CATEGORY, &category, -1);

            j = start_date;

            while (j <= current_date) {
                if (j == julian_day && (tasks_category_get_state (category, STATE_CALENDAR, appGUI) == TRUE ||
                                        tasks_category_get_state (category, STATE_TASKS, appGUI) == TRUE)) {
                    tasks_flag = TRUE;
                    break;
                }
                j++;
            };

            if (tasks_flag == TRUE) {
                g_free (category);
                break;
            }

            g_free (category);
            gtk_tree_path_next(path);
        }
        gtk_tree_path_free(path);

    }

    /* check birthdays */

    birthdays_flag = FALSE;

    if (appGUI->cnt->contacts_list_store != NULL) {

        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, 
                                COLUMN_BIRTH_DAY_DATE, &date, -1);
            if (date != 0) {

                new_cdate = g_date_new_julian (date);

                if (new_cdate != NULL) {

                    age = g_date_get_year (appGUI->cal->date) - g_date_get_year (new_cdate);

                    if (age >= 0) {

                        j = start_date;

                        while (j <= current_date) {
                            g_date_set_year (new_cdate, julian_to_year(j));
                            date = g_date_get_julian (new_cdate);
                            if (j == date) {
                                birthdays_flag = TRUE;
                                break;
                            }
                            j++;
                        };
                    }

                    g_date_free(new_cdate);
                }
            }

            if (birthdays_flag == TRUE) break;

            gtk_tree_path_next(path);
        }
        gtk_tree_path_free(path);
    }

    /* any events available ? */

    if (day_notes_flag == FALSE && tasks_flag == FALSE && birthdays_flag == FALSE) {
        return FALSE;           /* if not then quit */
    }
        
    appGUI->event_checker_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->event_checker_window), _("Events"));
    gtk_window_set_position (GTK_WINDOW (appGUI->event_checker_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW(appGUI->event_checker_window), 750, 650);
    gtk_window_set_modal (GTK_WINDOW (appGUI->event_checker_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->event_checker_window), "delete_event",
                      G_CALLBACK(button_event_checker_window_delete_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->event_checker_window), "key_press_event",
                      G_CALLBACK (event_checker_key_press_cb), appGUI);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->event_checker_window), 8);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->event_checker_window), vbox1);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->event_checker_list_store = gtk_list_store_new (CHECK_EVENTS_NUM_COLUMNS, 
                                                           G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);

    appGUI->event_checker_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->event_checker_list_store));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->event_checker_list), TRUE);
    gtk_widget_show (appGUI->event_checker_list);
    GTK_WIDGET_SET_FLAGS (appGUI->event_checker_list, GTK_CAN_DEFAULT);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->event_checker_list);
    g_signal_connect(G_OBJECT(appGUI->event_checker_list), "button_press_event",
                     G_CALLBACK(event_checker_list_dbclick_cb), appGUI);

    appGUI->event_checker_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->event_checker_list));

    /* create columns */

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes (_("Date"), renderer, 
                                                       "text", CE_COLUMN_DATE, 
                                                       NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->event_checker_list), column);

    renderer = gtk_cell_renderer_text_new();
	g_object_set (G_OBJECT(renderer), "xpad", 8, NULL);
    julian_day_column = gtk_tree_view_column_new_with_attributes ("Julian", renderer, 
                                                                  "text", CE_COLUMN_DATE_JULIAN, 
                                                                  NULL);
    gtk_tree_view_column_set_visible (julian_day_column, FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->event_checker_list), julian_day_column);
    gtk_tree_view_column_set_sort_column_id (julian_day_column, CE_COLUMN_DATE_JULIAN);

    renderer = gtk_cell_renderer_text_new();
	g_object_set (G_OBJECT(renderer), "xpad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes (_("Type"), renderer, 
                                                       "text", CE_COLUMN_EVENT_TYPE, 
                                                       NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->event_checker_list), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set (G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    column = gtk_tree_view_column_new_with_attributes (_("Event"), renderer, 
                                                       "text", CE_COLUMN_EVENT_LINE, 
                                                       NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->event_checker_list), column);

    /* insert day notes */

    if (day_notes_flag == TRUE) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            day_note = g_slist_nth_data (appGUI->cal->notes_list, i);
            julian_day = utl_dmy_to_julian (day_note->day, day_note->month, day_note->year);

            j = start_date;

            while (j <= current_date) {
                if (j == julian_day) {                  
                    tmp_note = cal_note_remove_empty_lines (day_note->note);
                    gtk_list_store_append(appGUI->event_checker_list_store, &iter);
                    sprintf(date_str, "%s\n(%s)", 
                            julian_to_str (julian_day, config.date_format), utl_get_julian_day_name (julian_day));
                    gtk_list_store_set(appGUI->event_checker_list_store, &iter, 
                                       CE_COLUMN_DATE, date_str,
                                       CE_COLUMN_DATE_JULIAN, julian_day, 
                                       CE_COLUMN_EVENT_TYPE, _("Day note"),
                                       CE_COLUMN_EVENT_LINE, tmp_note, -1);
                    g_free (tmp_note);
                }
                j++;
            };
        }

    }

    /* insert tasks */

    if (tasks_flag == TRUE) {

        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->tsk->tasks_list_store), &iter, 
                                TA_COLUMN_DUE_DATE_JULIAN, &julian_day, 
                                TA_COLUMN_SUMMARY, &summary, 
                                TA_COLUMN_CATEGORY, &category, -1);

            j = start_date;

            while (j <= current_date) {
                if (j == julian_day && (tasks_category_get_state (category, STATE_CALENDAR, appGUI) == TRUE ||
                                        tasks_category_get_state (category, STATE_TASKS, appGUI) == TRUE)) {
                    gtk_list_store_append(appGUI->event_checker_list_store, &iter);
                    g_snprintf (date_str, BUFFER_SIZE, "%s\n(%s)", 
                            julian_to_str (julian_day, config.date_format), utl_get_julian_day_name (julian_day));
                    gtk_list_store_set(appGUI->event_checker_list_store, &iter, 
                                       CE_COLUMN_DATE, date_str,
                                       CE_COLUMN_DATE_JULIAN, julian_day, 
                                       CE_COLUMN_EVENT_TYPE, _("Task"),
                                       CE_COLUMN_EVENT_LINE, summary, -1);
                }
                j++;
            };

            g_free (summary);
            g_free (category);

            gtk_tree_path_next(path);
        }
        gtk_tree_path_free(path);
    }

    /* insert birthdays */

    if (birthdays_flag == TRUE) {

        sprintf(template, "(%s)", _("None"));

        path = gtk_tree_path_new_first();

        while (gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, path) == TRUE) {
            gtk_tree_model_get (GTK_TREE_MODEL(appGUI->cnt->contacts_list_store), &iter, 
                                COLUMN_BIRTH_DAY_DATE, &date, 
                                COLUMN_FIRST_NAME, &first_name,
                                COLUMN_LAST_NAME, &last_name, -1);

            if (date != 0) {

                new_cdate = g_date_new_julian (date);

                if (new_cdate != NULL) {

                    age = g_date_get_year (appGUI->cal->date) - g_date_get_year(new_cdate);

                    if (age >= 0) {

                        j = start_date;

                        while (j <= current_date) {
                            g_date_set_year (new_cdate, julian_to_year(j));
                            date = g_date_get_julian (new_cdate);
                            if (j == date) {
                                if (g_utf8_collate(first_name, template) == 0) {
                                    sprintf(tmpbuf, "%s", last_name);
                                } else if (g_utf8_collate(last_name, template) == 0) {
                                    sprintf(tmpbuf, "%s", first_name);
                                } else {
                                    sprintf(tmpbuf, "%s %s", first_name, last_name);
                                }
                                gtk_list_store_append(appGUI->event_checker_list_store, &iter);
                                sprintf(date_str, "%s\n(%s)", 
                                        julian_to_str (j, config.date_format), utl_get_julian_day_name (j));
                                gtk_list_store_set(appGUI->event_checker_list_store, &iter, 
                                                   CE_COLUMN_DATE, date_str,
                                                   CE_COLUMN_DATE_JULIAN, j, 
                                                   CE_COLUMN_EVENT_TYPE, _("Birthday"),
                                                   CE_COLUMN_EVENT_LINE, tmpbuf, -1);
                            }
                            j++;
                        };
                    }

                    g_date_free(new_cdate);
                }
            }

            g_free(first_name);
            g_free(last_name);

            gtk_tree_path_next(path);
        }
        gtk_tree_path_free(path);
    }

    g_signal_emit_by_name(julian_day_column, "clicked");

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 8);

    show_osmo_button = gtk_button_new_with_label (_("Show Osmo"));
    gtk_widget_show (show_osmo_button);
    GTK_WIDGET_UNSET_FLAGS(show_osmo_button, GTK_CAN_FOCUS);
    g_signal_connect(show_osmo_button, "clicked", G_CALLBACK(button_event_checker_show_osmo_window_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), show_osmo_button);

    if (config.default_stock_icons) {
        close_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    } else {
        close_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CLOSE);
    }
    gtk_widget_show (close_button);
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    g_signal_connect(close_button, "clicked", G_CALLBACK(button_event_checker_window_close_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), close_button);

    gtk_widget_show(appGUI->event_checker_window);

    return TRUE;
}

/*------------------------------------------------------------------------------*/

