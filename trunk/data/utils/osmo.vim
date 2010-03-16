" Vim syntax file
" Language: C osmo extension (for version 0.2.6)
" Maintainer: Piotr Mąka (Sill) <silloz@users.sourceforge.net>
" Last Change: 2008-12-13
" Options:
"    Deprecated declarations are not highlighted by default.
"    let osmo_enable_deprecated = 1
"       highlights deprecated declarations too (like normal declarations)
"    let osmo_deprecated_errors = 1
"       highlights deprecated declarations as Errors

syn keyword osmoFunction utl_date_get_julian_today utl_get_current_day utl_get_current_month utl_get_current_year utl_get_day_name utl_get_julian_day_name utl_get_date_name utl_get_weekend_days_in_month utl_get_weekend_days_in_month_my utl_get_days_per_year utl_gdate_to_dmy utl_julian_to_dmy utl_subtract_from_date utl_dmy_to_julian utl_calc_moon_phase utl_get_moon_phase_name utl_xml_get_int utl_xml_get_uint utl_xml_get_char utl_xml_get_str utl_xml_get_strn utl_xml_put_int utl_xml_put_uint utl_xml_put_char utl_xml_put_str utl_xml_put_strn utl_name_strcat utl_run_helper utl_run_command utl_is_valid_command utl_get_link_type utl_cairo_set_color utl_cairo_draw utl_draw_rounded_rectangle utl_draw_left_arrow
syn keyword osmoFunction gui_scrolled_window_move_position gui_calendar_get_type gui_calendar_new gui_calendar_select_month gui_calendar_select_day gui_calendar_mark_day gui_calendar_tc_mark_day gui_calendar_mark_day_color gui_calendar_unmark_day gui_calendar_tc_unmark_day gui_calendar_clear_marks gui_calendar_tc_clear_marks gui_calendar_set_display_options gui_calendar_get_gdate gui_calendar_set_header_color gui_calendar_set_weekend_color gui_calendar_set_selector_color gui_calendar_set_selector_alpha gui_calendar_set_event_marker_color gui_calendar_set_today_marker_color gui_calendar_set_today_marker_alpha gui_calendar_enable_cursor gui_calendar_set_cursor_type gui_calendar_set_frame_cursor_thickness gui_calendar_set_day_note_marker_type gui_calendar_set_event_marker_type gui_calendar_set_today_marker_type gui_create_calendar
syn keyword osmoFunction cal_jump_to_date calendar_set_today calendar_update_date calendar_update_note calendar_clear_text_cb enable_disable_note_buttons get_marker_symbol calendar_get_note_text calendar_create_color_selector_window calendar_create_popup_menu cal_set_day_info mark_events cal_refresh_marks update_clock update_aux_calendars calendar_store_note calendar_btn_prev_day calendar_btn_next_day calendar_btn_prev_week calendar_btn_next_week calendar_btn_prev_month calendar_btn_next_month calendar_btn_prev_year calendar_btn_next_year calendar_create_calc_window calendar_create_fullyear_window
syn keyword osmoFunction calendar_display_ics ics_initialize_timezone ics_check_if_valid ics_calendar_refresh ical_events_browser ical_export read_ical_entries write_ical_entries calendar_create_jumpto_window cal_read_notes cal_write_notes cal_free_notes_list cal_add_note cal_get_note cal_remove_note cal_check_note cal_get_note_color cal_replace_note_color cal_notes_browser cal_note_remove_empty_lines calendar_create_insert_timeline_window get_seconds_for_today time_to_seconds seconds_to_time get_tm_struct current_time_to_str time_to_str get_current_hour get_current_minute get_current_second is_date_in_the_past calculate_date_time_diff get_day_of_week current_date_text calendar_get_date_name julian_to_str str_to_julian julian_to_year month_name_to_number get_current_date_distance_str get_date_time_str get_date_time_full_str parse_numeric_date utl_get_zodiac_name get_chinese_year_name free_notifications_list time_handler
syn keyword osmoFunction check_contacts check_tasks_contacts create_event_checker_window gui_create_contacts show_contacts_desc_panel set_export_active contacts_select_first_position_in_list insert_photo contacts_selection_activate contacts_create_birthdays_window read_contacts_entries write_contacts_entries contacts_remove_dialog_show contacts_add_edit_dialog_show import_contacts_from_syncml csv_get_field csv_get_line get_number_of_records export_contacts_to_file contacts_create_export_window import_contacts_show_dialog import_contacts_from_csv_file add_csv_records notes_show_selector_editor gui_create_notes update_notes_items notes_add_entry notes_edit_dialog_show notes_remove_dialog_show notes_enter_password notes_cleanup_files notes_get_filename read_notes_entries write_notes_entries
syn keyword osmoFunction gui_create_window gui_add_to_notebook gui_create_about gui_save_all_data gui_save_data_and_run_command gui_quit_osmo gui_systray_initialize gui_systray_update_icon gui_systray_tooltip_update about_switch_buttons about_set_first_page gui_create_options options_switch_buttons gui_create_calendar_options_page calendar_cursor_settings_enable_disable get_visible_tabs gui_create_general_options_page get_enc_algorithm_value get_enc_hashing_value get_comp_algorithm_value get_comp_ratio_value gui_create_notes_options_page gui_create_contacts_options_page gui_create_tasks_options_page gui_create_tasks update_tasks_number task_calculate_new_date tasks_repeat_done show_tasks_desc_panel tasks_select_first_position_in_list apply_task_attributes refresh_tasks add_item_to_list get_date_color tasks_selection_activate read_tasks_entries write_tasks_entries tasks_remove_dialog_show tasks_add_edit_dialog_show tasks_print osmo_register_stock_icons prefs_get_config_filename prefs_read_config prefs_write_config calendar_create_print_window store_task_columns_info

