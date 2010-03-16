
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

#include "notes.h"
#include "i18n.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "options_prefs.h"
#include "notes_items.h"
#include "calendar_utils.h"
#include "utils.h"
#include "stock_icons.h"

#ifdef HAVE_LIBGRINGOTTS

#include <libgringotts.h>

/*------------------------------------------------------------------------------*/

void
notes_show_selector_editor (gint mode, GUI *appGUI) {

GtkTextBuffer *buffer;
GtkTextMark *cursor;
GtkTextIter iter;
GtkTreeIter n_iter;
GtkTreePath *path, *sort_path, *filter_path;
gchar *name;
gint line;
gchar tmpbuf[BUFFER_SIZE];

    if (mode == SELECTOR) { 

        gtk_widget_show (appGUI->nte->vbox_selector);
        gtk_widget_hide (appGUI->nte->vbox_editor);
        gtk_widget_grab_focus (appGUI->nte->notes_list);
        appGUI->nte->editor_active = FALSE;

    } else if (mode == EDITOR) {

        gtk_widget_hide (appGUI->nte->find_hbox);
        gtk_widget_hide (appGUI->nte->vbox_selector);
        gtk_widget_show (appGUI->nte->vbox_editor);
        gtk_entry_set_text(GTK_ENTRY(appGUI->nte->find_entry), "");
        gtk_widget_grab_focus (appGUI->nte->editor_textview);
        appGUI->nte->editor_active = TRUE;

        gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->nte->notes_list), &sort_path, NULL);

        if (sort_path == NULL) return;
        filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->nte->notes_sort), sort_path);
        gtk_tree_path_free (sort_path);

        if (filter_path == NULL) return;
        path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter), filter_path);
        gtk_tree_path_free (filter_path);

        gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->nte->notes_list_store), &n_iter, path);
        gtk_tree_model_get (GTK_TREE_MODEL(appGUI->nte->notes_list_store), &n_iter, 
                            N_COLUMN_NAME, &name, 
                            N_COLUMN_EDITOR_LINE, &line, -1);
        sprintf (tmpbuf, "<i>%s</i>", name);
        gtk_label_set_markup (GTK_LABEL (appGUI->nte->title_label), tmpbuf);
        g_free(name);
        gtk_tree_path_free (path);

        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));
        gtk_text_buffer_get_iter_at_line (GTK_TEXT_BUFFER (buffer), &iter, line);
        gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER (buffer), &iter);
        cursor = gtk_text_buffer_get_mark (buffer, "insert");
        gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (appGUI->nte->editor_textview), cursor, 0.0, TRUE, 0.0, 0.0);
    }
}

/*------------------------------------------------------------------------------*/

void 
editor_save_buffer_cb (GtkWidget *widget, gpointer user_data) {

GtkTextBuffer *buffer;
gint ret;
guchar *text;
guint32 current_date;
gint current_time;
GtkTreeIter iter;
GtkTreeIter sort_child_iter, filter_child_iter;
GtkTreeModel *model;

    GUI *appGUI = (GUI *)user_data;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));

    if (appGUI->nte->keyholder != NULL && appGUI->nte->context != NULL && appGUI->nte->filename != NULL) {

        text = (unsigned char*) gui_text_buffer_get_text_with_tags (buffer);

        ret = grg_encrypt_file (appGUI->nte->context, appGUI->nte->keyholder, 
                                (unsigned char*) appGUI->nte->filename, 
                                (guchar *) text, -1);

        grg_free (appGUI->nte->context, text, -1);

        appGUI->nte->changed = FALSE;
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save"), FALSE);

        if (gtk_tree_selection_get_selected (appGUI->nte->notes_list_selection, &model, &iter)) {

            current_date = utl_get_current_julian ();
            current_time = get_seconds_for_today ();

            gtk_tree_model_sort_convert_iter_to_child_iter (GTK_TREE_MODEL_SORT(appGUI->nte->notes_sort), 
                                                              &sort_child_iter, &iter);
            gtk_tree_model_filter_convert_iter_to_child_iter (GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter), 
                                                              &filter_child_iter, &sort_child_iter);

            gtk_list_store_set(appGUI->nte->notes_list_store, &filter_child_iter, 
                               N_COLUMN_LAST_CHANGES_DATE, get_date_time_str (current_date, current_time),
                               N_COLUMN_LAST_CHANGES_DATE_JULIAN, current_date,
                               N_COLUMN_LAST_CHANGES_TIME, current_time, -1);
        }
    }
}

/*------------------------------------------------------------------------------*/

void 
editor_find_text_show_cb (GtkWidget *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_widget_show (appGUI->nte->find_hbox);
    gtk_widget_grab_focus (appGUI->nte->find_entry);

}

/*------------------------------------------------------------------------------*/

void 
editor_find_text_hide_cb (GtkWidget *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
        
    gui_change_bg_widget_state (appGUI->nte->find_entry, NULL, appGUI);

    gtk_widget_hide (appGUI->nte->find_hbox);
    gtk_widget_grab_focus (appGUI->nte->editor_textview);

}

/*------------------------------------------------------------------------------*/

