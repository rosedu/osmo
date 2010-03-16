
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
#include <glib/gprintf.h>
#include <glib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "i18n.h"
#include "options_prefs.h"
#include "contacts.h"
#include "contacts_export.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "calendar_notes.h"
#include "calendar_utils.h"


/*------------------------------------------------------------------------------*/

gchar* 
prefs_get_config_filename (gchar *config_filename) {

static gchar dirname[PATH_MAX], filename[PATH_MAX];
struct stat cfg;
gchar separator[2] = { G_DIR_SEPARATOR, '\0' };

    strncpy (dirname, g_get_home_dir(), PATH_MAX-1);
    strcat (dirname, separator);
    strcat (dirname, CONFIG_DIRNAME);

    if(stat (dirname, &cfg) < 0)
        mkdir (dirname, S_IRUSR | S_IWUSR | S_IXUSR);

    strcpy (filename, dirname);
    strcat (filename, separator);
    strcat (filename, config_filename);

    return filename;
}

/*------------------------------------------------------------------------------*/

void 
prefs_set_default_values (void) {

gint i;

    /* general */
    config.window_x = 180;
    config.window_y = 120;
    config.window_size_x = 540;
    config.window_size_y = 690;
    config.enable_tooltips = TRUE;
    config.latest_tab = 0;      /* calendar */
    config.remember_latest_tab = TRUE;
    config.default_stock_icons = FALSE;
    config.disable_underline_links = FALSE;
    config.date_format = DATE_YYYY_MM_DD;
    strncpy(config.web_browser, "firefox %s", MAXHELPERCMD-1);
    strncpy(config.email_client, "claws-mail --compose %s", MAXHELPERCMD-1);

    /* calendar */
    config.fy_window_size_x = 750;
    config.fy_window_size_y = 550;
    config.fy_simple_view = TRUE;
    config.display_options = GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_NO_MONTH_CHANGE;
    config.day_notes_visible = FALSE;
    config.timeline_start = 8*60;
    config.timeline_end = 15*60;
    config.timeline_step = 60;
    config.mark_symbol = 1;
    config.notes_in_info_panel = FALSE;
    config.cursor_type = CURSOR_FRAME;
    config.frame_cursor_thickness = 2;
    strncpy(config.header_color, "#004048", MAXCOLORNAME-1);
    strncpy(config.weekend_color, "#880000", MAXCOLORNAME-1);
    strncpy(config.selection_color, "#526565", MAXCOLORNAME-1);
    strncpy(config.mark_color, "#dddddd", MAXCOLORNAME-1);
    strncpy(config.day_name_font, "Sans Bold 26", MAXFONTNAME-1);
    strncpy(config.calendar_font, "Sans 20", MAXFONTNAME-1);
    strncpy(config.notes_font, "Sans 11", MAXFONTNAME-1);

    /* tasks */
    config.tasks_rules_hint = TRUE;
    config.hide_completed = FALSE;
    config.delete_completed = FALSE;
    config.tasks_pane_pos = 275;
    config.tasks_sorting_order = GTK_SORT_ASCENDING;
    config.tasks_sorting_mode = 0;
    config.visible_due_date_column = TRUE;
    config.visible_priority_column = FALSE;
    config.visible_category_column = FALSE;
    strncpy(config.due_today_color, "#00981E", MAXCOLORNAME-1);
    strncpy(config.due_7days_color, "#0047B7", MAXCOLORNAME-1);
    strncpy(config.past_due_color, "#CB362C", MAXCOLORNAME-1);
    strncpy(config.task_info_font, "Sans 14", MAXFONTNAME-1);

    /* contacts */
    config.contacts_add_edit_window_x = 100;
    config.contacts_add_edit_window_y = 100;
    config.find_mode = CONTACTS_FF_FIRST_NAME;
    config.show_after_search = TRUE;
    config.hide_group_column = FALSE;
    config.contacts_pane_pos = 275;
    config.contacts_rules_hint = TRUE;
    config.photo_width = 80;
    config.name_font_size = 16;
    config.tags_font_size = 11;
    config.contacts_sorting_order = GTK_SORT_ASCENDING;
    config.contacts_sorting_mode = 0;
    strncpy(config.contact_tag_color, "#228B22", MAXCOLORNAME-1);
    strncpy(config.contact_link_color, "blue", MAXCOLORNAME-1);

    config.export_format = EXPORT_TO_CSV;

    for(i=0; i < CONTACTS_NUM_COLUMNS; i++) {
        config.export_fields[i] = '-';
    }
    config.export_fields[0] = '\0';
    config.export_fields[COLUMN_GROUP] = '+';
    config.export_fields[COLUMN_FIRST_NAME] = '+';
    config.export_fields[COLUMN_LAST_NAME] = '+';
    config.export_fields[COLUMN_NICK_NAME] = '+';

}

