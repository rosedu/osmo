
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

#include "options_gui.h"
#include "utils.h"
#include "utils_gui.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "options_gui_calendar.h"
#include "options_gui_tasks.h"
#include "options_gui_contacts.h"
#include "options_gui_notes.h"
#include "options_gui_general.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"

#ifdef HAVE_GTKSPELL
#include <gtkspell/gtkspell.h>
#endif /* HAVE_GTKSPELL */

struct  osmo_prefs              config;

/*------------------------------------------------------------------------------*/

gint
options_get_first_active_page (GUI *appGUI) {

    if (!config.hide_calendar) 
        return OPT_PAGE_CALENDAR;
    if (!config.hide_tasks) 
        return OPT_PAGE_TASKS;
#ifdef CONTACTS_ENABLED
    if (!config.hide_contacts) 
        return OPT_PAGE_CONTACTS;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    if (!config.hide_notes) 
        return OPT_PAGE_NOTES;
#endif  /* NOTES_ENABLED */

    return OPT_PAGE_GENERAL;
}

/*------------------------------------------------------------------------------*/

void
options_switch_buttons(gboolean left, GUI *appGUI) {

gboolean flag;
gint n = 1;

    if (left == TRUE) n = -1;

    appGUI->opt->options_counter += n;

    flag = TRUE;

    while (flag) {

        flag = FALSE;

        switch (appGUI->opt->options_counter) {
            case OPT_PAGE_CALENDAR:
                if (config.hide_calendar) {
                    appGUI->opt->options_counter += n;
                    flag = TRUE;
                }
                break;
#ifdef TASKS_ENABLED
            case OPT_PAGE_TASKS:
                if (config.hide_tasks) {
                    appGUI->opt->options_counter += n;
                    flag = TRUE;
                }
                break;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
            case OPT_PAGE_CONTACTS:
                if (config.hide_contacts) {
                    appGUI->opt->options_counter += n;
                    flag = TRUE;
                }
                break;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
            case OPT_PAGE_NOTES:
                if (config.hide_notes) {
                    appGUI->opt->options_counter += n;
                    flag = TRUE;
                }
                break;
#endif  /* NOTES_ENABLED */
        }
    }

    if (left == TRUE) {
        appGUI->opt->options_counter = (appGUI->opt->options_counter < OPT_PAGE_CALENDAR) ? options_get_first_active_page(appGUI):appGUI->opt->options_counter;
    } else {
        appGUI->opt->options_counter = (appGUI->opt->options_counter > OPT_PAGE_GENERAL) ? OPT_PAGE_GENERAL:appGUI->opt->options_counter;
    }

    if (appGUI->opt->options_counter == OPT_PAGE_CALENDAR){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->calendar_options_radiobutton), "released");
#ifdef TASKS_ENABLED
    } else if (appGUI->opt->options_counter == OPT_PAGE_TASKS) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->tasks_options_radiobutton), "released");
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    } else if (appGUI->opt->options_counter == OPT_PAGE_CONTACTS) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->contacts_options_radiobutton), "released");
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    } else if (appGUI->opt->options_counter == OPT_PAGE_NOTES) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->notes_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->notes_options_radiobutton), "released");
#endif  /* NOTES_ENABLED */
    } else {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->general_options_radiobutton), "released");
    }
}

/*------------------------------------------------------------------------------*/

void
options_button_released_cb (GtkButton *button, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_hide (appGUI->opt->calendar_vbox);
#ifdef TASKS_ENABLED
    gtk_widget_hide (appGUI->opt->tasks_vbox);
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    gtk_widget_hide (appGUI->opt->contacts_vbox);
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    gtk_widget_hide (appGUI->opt->notes_vbox);
#endif  /* NOTES_ENABLED */
    gtk_widget_hide (appGUI->opt->general_vbox);

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->calendar_vbox);
        appGUI->opt->options_counter = OPT_PAGE_CALENDAR;

#ifdef TASKS_ENABLED
    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->tasks_vbox);
        appGUI->opt->options_counter = OPT_PAGE_TASKS;
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->contacts_vbox);
        appGUI->opt->options_counter = OPT_PAGE_CONTACTS;
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->notes_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->notes_vbox);
        appGUI->opt->options_counter = OPT_PAGE_NOTES;
