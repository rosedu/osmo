
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
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <config.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifdef HAVE_LIBICAL
#include <ical.h>
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION
#endif  /* HAVE_LIBICAL */

#include "i18n.h"
#include "calendar.h"
#include "gui.h"
#include "gui_utils.h"
#include "calendar_notes.h"
#include "options_prefs.h"
#include "options_gui_calendar.h"
#include "calendar_ical.h"
#include "time_utils.h"
#include "date_utils.h"
#include "stock_icons.h"


#ifdef HAVE_LIBICAL

/*------------------------------------------------------------------------------*/

gboolean
ics_initialize_timezone (void) {

gchar *tz_search_paths [] = {
    "/opt/usr/share/libical/zoneinfo/",
    "/usr/share/libical/zoneinfo/",
    "/usr/local/share/libical/zoneinfo/"
};

gint i;
gboolean ret = FALSE;

#define N_SEARCH_PATHS (sizeof (tz_search_paths) / sizeof (tz_search_paths [0]))

    for (i = 0; i < N_SEARCH_PATHS; i++) {
		if (!g_access (tz_search_paths[i], F_OK | R_OK)) {
            set_zone_directory (tz_search_paths[i]);
            ret = TRUE;
			break;
		}
	}

    return ret;
}

/*------------------------------------------------------------------------------*/

gchar* 
ics_read_stream (gchar *s, size_t size, void *d) {   
    return fgets (s, size, (FILE*)d); 
} 

/*------------------------------------------------------------------------------*/

gchar*
str_remove_backslash (gchar *text) {

static gchar tmpbuf[BUFFER_SIZE];
gint i, j;

    i = j = 0;

    while (text[i] != '\0') {
        if (text[i] == '\\' && text[i+1] != 'n') i++;
        tmpbuf[j++] = text[i++];
    }

    tmpbuf[j] = '\0';
    return tmpbuf;
}

/*------------------------------------------------------------------------------*/

void
add_ics_entry_to_day_info_panel (gchar *name, gchar *entry, gchar *entry_desc, gboolean desc_flag, GUI *appGUI) {

gchar tmpbuf[BUFFER_SIZE];

    sprintf(tmpbuf, "%s:\n", name);
    gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1, "bold", NULL);

    sprintf(tmpbuf, "%s\n", str_remove_backslash(entry));
    gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1);

    if (entry_desc != NULL && desc_flag == TRUE) {
        sprintf(tmpbuf, "(%s)\n\n", str_remove_backslash(entry_desc));
        gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1);
    } else {
        gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n", -1);
    }
}

/*------------------------------------------------------------------------------*/

