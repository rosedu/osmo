
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
#include <glib.h>
#include <glib/gprintf.h>
#include <time.h>
#include <ctype.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "i18n.h"
#include "gui.h"
#include "options_prefs.h"
#include "calendar.h"
#include "calendar_notes.h"


extern GtkWidget    *main_window;
extern gchar        tmpbuf[];

GSList              *notes_list;

/*------------------------------------------------------------------------------*/

gboolean
check_note (guint day, guint month, guint year) {

GSList *lnode;
struct note *a;
gint i;

    if (notes_list != NULL) {

        for (i = 0, lnode = notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (notes_list, i);

            if (a->day == day && a->month == month && a->year == year)
                return TRUE;
        }

    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gchar *
get_note (guint day, guint month, guint year) {

GSList *lnode;
struct note *a;
gint i;

    if (notes_list != NULL) {

        for (i = 0, lnode = notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (notes_list, i);

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

void
add_note (guint day, guint month, guint year, gchar *text_note) {

gchar *text_buffer;
gint len;
struct note *a;

    if (text_note != NULL) {

        len = strlen(text_note);

        if (len) {

            remove_note (day, month, year);

            a = g_malloc(sizeof(struct note));

            if (a != NULL) {

                a->day = day;
                a->month = month;
                a->year = year;

                text_buffer = g_malloc((len+1)*sizeof(gchar));

                if (text_buffer != NULL) {
                    strncpy(text_buffer, text_note, len);
                    text_buffer[len] = '\0';
                    a->note = text_buffer;

                    notes_list = g_slist_append (notes_list, a);

                } else {
                    g_fprintf(stderr, "*** ERROR: cannot allocate memory for text buffer\n");
                }

            } else {
                g_fprintf(stderr, "*** ERROR: cannot allocate memory for note structure\n");
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
remove_note (guint day, guint month, guint year) {

GSList *lnode;
struct note *a;
gint i;

    if (notes_list != NULL) {

        for (i = 0, lnode = notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {
                if (a->note != NULL) {
                    g_free(a->note);
                }
                notes_list = g_slist_remove (notes_list, a);
                break;
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
free_notes_list (void) {

GSList *node;
struct note *a;
gint i = 0;

    for (node = notes_list; node != NULL; node = node->next, i++) {
        a = g_slist_nth_data (notes_list, i);
        if (a->note != NULL) {
            g_free(a->note);
        }
    }

    if (notes_list != NULL) {
        g_slist_free(notes_list);
        notes_list = NULL;
    }
}

/*------------------------------------------------------------------------------*/

void
read_notes (void) {

xmlDocPtr doc;
xmlChar *key;
xmlNodePtr node, cnode;
guint day, month, year;

    notes_list = NULL;

    if (g_file_test (prefs_get_config_filename(NOTES_FILENAME), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(NOTES_FILENAME)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            return;
        }

        if(!xmlStrcmp(node->name, (xmlChar *) NOTES_NAME)) {

            /* read note */
            node = node->xmlChildrenNode;

            while (node != NULL) {

                if(!xmlStrcmp(node->name, (xmlChar *) "note")) {

                    cnode = node->xmlChildrenNode;

                    day = month = year = 0;

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
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "message"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                add_note (day, month, year, (gchar *) key);
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
write_notes (void) {

GSList *lnode;
struct note *a;
gint i;
xmlDocPtr doc;
xmlNodePtr node, note_node;
xmlAttrPtr attr;
gchar temp[BUFFER_SIZE];


    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    node = xmlNewNode(NULL, (const xmlChar *) NOTES_NAME);
    xmlDocSetRootElement(doc, node);

    for (i = 0, lnode = notes_list; lnode; lnode = lnode->next, i++) {

        a = g_slist_nth_data (notes_list, i);
        note_node = xmlNewChild(node, NULL, (const xmlChar *) "note", (xmlChar *) NULL);
        sprintf(temp, "%d", a->day);
        xmlNewChild(note_node, NULL, (const xmlChar *) "day", (xmlChar *) temp);
        sprintf(temp, "%d", a->month);
        xmlNewChild(note_node, NULL, (const xmlChar *) "month", (xmlChar *) temp);
        sprintf(temp, "%d", a->year);
        xmlNewChild(note_node, NULL, (const xmlChar *) "year", (xmlChar *) temp);
        xmlNewChild(note_node, NULL, (const xmlChar *) "message", (xmlChar *) a->note);
    }

    free_notes_list();

    xmlSaveFormatFileEnc(prefs_get_config_filename(NOTES_FILENAME), doc, "utf-8", 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/