#endif  /* NOTES_ENABLED */

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->general_vbox);
        appGUI->opt->options_counter = OPT_PAGE_GENERAL;
    }
}

/*------------------------------------------------------------------------------*/

void
gui_create_options(GUI *appGUI) {

GtkWidget           *vbox1, *vbox2, *hbox;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *scrolledwindow;
GtkWidget           *viewport;
GSList              *radiobutton_group = NULL;

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gui_add_to_notebook (vbox1, _("Options"), appGUI);

    appGUI->opt->vbox = GTK_BOX(vbox1);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    if (appGUI->tiny_gui == TRUE) {
        hbox = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox);
        gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, TRUE, 0);
    } else {
        hbuttonbox = gtk_hbutton_box_new ();
        gtk_widget_show (hbuttonbox);
        gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
        gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    }

    if (appGUI->tiny_gui == TRUE) {
        appGUI->opt->calendar_options_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_CALENDAR, 
																					  GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->opt->calendar_options_radiobutton = utl_gui_stock_label_radio_button(_("Calendar"), 
                                                                 OSMO_STOCK_CALENDAR, GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->calendar_options_radiobutton);
    }
    gtk_widget_show (appGUI->opt->calendar_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->calendar_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->opt->calendar_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

#ifdef TASKS_ENABLED

    if (appGUI->tiny_gui == TRUE) {
        appGUI->opt->tasks_options_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_TASKS, 
																				   GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->opt->tasks_options_radiobutton = utl_gui_stock_label_radio_button(_("Tasks"), 
                                                                 OSMO_STOCK_TASKS, GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->tasks_options_radiobutton);
    }
    gtk_widget_show (appGUI->opt->tasks_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->tasks_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->tasks_options_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->opt->tasks_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED

    if (appGUI->tiny_gui == TRUE) {
        appGUI->opt->contacts_options_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_CONTACTS, 
																					  GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->opt->contacts_options_radiobutton = utl_gui_stock_label_radio_button (_("Contacts"), 
                                                                 OSMO_STOCK_CONTACTS, GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->contacts_options_radiobutton);
    }
    gtk_widget_show (appGUI->opt->contacts_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->contacts_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->contacts_options_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->opt->contacts_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED

    if (appGUI->tiny_gui == TRUE) {
        appGUI->opt->notes_options_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_NOTES, 
																			       GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->opt->notes_options_radiobutton = utl_gui_stock_label_radio_button (_("Notes"), 
                                                                 OSMO_STOCK_NOTES, GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->notes_options_radiobutton);
    }
    gtk_widget_show (appGUI->opt->notes_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->notes_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->notes_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->notes_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->notes_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->notes_options_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->opt->notes_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

#endif  /* NOTES_ENABLED */

    if (appGUI->tiny_gui == TRUE) {
        appGUI->opt->general_options_radiobutton = utl_gui_stock_label_radio_button (NULL, OSMO_STOCK_PREFERENCES, 
																					 GTK_ICON_SIZE_LARGE_TOOLBAR);
    } else {
        appGUI->opt->general_options_radiobutton = utl_gui_stock_label_radio_button(_("General"), 
                                                                 OSMO_STOCK_PREFERENCES, GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->general_options_radiobutton);
    }
    gtk_widget_show (appGUI->opt->general_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->general_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->general_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->general_options_radiobutton), FALSE);

    g_signal_connect (G_OBJECT (appGUI->opt->general_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    if (appGUI->tiny_gui == TRUE) {
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->opt->general_options_radiobutton, FALSE, TRUE, 0);
#ifdef NOTES_ENABLED
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->opt->notes_options_radiobutton, FALSE, TRUE, 0);
#endif  /* NOTES_ENABLED */
#ifdef CONTACTS_ENABLED
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->opt->contacts_options_radiobutton, FALSE, TRUE, 0);
#endif  /* CONTACTS_ENABLED */
#ifdef TASKS_ENABLED
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->opt->tasks_options_radiobutton, FALSE, TRUE, 0);
#endif  /* TASKS_ENABLED */
        gtk_box_pack_end (GTK_BOX (hbox), appGUI->opt->calendar_options_radiobutton, FALSE, TRUE, 0);
    }

    /*--------------------------------------------------------------------------*/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_NONE);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), viewport);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (viewport), vbox2);

    gui_create_calendar_options_page (vbox2, appGUI);
