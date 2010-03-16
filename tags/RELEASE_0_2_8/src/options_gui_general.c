
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

#include "options_gui_general.h"
#include "utils.h"
#include "utils_gui.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_widget.h"
#include "i18n.h"
#include "tasks.h"
#include "notes.h"
#include "options_gui.h"
#include "options_gui_calendar.h"
#include "options_gui_tasks.h"
#include "options_gui_contacts.h"
#include "options_prefs.h"
#include "contacts.h"
#include "backup.h"
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
#ifdef CONTACTS_ENABLED
    g_signal_emit_by_name(G_OBJECT(appGUI->cnt->contacts_list_selection), "changed");
#endif  /* CONTACTS_ENABLED */
    about_set_first_page(appGUI);

}

/*------------------------------------------------------------------------------*/

void
enable_tooltips_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_tooltips_checkbutton)) == TRUE) {
        config.enable_tooltips = 1;
    } else {
        config.enable_tooltips = 0;
    }
}

/*------------------------------------------------------------------------------*/

void
override_locale_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->override_locale_checkbutton)) == TRUE) {
        config.override_locale_settings = TRUE;
    } else {
        config.override_locale_settings = FALSE;
    }

	gtk_widget_set_sensitive (appGUI->opt->override_locale_label_1, config.override_locale_settings);
	gtk_widget_set_sensitive (appGUI->opt->override_locale_label_2, config.override_locale_settings);
	gtk_widget_set_sensitive (appGUI->opt->date_format_combobox, config.override_locale_settings);
	gtk_widget_set_sensitive (appGUI->opt->time_format_combobox, config.override_locale_settings);

#ifdef HAVE_GTKSPELL
	gtk_widget_set_sensitive (appGUI->opt->override_locale_label_3, config.override_locale_settings);
	gtk_widget_set_sensitive (appGUI->opt->entry_spell_lang, config.override_locale_settings);
#endif  /* HAVE_GTKSPELL */

#ifdef TASKS_ENABLED
	refresh_tasks (appGUI);
#endif  /* TASKS_ENABLED */

#ifdef NOTES_ENABLED
	refresh_notes (appGUI);
#endif  /* NOTES_ENABLED */

}

/*------------------------------------------------------------------------------*/

gint
get_visible_tabs (GUI *appGUI) {

    gint i = 2;

    if (!config.hide_calendar) i++;
#ifdef TASKS_ENABLED
    if (!config.hide_tasks) i++;
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    if (!config.hide_contacts) i++;
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    if (!config.hide_notes) i++;
#endif  /* NOTES_ENABLED */

    return i;
}

/*------------------------------------------------------------------------------*/

void
hide_module_cb (GtkComboBox *widget, gpointer user_data) {

gboolean ga_calendar, result;

#ifdef TASKS_ENABLED
gboolean ga_tasks;
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
gboolean ga_contacts;
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
gboolean ga_notes;
#endif  /* NOTES_ENABLED */

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    ga_calendar = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_calendar_checkbutton));
#ifdef TASKS_ENABLED
    ga_tasks = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_tasks_checkbutton));
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    ga_contacts = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_contacts_checkbutton));
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    ga_notes = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_notes_checkbutton));
#endif  /* NOTES_ENABLED */

#ifdef TASKS_ENABLED

    result = (ga_calendar == TRUE) && (ga_tasks == TRUE);
    #if defined(NOTES_ENABLED) && defined(CONTACTS_ENABLED)
        result = (ga_calendar == TRUE) && (ga_tasks == TRUE) && (ga_contacts == TRUE) && (ga_notes == TRUE);
    #elif CONTACTS_ENABLED
        result = (ga_calendar == TRUE) && (ga_tasks == TRUE) && (ga_contacts == TRUE);
    #endif  /* NOTES_ENABLED */

#else
        result = (ga_calendar == TRUE);
    #if defined(NOTES_ENABLED) && defined(CONTACTS_ENABLED)
        result = (ga_calendar == TRUE) && (ga_contacts == TRUE) && (ga_notes == TRUE);
    #elif CONTACTS_ENABLED
        result = (ga_calendar == TRUE) && (ga_contacts == TRUE);
    #endif  /* NOTES_ENABLED */