/*------------------------------------------------------------------------------*/

void 
prefs_read_config (void) {

gboolean cfg_file;
xmlDocPtr doc;
xmlNodePtr node, general_node, calendar_node, tasks_node, contacts_node;
xmlChar *key;

    cfg_file = g_file_test(prefs_get_config_filename(CONFIG_FILENAME), G_FILE_TEST_IS_REGULAR);

    if (cfg_file == TRUE) {

        doc = xmlParseFile(prefs_get_config_filename(CONFIG_FILENAME));
        if (doc == NULL) 
                return;

        node = xmlDocGetRootElement(doc);
        if (node == NULL) {
                xmlFreeDoc(doc);
                return;
        }

        if (xmlStrcmp(node->name, (const xmlChar *) CONFIG_NAME)) {
                xmlFreeDoc(doc);
                return;
        }

        prefs_set_default_values();

        node = node->xmlChildrenNode;

        while (node != NULL) {

            /*---------------------------------------------------------------------------------------*/
            /* general */

                if ((!xmlStrcmp(node->name, (const xmlChar *) "general"))) {

                    general_node = node->xmlChildrenNode;

                    while (general_node != NULL) {

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "window_x"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.window_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "window_y"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.window_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "window_size_x"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.window_size_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "window_size_y"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.window_size_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "enable_tooltips"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.enable_tooltips);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "latest_tab"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.latest_tab);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "remember_latest_tab"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.remember_latest_tab);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "default_stock_icons"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.default_stock_icons);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "disable_underline_links"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.disable_underline_links);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "date_format"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.date_format);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *)"web_browser"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.web_browser, (gchar *) key, MAXHELPERCMD-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *)"email_client"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.email_client, (gchar *) key, MAXHELPERCMD-1);
                            xmlFree(key);
                        }

                        general_node = general_node->next;
                    }

                }

            /*---------------------------------------------------------------------------------------*/
            /* calendar */

                if ((!xmlStrcmp(node->name, (const xmlChar *) "calendar"))) {

                    calendar_node = node->xmlChildrenNode;

                    while (calendar_node != NULL) {

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "fy_window_size_x"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.fy_window_size_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "fy_window_size_y"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.fy_window_size_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "fy_simple_view"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.fy_simple_view);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "display_options"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.display_options);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "day_notes_visible"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.day_notes_visible);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "timeline_start"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.timeline_start);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "timeline_end"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.timeline_end);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "timeline_step"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.timeline_step);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "mark_symbol"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.mark_symbol);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "notes_in_info_panel"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_in_info_panel);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "cursor_type"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.cursor_type);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "frame_cursor_thickness"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.frame_cursor_thickness);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"header_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.header_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"weekend_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.weekend_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"selection_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.selection_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"mark_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.mark_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"day_name_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.day_name_font, (gchar *) key, MAXFONTNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"calendar_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.calendar_font, (gchar *) key, MAXFONTNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"notes_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.notes_font, (gchar *) key, MAXFONTNAME-1);
                            xmlFree(key);
                        }

                        calendar_node = calendar_node->next;
                    }
                }

            /*---------------------------------------------------------------------------------------*/
            /* tasks */

                if ((!xmlStrcmp(node->name, (const xmlChar *) "tasks"))) {

                    tasks_node = node->xmlChildrenNode;

                    while (tasks_node != NULL) {

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "rules_hint"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_rules_hint);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "hide_completed"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_completed);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "delete_completed"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.delete_completed);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "pane_pos"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_pane_pos);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_sorting_order"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_sorting_order);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_sorting_mode"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_sorting_mode);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "visible_due_date_column"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.visible_due_date_column);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "visible_priority_column"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.visible_priority_column);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "visible_category_column"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.visible_category_column);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"due_today_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.due_today_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"due_7days_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.due_7days_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"past_due_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.past_due_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"task_info_font"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.task_info_font, (gchar *) key, MAXFONTNAME-1);
                            xmlFree(key);
                        }

                        tasks_node = tasks_node->next;
                    }
                }

            /*---------------------------------------------------------------------------------------*/
            /* contacts */

                if ((!xmlStrcmp(node->name, (const xmlChar *) "contacts"))) {

                    contacts_node = node->xmlChildrenNode;

                    while (contacts_node != NULL) {

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "add_edit_window_x"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_add_edit_window_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "add_edit_window_y"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_add_edit_window_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "find_mode"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.find_mode);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "show_after_search"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.show_after_search);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "hide_group_column"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_group_column);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "pane_pos"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_pane_pos);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "rules_hint"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_rules_hint);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "photo_width"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.photo_width);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "name_font_size"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.name_font_size);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "tags_font_size"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tags_font_size);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_tag_color"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.contact_tag_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_link_color"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.contact_link_color, (gchar *) key, MAXCOLORNAME-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "export_format"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.export_format);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "export_fields"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    strncpy(config.export_fields, (gchar *) key, MAXCONTACTFIELDS-1);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_sorting_order"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_sorting_order);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_sorting_mode"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_sorting_mode);
                            xmlFree(key);
                        }

                        contacts_node = contacts_node->next;
                    }
                }

            /*---------------------------------------------------------------------------------------*/

            node = node->next;
        }

        xmlFreeDoc(doc);

    } else {

        prefs_set_default_values();
        prefs_write_config ();
    }

}

