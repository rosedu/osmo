
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

#include "options_prefs.h"
#include "i18n.h"
#include "contacts.h"
#include "contacts_import.h"
#include "contacts_export.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "calendar_notes.h"
#include "calendar_utils.h"

/*------------------------------------------------------------------------------*/

gchar* 
prefs_get_config_filename (gchar *config_filename, GUI *appGUI) {

static gchar dirname[PATH_MAX], filename[PATH_MAX];
struct stat cfg;

    if (appGUI->config_path == NULL) {
#if defined(CONFIG_PATH) && defined(CONFIG_DIR)
        g_snprintf(dirname, PATH_MAX, "%s%c%s", CONFIG_PATH, G_DIR_SEPARATOR, CONFIG_DIR);
#elif defined(CONFIG_DIR)
        g_snprintf(dirname, PATH_MAX, "%s%c%s", g_get_home_dir(), G_DIR_SEPARATOR, CONFIG_DIR);
#elif defined(CONFIG_PATH)
        g_snprintf(dirname, PATH_MAX, "%s%c%s", CONFIG_PATH, G_DIR_SEPARATOR, CONFIG_DIRNAME);
#else
        g_snprintf(dirname, PATH_MAX, "%s%c%s", g_get_home_dir(), G_DIR_SEPARATOR, CONFIG_DIRNAME);
#endif
    } else {
        g_strlcpy (dirname, appGUI->config_path, PATH_MAX);
    }

    if(g_stat (dirname, &cfg) < 0)
        g_mkdir (dirname, S_IRUSR | S_IWUSR | S_IXUSR);

    if (g_access (dirname, R_OK | W_OK) == -1) {
        return NULL;
    }

    g_snprintf(filename, PATH_MAX, "%s%c%s", dirname, G_DIR_SEPARATOR, config_filename);

    return filename;
}

/*------------------------------------------------------------------------------*/

