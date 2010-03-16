
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

#ifndef _GUI_H
#define _GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include <libnotify/notify.h>

#include <config.h>

#ifdef HAVE_LIBGRINGOTTS
#include <libgringotts.h>
#endif  /* HAVE_LIBGRINGOTTS */

#define     TRANSLATION_DOMAIN      "osmo"

#define     MAX_MONTHS              12
#define     MAX_WEEKS               6
#define     DAYS_PER_WEEK           7
#define     FULL_YEAR_COLS          37
#define     BUFFER_SIZE             2048

enum notebook_page {
    PAGE_CALENDAR = 0,
    PAGE_TASKS,
    PAGE_CONTACTS,
#ifdef HAVE_LIBGRINGOTTS
    PAGE_NOTES,
#endif  /* HAVE_LIBGRINGOTTS */
    PAGE_OPTIONS,
    PAGE_ABOUT,
    NUMBER_OF_TABS
};

enum {                  /* tasks columns */
    TA_COLUMN_DONE = 0,
    TA_COLUMN_TYPE,
    TA_COLUMN_DUE_DATE,
    TA_COLUMN_DUE_DATE_JULIAN,
    TA_COLUMN_DUE_TIME,
    TA_COLUMN_START_DATE_JULIAN,
    TA_COLUMN_PRIORITY,
    TA_COLUMN_CATEGORY,
    TA_COLUMN_SUMMARY,
    TA_COLUMN_DESCRIPTION,
    TA_COLUMN_COLOR,
    TA_COLUMN_BOLD,
	TA_COLUMN_ACTIVE,
	TA_COLUMN_ACTIVE_WARNING,
	TA_COLUMN_OFFLINE_IGNORE,
	TA_COLUMN_REPEAT,
	TA_COLUMN_REPEAT_DAY,
	TA_COLUMN_REPEAT_MONTH_INTERVAL,
	TA_COLUMN_REPEAT_DAY_INTERVAL,
	TA_COLUMN_REPEAT_START_DAY,
	TA_COLUMN_REPEAT_TIME_START,
	TA_COLUMN_REPEAT_TIME_END,
	TA_COLUMN_REPEAT_TIME_INTERVAL,
	TA_COLUMN_REPEAT_COUNTER,
	TA_COLUMN_ALARM_COMMAND,
	TA_COLUMN_WARNING_DAYS,
	TA_COLUMN_WARNING_TIME,
	TA_COLUMN_ID,
    TASKS_NUM_COLUMNS
};

enum {                  /* contacts columns */
    COLUMN_GROUP = 0, COLUMN_FIRST_NAME, COLUMN_SECOND_NAME, 
    COLUMN_LAST_NAME, COLUMN_NICK_NAME, COLUMN_BIRTH_DAY_DATE, COLUMN_NAME_DAY_DATE,

    COLUMN_HOME_ADDRESS, COLUMN_HOME_POST_CODE, COLUMN_HOME_CITY, COLUMN_HOME_STATE, 
    COLUMN_HOME_COUNTRY,

    COLUMN_WORK_ORGANIZATION, COLUMN_WORK_DEPARTMENT, 
    
    COLUMN_WORK_ADDRESS, COLUMN_WORK_POST_CODE, COLUMN_WORK_CITY, COLUMN_WORK_STATE, 
    COLUMN_WORK_COUNTRY, 
    
    COLUMN_WORK_FAX,
    
    COLUMN_HOME_PHONE_1, COLUMN_HOME_PHONE_2, COLUMN_HOME_PHONE_3, COLUMN_HOME_PHONE_4,
    COLUMN_WORK_PHONE_1, COLUMN_WORK_PHONE_2, COLUMN_WORK_PHONE_3, COLUMN_WORK_PHONE_4,
    COLUMN_CELL_PHONE_1, COLUMN_CELL_PHONE_2, COLUMN_CELL_PHONE_3, COLUMN_CELL_PHONE_4,
    COLUMN_EMAIL_1, COLUMN_EMAIL_2, COLUMN_EMAIL_3, COLUMN_EMAIL_4,
    COLUMN_WWW_1, COLUMN_WWW_2, COLUMN_WWW_3, COLUMN_WWW_4,

