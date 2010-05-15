
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 TODO: Change here as needed Tomasz Maka <pasp@users.sourceforge.net>
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

#include "gui.h"
#include "tasks_gcal.h"
#include "tasks_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef HAVE_LIBGCAL

/*------------------------------------------------------------------------------*/

gchar* gcal_julian_to_date (guint32 julian_day, gint time) {
	GDate *date;
	GDateYear year;
	GDateMonth month;
	GDateDay day;
	gchar *cdate = NULL;
	gint hours, minutes, seconds;
	if (julian_day != 0) {
		if (time != -1) {
			hours = time / 3600;
			minutes = (time - hours * 3600) / 60;
			seconds = time - hours * 3600 - minutes * 60;
		}
		date = g_date_new_julian (julian_day);
		cdate = malloc (sizeof(gchar) * 21);
		if (cdate != NULL) {
			year = g_date_get_year (date);
			if (year > 2009) {
				g_sprintf (cdate, "%d-", year);
				month = g_date_get_month (date);
				if (month < 10) {
					strcat (cdate, "0");
				}
				g_sprintf (cdate, "%s%d-", cdate, month);
				day = g_date_get_day (date);
				if (day < 10) {
					strcat (cdate, "0");
				}
				g_sprintf (cdate, "%s%d", cdate, day);
				if (time != -1) {
					strcat (cdate, "T");
					if (hours < 10) {
						strcat (cdate, "0");
					}
					g_sprintf (cdate, "%s%d:", cdate, hours);
					if (minutes < 10) {
						strcat (cdate, "0");
					}
					g_sprintf (cdate, "%s%d:", cdate, minutes);
					if (seconds < 10) {
						strcat (cdate, "0");
					}
					g_sprintf (cdate, "%s%d", cdate, seconds);
				}
			}
			else {
				cdate = NULL;
			}
		}
		g_date_free (date);
	}
	return cdate;	
}

/*------------------------------------------------------------------------------*/
/**
  *@brief Compare 2 events
  *
  *@param a First event to compare
  *@param b Second event to compare
  *@return 0 if the events match certain relevant fields
  *
  *@author Vlad Bagrin <artee_md@yahoo.com>
  */

int gcal_events_compare (gcal_event_t a, gcal_event_t b) {
	/* Compare the titles */
	if (g_strcmp0 (gcal_event_get_title (a), gcal_event_get_title (b)) !=
					0) {
		return 1;
	}
	/* Compare the content */
	if (g_strcmp0 (gcal_event_get_content (a), gcal_event_get_content (b)) !=
					0) {
		return 1;
	}
	/* Compare the start date. First 19 chars for date and time. */
	if (strncmp (gcal_event_get_start (a), gcal_event_get_start (b), 19) !=
					0) {
		return 1;
	}
	/* Compare the end date */
	if (strncmp (gcal_event_get_end (a), gcal_event_get_end (b), 19) !=
					0) {
		return 1;
	}
	return 0;
}

/*------------------------------------------------------------------------------*/
/**
  *@brief Compare event to all the events returned by Google
  *
  *@param event Event to compare
  *@param events List of events from Google
  *@return 0 if found  a match
  *@todo Better search mechanism (binary maybe)
  *
  *@author Vlad Bagrin <artee_md@yahoo.com>
  */

int gcal_event_search_match (gcal_event_t event, struct gcal_event_array *events) {
	int i;
	/* TODO: Find a better way to deal with the NULL pointer */
	if (events == NULL) {
		return 1;
	}
	for (i = 0; i < events->length; i++) {
		/* Found a matching event in the list, not good */
		if (!gcal_events_compare (event, gcal_event_element (events, i))) {
			return 0;
		}
	}
	return 1;
}

/*------------------------------------------------------------------------------*/
/**
  *@brief Main function that sends the events
  *
  *The function sends all the visible items in the tasks list from Osmo
  *and sends them in the form of events, one by one, if possible. It also
  *checks to see if the event was previously added or an event with
  *similar functionality exists already (why have 2 events at the exact
  *same time with the same name and description?)
  *
  *@param parameter A void pointer that is in fact appGUI
  *
  *@author Vlad Bagrin <artee_md@yahoo.com>
  */