void 
prefs_set_default_values (void) {

gint i;

    /* general */
    config.window_x = 180;
    config.window_y = 80;
    config.window_size_x = 540;
    config.window_size_y = 690;
    config.enable_tooltips = TRUE;
    config.latest_tab = 0;      /* calendar */
    config.tabs_position = GTK_POS_TOP;
    config.remember_latest_tab = TRUE;
    config.default_stock_icons = FALSE;
    config.disable_underline_links = FALSE;
    config.date_format = DATE_YYYY_MM_DD;
    config.time_format = TIME_24;
    config.enable_systray = TRUE;
    config.start_minimised_in_systray = FALSE;
    config.run_counter = 0;
    config.lastrun_date = get_julian_for_today();
    config.lastrun_time = get_seconds_for_today();
    config.hide_calendar = FALSE;
    config.hide_tasks = FALSE;
    config.hide_contacts = FALSE;
    config.hide_notes = FALSE;
	g_strlcpy (config.web_browser, "firefox %s", MAXHELPERCMD);
	g_strlcpy (config.email_client, "claws-mail --compose %s", MAXHELPERCMD);

    /* calendar */
    config.fy_window_size_x = 750;
    config.fy_window_size_y = 550;
    config.fy_simple_view = TRUE;
    config.fy_alternative_view = FALSE;
    config.display_options = GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_NO_MONTH_CHANGE;
    config.day_notes_visible = FALSE;
    config.timeline_start = 8*60;
    config.timeline_end = 15*60;
    config.timeline_step = 60;
    config.di_show_current_time = TRUE;
    config.di_show_day_number = TRUE;
    config.di_show_current_day_distance = TRUE;
    config.di_show_marked_days = TRUE;
    config.di_show_week_number = TRUE;
    config.di_show_weekend_days = TRUE;
    config.di_show_day_category = TRUE;
    config.di_show_moon_phase = TRUE;
    config.di_show_notes = TRUE;
    config.di_show_zodiac_sign = FALSE;
    config.cursor_type = CURSOR_FRAME;
    config.frame_cursor_thickness = 2;
    config.enable_auxilary_calendars = TRUE;
    config.enable_day_mark = TRUE;
    config.strikethrough_past_notes = FALSE;
    config.ascending_sorting_in_day_notes_browser = FALSE;
    config.auxilary_calendars_state = FALSE;
    config.day_note_marker = '\'';
    config.event_marker_type = EVENT_MARKER_CIRCLE;
    config.today_marker_type = TODAY_MARKER_FREEHAND_CIRCLE;
    config.day_notes_browser_filter = DN_FILTER_CURRENT_MONTH;
	g_strlcpy (config.header_color, "#808080", MAXCOLORNAME);
	g_strlcpy (config.weekend_color, "#880000", MAXCOLORNAME);
	g_strlcpy (config.selection_color, "#526565", MAXCOLORNAME);
	g_strlcpy (config.mark_color, "#dddddd", MAXCOLORNAME);
	g_strlcpy (config.mark_current_day_color, "#34A434", MAXCOLORNAME);
    config.mark_current_day_alpha = 32768;
    config.selector_alpha = 32768;
	g_strlcpy (config.day_name_font, "Sans Bold 20", MAXFONTNAME);
	g_strlcpy (config.calendar_font, "Sans 18", MAXFONTNAME);
	g_strlcpy (config.notes_font, "Sans 11", MAXFONTNAME);

    /* tasks */
    config.tasks_rules_hint = TRUE;
    config.tasks_high_in_bold = TRUE;
    config.hide_completed = FALSE;
    config.delete_completed = FALSE;
    config.add_edit = FALSE;
    config.tasks_pane_pos = 275;
    config.tasks_sorting_order = GTK_SORT_ASCENDING;
    config.tasks_sorting_mode = 0;
    config.visible_due_date_column = TRUE;
    config.visible_type_column = TRUE;
    config.visible_priority_column = FALSE;
    config.visible_category_column = FALSE;
    config.tasks_addedit_win_w = 370;
    config.tasks_addedit_win_h = 440;
    config.tasks_addedit_win_x = config.window_x-config.tasks_addedit_win_w/2;
    config.tasks_addedit_win_y = config.window_y+20;
    config.postpone_time = 10;
	g_strlcpy (config.due_today_color, "#00981E", MAXCOLORNAME);
	g_strlcpy (config.due_7days_color, "#0047B7", MAXCOLORNAME);
	g_strlcpy (config.past_due_color, "#CB362C", MAXCOLORNAME);
	g_strlcpy (config.task_info_font, "Sans 14", MAXFONTNAME);

    /* contacts */
    config.find_mode = CONTACTS_FF_FIRST_NAME;
    config.show_after_search = TRUE;
    config.hide_group_column = FALSE;
    config.contacts_pane_pos = 275;
    config.contacts_rules_hint = TRUE;
    config.photo_width = 80;
    config.contacts_sorting_order = GTK_SORT_ASCENDING;
    config.contacts_sorting_mode = 0;
    config.contacts_addedit_win_w = 500;
    config.contacts_addedit_win_h = 600;
    config.contacts_addedit_win_x = config.window_x-config.contacts_addedit_win_w/2;
    config.contacts_addedit_win_y = config.window_y+20;
    config.contacts_export_win_w = 500;
    config.contacts_export_win_h = 400;
    config.contacts_export_win_x = config.window_x-config.contacts_export_win_w/2;
    config.contacts_export_win_y = config.window_y+20;
    config.contacts_import_win_w = 650;
    config.contacts_import_win_h = 450;
    config.contacts_import_win_x = config.window_x-config.contacts_import_win_w/2;
    config.contacts_import_win_y = config.window_y+20;
    config.contacts_import_sel_win_x = config.window_x+60;
    config.contacts_import_sel_win_y = config.window_y+20;
    config.import_type = IMPORT_TYPE_FILE;
    config.import_interface_type = 0;
    config.import_bluetooth_channel = 1;
    config.import_usb_interface = 0;
    config.import_binary_xml = 0;
	g_strlcpy (config.import_bluetooth_address, "00:00:00:00:00:00", MAXADDRESS);
	g_strlcpy (config.contact_tag_color, "#228B22", MAXCOLORNAME);
	g_strlcpy (config.contact_link_color, "blue", MAXCOLORNAME);
	g_strlcpy (config.contact_name_font, "Sans 18", MAXFONTNAME);
	g_strlcpy (config.contact_item_font, "Sans 11", MAXFONTNAME);

    config.export_format = EXPORT_TO_CSV;

    for(i=0; i < CONTACTS_NUM_COLUMNS; i++) {
        config.export_fields[i] = '-';
    }
    config.export_fields[0] = '\0';
    config.export_fields[COLUMN_GROUP] = '+';
    config.export_fields[COLUMN_FIRST_NAME] = '+';
    config.export_fields[COLUMN_LAST_NAME] = '+';
    config.export_fields[COLUMN_NICK_NAME] = '+';

    /* notes */
    config.notes_rules_hint = TRUE;
    config.notes_enc_algorithm = 1;     /* Serpent */
    config.notes_enc_hashing = 1;       /* RIPEMD-160 */
    config.notes_comp_algorithm = 0;    /* ZLib */
    config.notes_comp_ratio = 3;        /* BEST */
    config.notes_sorting_order = GTK_SORT_ASCENDING;
    config.notes_sorting_mode = 0;
	g_strlcpy (config.notes_editor_font, "Sans 11", MAXFONTNAME);
}