    COLUMN_IM_GG, COLUMN_IM_YAHOO, COLUMN_IM_MSN, COLUMN_IM_ICQ, COLUMN_IM_AOL, 
    COLUMN_IM_JABBER, COLUMN_IM_SKYPE, COLUMN_IM_TLEN, COLUMN_BLOG, COLUMN_PHOTO, COLUMN_INFO,
    COLUMN_ID,
    CONTACTS_NUM_COLUMNS
};

typedef struct {

    GtkWidget *calendar;
    gboolean dont_update;

    /* calendar toolbar */
    GtkToolbar *calendar_toolbar;
    GtkBox  *vbox;      
    PangoFontDescription *fd_cal_font;

    /* date label */
    GtkWidget *date_label;
    PangoFontDescription *fd_day_name_font;

    /* current selected date */
    guint day;
    guint month;
    guint year;
    GDate *date;

    /* used for 'jump to' button in date calculator */
    guint julian_jumpto;

    /* day notes */
    GSList *notes_list;
    GtkWidget *notes_button;
    GtkWidget *notes_vbox;
    GtkWidget *note_label;
    GtkWidget *calendar_note_textview;
    GtkWidget *day_info_vbox;
    GtkWidget *day_info_scrolledwindow;
    GtkWidget *time_label;
    GtkWidget *day_number_label;
    GtkWidget *day_number_year_label;
    GtkWidget *week_number_label;
    GtkWidget *moon_icon;
    GtkWidget *moon_phase_label;
    GtkWidget *marked_days_label;
    GtkWidget *weekend_days_label;
    GtkWidget *day_category_label;
    GtkWidget *day_desc_textview;
    GtkTextBuffer *day_desc_text_buffer;
    GtkTextIter day_desc_iter;
    PangoFontDescription *fd_notes_font;

    /* calendar popup menu */
    GtkWidget *popup_menu;
    GtkWidget *popup_menu_select_day_color_entry;

    /* month selector popup menu */
    GtkWidget *month_selector_menu;

    /* auxilary calendars */
    GtkWidget *aux_cal_expander;
    GtkWidget *calendars_table;
    GtkWidget *prev_month_label;
    GtkWidget *next_month_label;
    GtkWidget *calendar_prev;
    GtkWidget *calendar_next;

    /* date selector buttons */
    GtkWidget *prev_day_button;
    GtkWidget *next_day_button;
    GtkWidget *prev_month_button;
    GtkWidget *next_month_button;

    /* day note toolbar */
    GtkWidget *n_select_color_button;
    GtkWidget *n_timeline_button;
    GtkWidget *n_clear_button;
    GtkWidget *n_close_button;

    /* day category color selector */
    GtkWidget *select_bg_color_window;
    GtkListStore *colors_category_store;
    GtkTreeSelection *colors_category_select;

    /* full year calendar */
    GtkWidget *fullyear_window;
    GtkWidget *fycal_table_1;
    GtkWidget *fycal_table_2;
    GtkWidget *fy_spinbutton;
    GtkWidget *cyear_button;
    GtkWidget *fy_calendars[MAX_MONTHS];
    GtkWidget *calendar_buttons[MAX_MONTHS*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))];

    /* jump to date */
    GtkWidget *jumpto_window;
    guint jday;
    guint jmonth;
    guint jyear;
    GtkWidget *day_entry;
    GtkWidget *month_entry;
    GtkWidget *year_entry;

    /* insert timeline */
    GtkWidget *insert_timeline_window;
    GtkWidget *tl_start_h_spinbutton;
    GtkWidget *tl_start_m_spinbutton;
    GtkWidget *tl_end_h_spinbutton;
    GtkWidget *tl_end_m_spinbutton;
    GtkWidget *tl_step_spinbutton;
    GtkWidget *insert_timeline_button;
    GtkWidget *cancel_button;

    /* date calculator */
    GtkWidget *window_date_calculator;
    GtkWidget *spinbutton_start_year;
    GtkWidget *spinbutton_start_month;
    GtkWidget *spinbutton_start_day;
    GtkWidget *spinbutton_start_hour;
    GtkWidget *spinbutton_start_minute;
    GtkWidget *spinbutton_start_second;
    GtkWidget *spinbutton_end_year;
    GtkWidget *spinbutton_end_month;
    GtkWidget *spinbutton_end_day;
    GtkWidget *spinbutton_end_hour;
    GtkWidget *spinbutton_end_minute;
    GtkWidget *spinbutton_end_second;
    GtkWidget *label_result_1;
    GtkWidget *label_result_1_2;
    GtkWidget *label_result_2;
    GtkWidget *label_result_3;
    GtkWidget *spinbutton2_start_year;
    GtkWidget *spinbutton2_start_month;
    GtkWidget *spinbutton2_start_day;
    GtkWidget *spinbutton2_start_hour;
    GtkWidget *spinbutton2_start_minute;
    GtkWidget *spinbutton2_start_second;
    GtkWidget *spinbutton2_end_year;
    GtkWidget *spinbutton2_end_month;
    GtkWidget *spinbutton2_end_day;
    GtkWidget *spinbutton2_end_week;
    GtkWidget *spinbutton2_end_hour;
    GtkWidget *spinbutton2_end_minute;
    GtkWidget *spinbutton2_end_second;
    GtkWidget *label2_result;
    GtkWidget *radiobutton_add;
    
    /* iCalendar */
    GtkWidget *events_window;
    GtkWidget *ical_combobox;
    GtkWidget *n_items_label;
    GSList *ics_files_list;
    GtkWidget *ical_events_list;
    GtkTreeSelection *ical_events_list_selection;
    GtkListStore *ical_events_list_store;

    /* notes browser */
    GtkWidget *day_notes_window;
    GtkWidget *day_notes_list;
    GtkTreeSelection *day_notes_list_selection;
    GtkListStore *day_notes_list_store;
    GtkWidget *past_notes_checkbutton;
    GtkWidget *notes_month_spinbutton;
    GtkWidget *notes_year_spinbutton;
    GtkWidget *notes_filter_combobox;
    GtkWidget *notes_search_entry;
    GtkWidget *notes_search_checkbutton;

} CALENDAR;


