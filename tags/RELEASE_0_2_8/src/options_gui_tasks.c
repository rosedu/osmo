
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

#include "calendar.h"
#include "i18n.h"
#include "options_gui.h"
#include "options_gui_tasks.h"
#include "options_prefs.h"
#include "stock_icons.h"
#include "tasks.h"
#include "tasks_utils.h"
#include "utils.h"
#include "utils_gui.h"

#ifdef TASKS_ENABLED

/*------------------------------------------------------------------------------*/

void
ti_font_select_cb (GtkWidget *widget, gpointer data) {

GtkWidget *font_selector;
gchar *font_name;
gint response;

    GUI *appGUI = (GUI *)data;

    font_selector = gtk_font_selection_dialog_new (_("Select a font..."));
    gtk_window_set_modal(GTK_WINDOW(font_selector), TRUE);
    gtk_window_set_position(GTK_WINDOW(font_selector), GTK_WIN_POS_MOUSE);
    gtk_window_set_transient_for(GTK_WINDOW(font_selector), GTK_WINDOW(appGUI->main_window));
    gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_selector), config.task_info_font);
    gtk_widget_show (font_selector);
    response = gtk_dialog_run (GTK_DIALOG (font_selector));

    if (response == GTK_RESPONSE_OK) {

        font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(font_selector));
        strncpy(config.task_info_font, font_name, MAXFONTNAME-1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->ti_font_entry), font_name);
        g_free (font_name);

        /* task info font */
        g_object_set (G_OBJECT (appGUI->tsk->font_tag_object), "font", (gchar *)config.task_info_font, NULL);
    }

    gtk_widget_destroy (font_selector);
}

/*------------------------------------------------------------------------------*/

void
bold_items_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    config.tasks_high_in_bold = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_bold_items_checkbutton));
    apply_task_attributes (appGUI);
}

/*------------------------------------------------------------------------------*/

void
hide_items_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton), FALSE);
    } 
}

/*------------------------------------------------------------------------------*/

void
delete_items_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton), FALSE);
    } 
}

/*------------------------------------------------------------------------------*/

void
add_item_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    config.add_edit = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
}

/*------------------------------------------------------------------------------*/

void
remember_category_cb (GtkToggleButton *togglebutton, gpointer user_data) {

    config.remember_category_in_tasks = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
}

/*------------------------------------------------------------------------------*/

void
tasks_category_add_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
gint i;
const gchar *category_name;
gchar *item;

    GUI *appGUI = (GUI *)data;

    category_name = gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry));

    if(strlen(category_name)) {

        i = 0;

        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, i++)) {
            gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, TC_COLUMN_NAME, &item, -1);
            if (!strcmp(category_name, item)) {
                g_free(item);
                return;
            }
            g_free(item);
        }

        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, NULL, 0);

        gtk_list_store_append(appGUI->opt->tasks_category_store, &iter);
        gtk_list_store_set(appGUI->opt->tasks_category_store, &iter, 
                           TC_COLUMN_NAME, category_name, TC_COLUMN_CALENDAR, TRUE, TC_COLUMN_TASKS, TRUE, -1);
        gtk_entry_set_text(GTK_ENTRY(appGUI->opt->tasks_category_entry), "");
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, FALSE);

        utl_gui_create_category_combobox (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), 
										  appGUI->opt->tasks_category_store, FALSE);

        gtk_combo_box_set_active (GTK_COMBO_BOX (appGUI->tsk->cf_combobox), 0);

        apply_task_attributes (appGUI);

        gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
        update_tasks_number (appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
tasks_category_remove_cb (GtkWidget *widget, gpointer data) {

GtkTreeIter iter;
GtkTreePath *path;

    GUI *appGUI = (GUI *)data;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), &path, NULL);

    if (path != NULL) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(appGUI->opt->tasks_category_store), &iter, path);
        gtk_list_store_remove(appGUI->opt->tasks_category_store, &iter);
        gtk_tree_path_free(path);
        refresh_tasks (appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gint 
tasks_category_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

gboolean state;

    GUI *appGUI = (GUI *)data;

    if(strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry)))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, state);

    if (event->keyval == GDK_Return) {
        if (strlen(gtk_entry_get_text(GTK_ENTRY(appGUI->opt->tasks_category_entry)))) {
            tasks_category_add_cb (NULL, appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
due_today_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->due_today_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_today_color, tmpbuf, MAXCOLORNAME-1);
    refresh_tasks (appGUI);

}

/*------------------------------------------------------------------------------*/

void
due_7days_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->due_7days_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.due_7days_color, tmpbuf, MAXCOLORNAME-1);
    refresh_tasks (appGUI);

}