" tasks_utils.[ch]
syn keyword osmoFunction tsk_set_next_cycle tsk_set_prev_cycle tsk_set_current_cycle tsk_get_next_cycle tsk_get_prev_cycle tsk_get_tasks_num tsk_get_tasks_str tsk_get_category_state tsk_get_priority_text tsk_get_priority_index tsk_get_iter tsk_get_item tsk_item_free
syn keyword osmoConstant STATE_NONE STATE_CALENDAR STATE_TASKS TC_COLUMN_NAME TC_COLUMN_CALENDAR TC_COLUMN_TASKS

" utils_date.[ch]
syn keyword osmoFunction utl_date_get_current_julian utl_date_get_days_in_month utl_date_set_nearest_weekday

" utils_date_time.[ch]
syn keyword osmoFunction utl_date_time_compare

" utils_gui.[ch]
syn keyword osmoFunction utl_gui_font_select_cb utl_gui_get_sw_vscrollbar_width utl_gui_sw_vscrollbar_move_position utl_gui_get_column_position utl_gui_get_combobox_items utl_gui_create_category_combobox utl_gui_text_buffer_get_text_with_tags utl_gui_text_buffer_set_text_with_tags utl_gui_clear_text_buffer utl_gui_change_bg_widget_state utl_gui_create_color_swatch utl_gui_fill_iconlabel utl_gui_update_command_status utl_gui_create_dialog utl_gui_check_overwrite_file utl_gui_list_store_get_text_index utl_gui_stock_button utl_gui_stock_name_button utl_gui_stock_label_radio_button utl_gui_image_label_radio_button utl_gui_url_initialize utl_gui_url_setup utl_gui_url_insert_link utl_gui_url_remove_links 