void 
editor_close_cb (GtkWidget *widget, gpointer user_data) {

GtkTextBuffer *buffer;
GtkTextIter iter_s, iter_e, l_iter;
GtkTreeIter n_iter;
GtkTreePath *path, *sort_path, *filter_path;
gint response, line;
GtkTextMark *cursor;
gchar tmpbuf[BUFFER_SIZE];

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->nte->changed == TRUE) {

        sprintf (tmpbuf, "%s\n\n%s", _("The note has changed."), _("Do you want to save it?"));

        response = gui_create_dialog(GTK_MESSAGE_QUESTION, tmpbuf, GTK_WINDOW(appGUI->main_window));

        if (response == GTK_RESPONSE_YES) {   
            editor_save_buffer_cb (NULL, appGUI);
        } else if (response != GTK_RESPONSE_NO) {
            return;
        }
    }

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));

    if (appGUI->nte->keyholder != NULL && appGUI->nte->context != NULL && appGUI->nte->filename != NULL) {
        grg_key_free (appGUI->nte->context, appGUI->nte->keyholder);
        appGUI->nte->keyholder = NULL;
	    grg_context_free (appGUI->nte->context);
        appGUI->nte->context = NULL;
        g_free(appGUI->nte->filename);
        appGUI->nte->filename = NULL;
    }

    cursor = gtk_text_buffer_get_mark (buffer, "insert");
    gtk_text_buffer_get_iter_at_mark (buffer, &l_iter, cursor);
    line = gtk_text_iter_get_line (&l_iter);

    gtk_text_buffer_get_bounds(buffer, &iter_s, &iter_e);
    gtk_text_buffer_delete(buffer, &iter_s, &iter_e);  
    appGUI->nte->buffer_check_modify_enable = FALSE;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->nte->notes_list), &sort_path, NULL);

    if (sort_path == NULL) return;
    filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->nte->notes_sort), sort_path);
    gtk_tree_path_free (sort_path);

    if (filter_path == NULL) return;
    path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter), filter_path);
    gtk_tree_path_free (filter_path);

    gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->nte->notes_list_store), &n_iter, path);
    gtk_list_store_set (appGUI->nte->notes_list_store, &n_iter, 
                        N_COLUMN_EDITOR_LINE, line, -1);
    gtk_tree_path_free (path);

    notes_show_selector_editor (SELECTOR, appGUI);
}

/*------------------------------------------------------------------------------*/

void 
set_text_attribute_cb (GtkWidget *widget, gpointer user_data) {

GtkTextIter start, end;
GtkTextBuffer *buffer;
gchar *tagname;

    GUI *appGUI = (GUI *)user_data;

    tagname = (gchar*) g_object_get_data (G_OBJECT (widget), "tag");
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));
    gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
    gtk_text_buffer_apply_tag_by_name (buffer, tagname, &start, &end);
    g_signal_emit_by_name(G_OBJECT(buffer), "changed");
}

/*------------------------------------------------------------------------------*/

void
clear_text_attributes_cb (GtkButton *button, gpointer user_data) {

GtkTextIter start, end;
GtkTextBuffer *buffer;

    GUI *appGUI = (GUI *)user_data;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));
    gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
    gtk_text_buffer_remove_all_tags (buffer, &start, &end);
}

/*------------------------------------------------------------------------------*/

void
add_notes_toolbar_selector_widget (GtkUIManager *notes_uim_widget, GtkWidget *widget, gpointer user_data) {

GtkWidget *handle_box;

    GUI *appGUI = (GUI *)user_data;

    if (GTK_IS_TOOLBAR (widget)) {

        appGUI->nte->notes_toolbar_selector = GTK_TOOLBAR (widget);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox_selector), handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", 
                                  G_CALLBACK (gtk_widget_destroy), handle_box);

    } else {
        gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox_selector), widget, FALSE, FALSE, 0);
    }

    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

void
add_notes_toolbar_editor_widget (GtkUIManager *notes_uim_widget, GtkWidget *widget, gpointer user_data) {

GtkWidget *handle_box;

    GUI *appGUI = (GUI *)user_data;

    if (GTK_IS_TOOLBAR (widget)) {

        appGUI->nte->notes_toolbar_editor = GTK_TOOLBAR (widget);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox_editor), handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", 
                                  G_CALLBACK (gtk_widget_destroy), handle_box);

    } else {
        gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox_editor), widget, FALSE, FALSE, 0);
    }

    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

void
notes_add_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    notes_add_entry (appGUI);
}

/*------------------------------------------------------------------------------*/

void
notes_edit_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    notes_edit_dialog_show(appGUI->nte->notes_list, appGUI->nte->notes_filter, appGUI);
}

/*------------------------------------------------------------------------------*/

void
notes_delete_item_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    notes_remove_dialog_show(appGUI->nte->notes_list, appGUI->nte->notes_list_store, appGUI);
}

/*------------------------------------------------------------------------------*/

void
insert_current_date_and_time_cb (GtkWidget *widget, gpointer data) {

gchar temp[BUFFER_SIZE];
time_t tmm;

    GUI *appGUI = (GUI *)data;

    if (config.use_system_date_in_notes == TRUE) {
        tmm = time(NULL);
        gtk_text_buffer_insert_at_cursor (gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview)), 
                                          asctime (localtime (&tmm)), -1);
    } else {
        g_snprintf (temp, BUFFER_SIZE, "%s\n", 
                    get_date_time_full_str (utl_get_current_julian(), get_seconds_for_today()));
        gtk_text_buffer_insert_at_cursor (gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview)), 
                                          temp, -1);
    }
}

/*------------------------------------------------------------------------------*/

