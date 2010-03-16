
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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "i18n.h"
#include "gui_utils.h"
#include "options_prefs.h"
#include "calendar.h"
#include "calendar_notes.h"

/*------------------------------------------------------------------------------*/

gboolean
check_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year)
                return TRUE;
        }

    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gchar *
get_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

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

gchar *
get_color (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {

                if (a->color != NULL) {
                    return a->color;
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
replace_note_color (gchar *old_color, gchar *new_color, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->color != NULL) {
                if (!strncmp(a->color, old_color, MAXCOLORNAME)) {
                    g_free(a->color);
                    a->color = g_strdup(new_color);
                }
            }

        }
    }
}

/*------------------------------------------------------------------------------*/

void
add_note (guint day, guint month, guint year, gchar *color_str, gchar *text_note, GUI *appGUI) {

struct note *a;

    if (text_note != NULL) {

        a = g_malloc(sizeof(struct note));
        g_assert (a != NULL);

        a->day = day;
        a->month = month;
        a->year = year;

        if (color_str != NULL) {
            a->color = g_strdup(color_str);
        } else {
            a->color = NULL;
        }

        a->note = g_strdup(text_note);

        remove_note (day, month, year, appGUI);     /* FIXME: check STACK here! */

        appGUI->cal->notes_list = g_slist_append (appGUI->cal->notes_list, a);
    }
}

/*------------------------------------------------------------------------------*/

void
remove_note (guint day, guint month, guint year, GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;

    if (appGUI->cal->notes_list != NULL) {

        for (i = 0, lnode = appGUI->cal->notes_list; lnode != NULL; lnode = lnode->next, i++) {

            a = g_slist_nth_data (appGUI->cal->notes_list, i);

            if (a->day == day && a->month == month && a->year == year) {
                if (a->color != NULL) {
                    g_free(a->color);
                }
                if (a->note != NULL) {
                    g_free(a->note);
                }
                appGUI->cal->notes_list = g_slist_remove (appGUI->cal->notes_list, a);
                break;
            }
        }
    }
}

/*------------------------------------------------------------------------------*/

void
free_notes_list (GUI *appGUI) {

GSList *node;
struct note *a;
gint i = 0;

    for (node = appGUI->cal->notes_list; node != NULL; node = node->next, i++) {
        a = g_slist_nth_data (appGUI->cal->notes_list, i);
        if (a->color != NULL) {
            g_free(a->color);
        }
        if (a->note != NULL) {
            g_free(a->note);
        }
    }

    if (appGUI->cal->notes_list != NULL) {
        g_slist_free(appGUI->cal->notes_list);
        appGUI->cal->notes_list = NULL;
    }
}

/*------------------------------------------------------------------------------*/

void
read_notes (GUI *appGUI) {

xmlDocPtr doc;
xmlChar *key, *prop;
xmlNodePtr node, cnode, category_node;
GtkTreeIter iter;
GdkPixbuf *image;
guint day, month, year;
gchar *color_str;

    appGUI->cal->notes_list = NULL;

    if (g_file_test (prefs_get_config_filename(CALENDAR_NOTES_FILENAME), G_FILE_TEST_IS_REGULAR) == FALSE) 
        return;

    if((doc = xmlParseFile(prefs_get_config_filename(CALENDAR_NOTES_FILENAME)))) {

        if(!(node = xmlDocGetRootElement(doc))) {
            return;
        }

        if(!xmlStrcmp(node->name, (xmlChar *) CALENDAR_NOTES_NAME)) {

            /* read note */
            node = node->xmlChildrenNode;

            while (node != NULL) {

                if(!xmlStrcmp(node->name, (xmlChar *) CALENDAR_DAY_CATEGORIES_NAME)) {

                    category_node = node->xmlChildrenNode;

                    while (category_node != NULL) {

                        if ((!xmlStrcmp(category_node->name, (const xmlChar *) "name"))) {
                            key = xmlNodeListGetString(doc, category_node->xmlChildrenNode, 1);
                            prop = xmlGetProp(category_node, (const xmlChar *) "color");
                            if (key != NULL) {
                                image = gui_create_color_swatch ((gchar *) prop);
                                gtk_list_store_append(appGUI->opt->calendar_category_store, &iter);
                                gtk_list_store_set(appGUI->opt->calendar_category_store, &iter, 0, image, 
                                                   1, prop, 2, (gchar *) key, -1);
                                g_object_unref (image);
                            }
                            xmlFree(key);
                        }
                        category_node = category_node->next;
                    }
                }

                if(!xmlStrcmp(node->name, (xmlChar *) "note")) {

                    cnode = node->xmlChildrenNode;

                    day = month = year = 0;
                    color_str = NULL;

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
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "color"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL)
                                    color_str = g_strdup((gchar *) key);
                            xmlFree(key);
                        }
                        if ((!xmlStrcmp(cnode->name, (const xmlChar *) "message"))) {
                            key = xmlNodeListGetString(doc, cnode->xmlChildrenNode, 1);
                            if (key != NULL) {
                                add_note (day, month, year, color_str, (gchar *) key, appGUI);
                                if (color_str != NULL) {
                                    g_free(color_str);
                                }
                            }
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
write_notes (GUI *appGUI) {

GSList *lnode;
struct note *a;
gint i;
xmlDocPtr doc;
xmlNodePtr main_node, node, note_node, dc_node;
xmlAttrPtr attr;
gchar *category, *color_str;
GtkTreeIter iter;
gchar temp[BUFFER_SIZE];


    doc = xmlNewDoc((const xmlChar *) "1.0");
    attr = xmlNewDocProp (doc, (const xmlChar *) "encoding", (const xmlChar *) "utf-8");

    main_node = xmlNewNode(NULL, (const xmlChar *) CALENDAR_NOTES_NAME);
    xmlDocSetRootElement(doc, main_node);

    node = xmlNewChild(main_node, NULL, (const xmlChar *) CALENDAR_DAY_CATEGORIES_NAME, (xmlChar *) NULL);

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, 1, &color_str, 2, &category, -1);
        dc_node = xmlNewChild(node, NULL, (const xmlChar *) "name", (xmlChar *) category);
        g_free(category);
        xmlNewProp(dc_node, (const xmlChar *) "color", (xmlChar *) color_str);
        g_free(color_str);
    }

    for (i = 0, lnode = appGUI->cal->notes_list; lnode; lnode = lnode->next, i++) {

        a = g_slist_nth_data (appGUI->cal->notes_list, i);
        note_node = xmlNewChild(main_node, NULL, (const xmlChar *) "note", (xmlChar *) NULL);
        sprintf(temp, "%d", a->day);
        xmlNewChild(note_node, NULL, (const xmlChar *) "day", (xmlChar *) temp);
        sprintf(temp, "%d", a->month);
        xmlNewChild(note_node, NULL, (const xmlChar *) "month", (xmlChar *) temp);
        sprintf(temp, "%d", a->year);
        xmlNewChild(note_node, NULL, (const xmlChar *) "year", (xmlChar *) temp);
        xmlNewChild(note_node, NULL, (const xmlChar *) "color", (xmlChar *) a->color);
        xmlNewChild(note_node, NULL, (const xmlChar *) "message", (xmlChar *) a->note);
    }

    xmlSaveFormatFileEnc(prefs_get_config_filename(CALENDAR_NOTES_FILENAME), doc, "utf-8", 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/

