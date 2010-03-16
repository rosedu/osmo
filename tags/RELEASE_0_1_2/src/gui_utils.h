
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

#ifndef _GUI_UTILS_H
#define _GUI_UTILS_H

#include "gui.h"

enum {
    SW_MOVE_UP = 0,
    SW_MOVE_DOWN
};

enum helpers {
    UNKNOWN = 0,
    WWW,
    EMAIL
};

GdkPixbuf*  gui_create_color_swatch             (gchar *color);
gint        gui_list_store_get_text_index       (GtkListStore *store, gchar *text);
void        gui_scrolled_window_move_position   (GtkWidget *sw, gint direction);
GtkWidget*  gui_stock_button                    (const gchar *bstock, gboolean toggle);
GtkWidget*  gui_stock_label_radio_button        (gchar *label, const gchar *stock, GtkIconSize size);
GtkWidget*  gui_image_label_radio_button        (gchar *label, const guint8 *pix);
gboolean    gui_run_helper                      (gchar *parameter, gint helper);
void        gui_clear_text_buffer               (GtkTextBuffer *buffer, GtkTextIter *iter);
gint        gui_create_dialog                   (gint dialog_type, gchar *message, GtkWindow *parent);

gint        gui_get_link_type                   (gchar *link);
void        gui_url_initialize                  (GUI *appGUI);
void        gui_url_setup                       (GSList *links_list, gint *link_index, GtkWidget *textview, GUI *appGUI);
void        gui_url_insert_link                 (GSList *links_list, gint *link_index, GtkWidget *textview, GtkTextIter *iter, gchar *color, gint font_size, gchar *text, gboolean center, GUI *appGUI);
void        gui_url_remove_links                (GSList *links_list, gint *link_index);

#endif /* _GUI_UTILS_H */