/*------------------------------------------------------------------------------*/

void 
prefs_read_config (GUI *appGUI) {

gboolean cfg_file;
xmlDocPtr doc;
xmlNodePtr node, general_node, calendar_node, tasks_node, contacts_node, notes_node;
xmlChar *key;

    cfg_file = g_file_test(prefs_get_config_filename(CONFIG_FILENAME, appGUI), G_FILE_TEST_IS_REGULAR);

    if (cfg_file == TRUE) {

        doc = xmlParseFile(prefs_get_config_filename(CONFIG_FILENAME, appGUI));
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

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "tabs_position"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tabs_position);
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

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "time_format"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.time_format);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "enable_systray"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.enable_systray);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "start_minimised_in_systray"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.start_minimised_in_systray);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "run_counter"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.run_counter);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "lastrun_date"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.lastrun_date);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "lastrun_time"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.lastrun_time);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "hide_calendar"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_calendar);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "hide_tasks"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_tasks);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "hide_contacts"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_contacts);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *) "hide_notes"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.hide_notes);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *)"web_browser"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.web_browser, (gchar *) key, MAXHELPERCMD);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(general_node->name, (const xmlChar *)"email_client"))) {
                            key = xmlNodeListGetString(doc, general_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.email_client, (gchar *) key, MAXHELPERCMD);
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

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "fy_alternative_view"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.fy_alternative_view);
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

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_current_time"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_current_time);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_day_number"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_day_number);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_current_day_distance"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_current_day_distance);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_marked_days"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_marked_days);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_week_number"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_week_number);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_weekend_days"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_weekend_days);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_day_category"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_day_category);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_moon_phase"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_moon_phase);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_notes"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_notes);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "di_show_zodiac_sign"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.di_show_zodiac_sign);
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

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "enable_auxilary_calendars"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.enable_auxilary_calendars);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "enable_day_mark"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.enable_day_mark);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "strikethrough_past_notes"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.strikethrough_past_notes);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "ascending_sorting_in_day_notes_browser"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.ascending_sorting_in_day_notes_browser);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "auxilary_calendars_state"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.auxilary_calendars_state);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "day_note_marker"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    config.day_note_marker = key[0];
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "event_marker_type"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.event_marker_type);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "today_marker_type"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.today_marker_type);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "day_notes_browser_filter"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.day_notes_browser_filter);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"header_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.header_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"weekend_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.weekend_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"selection_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.selection_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"mark_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.mark_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"mark_current_day_color"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.mark_current_day_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "mark_current_day_alpha"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.mark_current_day_alpha);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *) "selector_alpha"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.selector_alpha);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"day_name_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.day_name_font, (gchar *) key, MAXFONTNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"calendar_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.calendar_font, (gchar *) key, MAXFONTNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(calendar_node->name, (const xmlChar *)"notes_font"))) {
                            key = xmlNodeListGetString(doc, calendar_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.notes_font, (gchar *) key, MAXFONTNAME);
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

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "high_priority_in_bold"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_high_in_bold);
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

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "add_edit"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.add_edit);
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

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "visible_type_column"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.visible_type_column);
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

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_addedit_win_x"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_addedit_win_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_addedit_win_y"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_addedit_win_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_addedit_win_w"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_addedit_win_w);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "tasks_addedit_win_h"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.tasks_addedit_win_h);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *) "postpone_time"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.postpone_time);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"due_today_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.due_today_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"due_7days_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.due_7days_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"past_due_color"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.past_due_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(tasks_node->name, (const xmlChar *)"task_info_font"))) {
                            key = xmlNodeListGetString(doc, tasks_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.task_info_font, (gchar *) key, MAXFONTNAME);
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

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_tag_color"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.contact_tag_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_link_color"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.contact_link_color, (gchar *) key, MAXCOLORNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_name_font"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.contact_name_font, (gchar *) key, MAXFONTNAME);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *)"contact_item_font"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.contact_item_font, (gchar *) key, MAXFONTNAME);
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
                                    g_strlcpy (config.export_fields, (gchar *) key, MAXCONTACTFIELDS);
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

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_addedit_win_x"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_addedit_win_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_addedit_win_y"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_addedit_win_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_addedit_win_w"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_addedit_win_w);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_addedit_win_h"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_addedit_win_h);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_export_win_x"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_export_win_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_export_win_y"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_export_win_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_export_win_w"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_export_win_w);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_export_win_h"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_export_win_h);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_sel_win_x"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_sel_win_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_sel_win_y"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_sel_win_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_win_x"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_win_x);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_win_y"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_win_y);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_win_w"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_win_w);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "contacts_import_win_h"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.contacts_import_win_h);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_type"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.import_type);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_interface_type"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.import_interface_type);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_bluetooth_channel"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.import_bluetooth_channel);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_usb_interface"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.import_usb_interface);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_binary_xml"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.import_binary_xml);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(contacts_node->name, (const xmlChar *) "import_bluetooth_address"))) {
                            key = xmlNodeListGetString(doc, contacts_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.import_bluetooth_address, (gchar *) key, MAXADDRESS);
                            xmlFree(key);
                        }

                        contacts_node = contacts_node->next;
                    }
                }

            /*---------------------------------------------------------------------------------------*/
            /* notes */

                if ((!xmlStrcmp(node->name, (const xmlChar *) "notes"))) {

                    notes_node = node->xmlChildrenNode;

                    while (notes_node != NULL) {

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "rules_hint"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_rules_hint);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "enc_algorithm"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_enc_algorithm);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "enc_hashing"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_enc_hashing);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "comp_algorithm"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_comp_algorithm);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "comp_ratio"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_comp_ratio);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "sorting_order"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_sorting_order);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *) "sorting_mode"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    sscanf((gchar *) key, "%d", &config.notes_sorting_mode);
                            xmlFree(key);
                        }

                        if ((!xmlStrcmp(notes_node->name, (const xmlChar *)"editor_font"))) {
                            key = xmlNodeListGetString(doc, notes_node->xmlChildrenNode, 1);
                            if (key != NULL)
                                    g_strlcpy (config.notes_editor_font, (gchar *) key, MAXFONTNAME);
                            xmlFree(key);
                        }

                        notes_node = notes_node->next;
                    }
                }

            /*---------------------------------------------------------------------------------------*/

            node = node->next;
        }

        xmlFreeDoc(doc);

    } else {

        prefs_set_default_values();
        prefs_write_config (appGUI);
    }

}

