
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
	gint hours = 0, minutes = 0, seconds = 0;
	if (julian_day != 0) {
		if (time != -1) {
			g_print ("Time from item stuct: %d\n", time);
			hours = time / 3600;
			g_print ("Hours: %d\n", hours);
			minutes = (time - hours * 3600) / 60;
			g_print ("Minutes: %d\n", minutes);
			seconds = time - hours * 3600 - minutes * 60;
			g_print ("Seconds: %d\n", seconds);
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
				g_sprintf (cdate, "%s%dT", cdate, day);
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
				/* TODO: Solve problem with the timezone */
				g_sprintf (cdate, "%s%d+02:00", cdate, seconds);
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

void tasks_export_gcal (GUI *appGUI) {
	gcal_t gcal;
	gcal_event_t event;
	int result, i;
	GtkTreeIter iter;
	TASK_ITEM *item;
	gchar username[] = {"osmosync1@gmail.com"},
		  password[] = {"osmopass"},
		  *gcal_date,
		  *xml = NULL;
	
	/*xml = read_raw_xml_file ("contacts.xml");
	g_print("%s\n", xml);*/
	gcal = gcal_new (GCALENDAR);
	if (gcal == NULL)
	{
		g_print ("Failed to initialize gcal\n");
		return;
	}
	result = gcal_get_authentication (gcal, username, password);
	if (result != 0)
	{
		g_print ("Failed to authenticate\n");
		return;
	}

	i = 0;

	while (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL
							(appGUI->tsk->tasks_list_store), &iter,
							NULL, i++)) {
			item = tsk_get_item (&iter, appGUI);
			if (item != NULL) {
				g_print ("Got item number %d\n", i);
				/* Create empty event and fill it up */
				/* TODO: Check if the event to add was added before */
				event = gcal_event_new (NULL);
				/*event = malloc (sizeof (gcal_event_t));*/
				if (event == NULL) {
						g_print ("Failed to allocate memory for event\n");
						tsk_item_free (item);
						continue;
				}
				gcal_event_set_title (event, (char*) item->summary);
				gcal_event_set_content (event, (char*) item->desc);
				gcal_date = gcal_julian_to_date (item->due_date_julian,
								item->due_time);
				g_print ("Formated due date: %s\n", gcal_date);
				if (gcal_date != NULL) {
					gcal_event_set_start (event, gcal_date);
					gcal_event_set_end (event, gcal_date);
				}
				free (gcal_date);
				/*g_print ("Event address: %d\n", (int)event);*/
				/*event->title = malloc (sizeof (char) *
								(strlen(item->summary) + 1));
				if (event->title == NULL) {
						g_print ("Failed to allocate memory\n");
						continue;
				}
				else {
					g_print ("%s\n", event->title);
				}*/
				//gcal_event_set_title (event, item->summary);
				//gcal_event_set_content (event, item->desc);
				/*g_print ("Entry fields:\n");*/
				/*temp = malloc (sizeof (char) * 33);
				g_sprintf (temp, "%d", item->id);
				gcal_event_set_id (event, temp);
				free (temp);*/
				/*g_print ("Entry ID: %s\n", gcal_event_get_id
				 * (event));*/
			    
				/*gcal_event_set_title (event, item->summary);*/
				/*g_print ("Entry title: \%s\n", gcal_event_get_title
								(event));*/

				//g_print("Store xml: %c\n", (char)event->store_xml);
				
				/*g_print ("Event fields:\n");
				gcal_event_set_content (event, item->desc);
				g_print ("Entry content: \%s\n",
								gcal_event_get_content
								(event));*/
				result = gcal_add_event (gcal, event);
				if (result == 0) {
					g_print ("Event added\n");
				}
				else {
					g_print ("Failed to add event to Google\n");
				}
				g_print ("Result: %d\n\n", result);
				gcal_destroy_entry (event);
				tsk_item_free (item);
			}
	}
	gcal_delete (gcal);
}

/*------------------------------------------------------------------------------*/

#endif