#endif  /* TASKS_ENABLED */

    if (result == TRUE) { 
        utl_gui_create_dialog (GTK_MESSAGE_ERROR, _("At least one module must be visible."), 
							   GTK_WINDOW(appGUI->main_window));
        ga_calendar = FALSE;
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->hide_calendar_checkbutton), ga_calendar);
    }

    if (ga_calendar == TRUE) {
        config.hide_calendar = TRUE;
        gtk_widget_hide (GTK_WIDGET(appGUI->cal->vbox));
        gtk_widget_hide (appGUI->opt->calendar_vbox);
        gtk_widget_hide (appGUI->opt->calendar_options_radiobutton);
        gtk_widget_hide (appGUI->trayicon_menu_calendar_item);
    } else {
        config.hide_calendar = FALSE;
        gtk_widget_show (GTK_WIDGET(appGUI->cal->vbox));
        gtk_widget_show (appGUI->opt->calendar_vbox);
        gtk_widget_show (appGUI->opt->calendar_options_radiobutton);
        gtk_widget_show (appGUI->trayicon_menu_calendar_item);
    }

#ifdef TASKS_ENABLED
    if (ga_tasks == TRUE) {
        config.hide_tasks = TRUE;
        gtk_widget_hide(GTK_WIDGET(appGUI->tsk->vbox));
        gtk_widget_hide (appGUI->opt->tasks_vbox);
        gtk_widget_hide (appGUI->opt->tasks_options_radiobutton);
        gtk_widget_hide (appGUI->trayicon_menu_tasks_item);
    } else {
        config.hide_tasks = FALSE;
        gtk_widget_show (GTK_WIDGET(appGUI->tsk->vbox));
        gtk_widget_show (appGUI->opt->tasks_vbox);
        gtk_widget_show (appGUI->opt->tasks_options_radiobutton);
        gtk_widget_show (appGUI->trayicon_menu_tasks_item);
    }
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    if (ga_contacts == TRUE) {
        config.hide_contacts = TRUE;
        gtk_widget_hide (GTK_WIDGET(appGUI->cnt->vbox));
        gtk_widget_hide (appGUI->opt->contacts_vbox);
        gtk_widget_hide (appGUI->opt->contacts_options_radiobutton);
        gtk_widget_hide (appGUI->trayicon_menu_contacts_item);
    } else {
        config.hide_contacts = FALSE;
        gtk_widget_show (GTK_WIDGET(appGUI->cnt->vbox));
        gtk_widget_show (appGUI->opt->contacts_vbox);
        gtk_widget_show (appGUI->opt->contacts_options_radiobutton);
        gtk_widget_show (appGUI->trayicon_menu_contacts_item);
    }
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
    if (ga_notes == TRUE) {
        config.hide_notes = TRUE;
        gtk_widget_hide (GTK_WIDGET(appGUI->nte->vbox));
        gtk_widget_hide (appGUI->opt->notes_vbox);
        gtk_widget_hide (appGUI->opt->notes_options_radiobutton);
        gtk_widget_hide (appGUI->trayicon_menu_notes_item);
    } else {
        config.hide_notes = FALSE;
        gtk_widget_show (GTK_WIDGET(appGUI->nte->vbox));
        gtk_widget_show (appGUI->opt->notes_vbox);
        gtk_widget_show (appGUI->opt->notes_options_radiobutton);
        gtk_widget_show (appGUI->trayicon_menu_notes_item);
    }
#endif  /* NOTES_ENABLED */

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(appGUI->opt->general_options_radiobutton), TRUE);
    g_signal_emit_by_name(G_OBJECT(appGUI->opt->general_options_radiobutton), "released");
}

/*------------------------------------------------------------------------------*/

