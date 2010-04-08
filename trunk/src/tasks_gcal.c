
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
	char *ret, *temp;
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
		fgets (temp, 100, f);
		strcat (ret, temp);
		fclose (f);
	}
	else {
			g_print("File not found\n");
	}
	return ret;
}

/*------------------------------------------------------------------------------*/

void tasks_export_gcal(GUI *appGUI) {
	gcal_t gcal;
	gcal_event_t event;
	int result, i;
	GtkTreeIter iter;
	TASK_ITEM *item;
	gchar username[] = {"osmosync1@gmail.com"},
		  password[] = {"osmopass"},
		  /*xml[] = { "<?xml version="1.0"?><entry
		   * xmlns=\"http://www.w3.org/2005/Atom\"
		   * xmlns:gd=\"http://schemas.google.com/g/2005\"
		   * gd:etag=\"&quot;RX8zezVSLip7ImA9WB5UEU4NQAc.&quot;\"><id>http://www.google.com/m8/feeds/contacts/vlad.bagrin\40gmail.com/base/2</id><updated>2007-08-15T00:28:54.183Z</updated><app:edited
		   * xmlns:app=\"http://www.w3.org/2007/app\">2007-08-15T00:28:54.183Z</app:edited><category
		   * scheme=\"http://schemas.google.com/g/2005#kind\"
		   * term=\"http://schemas.google.com/contact/2008#contact\"/><title/><link
		   * rel=\"http://schemas.google.com/contacts/2008/rel#photo\"
		   * type=\"image/*\"
		   * href=\"http://www.google.com/m8/feeds/photos/media/vlad.bagrin\%40gmail.com/2\"/><link
		   * rel=\"self\" type=\"application/atom+xml\"
		   * href=\"http://www.google.com/m8/feeds/contacts/vlad.bagrin\%40gmail.com/full/2\"/><link
		   * rel=\"edit\" type=\"application/atom+xml\"
		   * href=\"http://www.google.com/m8/feeds/contacts/vlad.bagrin\%40gmail.com/full/2\"/><gd:email
		   * rel=\"http://schemas.google.com/g/2005#other\"
		   * address=\"xp_firewall@yahoo.com\"
		   * primary="true"/></entry>\"" },*/
		  *temp;
		  /**xml = NULL;*/
	/*xml = read_raw_xml_file
	 * ("osmo/gitosmo/osmo/trunk/src/contacts.xml");*/
	/*g_print("%s\n", xml);*/
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
					event = gcal_event_new (NULL);
					/*event = malloc (sizeof (gcal_event_t));*/
					if (event == NULL) {
							g_print ("Failed to allocate memory for event\n");
							tsk_item_free (item);
							continue;
					}
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
					g_print ("Entry fields:\n");
					temp = malloc (sizeof (char) * 33);
					g_sprintf (temp, "%d", item->id);
					gcal_event_set_id (event, temp);
					g_print ("Entry ID: %s\n", gcal_event_get_id (event));
				    
					gcal_event_set_title (event, item->summary);
					g_print ("Entry title: \%s\n", gcal_event_get_title
									(event));

					//g_print("Store xml: %c\n", (char)event->store_xml);
					
					g_print("Event fields:\n");
					gcal_event_set_content (event, item->desc);
					g_print ("Entry content: \%s\n",
									gcal_event_get_content
									(event));

					
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