typedef struct {

    /* tasks toolbar */
    GtkToolbar *tasks_toolbar;
    GtkBox  *vbox;      

    /* number of items label */
    GtkWidget *n_items_label;

    GtkWidget *desc_textview;
    GtkWidget *tasks_desc_textview;
    GtkWidget *tasks_list;
    GtkTreeSelection *tasks_list_selection; 

    gboolean tasks_panel_status;
    GtkListStore *tasks_list_store;
    GtkWidget *panel_hbox;
    GtkWidget *panel_scrolledwindow;
    GtkTreeModel *tasks_filter;
    GtkTreeModel *tasks_sort;
    GtkWidget *tasks_paned;
    GtkWidget *cf_combobox;
    GtkWidget *priority_combobox;
    GtkWidget *category_combobox;
    GtkTreeViewColumn *tasks_columns[TASKS_NUM_COLUMNS];
    GtkTextTag *font_tag_object;
    GtkCellRenderer *done_renderer;
    gint filter_index;
    gboolean tasks_filter_disabled;
    GtkUIManager *tasks_uim_widget;
    guint id_counter;

    /* tasks items */
    GtkWidget *td_calendar;
    GtkWidget *time_expander;
    GtkWidget *due_date_entry;
    GtkWidget *tasks_add_window;
    GtkWidget *td_calendar_window;
    gboolean tasks_accept_state, tasks_edit_state;
    GtkWidget *tasks_ok_button;
    GtkWidget *summary_entry;
    guint32 tasks_due_julian_day;
    gint tasks_due_time;
    GtkWidget *aw_days_spinbutton;
    GtkWidget *aw_hours_spinbutton;
    GtkWidget *aw_minutes_spinbutton;
    GtkWidget *alarm_cmd_entry;
    GtkWidget *alarm_cmd_valid_image;
    GtkWidget *recurrent_task_vbox;
    GtkWidget *rt_expander;
    GtkWidget *rt_enable_checkbutton;
    GtkWidget *ignore_alarm_checkbutton;
    GtkWidget *rt_start_hour_spinbutton;
    GtkWidget *rt_start_minute_spinbutton;
    GtkWidget *rt_end_hour_spinbutton;
    GtkWidget *rt_end_minute_spinbutton;
    GtkWidget *rt_interval_hour_spinbutton;
    GtkWidget *rt_interval_minute_spinbutton;
    GtkWidget *dp_1_checkbutton;
    GtkWidget *dp_2_checkbutton;
    GtkWidget *dp_3_checkbutton;
    GtkWidget *dp_4_checkbutton;
    GtkWidget *dp_5_checkbutton;
    GtkWidget *dp_6_checkbutton;
    GtkWidget *dp_7_checkbutton;
    GtkWidget *rt_dp_day_spinbutton;
    GtkWidget *rt_dp_month_spinbutton;
    GtkWidget *repeat_counter_spinbutton;
    GSList *notifications;

} TASKS;