void
enable_systray_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->enable_systray_checkbutton)) == TRUE) {
        config.enable_systray = 1;
        gtk_widget_set_sensitive(appGUI->opt->start_minimised_checkbutton, TRUE);
        gtk_widget_set_sensitive(appGUI->opt->blink_on_events_checkbutton, TRUE);
        gtk_widget_set_sensitive(appGUI->opt->ignore_day_note_events_checkbutton, TRUE);
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, TRUE);
    } else {
        config.enable_systray = 0;
        gtk_status_icon_set_visible(appGUI->osmo_trayicon, FALSE);
        gtk_widget_set_sensitive(appGUI->opt->start_minimised_checkbutton, FALSE);
        gtk_widget_set_sensitive(appGUI->opt->blink_on_events_checkbutton, FALSE);
        gtk_widget_set_sensitive(appGUI->opt->ignore_day_note_events_checkbutton, FALSE);
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
blink_on_events_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->blink_on_events_checkbutton)) == TRUE) {
        config.blink_on_events = 1;
    } else {
        config.blink_on_events = 0;
    }

}

/*------------------------------------------------------------------------------*/

void
ignore_day_note_events_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->ignore_day_note_events_checkbutton)) == TRUE) {
        config.ignore_day_note_events = 1;
    } else {
        config.ignore_day_note_events = 0;
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
save_data_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;
 
    config.save_data_after_modification = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->opt->save_data_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
helpers_entry_changed_cb (GtkEntry *entry, gpointer  user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_web_browser)))) {
        strncpy (config.web_browser, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_web_browser)), MAXHELPERCMD-1);
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_email_client)))) {
        strncpy (config.email_client, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_email_client)), MAXHELPERCMD-1);
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_sound_player)))) {
        strncpy (config.sound_player, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_sound_player)), MAXHELPERCMD-1);
    }
    
    utl_gui_change_bg_widget_state (appGUI->opt->entry_web_browser, NULL, appGUI);
    utl_gui_change_bg_widget_state (appGUI->opt->entry_email_client, NULL, appGUI);
    utl_gui_change_bg_widget_state (appGUI->opt->entry_sound_player, NULL, appGUI);

}

/*------------------------------------------------------------------------------*/

#ifdef HAVE_GTKSPELL
void
spell_checker_entry_changed_cb (GtkEntry *entry, gpointer  user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_spell_lang)))) {
        strncpy(config.spell_lang, gtk_entry_get_text(GTK_ENTRY(appGUI->opt->entry_spell_lang)), MAXNAME-1);
    }
    
    utl_gui_change_bg_widget_state (appGUI->opt->entry_spell_lang, NULL, appGUI);

}


gint 
spell_checker_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    utl_gui_change_bg_widget_state (appGUI->opt->entry_spell_lang, COLOR_BG_OK, appGUI);
    return FALSE;
}
#endif  /* HAVE_GTKSPELL */

/*------------------------------------------------------------------------------*/

gint 
helper_www_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    utl_gui_change_bg_widget_state (appGUI->opt->entry_web_browser, COLOR_BG_OK, appGUI);
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint 
helper_email_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    utl_gui_change_bg_widget_state (appGUI->opt->entry_email_client, COLOR_BG_OK, appGUI);
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gint 
helper_sound_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    utl_gui_change_bg_widget_state (appGUI->opt->entry_sound_player, COLOR_BG_OK, appGUI);
    return FALSE;
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
#ifdef TASKS_ENABLED
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->tsk->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
#endif  /* TASKS_ENABLED */
#ifdef CONTACTS_ENABLED
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->cnt->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
#endif  /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
    label = gtk_notebook_get_tab_label (GTK_NOTEBOOK(appGUI->notebook), GTK_WIDGET(appGUI->nte->vbox));
    gtk_label_set_angle (GTK_LABEL(label), angle);
#endif  /* NOTES_ENABLED */
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

#ifdef TASKS_ENABLED
    refresh_tasks (appGUI);
#endif  /* TASKS_ENABLED */

}

/*------------------------------------------------------------------------------*/

void
time_format_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.time_format = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->time_format_combobox));

#ifdef TASKS_ENABLED
    refresh_tasks (appGUI);
#endif  /* TASKS_ENABLED */

}

/*------------------------------------------------------------------------------*/