gint
ics_parse_file (struct ics_file *file_entry, gchar *name, gchar *filename, gboolean desc_flag, 
                gboolean count_only, GUI *appGUI) {

FILE *ics_stream;
icalparser *ics_parser;
icalcomponent *ics_component; 
icalcomponent *h, *l; 
icalproperty *p, *r;
struct icaltimetype tt;
struct ics_entry *item;
gint n = 0;

    ics_parser = icalparser_new ();

    if (ics_parser != NULL) {

        ics_stream = fopen(filename, "r");

        if (ics_stream != NULL) {

            icalparser_set_gen_data(ics_parser, ics_stream);

            ics_component = icalparser_parse(ics_parser, ics_read_stream);

            if (ics_component != NULL) {

                if (icalcomponent_get_first_component(ics_component, ICAL_VEVENT_COMPONENT) != NULL) {

                    for (h = icalcomponent_get_first_component(ics_component, ICAL_VEVENT_COMPONENT); h;
                         h = icalcomponent_get_next_component(ics_component, ICAL_VEVENT_COMPONENT)) {
                      
                        p = icalcomponent_get_first_property(h, ICAL_DTSTART_PROPERTY);
                        tt = icaltime_from_string(icalproperty_get_value_as_string(p));

                            p = icalcomponent_get_first_property(h, ICAL_SUMMARY_PROPERTY);
                            if (p != NULL && count_only == FALSE) {
                                r = icalcomponent_get_first_property(h, ICAL_DESCRIPTION_PROPERTY);

                                item = g_malloc(sizeof(struct ics_entry));

                                if (item != NULL) {
 
                                    item->date.day = tt.day;
                                    item->date.month = tt.month;
                                    item->date.year = tt.year;
                                    item->summary = g_strdup(icalproperty_get_value_as_string(p));

                                    if (r != NULL) {
                                        item->description = g_strdup(icalproperty_get_value_as_string(r));
                                    } else {
                                        item->description = NULL;
                                    }

                                    file_entry->entries_list = g_slist_append (file_entry->entries_list, item);
                                }
                            }
                        n++;
                    }

                } else if (icalcomponent_get_first_component(ics_component, ICAL_VCALENDAR_COMPONENT) != NULL) {

                    for (l = icalcomponent_get_first_component(ics_component, ICAL_VCALENDAR_COMPONENT); l;
                         l = icalcomponent_get_next_component(ics_component, ICAL_VCALENDAR_COMPONENT)) {
 
                        h = icalcomponent_get_first_component(l, ICAL_VEVENT_COMPONENT);
                        p = icalcomponent_get_first_property(h, ICAL_DTSTART_PROPERTY);
                        tt = icaltime_from_string(icalproperty_get_value_as_string(p));

                            p = icalcomponent_get_first_property(h, ICAL_SUMMARY_PROPERTY);
                            if (p != NULL && count_only == FALSE) {
                                r = icalcomponent_get_first_property(h, ICAL_DESCRIPTION_PROPERTY);
 
                                item = g_malloc(sizeof(struct ics_entry));

                                if (item != NULL) {
 
                                    item->date.day = tt.day;
                                    item->date.month = tt.month;
                                    item->date.year = tt.year;
                                    item->summary = g_strdup(icalproperty_get_value_as_string(p));

                                    if (r != NULL) {
                                        item->description = g_strdup(icalproperty_get_value_as_string(r));
                                    } else {
                                        item->description = NULL;
                                    }

                                    file_entry->entries_list = g_slist_append (file_entry->entries_list, item);
                                }
                            }
                        n++;
                    }
                }

                icalcomponent_free (ics_component);
            }

            fclose (ics_stream);
        }

        icalparser_free (ics_parser);
    }

    return n;
}

/*------------------------------------------------------------------------------*/

void
ics_check_if_valid (GUI *appGUI) {

gint i;
gchar *ical_filename, *ical_name;
GtkTreeIter iter;
GdkPixbuf *image;
GtkWidget *helper;
const gchar *stock_id;
gboolean status;

    helper = gtk_image_new ();
    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                           ICAL_COLUMN_NAME, &ical_name, ICAL_COLUMN_FILENAME, &ical_filename, -1);

		status = !g_access (ical_filename, F_OK | R_OK);

        if (status == TRUE) {
            if (ics_parse_file (NULL, ical_name, ical_filename, FALSE, TRUE, appGUI) == 0) {
                status = FALSE;
            }
        }

        stock_id = (status == TRUE) ? OSMO_STOCK_LIST_VALID : OSMO_STOCK_LIST_INVALID;
        image = gtk_widget_render_icon (helper, stock_id, GTK_ICON_SIZE_MENU, NULL);
        gtk_list_store_set(appGUI->opt->calendar_ical_files_store, &iter,
                           ICAL_COLUMN_VALID_ICON, image, ICAL_COLUMN_VALID_FLAG, status, -1);

        g_object_unref (image);
        g_free (ical_name);
        g_free (ical_filename);
    }

}

/*------------------------------------------------------------------------------*/

