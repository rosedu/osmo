/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GTK Calendar Widget
 * Copyright (C) 1998 Cesar Miquel and Shawn T. Amundson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

/*
 * Some minor modifications and adaptation to OSMO - a handy personal
 * manager by pasp@users.sourceforge.net
 */

#ifndef _CALENDAR_WIDGET_H
#define _CALENDAR_WIDGET_H

#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>

/* Not needed, retained for compatibility -Yosh */
#include <gtk/gtksignal.h>


G_BEGIN_DECLS

#define GUI_TYPE_CALENDAR                  (gui_calendar_get_type ())
#define GUI_CALENDAR(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GUI_TYPE_CALENDAR, GuiCalendar))
#define GUI_CALENDAR_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GUI_TYPE_CALENDAR, GuiCalendarClass))
#define GUI_IS_CALENDAR(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GUI_TYPE_CALENDAR))
#define GUI_IS_CALENDAR_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GUI_TYPE_CALENDAR))
#define GUI_CALENDAR_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GUI_TYPE_CALENDAR, GuiCalendarClass))


typedef struct _GuiCalendar	            GuiCalendar;
typedef struct _GuiCalendarClass       GuiCalendarClass;

typedef struct _GuiCalendarPrivate     GuiCalendarPrivate;

typedef enum
{
  GUI_CALENDAR_SHOW_HEADING		    = 1 << 0,
  GUI_CALENDAR_SHOW_DAY_NAMES		= 1 << 1,
  GUI_CALENDAR_NO_MONTH_CHANGE		= 1 << 2,
  GUI_CALENDAR_SHOW_WEEK_NUMBERS	= 1 << 3,
  GUI_CALENDAR_WEEK_START_MONDAY	= 1 << 4
} GuiCalendarDisplayOptions;

struct _GuiCalendar
{
  GtkWidget widget;
  
  GtkStyle  *header_style;
  GtkStyle  *label_style;
  
  gint month;
  gint year;
  gint selected_day;
  
  gint day_month[6][7];
  gint day[6][7];
  
  gint num_marked_dates;
  gint marked_date[31];
  GuiCalendarDisplayOptions  display_flags;
  GdkColor marked_date_color[31];

  GdkColor header_color;
  GdkColor weekend_color;
  GdkColor selector_color;
  
  gunichar mark_sign;

  GdkGC *gc;			/* unused */
  GdkGC *xor_gc;		/* unused */

  gint focus_row;
  gint focus_col;

  gint highlight_row;
  gint highlight_col;
  
  GuiCalendarPrivate *priv;
  gchar grow_space [32];

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _GuiCalendarClass
{
  GtkWidgetClass parent_class;
  
  /* Signal handlers */
  void (* month_changed)		            (GuiCalendar *calendar);
  void (* day_selected)			            (GuiCalendar *calendar);
  void (* day_selected_double_click)	    (GuiCalendar *calendar);
  void (* prev_month)			            (GuiCalendar *calendar);
  void (* next_month)			            (GuiCalendar *calendar);
  void (* prev_year)			            (GuiCalendar *calendar);
  void (* next_year)			            (GuiCalendar *calendar);
  
};

GType	   gui_calendar_get_type	        (void) G_GNUC_CONST;
GtkWidget* gui_calendar_new		            (void);

gboolean   gui_calendar_select_month        (GuiCalendar *calendar, guint month, guint year);
void	   gui_calendar_select_day	        (GuiCalendar *calendar, guint day);

gboolean   gui_calendar_mark_day	        (GuiCalendar *calendar, guint day);
gboolean   gui_calendar_unmark_day	        (GuiCalendar *calendar, guint day);
void	   gui_calendar_clear_marks	        (GuiCalendar *calendar);

void	   gui_calendar_set_display_options (GuiCalendar *calendar, GuiCalendarDisplayOptions flags);
GuiCalendarDisplayOptions
           gui_calendar_get_display_options (GuiCalendar *calendar);

void	   gui_calendar_get_date	        (GuiCalendar *calendar, guint *year, guint *month, guint *day);

void       gui_calendar_set_header_color    (GuiCalendar *calendar, const char *color);
void       gui_calendar_set_weekend_color   (GuiCalendar *calendar, const char *color);
void       gui_calendar_set_selector_color  (GuiCalendar *calendar, const char *color);
void       gui_calendar_set_mark_symbol     (GuiCalendar *calendar, const gchar *symbol);

G_END_DECLS

#endif /* _CALENDAR_WIDGET_H */