void
insert_separator_cb (GtkWidget *widget, gpointer data) {

gint chars, i, sbwidth;
gchar tmpbuf[BUFFER_SIZE];
PangoLayout *layout;
PangoRectangle logical_rect;
GtkWidget *vscrollbar;
GValue *value;

    GUI *appGUI = (GUI *)data;

    value = g_new0 (GValue, 1);
	value = g_value_init (value, G_TYPE_INT);
    vscrollbar = gtk_scrolled_window_get_vscrollbar (GTK_SCROLLED_WINDOW (appGUI->nte->editor_scrolledwindow));
    gtk_widget_style_get_property (vscrollbar, "slider-width", value);
    sbwidth = g_value_get_int (value);
	g_value_unset (value);
    g_free (value);

    memset (tmpbuf, 0, BUFFER_SIZE);

    tmpbuf[0] = config.text_separator;
    layout = gtk_widget_create_pango_layout (widget, NULL);
    pango_layout_set_font_description (layout, appGUI->nte->fd_notes_font);
    pango_layout_set_text (layout, tmpbuf, -1);
    pango_layout_get_pixel_extents (layout, NULL, &logical_rect);

    if (GTK_WIDGET_VISIBLE(vscrollbar) == TRUE) {
        chars = ((appGUI->nte->editor_textview)->allocation.width) / logical_rect.width;
    } else {
        chars = ((appGUI->nte->editor_textview)->allocation.width - sbwidth) / logical_rect.width;
    }
    chars = (chars > BUFFER_SIZE) ? BUFFER_SIZE - 2 : chars;

    g_object_unref (G_OBJECT(layout));

    i = 0;
    tmpbuf[i++] = '\n';
    while (i < chars) tmpbuf[i++] = config.text_separator;
    tmpbuf[i++] = '\n';

    gtk_text_buffer_insert_at_cursor (gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview)), 
                                      tmpbuf, -1);
}

/*------------------------------------------------------------------------------*/

void
text_info_cb (GtkWidget *widget, gpointer user_data)
{
GtkTreeIter n_iter;
GtkTreePath *path, *sort_path, *filter_path;
GtkTextIter start_iter, end_iter;
gchar tmpbuf[BUFFER_SIZE], temp[BUFFER_SIZE];
GtkTextBuffer *buffer;
guint32 modified_date, created_date;
gint modified_time, created_time;
gchar *text;
gchar *modified_date_str, *created_date_str;
PangoLogAttr *attrs;
gint i, words, lines, chars, white_chars, bytes;

	GUI *appGUI = (GUI *) user_data;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));

    if (gtk_text_buffer_get_has_selection (buffer) == TRUE) {
        gtk_text_buffer_get_selection_bounds (buffer, &start_iter, &end_iter);
    } else {
        gtk_text_buffer_get_start_iter (buffer, &start_iter);
        gtk_text_buffer_get_end_iter (buffer, &end_iter);
    }

    text = gtk_text_buffer_get_text (GTK_TEXT_BUFFER (buffer), &start_iter, &end_iter, FALSE);
    if (text == NULL) return;

    words = 0;
    white_chars = 0;
    lines = gtk_text_buffer_get_line_count (buffer);
    bytes = strlen (text);
    chars = g_utf8_strlen (text, -1);

    attrs = g_new0 (PangoLogAttr, chars + 1);   /* based on code by Paolo Maggi */
    pango_get_log_attrs (text, -1, 0, pango_language_from_string ("C"), attrs, chars + 1);

    for (i=0; i < chars; i++) {
        if (attrs[i].is_white) white_chars++;
        if (attrs[i].is_word_start) words++;
    }

    if (chars == 0) lines = 0;

    gtk_tree_view_get_cursor (GTK_TREE_VIEW (appGUI->nte->notes_list), &sort_path, NULL);

    if (sort_path == NULL) return;
    filter_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT(appGUI->nte->notes_sort), sort_path);
    gtk_tree_path_free (sort_path);

    if (filter_path == NULL) return;
    path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter), filter_path);
    gtk_tree_path_free (filter_path);

    gtk_tree_model_get_iter (GTK_TREE_MODEL(appGUI->nte->notes_list_store), &n_iter, path);
    gtk_tree_model_get (GTK_TREE_MODEL(appGUI->nte->notes_list_store), &n_iter, 
                        N_COLUMN_LAST_CHANGES_DATE_JULIAN, &modified_date, 
                        N_COLUMN_LAST_CHANGES_TIME, &modified_time,
                        N_COLUMN_CREATE_DATE_JULIAN, &created_date,
                        N_COLUMN_CREATE_TIME, &created_time,
                        -1);

    g_snprintf (temp, BUFFER_SIZE, "%s", 
                get_date_time_full_str (modified_date, modified_time));
    g_snprintf (tmpbuf, BUFFER_SIZE, " (%s)", julian_to_str(modified_date, DATE_DAY_OF_WEEK_NAME));
	g_strlcat (temp, tmpbuf, BUFFER_SIZE);
    modified_date_str = g_strdup(temp);

    g_snprintf (temp, BUFFER_SIZE, "%s", 
                get_date_time_full_str (created_date, created_time));
    g_snprintf (tmpbuf, BUFFER_SIZE, " (%s)", julian_to_str(created_date, DATE_DAY_OF_WEEK_NAME));
	g_strlcat (temp, tmpbuf, BUFFER_SIZE);
    created_date_str = g_strdup(temp);

	g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s</b>: %d\n<b>%s</b>: %d\n<b>%s</b>: %d\n<b>%s</b>: %d\n<b>%s</b>: %d\n\n<b>%s</b>: %s\n<b>%s</b>: %s\n",
                _("Words"), words,
                _("Lines"), lines,
                _("Characters"), chars,
                _("White characters"), white_chars,
                _("Bytes"), bytes,
                _("Modified"), modified_date_str,
                _("Created"), created_date_str);

    g_free (modified_date_str);
    g_free (created_date_str);
    gtk_tree_path_free (path);

    g_free (text);
    g_free (attrs);

	gui_create_dialog (GTK_MESSAGE_INFO, tmpbuf, GTK_WINDOW (appGUI->main_window));
}