void *tasks_export_gcal (exportData *data) {
	GUI *appGUI = data->appGUI;
	gcal_t gcal;
	gcal_event_t event;
	struct gcal_event_array events;
	int result, i;
	GtkTreeIter iter;
	TASK_ITEM *item;
	//gchar username[] = {"osmosync1@gmail.com"},
	//	  password[] = {"osmopass"};

	gchar *usr_entry_text,*pas_entry_text;
	usr_entry_text = gtk_entry_get_text(GTK_ENTRY(data->usrEntry));
	
	pas_entry_text = gtk_entry_get_text(GTK_ENTRY(data->pasEntry));
	

	gchar	  *gcal_date;
	
	gcal = gcal_new (GCALENDAR);
	if (gcal == NULL) {
		g_print ("Failed to initialize gcal\n");
		exit(1);
	}
	result = gcal_get_authentication (gcal, usr_entry_text, pas_entry_text);
	if (result != 0) {
		g_print ("Failed to authenticate\n");
		goto cleanup;
	}
	
	/* Load all the events from Google
	   to avoid redundancy
	   */
	result = gcal_get_events (gcal, &events);
	if (result != 0) {
		g_print ("tasks_export_gcal(): Failed to load the events from Google\n");
	}
	else {
		g_print ("Loaded the events list from Google\n");
	}

	i = 0;

	while (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL
							(appGUI->tsk->tasks_list_store), &iter,
							NULL, i++)) {
			item = tsk_get_item (&iter, appGUI);
			if (item != NULL) {
				g_print ("------------------\nGot item number %d\n\"%s\"\n", i, (char *)
								item->summary);
				/* Create empty event and fill it up */
				event = gcal_event_new (NULL);
				if (event == NULL) {
						g_print ("Failed to allocate memory for event\n");
						tsk_item_free (item);
						continue;
				}
				gcal_event_set_title (event, (char*) item->summary);
				if (item->desc == NULL) {
					gcal_event_set_content (event, "");
				}
				else {
					gcal_event_set_content (event, (char*) item->desc);
				}
				gcal_date = gcal_julian_to_date (item->due_date_julian,
								item->due_time);
				g_print ("Date: %s\n", gcal_date);
				/* Google will reject events with no date so no point
				   in wasting time waiting for a failed response
				   */
				if (gcal_date != NULL) {
					gcal_event_set_start (event, gcal_date);
					gcal_event_set_end (event, gcal_date);
					/* Search similar event and ignore it */
					if (gcal_event_search_match (event, &events)) {
						result = gcal_add_event (gcal, event);
							if (result == 0) {
								g_print ("Event \"%s\" added\n", (char *)
												item->summary);
							}
							else {		
								g_print ("Event \"%s\" rejected by Google\n", (char *)
												item->summary);
							}
					}
					else {
						g_print ("Event \"%s\" already synched\n", (char *)
										item->summary);
					}
					free (gcal_date);
				}
				else {
					g_print ("This event lacks a due date and will not be sent\n");
				}
				gcal_destroy_entry (event);
				tsk_item_free (item);
			}
	}
	gcal_cleanup_events (&events);
cleanup:
	gcal_delete (gcal);
}

/*------------------------------------------------------------------------------*/

guint32 date_to_julian (char *date) {
	int year, month, day;

	/* In case Google returns only iCal recurrent code */	
	if (*date == '\0') {
		g_print ("Recurrent events not supported yet\n");
		return 0;
	}

	year = atoi (strndup (date, 4));
	date = date + 5;
	month = atoi (strndup (date, 2));
	date = date + 3;
	day = atoi (strndup (date, 2));

	return g_date_get_julian ( g_date_new_dmy (day, month, year));
}

/*------------------------------------------------------------------------------*/
/**
  *@brief Extract due time
  *A very risky function here, I'm not sure about it. Seems fine atm.
  *@param date String from Google
  *@return Number of seconds from 00:00
  */

gint date_to_time (char *date) {
	int hours = 0, minutes = 0, seconds = 0;
	/* In case Google returns only iCal recurrent code */	
	if (*date == '\0') {
		return -1;
	}
	date = date + strlen ("2010-01-15T");
	if (*date == '\0') {
		return -1;
	}
	hours = atoi (strndup (date, 2));
	date = date + 3;
	minutes = atoi (strndup (date, 2));
	date = date + 3;
	seconds = atoi (strndup (date, 2));

	return hours * 3600 + minutes * 60 + seconds;
}

/*------------------------------------------------------------------------------*/

int items_compare (TASK_ITEM *item_a, TASK_ITEM *item_b) {
	/* Compare due dates */
	if (item_a->due_date_julian != item_b->due_date_julian) {
		return 1;
	}
	/* Compare due times */
	if (item_a->due_time != item_b->due_time) {
		return 1;
	}
	/* Compare summaries */
	if (g_strcmp0 (item_a->summary, item_b->summary) != 0) {
		return 1;
	}
	/* Compare descriptions */
	if (g_strcmp0 (item_a->desc, item_b->desc) != 0) {
		return 1;
	}
	/* If all fields were equal, return 0 */
	return 0;
}

