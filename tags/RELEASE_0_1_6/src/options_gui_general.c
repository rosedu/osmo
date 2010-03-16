
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
#include "options_prefs.h"
#include "contacts.h"
#include "stock_icons.h"


/*------------------------------------------------------------------------------*/

void
default_stock_icons_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->default_stock_icons_checkbutton)) == TRUE) {
        config.default_stock_icons = 1;
    } else {
        config.default_stock_icons = 0;
    }
}

/*------------------------------------------------------------------------------*/

void
disable_underline_in_links_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.disable_underline_links = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->disable_underline_in_links_checkbutton));
    g_signal_emit_by_name(G_OBJECT(appGUI->cnt->contacts_list_selection), "changed");
    about_set_first_page(appGUI);

}

/*------------------------------------------------------------------------------*/

void
enable_tooltips_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton)) == TRUE) {
        config.enable_tooltips = 1;
        gtk_tooltips_enable(appGUI->osmo_tooltips);
    } else {
        config.enable_tooltips = 0;
        gtk_tooltips_disable(appGUI->osmo_tooltips);
    }

    gtk_toolbar_set_tooltips (appGUI->cal->calendar_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (appGUI->tsk->tasks_toolbar, config.enable_tooltips);
    gtk_toolbar_set_tooltips (appGUI->cnt->contacts_toolbar, config.enable_tooltips);

}

/*------------------------------------------------------------------------------*/

void
enable_systray_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_systray_checkbutton)) == TRUE) {
        config.enable_systray = 1;
        gtk_widget_set_sensitive(appGUI->opt->start_minimised_checkbutton, TRUE);
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, TRUE);
        if (gtk_status_icon_is_embedded(appGUI->osmo_trayicon) == FALSE) {
            appGUI->no_tray = TRUE;
        }
    } else {
        config.enable_systray = 0;
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
        gtk_widget_set_sensitive(appGUI->opt->start_minimised_checkbutton, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void
start_minimised_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->start_minimised_checkbutton)) == TRUE) {
        config.start_minimised_in_systray = 1;
    } else {
        config.start_minimised_in_systray = 0;
    }

}

/*------------------------------------------------------------------------------*/

void
remember_latest_tab_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;
 
    config.remember_latest_tab = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->remember_latest_tab_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
helpers_entry_changed_cb (GtkEntry *entry, gpointer  user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    strncpy(config.web_browser, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_web_browser)), MAXHELPERCMD-1);
    strncpy(config.email_client, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_email_client)), MAXHELPERCMD-1);

}

/*------------------------------------------------------------------------------*/

void
tabs_position_changed_cb (GtkComboBox *widget, gpointer user_data) {

GtkWidget *label;
gint angle;

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tabs_position = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->tabs_position_combobox));

    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(appGUI->notebook), config.tabs_position);

    if (config.tabs_position == GTK_POS_LEFT) {
        angle = 90;
    } else if (config.tabs_position == GTK_POS_RIGHT) {
        angle = -90;
    } else {
        angle = 0;
    }

    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->cal->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->tsk->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->cnt->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->opt->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->about_vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);

}

/*------------------------------------------------------------------------------*/

void
date_format_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.date_format = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->date_format_combobox));
    refresh_due_dates(appGUI);

}

/*------------------------------------------------------------------------------*/

void
time_format_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.time_format = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->time_format_combobox));
    refresh_due_dates(appGUI);

}

/*------------------------------------------------------------------------------*/