void
calendar_display_ics (DATE *date, GUI *appGUI) {

gchar *ical_name, *ical_filename;
GtkTreeIter iter;
gboolean flag, desc_flag, year_flag;
GSList *node, *node2;
struct ics_file *entry;
struct ics_entry *item;
gint i, j, k;

    for (i = 0, node = appGUI->cal->ics_files_list; node != NULL; node = node->next, i++) {

        entry = g_slist_nth_data (appGUI->cal->ics_files_list, i);

        k = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, k++)) {

            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                               ICAL_COLUMN_NAME, &ical_name, ICAL_COLUMN_FILENAME, &ical_filename, 
                               ICAL_COLUMN_VALID_FLAG, &flag, ICAL_COLUMN_ENABLE_DESC, &desc_flag, 
                               ICAL_COLUMN_USE_YEAR, &year_flag, -1);

            if (!strcmp(entry->filename, ical_filename)) break;

            g_free (ical_name);
            g_free (ical_filename);
        }

        if (ical_filename != NULL) {
            g_free (ical_filename);
        }

        if (entry->entries_list != NULL && flag == TRUE) {
        
            for (j = 0, node2 = entry->entries_list; node2 != NULL; node2 = node2->next, j++) {
 
                item = g_slist_nth_data (entry->entries_list, j);

                if (year_flag == TRUE) {

                    if (date->day == item->date.day && date->month+1 == item->date.month && date->year == item->date.year) {
                        add_ics_entry_to_day_info_panel (ical_name, item->summary, item->description, desc_flag, appGUI);
                        break;
                    }

                } else if (date->day == item->date.day && date->month+1 == item->date.month) {
                    add_ics_entry_to_day_info_panel (ical_name, item->summary, item->description, desc_flag, appGUI);
                    break;
                }
            }
        }

        if (ical_name != NULL) {
            g_free (ical_name);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
ics_calendar_init (GUI *appGUI) {

struct ics_file *ics_file_entry;
gchar *ical_name, *ical_filename;
GtkTreeIter iter;
gint i;
gboolean flag, desc_flag;

    if (ics_initialize_timezone () == FALSE) {
        return;
    }

    appGUI->cal->ics_files_list = NULL;

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                           ICAL_COLUMN_NAME, &ical_name, ICAL_COLUMN_FILENAME, &ical_filename, 
                           ICAL_COLUMN_VALID_FLAG, &flag, ICAL_COLUMN_ENABLE_DESC, &desc_flag, -1);

        if (flag == TRUE) {

            ics_file_entry = g_malloc(sizeof(struct ics_file));

            if (ics_file_entry != NULL) {

                ics_file_entry->filename = g_strdup(ical_filename);
                ics_file_entry->entries_list = NULL;
    
                ics_parse_file (ics_file_entry, ical_name, ical_filename, desc_flag, FALSE, appGUI);

                appGUI->cal->ics_files_list = g_slist_append (appGUI->cal->ics_files_list, ics_file_entry);

            }
        }

        g_free (ical_name);
        g_free (ical_filename);
    }

}

/*------------------------------------------------------------------------------*/

void
ics_calendar_close (GUI *appGUI) {

GSList *node, *node2;
struct ics_file *entry;
struct ics_entry *item;
gint i, j;

    for (i = 0, node = appGUI->cal->ics_files_list; node != NULL; node = node->next, i++) {

        entry = g_slist_nth_data (appGUI->cal->ics_files_list, i);

        if (entry->filename != NULL) {
            g_free(entry->filename);
        }

        if (entry->entries_list != NULL) {
        
            for (j = 0, node2 = entry->entries_list; node2 != NULL; node2 = node2->next, j++) {
 
                item = g_slist_nth_data (entry->entries_list, j);

                if (item->summary != NULL) {
                    g_free(item->summary);
                }
                if (item->description != NULL) {
                    g_free(item->description);
                }

            }

            if (entry->entries_list != NULL) {
                g_slist_free(entry->entries_list);
                entry->entries_list = NULL;
            }

        }
    }

    if (appGUI->cal->ics_files_list != NULL) {
        g_slist_free(appGUI->cal->ics_files_list);
        appGUI->cal->ics_files_list = NULL;
    }
}

/*------------------------------------------------------------------------------*/