" utils_time.[ch]
syn keyword osmoFunction utl_time_new utl_time_new_hms utl_time_new_seconds utl_time_new_now utl_time_set_hms utl_time_set_hour utl_time_set_minute utl_time_set_second utl_time_set_seconds utl_time_get_hms utl_time_get_hour utl_time_get_minute utl_time_get_second utl_time_get_seconds utl_time_get_current_seconds utl_time_add utl_time_add_hours utl_time_add_minutes utl_time_add_seconds utl_time_subtract utl_time_subtract_hours utl_time_subtract_minutes utl_time_subtract_seconds utl_time_clamp utl_time_compare utl_time_seconds_between utl_time_print utl_time_valid utl_time_valid_hms utl_time_valid_hour utl_time_valid_minute utl_time_valid_second utl_time_valid_seconds utl_time_free

syn keyword osmoTypedef GuiCalendar GuiCalendarClass GuiCalendarPrivate GuiCalendarDisplayOptions TASK_NTF OsmoSyncMLData CALENDAR TASKS CONTACTS NOTES OPTIONS GUI MESSAGE TASK_ITEM
syn keyword osmoConstant TAG_CHAR UNKNOWN EMAIL WWW SW_MOVE_UP SW_MOVE_DOWN CURSOR_BLOCK CURSOR_FRAME EVENT_MARKER_CIRCLE EVENT_MARKER_ELLIPSE EVENT_MARKER_WAVE TODAY_MARKER_ARROW TODAY_MARKER_FREEHAND_CIRCLE GUI_CALENDAR_SHOW_HEADING GUI_CALENDAR_SHOW_DAY_NAMES GUI_CALENDAR_NO_MONTH_CHANGE GUI_CALENDAR_SHOW_WEEK_NUMBERS GUI_CALENDAR_WEEK_START_MONDAY I_COLUMN_DATE I_COLUMN_DATE_JULIAN I_COLUMN_SUMMARY I_COLUMN_FONT_WEIGHT ICAL_EVENTS_NUM_COLUMNS IE_COLUMN_DATE IE_COLUMN_DATE_JULIAN IE_COLUMN_SUMMARY IE_COLUMN_DESCRIPTION ICAL_EXPORT_NUM_COLUMNS DN_FILTER_CURRENT_MONTH DN_FILTER_SELECTED_MONTH DN_FILTER_CURRENT_YEAR DN_FILTER_SELECTED_YEAR DN_FILTER_SELECTED_MONTH_YEAR DN_FILTER_ALL_NOTES DN_COLUMN_DATE DN_COLUMN_DATE_JULIAN DN_COLUMN_NOTE_LINE DN_COLUMN_DONE DAY_NOTES_NUM_COLUMNS DATE_DD_MM_YYYY DATE_MM_DD_YYYY DATE_YYYY_MM_DD DATE_YYYY_DD_MM DATE_NAME_DAY DATE_DAY_OF_WEEK_NAME DATE_FULL TIME_HH_MM TIME_HH_MM_AMPM TIME_HH_MM_SS TIME_HH_MM_SS_AMPM TIME_TIMEZONE TIME_24 TIME_12 CE_COLUMN_DATE CE_COLUMN_DATE_JULIAN CE_COLUMN_EVENT_TYPE CE_COLUMN_EVENT_LINE CHECK_EVENTS_NUM_COLUMNS CONTACTS_FF_FIRST_NAME CONTACTS_FF_LAST_NAME CONTACTS_FF_ALL_FIELDS B_COLUMN_NAME B_COLUMN_DAYS_NUM B_COLUMN_DAYS B_COLUMN_AGE B_COLUMN_DATE B_COLUMN_ZODIAC B_COLUMN_ID BIRTHDAYS_NUM_COLUMNS EXPORT_TO_CSV EXPORT_TO_XHTML IMPORT_TYPE_FILE IMPORT_TYPE_SYNCML SYNCML_BLUETOOTH SYNCML_USB SELECT_ALL SELECT_NONE SELECT_INVERT N_COLUMN_NAME N_COLUMN_CATEGORY N_COLUMN_LAST_CHANGES_DATE N_COLUMN_LAST_CHANGES_DATE_JULIAN N_COLUMN_LAST_CHANGES_TIME N_COLUMN_CREATE_DATE N_COLUMN_CREATE_DATE_JULIAN N_COLUMN_CREATE_TIME N_COLUMN_EDITOR_LINE N_COLUMN_FILENAME NOTES_NUM_COLUMNS SELECTOR EDITOR PAGE_CALENDAR PAGE_TASKS PAGE_CONTACTS PAGE_NOTES PAGE_OPTIONS PAGE_ABOUT NUMBER_OF_TABS
syn keyword osmoConstant TA_COLUMN_DONE TA_COLUMN_TYPE TA_COLUMN_DUE_DATE TA_COLUMN_DUE_DATE_JULIAN TA_COLUMN_DUE_TIME TA_COLUMN_START_DATE_JULIAN TA_COLUMN_DONE_DATE_JULIAN TA_COLUMN_PRIORITY TA_COLUMN_CATEGORY TA_COLUMN_SUMMARY TA_COLUMN_DESCRIPTION TA_COLUMN_COLOR TA_COLUMN_BOLD TA_COLUMN_ACTIVE TA_COLUMN_ACTIVE_WARNING TA_COLUMN_OFFLINE_IGNORE TA_COLUMN_REPEAT TA_COLUMN_REPEAT_DAY TA_COLUMN_REPEAT_MONTH_INTERVAL TA_COLUMN_REPEAT_DAY_INTERVAL TA_COLUMN_REPEAT_START_DAY TA_COLUMN_REPEAT_TIME_START TA_COLUMN_REPEAT_TIME_END TA_COLUMN_REPEAT_TIME_INTERVAL TA_COLUMN_REPEAT_COUNTER TA_COLUMN_ALARM_COMMAND TA_COLUMN_WARNING_DAYS TA_COLUMN_WARNING_TIME TA_COLUMN_POSTPONE_TIME TA_COLUMN_ID TASKS_NUM_COLUMNS COLUMN_GROUP COLUMN_FIRST_NAME COLUMN_SECOND_NAME COLUMN_LAST_NAME COLUMN_NICK_NAME COLUMN_BIRTH_DAY_DATE COLUMN_NAME_DAY_DATE COLUMN_HOME_ADDRESS COLUMN_HOME_POST_CODE COLUMN_HOME_CITY COLUMN_HOME_STATE COLUMN_HOME_COUNTRY COLUMN_WORK_ORGANIZATION COLUMN_WORK_DEPARTMENT COLUMN_WORK_ADDRESS COLUMN_WORK_POST_CODE COLUMN_WORK_CITY COLUMN_WORK_STATE COLUMN_WORK_COUNTRY COLUMN_WORK_FAX COLUMN_HOME_PHONE_1 COLUMN_HOME_PHONE_2 COLUMN_HOME_PHONE_3 COLUMN_HOME_PHONE_4 COLUMN_WORK_PHONE_1 COLUMN_WORK_PHONE_2 COLUMN_WORK_PHONE_3 COLUMN_WORK_PHONE_4 COLUMN_CELL_PHONE_1 COLUMN_CELL_PHONE_2 COLUMN_CELL_PHONE_3 COLUMN_CELL_PHONE_4 COLUMN_EMAIL_1 COLUMN_EMAIL_2 COLUMN_EMAIL_3 COLUMN_EMAIL_4 COLUMN_WWW_1 COLUMN_WWW_2 COLUMN_WWW_3 COLUMN_WWW_4 COLUMN_IM_GG COLUMN_IM_YAHOO COLUMN_IM_MSN COLUMN_IM_ICQ COLUMN_IM_AOL COLUMN_IM_JABBER COLUMN_IM_SKYPE COLUMN_IM_TLEN COLUMN_BLOG COLUMN_PHOTO COLUMN_INFO COLUMN_ID CONTACTS_NUM_COLUMNS WRT_CALENDAR_NOTES WRT_TASKS WRT_CONTACTS WRT_NOTES ICAL_COLUMN_VALID_ICON ICAL_COLUMN_NAME ICAL_COLUMN_FILENAME ICAL_COLUMN_VALID_FLAG ICAL_COLUMN_ENABLE_DESC ICAL_COLUMN_USE_YEAR OPT_PAGE_CALENDAR OPT_PAGE_TASKS OPT_PAGE_CONTACTS OPT_PAGE_NOTES OPT_PAGE_GENERAL OPT_NUMBER_OF_PAGES PORTRAIT LANDSCAPE
syn keyword osmoStruct _GuiCalendar _GuiCalendarClass _GuiCalendarPrivate ics_entry ics_file note TIME osmo_prefs
"syn keyword osmoMacro
syn keyword osmoEnum helpers notebook_page
syn keyword osmoVariable appGUI
syn keyword osmoDefine COLOR_BG_OK COLOR_BG_FAIL CALENDAR_NOTES_NAME CALENDAR_DAY_CATEGORIES_NAME CALENDAR_NOTES_FILENAME JULIAN_GREGORIAN_YEAR CONTACTS_NAME CONTACTS_ENTRIES_NAME CONTACTS_GROUP_ENTRIES_NAME CONTACTS_ENTRIES_FILENAME PHOTO_SMALL PHOTO_MEDIUM PHOTO_LARGE MAX_LINE_LENGTH MAX_FIELD_LENGTH FIELD_SEPARATOR NOTES_NAME NOTES_ENTRIES_NAME NOTES_CATEGORY_ENTRIES_NAME NOTES_DIRNAME NOTES_ENTRIES_FILENAME TRANSLATION_DOMAIN MAX_MONTHS MAX_WEEKS DAYS_PER_WEEK FULL_YEAR_COLS OPAQUE BUFFER_SIZE TASKS_NAME TASKS_ENTRIES_NAME TASKS_CATEGORY_ENTRIES_NAME TASKS_ENTRIES_FILENAME MAX_SUMMARY_SIZE MAX_DATE_SIZE WHOLE_WEEK CONFIG_DIRNAME CONFIG_FILENAME CONFIG_NAME MAXNAME MAXFONTNAME MAXCOLORNAME MAXHELPERCMD MAXCONTACTFIELDS MAXADDRESS OSMO_STOCK_BUTTON_ADD OSMO_STOCK_BUTTON_CLEAR OSMO_STOCK_BUTTON_CLOSE OSMO_STOCK_BUTTON_CANCEL OSMO_STOCK_BUTTON_JUMPTO OSMO_STOCK_BUTTON_CONTACTS_EXPORT OSMO_STOCK_BUTTON_CONTACTS_IMPORT OSMO_STOCK_BUTTON_EDIT OSMO_STOCK_BUTTON_DOWN OSMO_STOCK_BUTTON_INFO OSMO_STOCK_BUTTON_INSERT_TIMELINE OSMO_STOCK_BUTTON_NEXT_YEAR OSMO_STOCK_BUTTON_NO OSMO_STOCK_BUTTON_OK OSMO_STOCK_BUTTON_OPEN OSMO_STOCK_BUTTON_PREV_YEAR OSMO_STOCK_BUTTON_REMOVE OSMO_STOCK_BUTTON_SELECT_COLOR OSMO_STOCK_BUTTON_SELECT_DATE OSMO_STOCK_BUTTON_SELECT_FONT OSMO_STOCK_BUTTON_TODAY OSMO_STOCK_BUTTON_YES OSMO_STOCK_SYSTRAY_BIRTHDAY OSMO_STOCK_SYSTRAY_NORMAL OSMO_STOCK_SYSTRAY_TASK OSMO_STOCK_ABOUT OSMO_STOCK_CALENDAR OSMO_STOCK_CLOSE OSMO_STOCK_CALCULATOR OSMO_STOCK_CONTACTS_ADD OSMO_STOCK_CONTACTS_BIRTHDAYS OSMO_STOCK_CONTACTS_EDIT OSMO_STOCK_CONTACTS_EXPORT OSMO_STOCK_CONTACTS_IMPORT OSMO_STOCK_CONTACTS_REMOVE OSMO_STOCK_CONTACTS OSMO_STOCK_EDIT_NOTE OSMO_STOCK_EDITOR_BOLD OSMO_STOCK_EDITOR_CLEAR OSMO_STOCK_EDITOR_FIND OSMO_STOCK_EDITOR_HIGHLIGHT OSMO_STOCK_EDITOR_INFO OSMO_STOCK_EDITOR_INSERT_DATE_TIME OSMO_STOCK_EDITOR_INSERT_SEPARATOR OSMO_STOCK_EDITOR_ITALIC OSMO_STOCK_EDITOR_SAVE OSMO_STOCK_EDITOR_SPELL_CHECKER OSMO_STOCK_EDITOR_STRIKETHROUGH OSMO_STOCK_EDITOR_UNDERLINE OSMO_STOCK_FULLYEAR OSMO_STOCK_HELP OSMO_STOCK_INFO_HELP OSMO_STOCK_JUMPTO OSMO_STOCK_LICENSE OSMO_STOCK_LIST_INVALID OSMO_STOCK_LIST_VALID OSMO_STOCK_NEXT_DAY OSMO_STOCK_NEXT_MONTH OSMO_STOCK_NEXT_YEAR OSMO_STOCK_NOTES_ADD OSMO_STOCK_NOTES_EDIT OSMO_STOCK_NOTES_REMOVE OSMO_STOCK_NOTES OSMO_STOCK_PREFERENCES OSMO_STOCK_PREV_DAY OSMO_STOCK_PREV_MONTH OSMO_STOCK_PREV_YEAR OSMO_STOCK_TASKS_ADD OSMO_STOCK_TASKS_EDIT OSMO_STOCK_TASKS_REMOVE OSMO_STOCK_TASKS_PREV_DAY OSMO_STOCK_TASKS_NEXT_DAY OSMO_STOCK_TASKS_TYPE_NORMAL OSMO_STOCK_TASKS_TYPE_RECURRENT OSMO_STOCK_TASKS OSMO_STOCK_TODAY OSMO_STOCK_PRINT 
syn keyword osmoDeprecatedFunction weeks_in_year week_number day_of_week gui_calendar_get_date get_selected_date check_note get_note get_color julian_to_date sync_cal_date_with_gdate utl_get_current_julian
"syn keyword osmoDeprecatedTypedef
"syn keyword osmoDeprecatedConstant
syn keyword osmoDeprecatedStruct DATE
"syn keyword osmoDeprecatedMacro
"syn keyword osmoDeprecatedEnum
"syn keyword osmoDeprecatedDefine