void
enable_rules_hint_changed_cb (GtkComboBox *widget, GUI *appGUI)
{
	if (!appGUI->opt->callback_active) return;

	config.rules_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->enable_rules_hint_checkbutton));
#ifdef CONTACTS_ENABLED
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (appGUI->cnt->contacts_list), config.rules_hint);
#endif /* CONTACTS_ENABLED */
#ifdef NOTES_ENABLED
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (appGUI->nte->notes_list), config.rules_hint);
#endif /* NOTES_ENABLED */
#ifdef TASKS_ENABLED
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (appGUI->tsk->tasks_list), config.rules_hint);
#endif /* TASKS_ENABLED */
}

/*------------------------------------------------------------------------------*/

#if defined(BACKUP_SUPPORT) && defined(HAVE_LIBGRINGOTTS)

void
button_create_backup_cb (GtkWidget *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
	backup_create (appGUI);
}

void
button_restore_backup_cb (GtkWidget *widget, gpointer user_data) {

	GUI *appGUI = (GUI *)user_data;
	backup_restore (appGUI);
}

#endif  /* BACKUP_SUPPORT && HAVE_LIBGRINGOTTS */

/*------------------------------------------------------------------------------*/

void
gui_create_general_options_page (GtkWidget *vbox, GUI *appGUI) {

GtkWidget   *general_vbox, *appearance_vbox;
GtkWidget   *helpers_vbox, *system_tray_vbox;
GtkWidget   *label;
GtkWidget   *frame;
GtkWidget   *table;
GtkWidget   *alignment;

#if defined(BACKUP_SUPPORT) && defined(HAVE_LIBGRINGOTTS)
GtkWidget   *hbuttonbox;
GtkWidget   *create_button, *restore_button;
#endif  /* BACKUP_SUPPORT && HAVE_LIBGRINGOTTS */

#if defined(TASKS_ENABLED) || defined(CONTACTS_ENABLED) || defined(NOTES_ENABLED)
GtkWidget   *hide_vbox;
#endif

gint i = 0;
gchar tmpbuf[BUFFER_SIZE];


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

    appearance_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (appearance_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), appearance_vbox);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (appearance_vbox), table, FALSE, TRUE, 0);
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
    gtk_box_pack_start (GTK_BOX (appearance_vbox), appGUI->opt->default_stock_icons_checkbutton, FALSE, FALSE, 4);

    appGUI->opt->disable_underline_in_links_checkbutton = gtk_check_button_new_with_mnemonic (_("Disable underline in links"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->disable_underline_in_links_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->disable_underline_in_links_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->disable_underline_in_links_checkbutton), "toggled",
                      G_CALLBACK (disable_underline_in_links_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (appearance_vbox), appGUI->opt->disable_underline_in_links_checkbutton, FALSE, FALSE, 4);

    appGUI->opt->enable_rules_hint_checkbutton = gtk_check_button_new_with_mnemonic (_("Draw rows in alternating colors"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_rules_hint_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->enable_rules_hint_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_rules_hint_checkbutton), "toggled",
                      G_CALLBACK (enable_rules_hint_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (appearance_vbox), appGUI->opt->enable_rules_hint_checkbutton, FALSE, FALSE, 4);


#if defined(TASKS_ENABLED) || defined(CONTACTS_ENABLED) || defined(NOTES_ENABLED)

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Hide"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    hide_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (hide_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), hide_vbox);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (hide_vbox), table, TRUE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 16);

    appGUI->opt->hide_calendar_checkbutton = gtk_check_button_new_with_mnemonic (_("Calendar"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->hide_calendar_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->hide_calendar_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->hide_calendar_checkbutton), "toggled",
                      G_CALLBACK (hide_module_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->hide_calendar_checkbutton, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

#ifdef TASKS_ENABLED
    appGUI->opt->hide_tasks_checkbutton = gtk_check_button_new_with_mnemonic (_("Tasks"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->hide_tasks_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->hide_tasks_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->hide_tasks_checkbutton), "toggled",
                      G_CALLBACK (hide_module_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->hide_tasks_checkbutton, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
#endif  /* TASKS_ENABLED */

#ifdef CONTACTS_ENABLED
    appGUI->opt->hide_contacts_checkbutton = gtk_check_button_new_with_mnemonic (_("Contacts"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->hide_contacts_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->hide_contacts_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->hide_contacts_checkbutton), "toggled",
                      G_CALLBACK (hide_module_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->hide_contacts_checkbutton, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
#endif  /* CONTACTS_ENABLED */

#ifdef NOTES_ENABLED
    appGUI->opt->hide_notes_checkbutton = gtk_check_button_new_with_mnemonic (_("Notes"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->hide_notes_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->hide_notes_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->hide_notes_checkbutton), "toggled",
                      G_CALLBACK (hide_module_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->hide_notes_checkbutton, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
#endif  /* NOTES_ENABLED */

#endif

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

    general_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (general_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), general_vbox);

    table = gtk_table_new (6, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (general_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 16);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

	i = 0;

    appGUI->opt->override_locale_checkbutton = gtk_check_button_new_with_mnemonic (_("Override locale settings"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->override_locale_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->override_locale_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->override_locale_checkbutton), "toggled",
                      G_CALLBACK (override_locale_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->override_locale_checkbutton, 0, 4, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

	i++;

    sprintf(tmpbuf, "%s:", _("Date format"));
    appGUI->opt->override_locale_label_1 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->override_locale_label_1);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->override_locale_label_1, 0, 1, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->override_locale_label_1), 0, 0.5);

    appGUI->opt->date_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->date_format_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->date_format_combobox), "changed",
                      G_CALLBACK(date_format_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->date_format_combobox, 1, 2, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("DD-MM-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("MM-DD-YYYY"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-MM-DD"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->date_format_combobox), _("YYYY-DD-MM"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->date_format_combobox), config.date_format);

	i++;

    sprintf(tmpbuf, "%s:", _("Time format"));
    appGUI->opt->override_locale_label_2 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->override_locale_label_2);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->override_locale_label_2, 0, 1, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->override_locale_label_2), 0, 0.5);

    appGUI->opt->time_format_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->time_format_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->time_format_combobox), "changed",
                      G_CALLBACK(time_format_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->time_format_combobox, 1, 2, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    sprintf(tmpbuf, "24 %s", _("hours"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->time_format_combobox), tmpbuf);
    sprintf(tmpbuf, "12 %s", _("hours"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->time_format_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->time_format_combobox), config.time_format);

	i++;

#ifdef HAVE_GTKSPELL

    sprintf (tmpbuf, "%s:", _("Spell checker language"));
    appGUI->opt->override_locale_label_3 = gtk_label_new (tmpbuf);
    gtk_widget_show (appGUI->opt->override_locale_label_3);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->override_locale_label_3, 0, 1, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->opt->override_locale_label_3), 0, 0.5);

    appGUI->opt->entry_spell_lang = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->entry_spell_lang);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_spell_lang), "activate",
                      G_CALLBACK (spell_checker_entry_changed_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_spell_lang), "key_press_event",
                      G_CALLBACK (spell_checker_key_press_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_spell_lang, 1, 2, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_spell_lang), MAXNAME);

    i++;

