
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
#include <ctype.h>
#include <time.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <glib.h>
#include <limits.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "gui.h"
#include "gui_utils.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "options_gui.h"
#include "options_gui_calendar.h"
#include "options_gui_tasks.h"
#include "options_gui_contacts.h"
#include "options_gui_general.h"
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"


struct  osmo_prefs              config;

/*------------------------------------------------------------------------------*/

void
options_switch_buttons(gboolean left, GUI *appGUI) {

    if (left == TRUE && appGUI->opt->options_counter > 0) {
        --appGUI->opt->options_counter;
    } else if (left == FALSE && appGUI->opt->options_counter < 3) {
        appGUI->opt->options_counter++;
    }

    if (appGUI->opt->options_counter == 0){
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->calendar_options_radiobutton), "released");
    } else if (appGUI->opt->options_counter == 1) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->tasks_options_radiobutton), "released");
    } else if (appGUI->opt->options_counter == 2) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton), TRUE);
        g_signal_emit_by_name(G_OBJECT(appGUI->opt->contacts_options_radiobutton), "released");
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
    gtk_widget_hide (appGUI->opt->tasks_vbox);
    gtk_widget_hide (appGUI->opt->contacts_vbox);
    gtk_widget_hide (appGUI->opt->general_vbox);

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->calendar_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->calendar_vbox);
        appGUI->opt->options_counter = 0;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->tasks_vbox);
        appGUI->opt->options_counter = 1;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->contacts_vbox);
        appGUI->opt->options_counter = 2;

    } else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton)) == TRUE) {

        gtk_widget_show (appGUI->opt->general_vbox);
        appGUI->opt->options_counter = 3;
    }
}

/*------------------------------------------------------------------------------*/

void
gui_create_options(GUI *appGUI) {

GtkWidget           *vbox1, *vbox2;
GtkWidget           *label;
GtkWidget           *hseparator;
GtkWidget           *hbuttonbox;
GtkWidget           *scrolledwindow;
GtkWidget           *viewport;
GSList              *radiobutton_group = NULL;

    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    gtk_label_set_markup (GTK_LABEL (label), _("Options"));
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(appGUI->notebook), vbox1, label);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    /*--------------------------------------------------------------------------*/

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    appGUI->opt->calendar_options_radiobutton = gui_stock_label_radio_button(_("Calendar"), 
                                                             OSMO_STOCK_CALENDAR, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->calendar_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->calendar_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->calendar_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->calendar_options_radiobutton));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), TRUE);
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->calendar_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->calendar_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->calendar_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->tasks_options_radiobutton = gui_stock_label_radio_button(_("Tasks"), 
                                                             OSMO_STOCK_TASKS, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->tasks_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->tasks_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->tasks_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->tasks_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->tasks_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->tasks_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->contacts_options_radiobutton = gui_stock_label_radio_button(_("Contacts"), 
                                                             OSMO_STOCK_CONTACTS, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->contacts_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->contacts_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->contacts_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->contacts_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->contacts_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->contacts_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->contacts_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

    appGUI->opt->general_options_radiobutton = gui_stock_label_radio_button(_("General"), 
                                                             OSMO_STOCK_PREFERENCES, GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_widget_show (appGUI->opt->general_options_radiobutton);
    gtk_button_set_relief (GTK_BUTTON (appGUI->opt->general_options_radiobutton), GTK_RELIEF_NONE);  
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->general_options_radiobutton, GTK_CAN_FOCUS);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton), radiobutton_group);
    radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->opt->general_options_radiobutton));
    gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (appGUI->opt->general_options_radiobutton), FALSE);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), appGUI->opt->general_options_radiobutton);

    g_signal_connect (G_OBJECT (appGUI->opt->general_options_radiobutton), "released",
                      G_CALLBACK(options_button_released_cb), appGUI);

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

    gui_create_calendar_page (vbox2, appGUI);
    gui_create_tasks_page (vbox2, appGUI);
    gui_create_contacts_page (vbox2, appGUI);
    gui_create_general_page (vbox2, appGUI);

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
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->show_notes_in_info_panel_checkbutton), config.notes_in_info_panel);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_block_cursor_checkbutton), !config.cursor_type);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_auxilary_calendars_checkbutton), config.enable_auxilary_calendars);

    if (config.enable_auxilary_calendars == TRUE) {
        gtk_widget_show (appGUI->cal->aux_cal_expander);
    } else {
        gtk_widget_hide (appGUI->cal->aux_cal_expander);
    }

    calendar_cursor_settings_enable_disable(appGUI);

    gtk_range_set_value (GTK_RANGE (appGUI->opt->cft_hscale), config.frame_cursor_thickness);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->default_stock_icons_checkbutton), config.default_stock_icons);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->disable_underline_in_links_checkbutton), config.disable_underline_links);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton), config.enable_tooltips);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_systray_checkbutton), config.enable_systray);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->start_minimised_checkbutton), config.start_minimised_in_systray);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->remember_latest_tab_checkbutton), config.remember_latest_tab);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->tasks_enable_rules_hint_checkbutton), config.tasks_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_hide_items_checkbutton), config.hide_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->ct_delete_items_checkbutton), config.delete_completed);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_due_date_checkbutton), config.visible_due_date_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_priority_checkbutton), config.visible_priority_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->vc_category_checkbutton), config.visible_category_column);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_enable_rules_hint_checkbutton), config.contacts_rules_hint);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_select_first_entry_checkbutton), config.show_after_search);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->contacts_hide_group_column_checkbutton), config.hide_group_column);
    gtk_tree_view_column_set_visible (appGUI->cnt->contacts_columns[COLUMN_GROUP], !config.hide_group_column);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->day_name_font_entry), config.day_name_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->calendar_font_entry), config.calendar_font);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->notes_font_entry), config.notes_font);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->ti_font_entry), config.task_info_font);

    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_web_browser), config.web_browser);
    gtk_entry_set_text (GTK_ENTRY(appGUI->opt->entry_email_client), config.email_client);

    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->name_font_size_spinbutton), config.name_font_size);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->tags_font_size_spinbutton), config.tags_font_size);

    appGUI->opt->callback_active = TRUE;

    gtk_widget_show (appGUI->opt->calendar_vbox);
}

/*------------------------------------------------------------------------------*/