typedef struct {

    /* contacts toolbar */
    GtkToolbar *contacts_toolbar;
    GtkBox  *vbox;      

    GtkWidget *add_contact_button;
    GtkWidget *edit_contact_button;
    GtkWidget *delete_contact_button;

    GtkWidget *contacts_find_entry;
    GtkWidget *contacts_find_combobox;

    GtkWidget *contacts_paned;
    GtkWidget *panel_hbox;

    GtkWidget *contacts_list;
    GtkTreeSelection *contacts_list_selection;
    GtkListStore *contacts_list_store;
    GtkTreeModel *contacts_filter;
    GtkTreeModel *contacts_sort;

    GtkWidget *contacts_panel_scrolledwindow;
    GtkWidget *contacts_desc_textview;
    GtkWidget *contacts_www_textview;
    GtkWidget *contacts_email_textview;
    GtkWidget *contacts_blog_textview;
    GtkWidget *photo_image;
    GtkTextTag *contact_name_tag_object;
    GtkTextTag *contact_tags_value_tag_object;
    GtkUIManager *contacts_uim_widget;
    GSList contacts_links_list;
    GSList contacts_www_links_list;
    GSList contacts_email_links_list;
    GSList contacts_blog_links_list;
    gint contacts_link_index;
    gint contacts_www_link_index;
    gint contacts_email_link_index;
    gint contacts_blog_link_index;
    gboolean contacts_panel_status;
    gboolean contacts_filter_disabled;
    GtkWidget *next_field_button[CONTACTS_NUM_COLUMNS];
    GtkTreeViewColumn *contacts_columns[CONTACTS_NUM_COLUMNS];

    gchar **contact_fields_tags_name;

    GtkWidget *contacts_add_window;
    GtkWidget *contacts_ok_button;
    GtkWidget *contacts_browse_button;
    GtkWidget *add_info_textview;
    GtkWidget *photo_browse_button;
    GtkWidget *birth_day_date_button;
    GtkWidget *name_day_date_button;
    GtkWidget *combobox_group;
    GtkWidget *contact_entries[CONTACTS_NUM_COLUMNS];
    GtkWidget *second_name_label;
    GtkWidget *select_date_window;
    GtkWidget *select_date_calendar;
    gboolean contacts_accept_state;
    gboolean contacts_edit_state;
    guint32 birthday_date;
    guint32 nameday_date;

    /* show birthdays */
    GtkWidget *birthdays_window;
    GtkWidget *birthdays_list;
    GtkTreeSelection *birthdays_list_selection;
    GtkListStore *birthdays_list_store;

    /* export contacts */
    GtkWidget *export_window;
    GtkWidget *export_button;
    GtkWidget *format_csv_radiobutton;
    GtkWidget *output_file_entry;
    GtkWidget *first_row_header_check_button;
    GtkWidget *check_buttons[CONTACTS_NUM_COLUMNS];

    /* import contacts */
    GtkWidget *import_window;
    GtkWidget *import_sel_window;
    GtkWidget *import_button;
    GtkWidget *value_labels[CONTACTS_NUM_COLUMNS];
    GtkWidget *field_type_comboboxes[CONTACTS_NUM_COLUMNS];
    GtkWidget *first_row_as_header_check_button;
    GtkWidget *current_record_spinbutton;
    GtkObject *current_record_spinbutton_adj;
    GtkWidget *n_records_label;
    gint field_type[CONTACTS_NUM_COLUMNS];
    guint max_fields;
    gchar *file_buffer;
    guint file_length;

    PangoFontDescription *fd_ai_font;

    GtkWidget *input_file_entry;
    GtkWidget *import_type_combobox;
    GtkWidget *bluetooth_radiobutton;
    GtkWidget *usb_radiobutton;
    GtkWidget *bluetooth_address_entry;
    GtkWidget *file_import_vbox;
    GtkWidget *syncml_import_vbox;
    GtkWidget *bluetooth_params_hbox;
    GtkWidget *usb_params_hbox;
    GtkWidget *bluetooth_channel_spinbutton;
    GtkWidget *usb_interface_spinbutton;
    GtkWidget *use_wbxml_checkbutton;

} CONTACTS;