/*------------------------------------------------------------------------------*/

void prefs_write_config(void) {

xmlDocPtr doc;
xmlNodePtr node, general_node, calendar_node, tasks_node, contacts_node;
gchar tmpbuf[BUFFER_SIZE];

    doc = xmlNewDoc((const xmlChar *) "1.0");
    node = xmlNewNode(NULL, (const xmlChar *) CONFIG_NAME);
    xmlDocSetRootElement(doc, node);

    /*---------------------------------------------------------------------------------------*/
    /* general */

    general_node = xmlNewNode(NULL, (const xmlChar *) "general");
    xmlAddChild(node, general_node);

    snprintf(tmpbuf, 32, "%d", config.window_x);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "window_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.window_y);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "window_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.window_size_x);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "window_size_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.window_size_y);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "window_size_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.enable_tooltips);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "enable_tooltips", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.latest_tab);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "latest_tab", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.remember_latest_tab);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "remember_latest_tab", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.default_stock_icons);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "default_stock_icons", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.disable_underline_links);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "disable_underline_links", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.date_format);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "date_format", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXHELPERCMD, "%s", config.web_browser);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "web_browser", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXHELPERCMD, "%s", config.email_client);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "email_client", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/
    /* calendar */

    calendar_node = xmlNewNode(NULL, (const xmlChar *) "calendar");
    xmlAddChild(node, calendar_node);

    snprintf(tmpbuf, 32, "%d", config.fy_window_size_x);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "fy_window_size_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.fy_window_size_y);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "fy_window_size_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.fy_simple_view);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "fy_simple_view", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.display_options);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "display_options", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.day_notes_visible);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "day_notes_visible", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.timeline_start);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "timeline_start", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.timeline_end);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "timeline_end", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.timeline_step);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "timeline_step", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.mark_symbol);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "mark_symbol", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_in_info_panel);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "notes_in_info_panel", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.cursor_type);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "cursor_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.frame_cursor_thickness);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "frame_cursor_thickness", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.header_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "header_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.weekend_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "weekend_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.selection_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "selection_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.mark_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "mark_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.day_name_font);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "day_name_font", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.calendar_font);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "calendar_font", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.notes_font);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "notes_font", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/
    /* tasks */

    tasks_node = xmlNewNode(NULL, (const xmlChar *) "tasks");
    xmlAddChild(node, tasks_node);

    snprintf(tmpbuf, 32, "%d", config.tasks_rules_hint);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "rules_hint", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_completed);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "hide_completed", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.delete_completed);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "delete_completed", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_pane_pos);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "pane_pos", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_sorting_order);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_sorting_order", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_sorting_mode);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_sorting_mode", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_due_date_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_due_date_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_priority_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_priority_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_category_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_category_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.due_today_color);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "due_today_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.due_7days_color);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "due_7days_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.past_due_color);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "past_due_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.task_info_font);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "task_info_font", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/
    /* contacts */

    contacts_node = xmlNewNode(NULL, (const xmlChar *) "contacts");
    xmlAddChild(node, contacts_node);

    snprintf(tmpbuf, 32, "%d", config.contacts_add_edit_window_x);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "add_edit_window_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_add_edit_window_y);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "add_edit_window_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.find_mode);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "find_mode", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.show_after_search);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "show_after_search", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_group_column);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "hide_group_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_pane_pos);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "pane_pos", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_rules_hint);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "rules_hint", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.photo_width);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "photo_width", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.name_font_size);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "name_font_size", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tags_font_size);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "tags_font_size", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.contact_tag_color);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_tag_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.contact_link_color);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_link_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.export_format);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "export_format", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCONTACTFIELDS, "%s", config.export_fields);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "export_fields", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_sorting_order);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_sorting_order", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_sorting_mode);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_sorting_mode", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/

    xmlSaveFormatFile(prefs_get_config_filename(CONFIG_FILENAME), doc, 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/