/*------------------------------------------------------------------------------*/
/**
  *@brief Checks if a similar item is already listed
  *@param item_a Item to check
  *@param appGUI Need this to poke through data
  *@return 0 if found, 1 otherwise
  */

int search_item_in_list (TASK_ITEM *item_a, GUI *appGUI) {
	int i;
	GtkTreeIter iter;
	TASK_ITEM *item_b;

	i = 0;
	/* TODO: Better search algorithm */
	while (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL
							(appGUI->tsk->tasks_list_store), &iter,
							NULL, i++)) {
		/* Create TASK_ITEM structure with info */
		item_b = tsk_get_item (&iter, appGUI);
		if (item_b == NULL) {
			g_print ("search_item_in_list(): memory allocation error\n");
			continue;		
		}
		/* Compare the two items */
		if (items_compare (item_a, item_b) == 0) {
			tsk_item_free (item_b);
			return 0;
		}
		/* Don't forget to clean the mess */
		tsk_item_free (item_b);
	}
	return 1;
}

/*------------------------------------------------------------------------------*/

int add_event_to_list (gcal_event_t event, GUI *appGUI) {
	TASK_ITEM *item;
	
	/* Terminal delimiter */
	g_print ("--------------------------------\n");
	/* Memory for empty item */
	item = g_new0 (TASK_ITEM, 1);
	if (item == NULL) {
		g_print ("add_event_to_list(): No memory for item\n");
		return 1;
	}
	/* item initialization */
	item->done = FALSE;
	item->active = TRUE;
	item->offline_ignore = FALSE;
	item->repeat = FALSE;
	item->repeat_day = 127;
	item->repeat_day_interval = 0;
	item->repeat_start_day = 0;
	item->repeat_month_interval = 0;
	item->repeat_time_start = 0;
	item->repeat_time_end = 0;
	item->repeat_time_interval = 0;
	item->repeat_counter = 0;
	item->alarm_command = NULL;
	item->warning_days = 0;
	item->warning_time = 0;
	item->postpone_time = 0;
	/* Due date changes */
	item->due_date_julian = date_to_julian (gcal_event_get_start (event));
	/* Due time changes */
	item->due_time = date_to_time (gcal_event_get_start (event));
	item->start_date_julian = 0;
	item->done_date_julian = 0;
	item->priority = NULL;
	item->category = NULL;
	/* Summary changes */
	item->summary = gcal_event_get_title (event);
	/* Needed for compatibility with empty strings in Osmo */	
	if (g_strcmp0 (item->summary, "") == 0) {
		item->summary = NULL;
	}
	/* Description changes */
	item->desc = gcal_event_get_content (event);
	/* Needed for compatibility with empty strings in Osmo */	
	if (g_strcmp0 (item->desc, "") == 0) {
		item->desc = NULL;
	}
	item->sound_enable = TRUE;
	
	if (search_item_in_list (item, appGUI) != 0) {
		/* ID changes here */
		item->id = appGUI->tsk->next_id++;
		/* Item is put in the list */
		add_item_to_list (item, appGUI);
		g_print ("Event \"%s\" added\n", item->summary);
	}
	else {
		g_print ("Event \"%s\" already synched\n", item->summary);
	}
	tsk_item_free (item);
}

/*------------------------------------------------------------------------------*/

void tasks_import_gcal (exportData *data) {
	gcal_t gcal;
	int i;
	struct gcal_event_array events;
	
	gchar *usr_entry_text,*pas_entry_text;
	usr_entry_text = gtk_entry_get_text(GTK_ENTRY(data->usrEntry));
	
	pas_entry_text = gtk_entry_get_text(GTK_ENTRY(data->pasEntry));
	
	


	//gchar username[] = {"osmosync1@gmail.com"},
	//password[] = {"osmopass"};

	/* Initialize gcal structure */
	gcal = gcal_new (GCALENDAR);
	if (gcal == NULL) {
		g_print ("Failed to initialize gcal\n");
		return;
	}
	/* Connect */
	if (gcal_get_authentication (gcal, usr_entry_text, pas_entry_text) != 0) {
		g_print ("Failed to authenticate\n");
		gcal_delete (gcal);
		return;
	}
	/* Load the events */
	if (gcal_get_events (gcal, &events) != 0) {
		g_print ("tasks_import_gcal(): Failed to load the events from Google\n");
		return;
	}
	else {
		g_print ("Loaded the events list from Google\n");
	}
	/* Add the events to the visible list */
	for (i = 0; i < events.length; i++) {
		add_event_to_list(gcal_event_element (&events, i), data->appGUI);
	}
	/* Cleanup */
	gcal_delete (gcal);
}

/*------------------------------------------------------------------------------*/
#endif