/*------------------------------------------------------------------------------*/

void
past_due_color_changed_cb (GtkColorButton *widget, gpointer user_data) {

gchar tmpbuf[BUFFER_SIZE];
GdkColor color;

    GUI *appGUI = (GUI *)user_data;

    gtk_color_button_get_color(GTK_COLOR_BUTTON(appGUI->opt->past_due_color_picker), &color);
    sprintf(tmpbuf, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    strncpy(config.past_due_color, tmpbuf, MAXCOLORNAME-1);
    refresh_tasks (appGUI);

}

/*------------------------------------------------------------------------------*/

void
visible_columns_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tsk_visible_due_date_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_due_date_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_DUE_DATE], config.tsk_visible_due_date_column);
    config.tsk_visible_type_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_type_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_TYPE], config.tsk_visible_type_column);
    config.tsk_visible_category_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_category_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_CATEGORY], config.tsk_visible_category_column);
    config.tsk_visible_priority_column = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->vc_priority_checkbutton));
    gtk_tree_view_column_set_visible (appGUI->tsk->tasks_columns[TA_COLUMN_PRIORITY], config.tsk_visible_priority_column);

	set_tasks_columns_width (appGUI);
}

/*------------------------------------------------------------------------------*/

void
tasks_sort_order_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tasks_sorting_order = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_order_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DONE, config.tasks_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));
}

/*------------------------------------------------------------------------------*/

void
tasks_sort_mode_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.tasks_sorting_mode = gtk_combo_box_get_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_mode_combobox));
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DUE_DATE, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_PRIORITY, config.tasks_sorting_order);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->tsk->tasks_sort, 
                                          TA_COLUMN_DONE, config.tasks_sorting_order);

    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->tsk->tasks_filter));

}

/*------------------------------------------------------------------------------*/

void
hide_delete_changed_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.hide_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_hide_items_checkbutton));
    gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (appGUI->tsk->tasks_filter));
    config.delete_completed = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->opt->ct_delete_items_checkbutton));

}

/*------------------------------------------------------------------------------*/

void
tasks_category_selected_cb (GtkTreeSelection *selection, gpointer data) {

GtkTreeIter iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, FALSE);
    }
}

/*------------------------------------------------------------------------------*/

void 
tasks_category_cell_edited_cb (GtkCellRendererText *renderer, gchar *path, 
                               gchar *new_text, gpointer user_data) {

GtkTreeIter iter;
GtkTreeModel *model;
  
    GUI *appGUI = (GUI *)user_data;

    if (g_ascii_strcasecmp (new_text, "") != 0) {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview));
        if (gtk_tree_model_get_iter_from_string (model, &iter, path)) {
            gtk_list_store_set(appGUI->opt->tasks_category_store, &iter, TC_COLUMN_NAME, new_text, -1);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
postpone_time_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.postpone_time = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(appGUI->opt->postpone_time_spinbutton));

}

/*------------------------------------------------------------------------------*/

void
sound_alarm_repeat_changed_cb (GtkSpinButton *spinbutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (!appGUI->opt->callback_active) return;

    config.sound_alarm_repeat = (gint) gtk_spin_button_get_value (GTK_SPIN_BUTTON(appGUI->opt->sound_alarm_repeat_spinbutton));

}

/*------------------------------------------------------------------------------*/

void
tsk_show_in_calendar_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {

GtkTreePath *path;
GtkTreeIter  iter;
gboolean done_status;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;
    model = GTK_TREE_MODEL(appGUI->opt->tasks_category_store);

    path = gtk_tree_path_new_from_string (path_str);

    if (path != NULL) {
        gtk_tree_model_get_iter (model, &iter, path);   /* get toggled iter */
        gtk_tree_model_get (model, &iter, TC_COLUMN_CALENDAR, &done_status, -1);
        gtk_list_store_set (GTK_LIST_STORE(model), &iter, TC_COLUMN_CALENDAR, !done_status, -1);
        cal_set_day_info (appGUI);
        cal_refresh_marks (appGUI);
        gtk_tree_path_free (path);
    }
}