/*------------------------------------------------------------------------------*/

gint
notes_list_dbclick_cb(GtkWidget * widget, GdkEventButton * event, gpointer func_data) {

    GUI *appGUI = (GUI *)func_data;

    if ((event->type==GDK_2BUTTON_PRESS) && (event->button == 1)) {
        if (gtk_tree_selection_get_selected (appGUI->nte->notes_list_selection, NULL, NULL)) {
            notes_enter_password (appGUI);
        }
        return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
text_buffer_modified_cb (GtkTextBuffer *textbuffer, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->nte->buffer_check_modify_enable == FALSE) {
        appGUI->nte->changed = FALSE;
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save"), FALSE);
    } else {
        appGUI->nte->changed = TRUE;
        gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save"), TRUE);
    }
}

/*------------------------------------------------------------------------------*/

void
notes_item_selected (GtkTreeSelection *selection, gpointer data) {

    GUI *appGUI = (GUI *)data;

    gboolean state = gtk_tree_selection_get_selected (selection, NULL, NULL);

    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/edit"), state);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/delete"), state);

}    

/*------------------------------------------------------------------------------*/

void
find (gchar *find_text, GtkTextIter *iter, GUI *appGUI) {

GtkTextBuffer *buffer;
GtkTextIter match_start, match_end;
GtkTextMark *found_pos;
gchar c;
gint i;

    if (strlen(find_text)) {

        if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(appGUI->nte->find_case_checkbutton)) == FALSE) {
            i = 0;
            while((c = find_text[i])) {
                find_text[i++] =  g_unichar_tolower (c);
            }
        }

        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));

        if (gtk_text_iter_forward_search (iter, find_text, GTK_TEXT_SEARCH_TEXT_ONLY, 
                                          &match_start, &match_end, NULL)) {

            gui_change_bg_widget_state (appGUI->nte->find_entry, COLOR_BG_OK, appGUI);
            gtk_text_buffer_select_range (buffer, &match_start, &match_end);
            gtk_text_buffer_create_mark (buffer, "last_pos", &match_end, FALSE);

            found_pos = gtk_text_buffer_create_mark (buffer, "found_pos", &match_end, FALSE);
            gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW (appGUI->nte->editor_textview), found_pos); 
            gtk_text_buffer_delete_mark (buffer, found_pos);

        } else {
            gui_change_bg_widget_state (appGUI->nte->find_entry, COLOR_BG_FAIL, appGUI);
            if (gtk_text_buffer_get_mark (buffer, "last_pos") != NULL) {
                gtk_text_buffer_delete_mark_by_name (buffer, "last_pos");
            }
        }
    }
    
}

/*------------------------------------------------------------------------------*/

void
find_entry_action (GUI *appGUI) {

GtkTextBuffer *buffer;
GtkTextMark *last_pos;
GtkTextIter iter;
gchar *find_text;
   
    find_text = g_strdup(gtk_entry_get_text(GTK_ENTRY(appGUI->nte->find_entry)));
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));

    if (appGUI->nte->find_next_flag == TRUE) {

        last_pos = gtk_text_buffer_get_mark (buffer, "last_pos");

        if (last_pos == NULL) {
            gtk_text_buffer_get_start_iter (buffer, &iter);
            find (find_text, &iter, appGUI);
            g_free(find_text);
            return;
        }

        gtk_text_buffer_get_iter_at_mark (buffer, &iter, last_pos);
        find (find_text, &iter, appGUI);

    } else {
        gtk_text_buffer_get_start_iter (buffer, &iter);
        find (find_text, &iter, appGUI);
    }

    g_free(find_text);
}

/*------------------------------------------------------------------------------*/

gint 
find_entry_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (event->keyval == GDK_Escape) {
            editor_find_text_hide_cb (widget, appGUI);
            appGUI->nte->find_next_flag = FALSE;
            return TRUE;
    } else if (event->keyval == GDK_Return) {
            find_entry_action (appGUI);
            appGUI->nte->find_next_flag = TRUE;
            return TRUE;
    } else {
        appGUI->nte->find_next_flag = FALSE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
case_sensitive_toggle_cb (GtkToggleButton *togglebutton, gpointer data) {

    GUI *appGUI = (GUI *)data;

    appGUI->nte->find_next_flag = FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
notes_category_combo_box_focus_cb (GtkWidget *widget, GtkDirectionType *arg1, gpointer user_data) {
    return TRUE;
}

/*------------------------------------------------------------------------------*/

void
update_notes_items(GUI *appGUI) {

GtkTreeIter iter;
gint i;
gchar tmpbuf[BUFFER_SIZE];

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->nte->notes_filter), &iter, NULL, i++));
    --i;

    if (!i) {
        sprintf (tmpbuf, "<i>%s</i>", _("no entries"));
    } else {
        sprintf (tmpbuf, "<i>%4d %s</i>", i, ngettext ("entry", "entries", i));
    }

    gtk_label_set_markup (GTK_LABEL (appGUI->nte->n_items_label), tmpbuf);

}

/*------------------------------------------------------------------------------*/