void
gui_create_general_page (GtkWidget *vbox, GUI *appGUI) {

GtkWidget   *vbox4, *vbox8, *vbox9, *vbox14;
GtkWidget   *label;
GtkWidget   *frame;
GtkWidget   *table;
GtkWidget   *alignment;
gchar       tmpbuf[BUFFER_SIZE];


    appGUI->opt->general_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), appGUI->opt->general_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Appearance"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox8 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox8);
    gtk_container_add (GTK_CONTAINER (alignment), vbox8);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox8), table, FALSE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 16);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Tabs position"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->tabs_position_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->tabs_position_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->tabs_position_combobox), "changed",
                      G_CALLBACK(tabs_position_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tabs_position_combobox, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tabs_position_combobox), _("Left"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tabs_position_combobox), _("Right"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tabs_position_combobox), _("Top"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tabs_position_combobox), _("Bottom"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->tabs_position_combobox), config.tabs_position);

    appGUI->opt->default_stock_icons_checkbutton = gtk_check_button_new_with_mnemonic (_("Use default stock icons (needs restart)"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->default_stock_icons_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->default_stock_icons_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->default_stock_icons_checkbutton), "toggled",
                      G_CALLBACK (default_stock_icons_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->default_stock_icons_checkbutton, FALSE, FALSE, 4);

    appGUI->opt->disable_underline_in_links_checkbutton = gtk_check_button_new_with_mnemonic (_("Disable underline in links"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->disable_underline_in_links_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->disable_underline_in_links_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->disable_underline_in_links_checkbutton), "toggled",
                      G_CALLBACK (disable_underline_in_links_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (vbox8), appGUI->opt->disable_underline_in_links_checkbutton, FALSE, FALSE, 4);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("General"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (alignment), vbox4);

    table = gtk_table_new (3, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox4), table, FALSE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 16);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Date format"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->date_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->date_format_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->date_format_combobox), "changed",
                      G_CALLBACK(date_format_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->date_format_combobox, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("DD-MM-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("MM-DD-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-MM-DD"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-DD-MM"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->date_format_combobox), config.date_format);

    sprintf(tmpbuf, "%s:", _("Time format"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->time_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->time_format_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->time_format_combobox), "changed",
                      G_CALLBACK(time_format_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->time_format_combobox, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    sprintf(tmpbuf, "24 %s", _("hours"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->time_format_combobox), tmpbuf);
    sprintf(tmpbuf, "12 %s", _("hours"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->time_format_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->time_format_combobox), config.time_format);


    appGUI->opt->enable_tooltips_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable tooltips"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_tooltips_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->enable_tooltips_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_tooltips_checkbutton), "toggled",
                      G_CALLBACK (enable_tooltips_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_tooltips_checkbutton, 0, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->remember_latest_tab_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember last selected page"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->remember_latest_tab_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->remember_latest_tab_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->remember_latest_tab_checkbutton), "toggled",
                      G_CALLBACK (remember_latest_tab_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->remember_latest_tab_checkbutton, 0, 4, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Helpers"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox9 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox9);
    gtk_container_add (GTK_CONTAINER (alignment), vbox9);

    table = gtk_table_new (2, 2, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox9), table, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf (tmpbuf, "%s:", _("Web browser"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf (tmpbuf, "%s:", _("E-mail client"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->entry_web_browser = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->entry_web_browser);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_web_browser), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_web_browser, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_web_browser), MAXHELPERCMD);

    appGUI->opt->entry_email_client = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->entry_email_client);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_email_client), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), NULL);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_email_client, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_email_client), MAXHELPERCMD);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("System tray"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    vbox14 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox14);
    gtk_container_add (GTK_CONTAINER (alignment), vbox14);

    table = gtk_table_new (3, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox14), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    appGUI->opt->enable_systray_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable system tray"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_systray_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->enable_systray_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_systray_checkbutton), "toggled",
                      G_CALLBACK (enable_systray_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_systray_checkbutton, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->start_minimised_checkbutton = gtk_check_button_new_with_mnemonic (_("Start minimised"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->start_minimised_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->start_minimised_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->start_minimised_checkbutton), "toggled",
                      G_CALLBACK (start_minimised_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->start_minimised_checkbutton, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_sensitive(appGUI->opt->start_minimised_checkbutton, config.enable_systray);

}

/*------------------------------------------------------------------------------*/