void
read_ical_entries (GUI *appGUI) {

xmlDocPtr doc;
xmlChar *key;
xmlNodePtr node, entry_node, main_node;
gboolean desc_flag;
gchar item_name[BUFFER_SIZE], item_filename[BUFFER_SIZE];
GtkTreeIter iter;

    if (g_file_test (prefs_get_config_filename(ICAL_ENTRIES_FILENAME), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(ICAL_ENTRIES_FILENAME)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            xmlFreeDoc(doc);
            return;
        }

        if (xmlStrcmp(node->name, (const xmlChar *) ICAL_NAME)) {
            xmlFreeDoc(doc);
            return;
        }

        main_node = node->xmlChildrenNode;

        while (main_node != NULL) {

            if(!xmlStrcmp(main_node->name, (xmlChar *) "entry")) {

                entry_node = main_node->xmlChildrenNode;

                while (entry_node != NULL) {

                    if ((!xmlStrcmp(entry_node->name, (const xmlChar *) "name"))) {
                        key = xmlNodeListGetString(doc, entry_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            strncpy (item_name, (gchar *) key, BUFFER_SIZE-1);
                            xmlFree(key);
                        }
                    }
                    if ((!xmlStrcmp(entry_node->name, (const xmlChar *) "filename"))) {
                        key = xmlNodeListGetString(doc, entry_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            strncpy (item_filename, (gchar *) key, BUFFER_SIZE-1);
                            xmlFree(key);
                        }
                    }
                    if ((!xmlStrcmp(entry_node->name, (const xmlChar *) "description"))) {
                        key = xmlNodeListGetString(doc, entry_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            desc_flag = atoi((gchar *) key);
                            xmlFree(key);
                        }
                    }
                    if ((!xmlStrcmp(entry_node->name, (const xmlChar *) "use_year"))) {
                        key = xmlNodeListGetString(doc, entry_node->xmlChildrenNode, 1);
                        if (key != NULL) {
                            gtk_list_store_append(appGUI->opt->calendar_ical_files_store, &iter);
                            gtk_list_store_set(appGUI->opt->calendar_ical_files_store, &iter, 
                                               ICAL_COLUMN_NAME, item_name, ICAL_COLUMN_FILENAME, item_filename, 
                                               ICAL_COLUMN_ENABLE_DESC, desc_flag, 
                                               ICAL_COLUMN_USE_YEAR, atoi((gchar *) key), -1); 
                            xmlFree(key);
                        }
                    }

                    entry_node = entry_node->next;
                }
            }
            main_node = main_node->next;
        }
    }

    ics_check_if_valid (appGUI);
    ics_calendar_init (appGUI);
}

/*------------------------------------------------------------------------------*/

void
write_ical_entries (GUI *appGUI) {

xmlDocPtr doc;
xmlNodePtr main_node, entry_node;
xmlAttrPtr attr;
GtkTreeIter iter;
gint i;
gchar *ical_name, *ical_filename;
gboolean desc_flag, year_flag;
gchar tmpbuf[BUFFER_SIZE];

    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    main_node = xmlNewNode(NULL, (const xmlChar *) ICAL_NAME);
    xmlDocSetRootElement(doc, main_node);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                           ICAL_COLUMN_NAME, &ical_name, ICAL_COLUMN_FILENAME, &ical_filename, 
                           ICAL_COLUMN_ENABLE_DESC, &desc_flag, ICAL_COLUMN_USE_YEAR, &year_flag, -1);

        entry_node = xmlNewChild(main_node, NULL, (const xmlChar *) "entry", (xmlChar *) NULL);
        xmlNewChild(entry_node, NULL, (const xmlChar *) "name", (xmlChar *) ical_name);
        g_free(ical_name);
        xmlNewChild(entry_node, NULL, (const xmlChar *) "filename", (xmlChar *) ical_filename);
        g_free(ical_filename);
        sprintf(tmpbuf, "%d", desc_flag);
        xmlNewChild(entry_node, NULL, (const xmlChar *) "description", (xmlChar *) tmpbuf);
        sprintf(tmpbuf, "%d", year_flag);
        xmlNewChild(entry_node, NULL, (const xmlChar *) "use_year", (xmlChar *) tmpbuf);
    }

    xmlSaveFormatFileEnc(prefs_get_config_filename(ICAL_ENTRIES_FILENAME), doc, "utf-8", 1);
    xmlFreeDoc(doc);

    ics_calendar_close (appGUI);
}

/*------------------------------------------------------------------------------*/