#ifdef TASKS_ENABLED
    gui_create_tasks_options_page (vbox2, appGUI);
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    gui_create_contacts_options_page (vbox2, appGUI);
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    gui_create_notes_options_page (vbox2, appGUI);
#endif  /* NOTES_ENABLED */
    gui_create_general_options_page (vbox2, appGUI);

    appGUI->opt->callback_active = FALSE;

    if(config.display_options & GUI_CALENDAR_SHOW_DAY_NAMES)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_day_names_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->no_month_change_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_SHOW_WEEK_NUMBERS)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_week_numbers_checkbutton), TRUE);
    if(config.display_options & GUI_CALENDAR_WEEK_START_MONDAY)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->week_start_monday_checkbutton), TRUE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->simple_view_in_fy_calendar_checkbutton), config.fy_simple_view);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_block_cursor_checkbutton), !config.cursor_type);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_auxilary_calendars_checkbutton), config.enable_auxilary_calendars);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->strikethrough_past_notes_checkbutton), config.strikethrough_past_notes);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ascending_sorting_in_day_notes_checkbutton), config.ascending_sorting_in_day_notes_browser);

#ifdef HAVE_GTKSPELL
    if (config.day_note_spell_checker == TRUE) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->spell_checker_in_day_notes_checkbutton), TRUE);

        GtkSpell *edSpell = gtkspell_new_attach (GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), NULL, NULL);
        gtkspell_set_language (edSpell, config.spell_lang, NULL);
    }
#endif  /* HAVE_GTKSPELL */

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_current_time_checkbutton), 
                                  config.di_show_current_time);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_day_number_checkbutton), 
                                  config.di_show_day_number);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_current_day_distance_checkbutton), 
                                  config.di_show_current_day_distance);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_marked_days_checkbutton), 
                                  config.di_show_marked_days);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_week_number_checkbutton), 
                                  config.di_show_week_number);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_weekend_days_checkbutton), 
                                  config.di_show_weekend_days);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_day_category_checkbutton), 
                                  config.di_show_day_category);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_moon_phase_checkbutton), 
                                  config.di_show_moon_phase);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_notes_checkbutton), 
                                  config.di_show_notes);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->di_show_zodiac_sign_checkbutton), 
                                  config.di_show_zodiac_sign);

    if (config.enable_auxilary_calendars == TRUE) {
        gtk_widget_show (appGUI->cal->aux_cal_expander);
    } else {
        gtk_widget_hide (appGUI->cal->aux_cal_expander);
    }

    calendar_cursor_settings_enable_disable(appGUI);

    gtk_range_set_value (GTK_RANGE (appGUI->opt->cft_hscale), config.frame_cursor_thickness);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->default_stock_icons_checkbutton), 
                                  config.default_stock_icons);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->disable_underline_in_links_checkbutton), 
                                  config.disable_underline_links);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton), 
                                  config.enable_tooltips);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_systray_checkbutton), 
                                  config.enable_systray);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->start_minimised_checkbutton), 
                                  config.start_minimised_in_systray);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->blink_on_events_checkbutton), 
                                  config.blink_on_events);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ignore_day_note_events_checkbutton), 
                                  config.ignore_day_note_events);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->remember_latest_tab_checkbutton), 
                                  config.remember_latest_tab);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->save_data_checkbutton), 
                                  config.save_data_after_modification);