typedef struct {

    /* notes toolbar */
    GtkToolbar *notes_toolbar_selector;
    GtkToolbar *notes_toolbar_editor;

    GtkBox *vbox;
    GtkWidget *vbox_selector;
    GtkWidget *vbox_editor;

    GtkUIManager *notes_uim_selector_widget;
    GtkUIManager *notes_uim_editor_widget;

    gboolean    editor_active;

    /* selector */
    GtkWidget *notes_list;
    GtkListStore *notes_list_store;
    GtkTreeSelection *notes_list_selection; 

    GtkWidget *add_entry_window;
    GtkWidget *edit_entry_window;
    GtkWidget *enter_password_window;
    GtkWidget *add_entry_ok_button;

    GtkWidget *note_name_entry;
    GtkWidget *password_entry;
    GtkWidget *spassword_entry;
    GtkWidget *category_combobox;

#ifdef HAVE_LIBGRINGOTTS
    gchar *filename;
    GRG_CTX context;
    GRG_KEY keyholder;
#endif  /* HAVE_LIBGRINGOTTS */

    /* category filter */
    GtkTreeModel *notes_filter;
    GtkTreeModel *notes_sort;
    gint filter_index;
    GtkWidget *cf_combobox;
    GtkWidget *n_items_label;

    /* editor */
    PangoFontDescription *fd_notes_font;

    GtkWidget *title_label;
    GtkWidget *editor_textview;
    gboolean changed;
    gboolean find_next_flag;
    gboolean buffer_check_modify_enable;
    GtkWidget *find_hbox;
    GtkWidget *find_entry;
    GtkWidget *find_case_checkbutton;

} NOTES;