#endif  /* HAVE_GTKSPELL */

    appGUI->opt->enable_tooltips_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable tooltips"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->enable_tooltips_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->enable_tooltips_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->enable_tooltips_checkbutton), "toggled",
                      G_CALLBACK (enable_tooltips_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->enable_tooltips_checkbutton, 0, 4, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

	i++;

    appGUI->opt->remember_latest_tab_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember last selected page"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->remember_latest_tab_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->remember_latest_tab_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->remember_latest_tab_checkbutton), "toggled",
                      G_CALLBACK (remember_latest_tab_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->remember_latest_tab_checkbutton, 0, 4, i, i+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

	i++;

    appGUI->opt->save_data_checkbutton = gtk_check_button_new_with_mnemonic (_("Save data after every modification"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->save_data_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->save_data_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->save_data_checkbutton), "toggled",
                      G_CALLBACK (save_data_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->save_data_checkbutton, 0, 4, i, i+1,
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

    helpers_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (helpers_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), helpers_vbox);

    table = gtk_table_new (3, 2, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (helpers_vbox), table, FALSE, FALSE, 0);
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

	sprintf (tmpbuf, "%s:", _("Sound player"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->entry_web_browser = gtk_entry_new ();
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->entry_web_browser, _("The %s pattern will be replaced with web address"));
	}
    gtk_widget_show (appGUI->opt->entry_web_browser);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_web_browser), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_web_browser), "key_press_event",
                      G_CALLBACK (helper_www_key_press_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_web_browser, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_web_browser), MAXHELPERCMD);

    appGUI->opt->entry_email_client = gtk_entry_new ();
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->entry_email_client, _("The %s pattern will be replaced with e-mail address"));
	}
    gtk_widget_show (appGUI->opt->entry_email_client);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_email_client), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_email_client), "key_press_event",
                      G_CALLBACK (helper_email_key_press_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_email_client, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_email_client), MAXHELPERCMD);

	appGUI->opt->entry_sound_player = gtk_entry_new ();
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->entry_sound_player, _("The %s pattern will be replaced with sound filename"));
	}
    gtk_widget_show (appGUI->opt->entry_sound_player);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_sound_player), "activate",
                      G_CALLBACK (helpers_entry_changed_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->opt->entry_sound_player), "key_press_event",
                      G_CALLBACK (helper_sound_key_press_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->entry_sound_player, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (appGUI->opt->entry_sound_player), MAXHELPERCMD);