/*------------------------------------------------------------------------------*/

void
tsk_show_in_tasks_list_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {

GtkTreePath *path;
GtkTreeIter  iter;
gboolean done_status;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)data;
    model = GTK_TREE_MODEL(appGUI->opt->tasks_category_store);

    path = gtk_tree_path_new_from_string (path_str);

    if (path != NULL) {
        gtk_tree_model_get_iter (model, &iter, path);   /* get toggled iter */
        gtk_tree_model_get (model, &iter, TC_COLUMN_TASKS, &done_status, -1);
        gtk_list_store_set (GTK_LIST_STORE(model), &iter, TC_COLUMN_TASKS, !done_status, -1);
        refresh_tasks (appGUI);
        gtk_tree_path_free (path);
    }
}

/*------------------------------------------------------------------------------*/

gint
global_notification_entry_key_release_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
    utl_gui_update_command_status (appGUI->opt->global_notification_cmd_entry, 
								   appGUI->opt->global_notification_valid_image, appGUI);
    
	g_strlcpy (config.global_notification_command, 
               gtk_entry_get_text(GTK_ENTRY(appGUI->opt->global_notification_cmd_entry)), MAXHELPERCMD);

	return FALSE;
}

/*------------------------------------------------------------------------------*/

void
gui_create_tasks_options_page   (GtkWidget *vbox, GUI *appGUI) {

GtkWidget           *appearance_vbox, *categories_vbox;
GtkWidget           *sorting_vbox, *tasks_opt_vbox, *visible_columns_vbox;
GtkWidget           *label;
GtkWidget           *colors_hbox;
GtkWidget           *valid_hbox;
GtkWidget           *frame;
GtkWidget           *table;
GtkWidget           *alignment;
GtkWidget           *ti_font_button;
GtkWidget           *scrolledwindow;
GtkWidget           *tasks_category_table;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
GdkColor            color;
GtkObject           *postpone_time_spinbutton_adj;
GtkObject           *sound_alarm_repeat_spinbutton_adj;

gint i;
gchar tmpbuf[BUFFER_SIZE];


    appGUI->opt->tasks_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), appGUI->opt->tasks_vbox, FALSE, FALSE, 0);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
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

    table = gtk_table_new (4, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (appearance_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

    colors_hbox = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (colors_hbox);
    gtk_table_attach (GTK_TABLE (table), colors_hbox, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->due_today_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->due_today_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->due_today_color_picker), "color-set",
                      G_CALLBACK(due_today_color_changed_cb), appGUI);
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->due_today_color_picker, _("Color of items that are due today"));
	}
    gdk_color_parse(config.due_today_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->due_today_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (colors_hbox), appGUI->opt->due_today_color_picker, FALSE, FALSE, 0);

    appGUI->opt->due_7days_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->due_7days_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->due_7days_color_picker), "color-set",
                      G_CALLBACK(due_7days_color_changed_cb), appGUI);
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->due_7days_color_picker, _("Color of items that are due in the next 7 days"));
	}
    gdk_color_parse(config.due_7days_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->due_7days_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (colors_hbox), appGUI->opt->due_7days_color_picker, FALSE, FALSE, 0);

    appGUI->opt->past_due_color_picker = gtk_color_button_new ();
    gtk_widget_show (appGUI->opt->past_due_color_picker);
    g_signal_connect (G_OBJECT (appGUI->opt->past_due_color_picker), "color-set",
                      G_CALLBACK(past_due_color_changed_cb), appGUI);
	if (config.enable_tooltips) {
		gtk_widget_set_tooltip_text (appGUI->opt->past_due_color_picker, _("Color of items that are past due"));
	}
    gdk_color_parse(config.past_due_color, &color);
    gtk_color_button_set_color (GTK_COLOR_BUTTON (appGUI->opt->past_due_color_picker), &color);
    gtk_box_pack_start (GTK_BOX (colors_hbox), appGUI->opt->past_due_color_picker, FALSE, FALSE, 0);

    appGUI->opt->ti_font_entry = gtk_entry_new ();
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ti_font_entry, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ti_font_entry);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->ti_font_entry, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    if (config.default_stock_icons) {
        ti_font_button = utl_gui_stock_button (GTK_STOCK_SELECT_FONT, FALSE);
    } else {
        ti_font_button = utl_gui_stock_button (OSMO_STOCK_BUTTON_SELECT_FONT, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(ti_font_button, GTK_CAN_FOCUS);
    gtk_widget_show (ti_font_button);
    g_signal_connect (G_OBJECT (ti_font_button), "clicked",
                      G_CALLBACK (ti_font_select_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), ti_font_button, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    sprintf(tmpbuf, "%s:", _("Task info font"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Colors"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->ct_bold_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Show in bold tasks with high priority"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_bold_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_bold_items_checkbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->ct_bold_items_checkbutton, 0, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_bold_items_checkbutton), "toggled",
                      G_CALLBACK (bold_items_cb), appGUI);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Visible columns"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    visible_columns_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (visible_columns_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), visible_columns_vbox);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (visible_columns_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    appGUI->opt->vc_due_date_checkbutton = gtk_check_button_new_with_mnemonic (_("Due date"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_due_date_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_due_date_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_due_date_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_due_date_checkbutton, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->vc_type_checkbutton = gtk_check_button_new_with_mnemonic (_("Type"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_type_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_type_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_type_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_type_checkbutton, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->vc_priority_checkbutton = gtk_check_button_new_with_mnemonic (_("Priority"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_priority_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_priority_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_priority_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_priority_checkbutton, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);

    appGUI->opt->vc_category_checkbutton = gtk_check_button_new_with_mnemonic (_("Category"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->vc_category_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->vc_category_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->vc_category_checkbutton), "toggled",
                      G_CALLBACK (visible_columns_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->vc_category_checkbutton, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);


    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Categories"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    categories_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (categories_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), categories_vbox);

    tasks_category_table = gtk_table_new (4, 3, FALSE);
    gtk_widget_show (tasks_category_table);
    gtk_box_pack_start (GTK_BOX (categories_vbox), tasks_category_table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (tasks_category_table), 8);
    gtk_table_set_row_spacings (GTK_TABLE (tasks_category_table), 8);
    gtk_table_set_col_spacings (GTK_TABLE (tasks_category_table), 4);

    appGUI->opt->tasks_category_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->tasks_category_entry);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_entry, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_category_entry), "key_release_event",
                      G_CALLBACK (tasks_category_entry_key_release_cb), appGUI);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_table_attach (GTK_TABLE (tasks_category_table), scrolledwindow, 0, 3, 0, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->opt->tasks_category_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN);

    appGUI->opt->tasks_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->opt->tasks_category_store));
    appGUI->opt->tasks_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview));
    gtk_widget_show (appGUI->opt->tasks_category_treeview);
    g_signal_connect(G_OBJECT(appGUI->opt->tasks_category_select), "changed", 
                     G_CALLBACK(tasks_category_selected_cb), appGUI);   
    gtk_container_add (GTK_CONTAINER (scrolledwindow), appGUI->opt->tasks_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->opt->tasks_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), TRUE);
    gtk_tree_view_set_reorderable (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), TRUE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (appGUI->opt->tasks_category_treeview), FALSE);
    gtk_widget_set_size_request (appGUI->opt->tasks_category_treeview, -1, 120);

    renderer = gtk_cell_renderer_text_new();
    g_object_set (renderer, "editable", TRUE, "editable-set", TRUE, NULL);
    g_signal_connect (G_OBJECT (renderer), "edited", G_CALLBACK (tasks_category_cell_edited_cb), appGUI);

    column = gtk_tree_view_column_new_with_attributes(_("Name"), renderer, "text", TC_COLUMN_NAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview), column);
    gtk_tree_view_column_set_expand (column, TRUE);

    renderer = gtk_cell_renderer_toggle_new();    /* Show in calendar */
    column = gtk_tree_view_column_new_with_attributes(_("Calendar"), renderer, "active", TC_COLUMN_CALENDAR, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview), column);
    
    g_signal_connect (renderer, "toggled", G_CALLBACK (tsk_show_in_calendar_toggled), appGUI);

    renderer = gtk_cell_renderer_toggle_new();    /* Show in tasks list */
    column = gtk_tree_view_column_new_with_attributes(_("Tasks"), renderer, "active", TC_COLUMN_TASKS, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->opt->tasks_category_treeview), column);

    g_signal_connect (renderer, "toggled", G_CALLBACK (tsk_show_in_tasks_list_toggled), appGUI);

    if (config.default_stock_icons) {
        appGUI->opt->tasks_category_add_button = utl_gui_stock_button (GTK_STOCK_ADD, FALSE);
    } else {
        appGUI->opt->tasks_category_add_button = utl_gui_stock_button (OSMO_STOCK_BUTTON_ADD, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_category_add_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->tasks_category_add_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_add_button, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->tasks_category_add_button, "clicked", 
                     G_CALLBACK(tasks_category_add_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->tasks_category_add_button, FALSE);

    if (config.default_stock_icons) {
        appGUI->opt->tasks_category_remove_button = utl_gui_stock_button (GTK_STOCK_REMOVE, FALSE);
    } else {
        appGUI->opt->tasks_category_remove_button = utl_gui_stock_button (OSMO_STOCK_BUTTON_REMOVE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->tasks_category_remove_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->tasks_category_remove_button);
    gtk_table_attach (GTK_TABLE (tasks_category_table), appGUI->opt->tasks_category_remove_button, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(appGUI->opt->tasks_category_remove_button, "clicked", 
                     G_CALLBACK(tasks_category_remove_cb), appGUI);
    gtk_widget_set_sensitive(appGUI->opt->tasks_category_remove_button, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Sorting"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    sorting_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (sorting_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), sorting_vbox);

    table = gtk_table_new (1, 5, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (sorting_vbox), table, FALSE, FALSE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "%s:", _("Order"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "%s:", _("Mode"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->opt->tasks_sort_order_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->tasks_sort_order_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_sort_order_combobox), "changed",
                      G_CALLBACK(tasks_sort_order_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tasks_sort_order_combobox, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_order_combobox), _("Ascending"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_order_combobox), _("Descending"));
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_order_combobox), config.tasks_sorting_order);

    appGUI->opt->tasks_sort_mode_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->opt->tasks_sort_mode_combobox);
    g_signal_connect (G_OBJECT (appGUI->opt->tasks_sort_mode_combobox), "changed",
                      G_CALLBACK(tasks_sort_mode_changed_cb), appGUI);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->tasks_sort_mode_combobox, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                      (GtkAttachOptions) (0), 0, 0);
    sprintf (tmpbuf, "%s, %s, %s", _("Done"), _("Due date"), _("Priority"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Done"), _("Priority"), _("Due date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Priority"), _("Due date"), _("Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Priority"), _("Done"), _("Due date"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Due date"), _("Priority"), _("Done"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    sprintf (tmpbuf, "%s, %s, %s", _("Due date"), _("Done"), _("Priority"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (appGUI->opt->tasks_sort_mode_combobox), tmpbuf);
    gtk_combo_box_set_active (GTK_COMBO_BOX(appGUI->opt->tasks_sort_mode_combobox), config.tasks_sorting_mode);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    sprintf(tmpbuf, "<b>%s</b>", _("Tasks options"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    tasks_opt_vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (tasks_opt_vbox);
    gtk_container_add (GTK_CONTAINER (alignment), tasks_opt_vbox);

    appGUI->opt->ct_hide_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Hide completed tasks"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_hide_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_hide_items_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (tasks_opt_vbox), appGUI->opt->ct_hide_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_hide_items_checkbutton), "toggled",
                      G_CALLBACK (hide_items_cb), appGUI);

    appGUI->opt->ct_delete_items_checkbutton = gtk_check_button_new_with_mnemonic (_("Delete completed tasks without confirmation"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_delete_items_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_delete_items_checkbutton);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK (hide_delete_changed_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (tasks_opt_vbox), appGUI->opt->ct_delete_items_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_delete_items_checkbutton), "toggled",
                      G_CALLBACK (delete_items_cb), appGUI);

    appGUI->opt->ct_add_item_checkbutton = gtk_check_button_new_with_mnemonic (_("Add new task when double clicked on tasks list"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_add_item_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_add_item_checkbutton);
    gtk_box_pack_start (GTK_BOX (tasks_opt_vbox), appGUI->opt->ct_add_item_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_add_item_checkbutton), "toggled",
                      G_CALLBACK (add_item_cb), appGUI);

    appGUI->opt->ct_remember_category_checkbutton = gtk_check_button_new_with_mnemonic (_("Remember the last selected category"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->opt->ct_remember_category_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->opt->ct_remember_category_checkbutton);
    gtk_box_pack_start (GTK_BOX (tasks_opt_vbox), appGUI->opt->ct_remember_category_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (appGUI->opt->ct_remember_category_checkbutton), "toggled",
                      G_CALLBACK (remember_category_cb), appGUI);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (appGUI->opt->tasks_vbox), frame, FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s</b>", _("Reminder options"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 12, 12, 0);

    table = gtk_table_new (3, 4, FALSE);
    gtk_widget_show (table);
    gtk_container_add (GTK_CONTAINER (alignment), table);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 8);

	i = 0;

    g_snprintf (tmpbuf, BUFFER_SIZE, "%s:", _("Postpone time"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    postpone_time_spinbutton_adj = gtk_adjustment_new (0, 0, 1440, 1, 10, 0);
    appGUI->opt->postpone_time_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (postpone_time_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->postpone_time_spinbutton), "value-changed",
                      G_CALLBACK (postpone_time_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->postpone_time_spinbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->postpone_time_spinbutton, 1, 2, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (appGUI->opt->postpone_time_spinbutton), TRUE);

    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->postpone_time_spinbutton), config.postpone_time);

    g_snprintf (tmpbuf, BUFFER_SIZE, "%s (%s)", _("minutes"), _("0 for disable"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

	i++;

    g_snprintf (tmpbuf, BUFFER_SIZE, "%s:", _("Repeat sound alarm"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sound_alarm_repeat_spinbutton_adj = gtk_adjustment_new (0, 0, 100, 1, 10, 0);
    appGUI->opt->sound_alarm_repeat_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (sound_alarm_repeat_spinbutton_adj), 1, 0);
    g_signal_connect (G_OBJECT (appGUI->opt->sound_alarm_repeat_spinbutton), "value-changed",
                      G_CALLBACK (sound_alarm_repeat_changed_cb), appGUI);
    gtk_widget_show (appGUI->opt->sound_alarm_repeat_spinbutton);
    gtk_table_attach (GTK_TABLE (table), appGUI->opt->sound_alarm_repeat_spinbutton, 1, 2, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (appGUI->opt->sound_alarm_repeat_spinbutton), TRUE);

    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->opt->sound_alarm_repeat_spinbutton), config.sound_alarm_repeat);

    g_snprintf (tmpbuf, BUFFER_SIZE, "%s (%s)", _("times"), _("0 for disable"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

	i++;

    g_snprintf (tmpbuf, BUFFER_SIZE, "%s:", _("Global notification command"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    valid_hbox = gtk_hbox_new (FALSE, 2);
    gtk_widget_show (valid_hbox);
    gtk_table_attach (GTK_TABLE (table), valid_hbox, 1, 3, i, i+1,
                    (GtkAttachOptions) (GTK_FILL | GTK_EXPAND),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

    appGUI->opt->global_notification_cmd_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->opt->global_notification_cmd_entry);
    g_signal_connect (G_OBJECT (appGUI->opt->global_notification_cmd_entry), "key_release_event",
                      G_CALLBACK (global_notification_entry_key_release_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (valid_hbox), appGUI->opt->global_notification_cmd_entry, TRUE, TRUE, 0);

    appGUI->opt->global_notification_valid_image = gtk_image_new ();
    gtk_widget_show (appGUI->opt->global_notification_valid_image);
    gtk_box_pack_start (GTK_BOX (valid_hbox), appGUI->opt->global_notification_valid_image, FALSE, FALSE, 0);
}

/*------------------------------------------------------------------------------*/

#endif  /* TASKS_ENABLED */