void
ics_calendar_refresh (GUI *appGUI) {

    ics_calendar_close (appGUI);
    ics_calendar_init (appGUI);
    set_day_info (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
}

/*------------------------------------------------------------------------------*/

void
ical_events_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_destroy (appGUI->cal->events_window);
}

/*------------------------------------------------------------------------------*/

void
button_ical_events_window_close_cb (GtkButton *button, gpointer user_data) {

    ical_events_window_close_cb (GTK_WIDGET(button), NULL, user_data);

}

/*------------------------------------------------------------------------------*/

gint 
ical_events_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Escape:
            gtk_widget_destroy(appGUI->cal->events_window);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
ical_events_list_create(gint calendar, GUI *appGUI) {

struct ics_file *entry;
struct ics_entry *item;
gint j;
GtkTreeIter iter;
GSList *node;
guint32 julian;
gboolean year_flag;

    gtk_list_store_clear (GTK_LIST_STORE(appGUI->cal->ical_events_list_store));

    j = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, j++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                           ICAL_COLUMN_USE_YEAR, &year_flag, -1);
        if (j-1 == calendar) break;
    }

    entry = g_slist_nth_data (appGUI->cal->ics_files_list, calendar);

    if (entry->entries_list != NULL) {
    
        for (j = 0, node = entry->entries_list; node != NULL; node = node->next, j++) {
            item = g_slist_nth_data (entry->entries_list, j);

            if (year_flag == TRUE) {
                julian = date_to_julian(item->date.day, item->date.month-1, item->date.year);
            } else {
                julian = date_to_julian(item->date.day, item->date.month-1, get_current_year());
            }

            gtk_list_store_append(appGUI->cal->ical_events_list_store, &iter);

            if (year_flag == TRUE) {
                gtk_list_store_set(appGUI->cal->ical_events_list_store, &iter, 
                                   I_COLUMN_DATE, julian_to_str (julian, config.date_format),
                                   I_COLUMN_DATE_JULIAN, julian,
                                   I_COLUMN_SUMMARY, str_remove_backslash (item->summary), -1); 
            } else {
                gtk_list_store_set(appGUI->cal->ical_events_list_store, &iter, 
                                   I_COLUMN_DATE, julian_to_str (julian, DATE_NAME_DAY),
                                   I_COLUMN_DATE_JULIAN, julian,
                                   I_COLUMN_SUMMARY, str_remove_backslash (item->summary), -1); 
            }

            if (get_current_day() == item->date.day && get_current_month()+1 == item->date.month && year_flag == FALSE) {
                gtk_list_store_set(appGUI->cal->ical_events_list_store, &iter, 
                                   I_COLUMN_FONT_WEIGHT, PANGO_WEIGHT_BOLD, -1);
            } else if (get_current_day() == item->date.day && get_current_month()+1 == item->date.month && get_current_year() == item->date.year) {
                gtk_list_store_set(appGUI->cal->ical_events_list_store, &iter, 
                                   I_COLUMN_FONT_WEIGHT, PANGO_WEIGHT_BOLD, -1);
            } else {
                gtk_list_store_set(appGUI->cal->ical_events_list_store, &iter, 
                                   I_COLUMN_FONT_WEIGHT, PANGO_WEIGHT_NORMAL, -1);
            }
        }
    }

}

/*------------------------------------------------------------------------------*/

void
calendar_update_items(gint calendar, GUI *appGUI) {

struct ics_file *entry;
gint j;
GSList *node;
gchar tmpbuf[BUFFER_SIZE];

    entry = g_slist_nth_data (appGUI->cal->ics_files_list, calendar);

    if (entry->entries_list != NULL) {
        for (j = 0, node = entry->entries_list; node != NULL; node = node->next, j++);
    }

    sprintf (tmpbuf, "<i>%4d %s</i>", j, ngettext ("event", "events", j));
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->n_items_label), tmpbuf);
}

/*------------------------------------------------------------------------------*/

void
calendar_selected_cb (GtkComboBox *widget, gpointer user_data) {

gint n;

    GUI *appGUI = (GUI *)user_data;

    n = gtk_combo_box_get_active (GTK_COMBO_BOX (widget));
    calendar_update_items (n, appGUI);
    ical_events_list_create (n, appGUI);

}