#if defined(BACKUP_SUPPORT) && defined(HAVE_LIBGRINGOTTS)

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->general_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Backup"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

	hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_container_add (GTK_CONTAINER (alignment), hbuttonbox);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_START);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 4);

    create_button = gtk_button_new_with_label (_("Create"));
    GTK_WIDGET_UNSET_FLAGS (create_button, GTK_CAN_FOCUS);
    gtk_widget_show (create_button);
    g_signal_connect (create_button, "clicked", G_CALLBACK(button_create_backup_cb), appGUI);
    gtk_container_add (GTK_CONTAINER(hbuttonbox), create_button);

    restore_button = gtk_button_new_with_label (_("Restore"));
    GTK_WIDGET_UNSET_FLAGS (restore_button, GTK_CAN_FOCUS);
    gtk_widget_show (restore_button);
    g_signal_connect (restore_button, "clicked", G_CALLBACK(button_restore_backup_cb), appGUI);
    gtk_container_add (GTK_CONTAINER(hbuttonbox), restore_button);

#endif  /* BACKUP_SUPPORT && HAVE_LIBGRINGOTTS */

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

    system_tray_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (system_tray_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), system_tray_vbox);

    table = gtk_table_new (3, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (system_tray_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    appGUI->opt->enable_systray_checkbutton = gtk_check_button_new_with_mnemonic (_("Enable system tray"));
    GTK_WIDGET_UNSET_FLAGS (appGUI->opt->enable_systray_checkbutton, GTK_CAN_FOCUS);
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
    gtk_widget_set_sensitive (appGUI->opt->start_minimised_checkbutton, config.enable_systray);

	appGUI->opt->blink_on_events_checkbutton = gtk_check_button_new_with_mnemonic (_("Blink on events"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->blink_on_events_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->blink_on_events_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->blink_on_events_checkbutton), "toggled",
                      G_CALLBACK (blink_on_events_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->blink_on_events_checkbutton, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_sensitive (appGUI->opt->blink_on_events_checkbutton, config.enable_systray);

	appGUI->opt->ignore_day_note_events_checkbutton = gtk_check_button_new_with_mnemonic (_("Ignore day note events"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ignore_day_note_events_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ignore_day_note_events_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->ignore_day_note_events_checkbutton), "toggled",
                      G_CALLBACK (ignore_day_note_events_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->ignore_day_note_events_checkbutton, 0, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_widget_set_sensitive (appGUI->opt->ignore_day_note_events_checkbutton, config.enable_systray);
}

/*------------------------------------------------------------------------------*/