typedef struct {

    GtkWidget *calendar_options_radiobutton;
    GtkWidget *tasks_options_radiobutton;
    GtkWidget *contacts_options_radiobutton;
#ifdef HAVE_LIBGRINGOTTS
    GtkWidget *notes_options_radiobutton;
#endif  /* HAVE_LIBGRINGOTTS */
    GtkWidget *general_options_radiobutton;

    gboolean callback_active;
    gint options_counter;
    GtkBox *vbox;

    /* calendar */
    GtkWidget *calendar_vbox;
    GtkWidget *event_marker_type_combobox;
    GtkWidget *today_marker_type_combobox;
    GtkWidget *header_color_picker;
    GtkWidget *weekend_color_picker;
    GtkWidget *selection_color_picker;
    GtkWidget *mark_color_picker;
    GtkWidget *mark_current_day_color_picker;
    GtkWidget *cft_hscale;
    GtkWidget *cft_label_1, *cft_label_2, *cft_label_3;
    GtkWidget *show_day_names_checkbutton;
    GtkWidget *no_month_change_checkbutton;
    GtkWidget *show_week_numbers_checkbutton;
    GtkWidget *week_start_monday_checkbutton;
    GtkWidget *simple_view_in_fy_calendar_checkbutton;
    GtkWidget *enable_auxilary_calendars_checkbutton;
    GtkWidget *strikethrough_past_notes_checkbutton;
    GtkWidget *ascending_sorting_in_day_notes_checkbutton;
    GtkWidget *enable_block_cursor_checkbutton;
    GtkWidget *day_marker_entry;
    GtkWidget *day_name_font_entry;
    GtkWidget *calendar_font_entry;
    GtkWidget *notes_font_entry;
    GtkWidget *calendar_category_entry;
    GtkListStore *calendar_category_store;
    GtkWidget *calendar_category_treeview;
    GtkTreeSelection *calendar_category_select;
    GtkWidget *calendar_category_add_button;
    GtkWidget *calendar_category_edit_button;
    GtkWidget *calendar_category_remove_button;
    GtkWidget *day_category_color_picker;
    GtkWidget *color_edit_window;
    GtkWidget *color_edit_picker;
    GtkWidget *color_edit_name_entry;
    GtkWidget *color_edit_ok_button;
    GtkWidget *ical_edit_window;
    GtkWidget *ical_edit_name_entry;
    GtkWidget *ical_edit_filename_entry;
    GtkWidget *ical_edit_filename_browse_button;
    GtkWidget *ical_edit_ok_button;
    GtkWidget *calendar_ical_files_add_button;
    GtkWidget *calendar_ical_files_edit_button;
    GtkWidget *calendar_ical_files_remove_button;
    GtkWidget *calendar_ical_files_browse_button;
    GtkWidget *calendar_ical_files_name_entry;
    GtkWidget *calendar_ical_files_filename_entry;
    GtkListStore *calendar_ical_files_store;
    GtkWidget *calendar_ical_files_treeview;
    GtkTreeSelection *calendar_ical_files_select;
    GtkWidget *di_show_current_time_checkbutton;
    GtkWidget *di_show_day_number_checkbutton;
    GtkWidget *di_show_current_day_distance_checkbutton;
    GtkWidget *di_show_marked_days_checkbutton;
    GtkWidget *di_show_week_number_checkbutton;
    GtkWidget *di_show_weekend_days_checkbutton;
    GtkWidget *di_show_day_category_checkbutton;
    GtkWidget *di_show_moon_phase_checkbutton;
    GtkWidget *di_show_notes_checkbutton;
    GtkWidget *di_show_zodiac_sign_checkbutton;

    /* tasks */
    GtkWidget *tasks_vbox;
    GtkWidget *ti_font_entry;
    GtkWidget *due_today_color_picker;
    GtkWidget *due_7days_color_picker;
    GtkWidget *past_due_color_picker;
    GtkWidget *tasks_category_entry;
    GtkListStore *tasks_category_store;
    GtkWidget *tasks_category_treeview;
    GtkTreeSelection *tasks_category_select;
    GtkWidget *tasks_category_add_button;
    GtkWidget *tasks_category_remove_button;
    GtkWidget *tasks_sort_order_combobox;
    GtkWidget *tasks_sort_mode_combobox;
    GtkWidget *ct_bold_items_checkbutton;
    GtkWidget *ct_delete_items_checkbutton;
    GtkWidget *ct_hide_items_checkbutton;
    GtkWidget *ct_add_item_checkbutton;
    GtkWidget *tasks_enable_rules_hint_checkbutton;
    GtkWidget *vc_due_date_checkbutton;
    GtkWidget *vc_type_checkbutton;
    GtkWidget *vc_category_checkbutton;
    GtkWidget *vc_priority_checkbutton;
    GtkWidget *postpone_time_spinbutton;

    /* contacts */
    GtkWidget *contacts_vbox;
    GtkWidget *contacts_enable_rules_hint_checkbutton;
    GtkWidget *contacts_select_first_entry_checkbutton;
    GtkWidget *contacts_hide_group_column_checkbutton;
    GtkWidget *contacts_group_entry;
    GtkListStore *contacts_group_store;
    GtkWidget *contacts_group_treeview;
    GtkWidget *cn_font_entry;
    GtkWidget *ci_font_entry;
    GtkTreeSelection *contacts_group_select;
    GtkWidget *contacts_group_add_button;
    GtkWidget *contacts_group_remove_button;
    GtkWidget *contacts_sort_order_combobox;
    GtkWidget *contacts_sort_mode_combobox;
    GtkWidget *contacts_photo_size_combobox;
    GtkWidget *contact_tag_color_picker;
    GtkWidget *contact_link_color_picker;
    GtkWidget *name_font_size_spinbutton;
    GtkWidget *tags_font_size_spinbutton;

    /* notes */
    GtkWidget *notes_vbox;
    GtkWidget *editor_font_entry;
    GtkWidget *notes_enc_algorithm_combobox;
    GtkWidget *notes_enc_hashing_combobox;
    GtkWidget *notes_comp_algorithm_combobox;
    GtkWidget *notes_comp_ratio_combobox;
    GtkWidget *notes_enable_rules_hint_checkbutton;
    GtkWidget *notes_category_entry;
    GtkListStore *notes_category_store;
    GtkWidget *notes_category_treeview;
    GtkTreeSelection *notes_category_select;
    GtkWidget *notes_sort_order_combobox;
    GtkWidget *notes_sort_mode_combobox;
    GtkWidget *notes_category_add_button;
    GtkWidget *notes_category_remove_button;

    /* general */
    GtkWidget *general_vbox;
    GtkWidget *tabs_position_combobox;
    GtkWidget *date_format_combobox;
    GtkWidget *time_format_combobox;
    GtkWidget *entry_web_browser;
    GtkWidget *entry_email_client;
    GtkWidget *enable_tooltips_checkbutton;
    GtkWidget *disable_underline_in_links_checkbutton;
    GtkWidget *default_stock_icons_checkbutton;
    GtkWidget *remember_latest_tab_checkbutton;
    GtkWidget *enable_systray_checkbutton;
    GtkWidget *start_minimised_checkbutton;
    GtkWidget *hide_calendar_checkbutton;
    GtkWidget *hide_tasks_checkbutton;
    GtkWidget *hide_contacts_checkbutton;
    GtkWidget *hide_notes_checkbutton;

} OPTIONS;


