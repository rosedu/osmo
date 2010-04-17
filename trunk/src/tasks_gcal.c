
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

char *read_raw_xml_file(char *filename) {
	FILE *f;
	char *ret, *temp, *test;
	ret = malloc (sizeof (char) * 2000);
	temp = malloc (sizeof (char) * 2000);
	if (ret == NULL) {
			g_print("ret not created\n");
			return NULL;
	}
	else
			strcpy(ret, "");
	if (temp == NULL)
			return NULL;
	f = fopen (filename, "r");
	if (f != NULL) {
		g_print ("File %s found\n", filename);
		do {
			test = fgets (temp, 100, f);
			if (test != NULL) {
				strcat (ret, temp);
			}
		} while (!feof (f));
		fclose (f);
	}
	else {
			g_print("File %s not found\n", filename);
			free (temp);
			return NULL;
	}
	free (temp);
	return ret;
}

/*------------------------------------------------------------------------------*/

void temporary_test () {
	FILE *f;
	f = fopen ("test_file", "w");
	if (f != NULL) {
		g_print ("Test file written\n");
		fputs ("If you can read this then you found me", f);
		fclose (f);
	}
}

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
	gchar *temp;
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

void *tasks_export_gcal (void *parameter) {
	GUI *appGUI = (GUI *) parameter;
	gcal_t gcal;
	gcal_event_t event;
	struct gcal_event_array *events;
	int result, i;
	GtkTreeIter iter;
	TASK_ITEM *item;
	gchar username[] = {"osmosync1@gmail.com"},
		  password[] = {"osmopass"},
		  *gcal_date;
	
	gcal = gcal_new (GCALENDAR);
	if (gcal == NULL) {
		g_print ("Failed to initialize gcal\n");
		exit(1);
	}
	result = gcal_get_authentication (gcal, username, password);
	if (result != 0) {
		g_print ("Failed to authenticate\n");
		goto cleanup;
	}
	
	/* Load all the events from Google
	   to avoid redundancy
	   */
	events = malloc (sizeof (struct gcal_event_array));
	if (events == NULL) {
		g_print ("tasks_export_gcal(): Failed to allocate memory for events\n");
		result = 1;
	}
	else {
		result = gcal_get_events (gcal, events);
	}
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
				g_print ("Got item number %d\n\"%s\"\n", i, (char *)
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
					g_print ("item->desc == NULL\n");
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
					if (gcal_event_search_match (event, events)) {
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
	gcal_cleanup_events (events);
cleanup:
	gcal_delete (gcal);
}

/*------------------------------------------------------------------------------*/

#endif