#ifdef TASKS_ENABLED
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_enable_rules_hint_checkbutton), 
                                  config.tasks_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_bold_items_checkbutton), 
                                  config.tasks_high_in_bold);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton), 
                                  config.hide_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_delete_items_checkbutton), 
                                  config.delete_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_add_item_checkbutton), 
                                  config.add_edit);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_remember_category_checkbutton), 
                                  config.remember_category_in_tasks);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_due_date_checkbutton), 
                                  config.tsk_visible_due_date_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_type_checkbutton), 
                                  config.tsk_visible_type_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_priority_checkbutton), 
                                  config.tsk_visible_priority_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_category_checkbutton), 
                                  config.tsk_visible_category_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_tasks_checkbutton), 
                                  config.hide_tasks);

    gtk_entry_set_text(GTK_ENTRY(appGUI->opt->global_notification_cmd_entry), config.global_notification_command);
    utl_gui_update_command_status (appGUI->opt->global_notification_cmd_entry, 
								   appGUI->opt->global_notification_valid_image, appGUI);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->ti_font_entry), config.task_info_font);
#endif  /* CONTACTS_ENABLED */

#ifdef CONTACTS_ENABLED
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_enable_rules_hint_checkbutton), 
                                  config.contacts_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_select_first_entry_checkbutton), 
                                  config.show_after_search);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_hide_group_column_checkbutton), 
                                  config.hide_group_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_age_checkbutton), 
                                  config.cnt_visible_age_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_birthday_date_checkbutton), 
                                  config.cnt_visible_birthday_date_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_zodiac_sign_checkbutton), 
                                  config.cnt_visible_zodiac_sign_column);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->cn_font_entry), config.contact_name_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->ci_font_entry), config.contact_item_font);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_contacts_checkbutton), 
                                  config.hide_contacts);
    gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[COLUMN_GROUP], !config.hide_group_column);
#endif  /* CONTACTS_ENABLED */

#if defined(TASKS_ENABLED) || defined(CONTACTS_ENABLED) || defined(NOTES_ENABLED)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_calendar_checkbutton), 
                                  config.hide_calendar);
#endif

#ifdef NOTES_ENABLED
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_notes_checkbutton), 
                                  config.hide_notes);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->notes_enable_rules_hint_checkbutton), 
                                  config.notes_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->cn_remember_category_checkbutton), 
                                  config.remember_category_in_notes);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->cn_use_system_date_checkbutton), 
                                  config.use_system_date_in_notes);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->editor_font_entry), config.notes_editor_font);

	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->nvc_type_checkbutton), 
                                  config.nte_visible_type_column);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->nvc_category_checkbutton), 
                                  config.nte_visible_category_column);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->nvc_last_changes_checkbutton), 
                                  config.nte_visible_last_changes_column);
#endif  /* NOTES_ENABLED */

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->day_name_font_entry), config.day_name_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->calendar_font_entry), config.calendar_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->notes_font_entry), config.notes_font);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_web_browser), config.web_browser);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_email_client), config.email_client);

#ifdef HAVE_GTKSPELL
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_spell_lang), config.spell_lang);
#endif  /* HAVE_GTKSPELL */

    appGUI->opt->callback_active = TRUE;

    if (config.hide_calendar) {
        gtk_widget_hide (appGUI->opt->calendar_vbox);
        gtk_widget_hide (appGUI->opt->calendar_options_radiobutton);
    }
#ifdef TASKS_ENABLED
    if (config.hide_tasks) {
        gtk_widget_hide (appGUI->opt->tasks_vbox);
        gtk_widget_hide (appGUI->opt->tasks_options_radiobutton);
    }
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    if (config.hide_contacts) {
        gtk_widget_hide (appGUI->opt->contacts_vbox);
        gtk_widget_hide (appGUI->opt->contacts_options_radiobutton);
    }
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    if (config.hide_notes) {
        gtk_widget_hide (appGUI->opt->notes_vbox);
        gtk_widget_hide (appGUI->opt->notes_options_radiobutton);
    }
#endif  /* NOTES_ENABLED */

    switch(options_get_first_active_page (appGUI)) {
        case OPT_PAGE_CALENDAR:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton), TRUE);
            break;
#ifdef TASKS_ENABLED
        case OPT_PAGE_TASKS:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton), TRUE);
            break;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
        case OPT_PAGE_CONTACTS:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton), TRUE);
            break;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
        case OPT_PAGE_NOTES:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->notes_options_radiobutton), TRUE);
            break;
#endif  /* NOTES_ENABLED */
    }

    gtk_widget_show (appGUI->opt->calendar_vbox);

    options_button_released_cb (NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

