
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

#ifndef _UTILS_H
#define _UTILS_H

#include "gui.h"

enum helpers {
    UNKNOWN = 0,
    WWW,
    EMAIL
};

enum {
    SW_MOVE_UP = 0,
    SW_MOVE_DOWN
};

#define     COLOR_BG_OK     "#f9fbd9"
#define     COLOR_BG_FAIL   "#ff7770"

guint       utl_get_current_day                 (void);
guint       utl_get_current_month               (void);
guint       utl_get_current_year                (void);
guint32     utl_get_current_julian              (void);
gchar *     utl_get_day_name                    (guint day, gboolean short_name);
gchar *     utl_get_julian_day_name             (guint32 julian);
gchar *     utl_get_date_name                   (GDate *date);
guint       utl_get_days_in_month               (GDate *date);
guint       utl_get_weekend_days_in_month       (GDate *date);
guint       utl_get_weekend_days_in_month_my    (guint month, guint year);
guint       utl_get_days_per_year               (guint year);
void        utl_gdate_to_dmy                    (GDate *date, gint *day, gint *month, gint *year);
void        utl_julian_to_dmy                   (guint32 julian, guint *day, guint *month, guint *year);
guint32     utl_dmy_to_julian                   (guint day, guint month, guint year);
guint       utl_calc_moon_phase                 (GDate *date);
gchar *     utl_get_moon_phase_name             (gint phase);

void        utl_xml_get_int                     (gchar *name, gint *iname, xmlNodePtr node, xmlDocPtr doc);
void        utl_xml_get_uint                    (gchar *name, guint *uname, xmlNodePtr node, xmlDocPtr doc);
void        utl_xml_get_char                    (gchar *name, gchar *cname, xmlNodePtr node, xmlDocPtr doc);
void        utl_xml_get_str                     (gchar *name, gchar **sname, xmlNodePtr node, xmlDocPtr doc);
void        utl_xml_get_strn                    (gchar *name, gchar *sname, gint buffer_size, 
                                                 xmlNodePtr node, xmlDocPtr doc);
void        utl_xml_put_int                     (gchar *name, gint value, xmlNodePtr node);
void        utl_xml_put_uint                    (gchar *name, guint value, xmlNodePtr node);
void        utl_xml_put_char                    (gchar *name, gchar character, xmlNodePtr node);
void        utl_xml_put_str                     (gchar *name, gchar *string, xmlNodePtr node);
void        utl_xml_put_strn                    (gchar *name, gchar *string, gint buffer_size, xmlNodePtr node);

void        utl_name_strcat                     (gchar *first, gchar *second, gchar *buffer);
gboolean    utl_run_helper                      (gchar *parameter, gint helper);
void        utl_run_command                     (gchar *command);
gboolean    utl_is_valid_command                (gchar *command);
gint        utl_get_link_type                   (gchar *link);

void        utl_cairo_set_color                 (cairo_t *cr, GdkColor *color, gint alpha);
void        utl_cairo_draw                      (cairo_t *cr, gint stroke);
void        utl_draw_rounded_rectangle          (cairo_t *cr, gint x, gint y, gint w, gint h, gint a, gint s);
void        utl_draw_left_arrow                 (cairo_t *cr, gdouble x, gdouble y, gdouble w, gdouble h, gdouble a);

void        gui_create_category_combobox        (GtkComboBox *combo_box, GtkListStore *store, gboolean none);
gint        gui_get_combobox_items              (GtkComboBox *combo_box);

gchar*      gui_text_buffer_get_text_with_tags  (GtkTextBuffer *buffer);
void        gui_text_buffer_set_text_with_tags  (GtkTextBuffer *buffer, const gchar *text);

void        gui_change_bg_widget_state          (GtkWidget *widget, gchar *color_str, GUI *appGUI);
GdkPixbuf*  gui_create_color_swatch             (gchar *color);

gint        gui_list_store_get_text_index       (GtkListStore *store, gchar *text);
void        gui_scrolled_window_move_position   (GtkWidget *sw, gint direction);
GtkWidget*  gui_stock_button                    (const gchar *bstock, gboolean toggle);
GtkWidget*  gui_stock_label_radio_button        (gchar *label, const gchar *stock, GtkIconSize size);
GtkWidget*  gui_image_label_radio_button        (gchar *label, const guint8 *pix);
void        gui_clear_text_buffer               (GtkTextBuffer *buffer, GtkTextIter *iter);
gint        gui_create_dialog                   (gint dialog_type, gchar *message, GtkWindow *parent);

void        gui_url_initialize                  (GUI *appGUI);
void        gui_url_setup                       (GSList *links_list, gint *link_index, GtkWidget *textview, 
                                                 GUI *appGUI);
void        gui_url_insert_link                 (GSList *links_list, gint *link_index, GtkWidget *textview, 
                                                 GtkTextIter *iter, gchar *color, gchar *font, gchar *text, 
                                                 gboolean center, GUI *appGUI);
void        gui_url_remove_links                (GSList *links_list, gint *link_index);

void        gui_update_command_status           (GtkWidget *entry_widget, GtkWidget *icon_widget, GUI *appGUI);
gint        gui_check_overwrite_file            (gchar *filename, GtkWidget *window, GUI *appGUI);

#endif /* _UTILS_H */