void
notes_category_filter_cb (GtkComboBox *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    appGUI->nte->filter_index = gtk_combo_box_get_active (GTK_COMBO_BOX (widget));

    if (appGUI->nte->filter_index != -1) {
        gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter));
        update_notes_items(appGUI);
    }
}

/*------------------------------------------------------------------------------*/

gboolean
notes_list_filter_cb (GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {

gchar *category;

    GUI *appGUI = (GUI *)data;

    gtk_tree_model_get(model, iter, N_COLUMN_CATEGORY, &category, -1);

    if(appGUI->nte->filter_index) {

        if (gui_list_store_get_text_index (appGUI->opt->notes_category_store, category) == appGUI->nte->filter_index) {
            g_free (category);
            return TRUE;
        } else {
            g_free (category);
            return FALSE;
        }

    }

    g_free (category);
    return TRUE;
}

/*------------------------------------------------------------------------------*/

gint 
custom_notes_sort_function (GtkTreeModel *model, GtkTreeIter *iter_a, GtkTreeIter *iter_b, gpointer user_data) {

gchar *name_a, *name_b;
gchar *category_a, *category_b;
guint32 last_changes_date_a, last_changes_date_b;
gint category_s, name_s;
guint32 last_changes_date_s;

    if(iter_a == NULL || iter_b == NULL) {
        return 0;
    }

    gtk_tree_model_get (model, iter_a, N_COLUMN_NAME, &name_a, 
                        N_COLUMN_LAST_CHANGES_DATE_JULIAN, &last_changes_date_a, N_COLUMN_CATEGORY, &category_a, -1);
    gtk_tree_model_get (model, iter_b, N_COLUMN_NAME, &name_b, 
                        N_COLUMN_LAST_CHANGES_DATE_JULIAN, &last_changes_date_b, N_COLUMN_CATEGORY, &category_b, -1);

    
    if (name_a == NULL) {
        name_s = -1;
    } else if (name_b == NULL) {
        name_s = 1;
    } else if (name_a != NULL && name_b != NULL) {
        name_s = g_utf8_collate(name_a, name_b);
    } else {
        name_s = 0;
    }
    if (name_a != NULL) {
        g_free(name_a);
    }
    if (name_b != NULL) {
        g_free(name_b);
    }

    if (category_a == NULL) {
        category_s = -1;
    } else if (category_b == NULL) {
        category_s = 1;
    } else if (category_a != NULL && category_b != NULL) {
        category_s = g_utf8_collate(category_a, category_b);
    } else {
        category_s = 0;
    }
    if (category_a != NULL) {
        g_free(category_a);
    }
    if (category_b != NULL) {
        g_free(category_b);
    }

    if (!last_changes_date_a) last_changes_date_a = 1 << 31;
    if (!last_changes_date_b) last_changes_date_b = 1 << 31;
    last_changes_date_s = last_changes_date_a - last_changes_date_b;

    switch(config.notes_sorting_mode) {

        /* Name, Last changes, Category */
        case 0:
            if (name_s != 0)
                return name_s;
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            if (category_s != 0)
                return category_s;
            break;

        /* Name, Category, Last changes */
        case 1:
            if (name_s != 0)
                return name_s;
            if (category_s != 0)
                return category_s;
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            break;

        /* Category, Last changes, Name */
        case 2:
            if (category_s != 0)
                return category_s;
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            if (name_s != 0)
                return name_s;
            break;

        /* Category, Name, Last changes */
        case 3:
            if (category_s != 0)
                return category_s;
            if (name_s != 0)
                return name_s;
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            break;

        /* Last changes, Category, Name */
        case 4:
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            if (category_s != 0)
                return category_s;
            if (name_s != 0)
                return name_s;
            break;

        /* Last changes, Name, Category */
        case 5:
            if (last_changes_date_s != 0)
                return last_changes_date_s;
            if (name_s != 0)
                return name_s;
            if (category_s != 0)
                return category_s;
            break;

        default:
            break;
    }

    return 0;
}

/*------------------------------------------------------------------------------*/

void 
gui_create_notes (GUI *appGUI) {

GtkWidget           *vbox1;
GtkWidget           *scrolled_win;
GtkTextBuffer       *buffer;
GtkWidget           *viewport;
GError              *error = NULL;
GtkActionGroup      *action_group_selector = NULL;
GtkActionGroup      *action_group_editor = NULL;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
GtkWidget           *hseparator;
GtkWidget           *label;
GtkWidget           *vseparator;
GtkWidget           *close_button;
GtkWidget           *table;

gchar tmpbuf[BUFFER_SIZE];

 const gchar *ui_info_selector =
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"new\" action=\"new\" />\n"
"    <toolitem name=\"edit\" action=\"edit\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"delete\" action=\"delete\" />\n"
"  </toolbar>\n";

GtkActionEntry entries_selector[] = {
  { "new", OSMO_STOCK_NOTES_ADD, _("New note"), NULL, _("Add note"), NULL },
  { "edit", OSMO_STOCK_NOTES_EDIT, _("Edit note"), NULL, _("Edit note"), NULL },
  { "delete", OSMO_STOCK_NOTES_REMOVE, _("Delete note"), NULL, _("Remove note"), NULL },
};

guint n_entries_selector = G_N_ELEMENTS (entries_selector);

const gchar *ui_info_editor =
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"save\" action=\"save\" />\n"
"    <toolitem name=\"find\" action=\"find\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"bold\" action=\"bold\" />\n"
"    <toolitem name=\"italic\" action=\"italic\" />\n"
"    <toolitem name=\"underline\" action=\"underline\" />\n"
"    <toolitem name=\"strike\" action=\"strike\" />\n"
"    <toolitem name=\"mark_color\" action=\"mark_color\" />\n"
"    <toolitem name=\"clear\" action=\"clear\" />\n"
"    <separator name=\"sep2\" />\n"
"    <toolitem name=\"insert_date_time\" action=\"insert_date_time\" />\n"
"    <toolitem name=\"insert_separator\" action=\"insert_separator\" />\n"
"    <toolitem name=\"text_info\" action=\"text_info\" />\n"
"    <separator name=\"sep3\" />\n"
"    <toolitem name=\"close\" action=\"close\" />\n"
"  </toolbar>\n";

GtkActionEntry entries_editor[] = {
  { "find", OSMO_STOCK_EDITOR_FIND, _("Find"), NULL, _("Find"), NULL },
  { "bold", OSMO_STOCK_EDITOR_BOLD, _("Bold"), NULL, _("Bold"), NULL },
  { "italic", OSMO_STOCK_EDITOR_ITALIC, _("Italic"), NULL, _("Italic"), NULL },
  { "underline", OSMO_STOCK_EDITOR_UNDERLINE, _("Underline"), NULL, _("Underline"), NULL },
  { "strike", OSMO_STOCK_EDITOR_STRIKETHROUGH, _("Strikethrough"), NULL, _("Strikethrough"), NULL },
  { "mark_color", OSMO_STOCK_EDITOR_HIGHLIGHT, _("Highlight"), NULL, _("Highlight"), NULL },
  { "clear", OSMO_STOCK_EDITOR_CLEAR, _("Clear attributes"), NULL, _("Clear attributes"), NULL },
  { "save", OSMO_STOCK_EDITOR_SAVE, _("Save note"), NULL, _("Save note"), NULL },
  { "insert_date_time", OSMO_STOCK_EDITOR_INSERT_DATE_TIME, _("Insert current date and time"), NULL, _("Insert current date and time"), NULL },
  { "insert_separator", OSMO_STOCK_EDITOR_INSERT_SEPARATOR, _("Insert separator"), NULL, _("Insert separator"), NULL },
  { "text_info", OSMO_STOCK_EDITOR_INFO, _("Statistics"), NULL, _("Statistics"), NULL },
  { "close", OSMO_STOCK_CLOSE, _("Close editor"), NULL, _("Close editor"), NULL },
};

guint n_entries_editor = G_N_ELEMENTS (entries_editor);
  

    vbox1 = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    sprintf(tmpbuf, "<b>%s</b>", _("Notes"));
    gui_add_to_notebook (vbox1, tmpbuf, appGUI);

    appGUI->nte->vbox = GTK_BOX(vbox1);

    if (config.hide_notes == TRUE) {
        gtk_widget_hide(GTK_WIDGET(appGUI->nte->vbox));
    }

    appGUI->nte->vbox_selector = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (appGUI->nte->vbox_selector);
    gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox), appGUI->nte->vbox_selector, TRUE, TRUE, 0);
    appGUI->nte->vbox_editor = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (appGUI->nte->vbox_editor);
    gtk_box_pack_start (GTK_BOX(appGUI->nte->vbox), appGUI->nte->vbox_editor, TRUE, TRUE, 0);

    /*-------------------------------------------------------------------------------------*/

    action_group_selector = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group_selector, entries_selector, n_entries_selector, NULL);
    gtk_action_group_set_sensitive(action_group_selector, TRUE);

    appGUI->nte->notes_uim_selector_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (appGUI->nte->notes_uim_selector_widget, action_group_selector, 0);
    g_signal_connect (appGUI->nte->notes_uim_selector_widget, "add_widget", 
                      G_CALLBACK (add_notes_toolbar_selector_widget), appGUI);

    if (!gtk_ui_manager_add_ui_from_string (appGUI->nte->notes_uim_selector_widget, ui_info_selector, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (appGUI->nte->notes_uim_selector_widget);

    gtk_toolbar_set_style (appGUI->nte->notes_toolbar_selector, GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_tooltips (appGUI->nte->notes_toolbar_selector, config.enable_tooltips);

    /*-------------------------------------------------------------------------------------*/

    action_group_editor = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group_editor, entries_editor, n_entries_editor, NULL);
    gtk_action_group_set_sensitive(action_group_editor, TRUE);

    appGUI->nte->notes_uim_editor_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (appGUI->nte->notes_uim_editor_widget, action_group_editor, 0);
    g_signal_connect (appGUI->nte->notes_uim_editor_widget, "add_widget", 
                      G_CALLBACK (add_notes_toolbar_editor_widget), appGUI);

    if (!gtk_ui_manager_add_ui_from_string (appGUI->nte->notes_uim_editor_widget, ui_info_editor, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (appGUI->nte->notes_uim_editor_widget);

    gtk_toolbar_set_style (appGUI->nte->notes_toolbar_editor, GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_tooltips (appGUI->nte->notes_toolbar_editor, config.enable_tooltips);

    /*-------------------------------------------------------------------------------------*/
    /* assign callbacks */
    
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/new")), 
                      "clicked", G_CALLBACK(notes_add_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/edit")), 
                      "clicked", G_CALLBACK(notes_edit_item_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/delete")), 
                      "clicked", G_CALLBACK(notes_delete_item_cb), appGUI);

    /*-------------------------------------------------------------------------------------*/

    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/edit"), FALSE);
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_selector_widget, "/toolbar/delete"), FALSE);

    /* selector */

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_selector), hseparator, FALSE, TRUE, 4);

    table = gtk_table_new (1, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_selector), table, FALSE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "<b>%s:</b>", _("Category filter"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    appGUI->nte->cf_combobox = gtk_combo_box_new_text ();
    gtk_widget_show (appGUI->nte->cf_combobox);
    gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX (appGUI->nte->cf_combobox), FALSE);
    GTK_WIDGET_UNSET_FLAGS(appGUI->nte->cf_combobox, GTK_CAN_FOCUS);
    if (appGUI->tiny_gui == TRUE) {
        gtk_table_attach (GTK_TABLE (table), appGUI->nte->cf_combobox, 1, 2, 0, 1,
                         (GtkAttachOptions) (0),
                         (GtkAttachOptions) (0), 0, 0);
    } else {
        gtk_table_attach (GTK_TABLE (table), appGUI->nte->cf_combobox, 1, 2, 0, 1,
                         (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                         (GtkAttachOptions) (GTK_FILL), 0, 0);
    }
    g_signal_connect(appGUI->nte->cf_combobox, "changed", 
                     G_CALLBACK(notes_category_filter_cb), appGUI);
    g_signal_connect(G_OBJECT(appGUI->nte->cf_combobox), "focus", 
                     G_CALLBACK(notes_category_combo_box_focus_cb), NULL);

    appGUI->nte->n_items_label = gtk_label_new ("");
    gtk_widget_show (appGUI->nte->n_items_label);
    if (appGUI->tiny_gui == FALSE) {
        gtk_widget_set_size_request (appGUI->nte->n_items_label, 100, -1);
    }
    gtk_table_attach (GTK_TABLE (table), appGUI->nte->n_items_label, 3, 4, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                     (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (appGUI->nte->n_items_label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_selector), hseparator, FALSE, TRUE, 4);

    /*-------------------------------------------------------------------------------------*/

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_IN);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_selector), viewport, TRUE, TRUE, 0);
  
    scrolled_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolled_win);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (viewport), scrolled_win);

    appGUI->nte->notes_list_store = gtk_list_store_new(NOTES_NUM_COLUMNS, 
                                                       G_TYPE_STRING, G_TYPE_STRING,
                                                       G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT,
                                                       G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT,
                                                       G_TYPE_UINT, G_TYPE_STRING);

    appGUI->nte->notes_filter = gtk_tree_model_filter_new(GTK_TREE_MODEL(appGUI->nte->notes_list_store), NULL);
    gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER(appGUI->nte->notes_filter), 
                                            (GtkTreeModelFilterVisibleFunc)notes_list_filter_cb, 
                                            appGUI, NULL);

    appGUI->nte->notes_sort = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(appGUI->nte->notes_filter));

    appGUI->nte->notes_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->nte->notes_sort));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(appGUI->nte->notes_list), config.notes_rules_hint);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW(appGUI->nte->notes_list), FALSE);
    gtk_widget_show (appGUI->nte->notes_list);
    GTK_WIDGET_SET_FLAGS (appGUI->nte->notes_list, GTK_CAN_DEFAULT);
    gtk_container_add (GTK_CONTAINER (scrolled_win), appGUI->nte->notes_list);

    g_signal_connect(G_OBJECT(appGUI->nte->notes_list), "button_press_event",
                     G_CALLBACK(notes_list_dbclick_cb), appGUI);

    appGUI->nte->notes_list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (appGUI->nte->notes_list));
    g_signal_connect(G_OBJECT(appGUI->nte->notes_list_selection), "changed",
                     G_CALLBACK(notes_item_selected), appGUI);

    /* create columns */

    renderer = gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    column = gtk_tree_view_column_new_with_attributes(_("Note name"),
                                                      renderer,
                                                      "text", N_COLUMN_NAME,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Category"),
                                                      renderer,
                                                      "text", N_COLUMN_CATEGORY,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(_("Last changes"),
                                                      renderer,
                                                      "text", N_COLUMN_LAST_CHANGES_DATE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_LAST_CHANGES_DATE_JULIAN,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_LAST_CHANGES_TIME,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_CREATE_DATE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_CREATE_DATE_JULIAN,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_CREATE_TIME,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_EDITOR_LINE,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL,
                                                      renderer,
                                                      "text", N_COLUMN_FILENAME,
                                                      NULL);
    gtk_tree_view_column_set_visible (column, FALSE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(appGUI->nte->notes_list), column);

    /* configure sorting */

    gtk_tree_sortable_set_sort_func ((GtkTreeSortable *)appGUI->nte->notes_sort, N_COLUMN_NAME, 
                                     (GtkTreeIterCompareFunc)custom_notes_sort_function, NULL, NULL);
    gtk_tree_sortable_set_sort_column_id ((GtkTreeSortable *)appGUI->nte->notes_sort, N_COLUMN_NAME, 
                                          config.notes_sorting_order);

    /*-------------------------------------------------------------------------------------*/
    /* editor */

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_editor), hseparator, FALSE, TRUE, 4);

    appGUI->nte->title_label = gtk_label_new ("");
    gtk_widget_show (appGUI->nte->title_label);
    gtk_label_set_ellipsize (GTK_LABEL(appGUI->nte->title_label), PANGO_ELLIPSIZE_END);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_editor), appGUI->nte->title_label, FALSE, FALSE, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_editor), hseparator, FALSE, TRUE, 4);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_IN);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_editor), viewport, TRUE, TRUE, 0);
  
    appGUI->nte->editor_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (appGUI->nte->editor_scrolledwindow);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (appGUI->nte->editor_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (viewport), appGUI->nte->editor_scrolledwindow);

    appGUI->nte->editor_textview = gtk_text_view_new ();
    gtk_widget_show (appGUI->nte->editor_textview);
    gtk_container_add (GTK_CONTAINER (appGUI->nte->editor_scrolledwindow), appGUI->nte->editor_textview);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->nte->editor_textview), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->nte->editor_textview), 4);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->nte->editor_textview), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->nte->editor_textview), 4);
    gtk_widget_modify_font (GTK_WIDGET(appGUI->nte->editor_textview), appGUI->nte->fd_notes_font);

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->nte->editor_textview));
    appGUI->nte->buffer_check_modify_enable = FALSE;
    g_signal_connect (G_OBJECT (buffer), "changed", G_CALLBACK (text_buffer_modified_cb), appGUI);

    gtk_text_buffer_create_tag (buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
    g_object_set_data (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/bold")), 
                       "tag", "bold");
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/bold")), 
                      "clicked", G_CALLBACK (set_text_attribute_cb), appGUI);

    gtk_text_buffer_create_tag (buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
    g_object_set_data (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/italic")), 
                       "tag", "italic");
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/italic")), 
                      "clicked", G_CALLBACK (set_text_attribute_cb), appGUI);

    gtk_text_buffer_create_tag (buffer, "strike", "strikethrough", TRUE, NULL);
    g_object_set_data (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/strike")), 
                       "tag", "strike");
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/strike")), 
                      "clicked", G_CALLBACK (set_text_attribute_cb), appGUI);

    gtk_text_buffer_create_tag (buffer, "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);
    g_object_set_data (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/underline")), 
                       "tag", "underline");
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/underline")), 
                      "clicked", G_CALLBACK (set_text_attribute_cb), appGUI);

    gtk_text_buffer_create_tag (buffer, "mark_color", "background", "#FFFF00", NULL);
    g_object_set_data (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/mark_color")), 
                       "tag", "mark_color");
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/mark_color")), 
                      "clicked", G_CALLBACK (set_text_attribute_cb), appGUI);

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/insert_date_time")), 
                      "clicked", G_CALLBACK(insert_current_date_and_time_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/insert_separator")), 
                      "clicked", G_CALLBACK(insert_separator_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/text_info")), 
                      "clicked", G_CALLBACK(text_info_cb), appGUI);
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/clear")), 
                      "clicked", G_CALLBACK (clear_text_attributes_cb), appGUI);
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/close")), 
                      "clicked", G_CALLBACK (editor_close_cb), appGUI);
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save")), 
                      "clicked", G_CALLBACK (editor_save_buffer_cb), appGUI);
    g_signal_connect (G_OBJECT (gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/find")), 
                      "clicked", G_CALLBACK (editor_find_text_show_cb), appGUI);

    appGUI->nte->find_hbox = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (appGUI->nte->find_hbox);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->vbox_editor), appGUI->nte->find_hbox, FALSE, FALSE, 0);

    appGUI->nte->find_next_flag = FALSE;

    sprintf(tmpbuf, "<b>%s</b>:", _("Find"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->find_hbox), label, FALSE, FALSE, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    appGUI->nte->find_entry = gtk_entry_new ();
    gtk_widget_show (appGUI->nte->find_entry);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->find_hbox), appGUI->nte->find_entry, TRUE, TRUE, 4);
    g_signal_connect (G_OBJECT (appGUI->nte->find_entry), "key_press_event",
                      G_CALLBACK (find_entry_key_press_cb), appGUI);

    appGUI->nte->find_case_checkbutton = gtk_check_button_new_with_mnemonic (_("case sensitive"));
    GTK_WIDGET_UNSET_FLAGS(appGUI->nte->find_case_checkbutton, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->nte->find_case_checkbutton);
    gtk_box_pack_start (GTK_BOX (appGUI->nte->find_hbox), appGUI->nte->find_case_checkbutton, FALSE, FALSE, 4);
    g_signal_connect (GTK_TOGGLE_BUTTON(appGUI->nte->find_case_checkbutton), "toggled",
                      G_CALLBACK (case_sensitive_toggle_cb), appGUI);


    if (config.default_stock_icons) {
        close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
    } else {
        close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(close_button, GTK_CAN_FOCUS);
    gtk_button_set_relief (GTK_BUTTON(close_button), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, close_button, _("Close find entry"), NULL);
    gtk_box_pack_end (GTK_BOX (appGUI->nte->find_hbox), close_button, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (close_button), "clicked",
                      G_CALLBACK (editor_find_text_hide_cb), appGUI);

    vseparator = gtk_vseparator_new ();
    gtk_widget_show (vseparator);
    gtk_box_pack_end (GTK_BOX (appGUI->nte->find_hbox), vseparator, FALSE, TRUE, 0);

    gtk_widget_hide (appGUI->nte->find_hbox);

    appGUI->nte->changed = FALSE;
    gtk_widget_set_sensitive(gtk_ui_manager_get_widget (appGUI->nte->notes_uim_editor_widget, "/toolbar/save"), FALSE);

    /*-------------------------------------------------------------------------------------*/

    notes_show_selector_editor (SELECTOR, appGUI);

    appGUI->nte->filename = NULL;
    appGUI->nte->context = NULL;
    appGUI->nte->keyholder = NULL;
}

/*------------------------------------------------------------------------------*/

#endif  /* HAVE_LIBGRINGOTTS */