" Default highlighting
if version >= 508 || !exists("did_osmo_syntax_inits")
  if version < 508
    let did_osmo_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif
  HiLink osmoFunction Function
  HiLink osmoTypedef Type
  HiLink osmoConstant Constant
  HiLink osmoStruct Type
"  HiLink osmoMacro Macro
  HiLink osmoEnum Type
  HiLink osmoVariable Identifier
  HiLink osmoDefine Constant
  if exists("osmo_deprecated_errors")
    HiLink osmoDeprecatedFunction Error
"    HiLink osmoDeprecatedTypedef Error
"    HiLink osmoDeprecatedConstant Error
    HiLink osmoDeprecatedStruct Error
"    HiLink osmoDeprecatedMacro Error
"    HiLink osmoDeprecatedEnum Error
"    HiLink osmoDeprecatedDefine Error
  elseif exists("osmo_enable_deprecated")
    HiLink osmoDeprecatedFunction Function
"    HiLink osmoDeprecatedTypedef Type
"    HiLink osmoDeprecatedConstant Constant
    HiLink osmoDeprecatedStruct Type
"    HiLink osmoDeprecatedMacro Macro
"    HiLink osmoDeprecatedEnum Type
"    HiLink osmoDeprecatedDefine Constant
  endif

  delcommand HiLink
endif