/*------------------------------------------------------------------------------*/

void 
prefs_write_config(GUI *appGUI) {

xmlDocPtr doc;
xmlNodePtr node, general_node, calendar_node, tasks_node, contacts_node, notes_node;
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
    snprintf(tmpbuf, 32, "%d", config.tabs_position);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "tabs_position", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.remember_latest_tab);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "remember_latest_tab", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.default_stock_icons);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "default_stock_icons", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.disable_underline_links);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "disable_underline_links", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.date_format);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "date_format", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.time_format);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "time_format", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.enable_systray);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "enable_systray", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.start_minimised_in_systray);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "start_minimised_in_systray", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.run_counter);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "run_counter", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.lastrun_date);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "lastrun_date", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.lastrun_time);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "lastrun_time", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_calendar);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "hide_calendar", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_tasks);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "hide_tasks", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_contacts);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "hide_contacts", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_notes);
    xmlNewTextChild(general_node, NULL, (const xmlChar *) "hide_notes", (xmlChar *) tmpbuf);
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
    snprintf(tmpbuf, 32, "%d", config.fy_alternative_view);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "fy_alternative_view", (xmlChar *) tmpbuf);
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
    snprintf(tmpbuf, 32, "%d", config.di_show_current_time);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_current_time", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_day_number);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_day_number", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_current_day_distance);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_current_day_distance", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_marked_days);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_marked_days", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_week_number);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_week_number", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_weekend_days);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_weekend_days", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_day_category);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_day_category", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_moon_phase);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_moon_phase", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_notes);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_notes", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.di_show_zodiac_sign);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "di_show_zodiac_sign", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.cursor_type);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "cursor_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.frame_cursor_thickness);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "frame_cursor_thickness", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.enable_auxilary_calendars);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "enable_auxilary_calendars", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.enable_day_mark);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "enable_day_mark", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.strikethrough_past_notes);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "strikethrough_past_notes", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.ascending_sorting_in_day_notes_browser);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "ascending_sorting_in_day_notes_browser", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.auxilary_calendars_state);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "auxilary_calendars_state", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%c", config.day_note_marker);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "day_note_marker", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.event_marker_type);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "event_marker_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.today_marker_type);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "today_marker_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.day_notes_browser_filter);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "day_notes_browser_filter", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.header_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "header_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.weekend_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "weekend_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.selection_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "selection_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.selector_alpha);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "selector_alpha", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.mark_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "mark_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.mark_current_day_color);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "mark_current_day_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.mark_current_day_alpha);
    xmlNewTextChild(calendar_node, NULL, (const xmlChar *) "mark_current_day_alpha", (xmlChar *) tmpbuf);
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
    snprintf(tmpbuf, 32, "%d", config.tasks_high_in_bold);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "high_priority_in_bold", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.hide_completed);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "hide_completed", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.delete_completed);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "delete_completed", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.add_edit);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "add_edit", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_pane_pos);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "pane_pos", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_sorting_order);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_sorting_order", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_sorting_mode);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_sorting_mode", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_due_date_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_due_date_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_type_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_type_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_priority_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_priority_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.visible_category_column);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "visible_category_column", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_addedit_win_x);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_addedit_win_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_addedit_win_y);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_addedit_win_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_addedit_win_w);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_addedit_win_w", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.tasks_addedit_win_h);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "tasks_addedit_win_h", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.postpone_time);
    xmlNewTextChild(tasks_node, NULL, (const xmlChar *) "postpone_time", (xmlChar *) tmpbuf);
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
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.contact_tag_color);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_tag_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCOLORNAME, "%s", config.contact_link_color);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_link_color", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.contact_name_font);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_name_font", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.contact_item_font);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contact_item_font", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.export_format);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "export_format", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXCONTACTFIELDS, "%s", config.export_fields);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "export_fields", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_sorting_order);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_sorting_order", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_sorting_mode);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_sorting_mode", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_addedit_win_x);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_addedit_win_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_addedit_win_y);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_addedit_win_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_addedit_win_w);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_addedit_win_w", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_addedit_win_h);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_addedit_win_h", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_export_win_x);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_export_win_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_export_win_y);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_export_win_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_export_win_w);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_export_win_w", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_export_win_h);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_export_win_h", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_sel_win_x);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_sel_win_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_sel_win_y);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_sel_win_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_win_x);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_win_x", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_win_y);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_win_y", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_win_w);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_win_w", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.contacts_import_win_h);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "contacts_import_win_h", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.import_type);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.import_interface_type);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_interface_type", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.import_bluetooth_channel);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_bluetooth_channel", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.import_usb_interface);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_usb_interface", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.import_binary_xml);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_binary_xml", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXADDRESS, "%s", config.import_bluetooth_address);
    xmlNewTextChild(contacts_node, NULL, (const xmlChar *) "import_bluetooth_address", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/
    /* notes */

    notes_node = xmlNewNode(NULL, (const xmlChar *) "notes");
    xmlAddChild(node, notes_node);

    snprintf(tmpbuf, 32, "%d", config.notes_rules_hint);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "rules_hint", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_enc_algorithm);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "enc_algorithm", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_enc_hashing);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "enc_hashing", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_comp_algorithm);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "comp_algorithm", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_comp_ratio);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "comp_ratio", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_sorting_order);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "sorting_order", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, 32, "%d", config.notes_sorting_mode);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "sorting_mode", (xmlChar *) tmpbuf);
    snprintf(tmpbuf, MAXFONTNAME, "%s", config.notes_editor_font);
    xmlNewTextChild(notes_node, NULL, (const xmlChar *) "editor_font", (xmlChar *) tmpbuf);

    /*---------------------------------------------------------------------------------------*/

    xmlSaveFormatFile(prefs_get_config_filename(CONFIG_FILENAME, appGUI), doc, 1);
    xmlFreeDoc(doc);
}

/*------------------------------------------------------------------------------*/