/*------------------------------------------------------------------------------*/

void
ical_events_browser (GUI *appGUI) {

GtkWidget           *vbox1;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *close_button;
GtkTreeIter         iter;
GtkTreeViewColumn   *column;
GtkCellRenderer     *renderer;
GtkWidget           *scrolledwindow;
GtkWidget           *table;
GtkWidget           *label;
gchar tmpbuf[BUFFER_SIZE];
gchar *ical_name;
gint i, k;

    i = 0;
    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, i++));

    if (i-1 == 0) {
        gui_create_dialog(GTK_MESSAGE_INFO, _("No calendars defined"), GTK_WINDOW(appGUI->cal->fullyear_window));
        return;
    }

    appGUI->cal->events_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->cal->events_window), _("iCalendar events"));
    gtk_window_set_position (GTK_WINDOW (appGUI->cal->events_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_default_size (GTK_WINDOW(appGUI->cal->events_window), 550, 650);
    gtk_window_set_modal (GTK_WINDOW (appGUI->cal->events_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cal->events_window), "delete_event",
                      G_CALLBACK(ical_events_window_close_cb), appGUI);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->cal->events_window), GTK_WINDOW(appGUI->cal->fullyear_window));
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cal->events_window), 8);
    g_signal_connect (G_OBJECT (appGUI->cal->events_window), "key_press_event",
                      G_CALLBACK (ical_events_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->cal->events_window), vbox1);


    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 6);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox1), table, FALSE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "<b>%s:</b>", _("Calendar"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    appGUI->cal->ical_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->cal->ical_combobox);
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->cal->ical_combobox), FALSE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->cal->ical_combobox, GTK_CAN_FOCUS);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->ical_combobox, 1, 2, 0, 1,
                     (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions) (GTK_FILL), 0, 0);
    g_signal_connect(appGUI->cal->ical_combobox, "changed", 
                     G_CALLBACK(calendar_selected_cb), appGUI);

    k = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, NULL, k++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_ical_files_store), &iter, 
                           ICAL_COLUMN_NAME, &ical_name, -1);

        gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->cal->ical_combobox), ical_name);
        g_free (ical_name);
    }

    appGUI->cal->n_items_label = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->n_items_label);
    gtk_widget_set_size_request (appGUI->cal->n_items_label, 100, -1);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->n_items_label, 3, 4, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->cal->n_items_label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 6);


    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);

    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    appGUI->cal->ical_events_list_store = gtk_list_store_new(ICAL_EVENTS_NUM_COLUMNS, 
                                                             G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

    appGUI->cal->ical_events_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->cal->ical_events_list_store));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->cal->ical_events_list), FALSE);
    gtk_widget_show (appGUI->cal->ical_events_list);
    GTK_WIDGET_SET_FLAGS (appGUI->cal->ical_events_list, GTK_CAN_DEFAULT);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->cal->ical_events_list);

    /* create columns */

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Date"), renderer, 
                                                      "text", I_COLUMN_DATE, 
                                                      "weight", I_COLUMN_FONT_WEIGHT,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->ical_events_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Julian", renderer, 
                                                      "text", I_COLUMN_DATE_JULIAN, 
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->ical_events_list), column);
    gtk_tree_view_column_set_sort_column_id (column, I_COLUMN_DATE_JULIAN);
    g_signal_emit_by_name(column, "clicked");

    renderer = gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    column = gtk_tree_view_column_new_with_attributes(_("Summary"), renderer, 
                                                      "text", I_COLUMN_SUMMARY, 
                                                      "weight", I_COLUMN_FONT_WEIGHT,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->ical_events_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", I_COLUMN_FONT_WEIGHT, NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->cal->ical_events_list), column);

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
    g_signal_connect(close_button, "clicked", G_CALLBACK(button_ical_events_window_close_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), close_button);

    gtk_widget_show(appGUI->cal->events_window);
    gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->cal->ical_combobox), 0);
    gtk_widget_grab_focus (close_button);
}

/*------------------------------------------------------------------------------*/

#endif  /* HAVE_LIBICAL */