typedef struct {

    /* gui */
    GtkWidget *main_window;
    GtkWidget *notebook;
    gboolean all_pages_added;
    gboolean window_visible;
    gint current_tab;
    gint number_of_tabs;

    /* tooltips */
    GtkTooltips *osmo_tooltips; 

    /* tray icon */
    GtkStatusIcon *osmo_trayicon;
    GtkWidget *trayicon_popup_menu;
    gboolean no_tray;

    /* command line parameters */
    gboolean calendar_only;
    gboolean check_events;
    gint check_ndays_events;
    gchar *config_path;
    
    /* links handling */
    gboolean hovering_over_link;
    GdkCursor *hand_cursor;
    GdkCursor *regular_cursor;
    GtkTextTag *gui_url_tag;

    /* check events */
    GtkWidget *event_checker_window;
    GtkWidget *event_checker_list;
    GtkTreeSelection *event_checker_list_selection;
    GtkListStore *event_checker_list_store;

    /* about page */
    GtkWidget *about_radiobutton;
    GtkWidget *license_radiobutton;
    GtkWidget *help_radiobutton;
    GtkWidget *scrolled_window;
    GtkWidget *about_textview;
    GtkBox *about_vbox;
    gint about_counter;
    gint about_link_index;
    GSList about_links_list;
    GtkTextIter about_entry_iter;
    GtkTextBuffer *about_entry_buffer;

	/* miscellaneous */
	guint32 run_date;
	gint key_counter;
	gint run_time;

    /* modules */
    CALENDAR *cal;
    TASKS *tsk;
    CONTACTS *cnt;
    NOTES *nte;

    OPTIONS *opt;

} GUI;

typedef struct {
    gpointer *data;
    GUI *appGUI;
} MESSAGE;

gboolean    gui_create_window               (GUI *appGUI);
void        gui_add_to_notebook             (GtkWidget *widget, gchar *text, GUI *appGUI);
void        gui_create_about                (GUI *appGUI);
void        gui_save_all_data               (GUI *appGUI);
void        gui_save_data_and_run_command   (gchar *command, GUI *appGUI);
void        gui_quit_osmo                   (gboolean ignore_geometry, GUI *appGUI);
void        gui_systray_initialize          (GUI *appGUI);
void        gui_systray_tooltip_update      (GUI *appGUI);
void        about_switch_buttons            (gboolean left, GUI *appGUI);
void        about_set_first_page            (GUI *appGUI);

#endif /* _GUI_H */

