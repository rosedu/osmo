
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
#include <glib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

#include "i18n.h"
#include "calendar.h"
#include "calendar_widget.h"
#include "calendar_jumpto.h"
#include "calendar_fullyear.h"
#include "tasks.h"
#include "contacts.h"
#include "gui.h"
#include "gui_utils.h"
#include "options_prefs.h"
#include "calendar_notes.h"
#include "calendar_utils.h"
#include "calendar_moon.h"
#include "stock_icons.h"


extern gboolean             calendar_only;
extern gboolean             all_pages_added;

extern GtkWidget            *main_window;
extern GtkTooltips          *osmo_tooltips; 
extern PangoFontDescription *fd_day_name;
extern PangoFontDescription *fd_calendar;
extern PangoFontDescription *fd_notes;
extern gchar                tmpbuf[];

extern  GtkListStore        *tasks_list_store;
extern  GtkListStore        *contacts_list_store;

GtkToolbar                  *calendar_toolbar;
GtkWidget                   *notes_vbox;
GtkWidget                   *day_info_vbox;
GtkWidget                   *day_number_label;
GtkWidget                   *day_number_year_label;
GtkWidget                   *week_number_label;
GtkWidget                   *moon_icon;
GtkWidget                   *moon_phase_label;
GtkWidget                   *marked_days_label;
GtkWidget                   *weekend_days_label;
GtkWidget                   *day_desc_textview;
GtkTextBuffer               *day_desc_text_buffer;
GtkTextIter                 day_desc_iter;
GtkWidget                   *calendar;
GtkWidget                   *textview;
GtkWidget                   *prev_day_button;
GtkWidget                   *next_day_button;
GtkWidget                   *prev_month_button;
GtkWidget                   *next_month_button;
GtkWidget                   *date_label;
GtkWidget                   *note_label;
GtkWidget                   *n_timeline_button;
GtkWidget                   *n_clear_button;
GtkWidget                   *n_close_button;
GtkWidget                   *note_scrolledwindow;
GtkWidget                   *notes_button;
gboolean                    dont_update;

guint                       day, month, year;
guint                       prev_day = 0, prev_month = 0, prev_year = 0;

/*------------------------------------------------------------------------------*/

void
set_moon_icon (gint moon_phase) {

const guint8 *icons_table[] = {
    moon_phase_0, moon_phase_1, moon_phase_2, moon_phase_3,
    moon_phase_4, moon_phase_5, moon_phase_6, moon_phase_7
};

GdkPixbuf *icon;

    icon = gdk_pixbuf_new_from_inline (-1, icons_table[moon_phase], FALSE, NULL);
    gtk_image_set_from_pixbuf (GTK_IMAGE(moon_icon), icon);
    g_object_unref (icon);

    sprintf(tmpbuf, "(%s)", get_moon_phase_name(moon_phase));
    gtk_label_set_text (GTK_LABEL(moon_phase_label), tmpbuf);
}

/*------------------------------------------------------------------------------*/

void
mark_days_with_notes (guint day, guint month, guint year) {

gint i;

    gui_calendar_clear_marks (GUI_CALENDAR(calendar));

    for (i = 1; i <= get_number_of_days_in_month(month, year); i++) {

        if (check_note(i, month+1, year) == TRUE) {
            gui_calendar_mark_day (GUI_CALENDAR(calendar), i);
        }
    }
}

/*------------------------------------------------------------------------------*/

gint
get_marked_days (guint month, guint year) {

gint i, n;

    n = 0;

    for (i = 1; i <= get_number_of_days_in_month(month, year); i++) {
        if (check_note(i, month+1, year) == TRUE) {
            n++;
        }
    }

    return n;
}

/*------------------------------------------------------------------------------*/

void
calendar_update_note (guint uday, guint umonth, guint uyear) {

GtkTextBuffer *textbuffer;
GtkTextIter iter_start, iter_end;
gchar *text;

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_start, 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_end, -1);

    text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &iter_start, &iter_end, TRUE);
 
    if (text != NULL) {
        if (strlen(text)) {
            add_note (uday, umonth+1, uyear, text);
        } else {
            remove_note (uday, umonth+1, uyear);
        }
    }

    set_day_info (uday, umonth, uyear);
}

/*------------------------------------------------------------------------------*/

gint
check_add_tasks(guint32 selected_julian, gboolean count) {

GtkTreePath *path;
GtkTreeIter iter;
guint32 date;
gboolean done;
gchar *summary;
gint i;

    i = 0;

    path = gtk_tree_path_new_first();
 
    while (gtk_tree_model_get_iter (GTK_TREE_MODEL(tasks_list_store), &iter, path) == TRUE) {
        gtk_tree_model_get (GTK_TREE_MODEL(tasks_list_store), &iter, 
                            COLUMN_DUE_DATE_JULIAN, &date, COLUMN_DONE, &done, -1);

        if (date != 0 && done != TRUE) {
            if (date == selected_julian) {
                if (count == FALSE) {
                    gtk_tree_model_get (GTK_TREE_MODEL(tasks_list_store), &iter, COLUMN_SUMMARY, &summary, -1);
                    sprintf(tmpbuf, " %d. ", i+1);
                    gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, tmpbuf, -1);
                    gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, summary, -1);
                    gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n", -1);
                    g_free(summary);
                }
                i++;
            }
        }

        gtk_tree_path_next(path);
    }
    gtk_tree_path_free(path);

    return i;
}

/*------------------------------------------------------------------------------*/

gint
check_add_contacts(guint32 selected_julian, gboolean count) {

GtkTreePath *path;
GtkTreeIter iter;
guint32 date, age;
gchar *first_name, *last_name;
GDate *new_cdate;
gint i;

    i = 0;

    path = gtk_tree_path_new_first();
 
    while (gtk_tree_model_get_iter (GTK_TREE_MODEL(contacts_list_store), &iter, path) == TRUE) {
        gtk_tree_model_get (GTK_TREE_MODEL(contacts_list_store), &iter, COLUMN_BIRTH_DAY_DATE, &date, -1);
        if (date != 0) {

            new_cdate = g_date_new_julian (date);

            if (new_cdate != NULL) {

                age = year - g_date_get_year(new_cdate);

                if (age >= 0) {

                    g_date_set_year (new_cdate, year);
                    date = g_date_get_julian (new_cdate);
     
                    if (date != 0 && selected_julian == date) {

                        if (count == FALSE) {
                            gtk_tree_model_get (GTK_TREE_MODEL(contacts_list_store), &iter, 
                                                COLUMN_FIRST_NAME, &first_name, 
                                                COLUMN_LAST_NAME, &last_name, 
                                                -1);
                            if (age == 0) {
                                sprintf(tmpbuf, " %s %s %s\n", first_name, last_name, _("was born"));
                            } else if (age == 1) {
                                sprintf(tmpbuf, " %s %s (1 %s)\n", first_name, last_name, _("year old"));
                            } else {
                                sprintf(tmpbuf, " %s %s (%d %s)\n", first_name, last_name, age, _("years old"));
                            }

                            gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, tmpbuf, -1);

                            g_free(first_name);
                            g_free(last_name);
                        }
                        i++;
                    }

                    g_date_free(new_cdate);
                }
            }
        }
        gtk_tree_path_next(path);
    }
    gtk_tree_path_free(path);

    return i;
}

/*------------------------------------------------------------------------------*/

void
check_tasks_and_contacts(guint32 selected_julian) {

gint i;

    if (calendar_only == TRUE || all_pages_added == FALSE) {
        return;
    }

    /* check tasks */

    i = check_add_tasks(selected_julian, TRUE);

    if (i) {
        sprintf(tmpbuf, "%s:\n", _("Day tasks"));
        gtk_text_buffer_insert_with_tags_by_name (day_desc_text_buffer, &day_desc_iter, tmpbuf, -1, "bold", NULL);
        check_add_tasks(selected_julian, FALSE);
        gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n", -1);
    }

    /* check contacts */

    i = check_add_contacts(selected_julian, TRUE);

    if (i) {
        sprintf(tmpbuf, "%s:\n", _("Birthday"));
        gtk_text_buffer_insert_with_tags_by_name (day_desc_text_buffer, &day_desc_iter, tmpbuf, -1, "bold", NULL);
        check_add_contacts(selected_julian, FALSE);
        gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n", -1);
    }
}

/*------------------------------------------------------------------------------*/

void
set_day_info (guint dday, guint dmonth, guint dyear) {

GtkTextChildAnchor *anchor;
GtkWidget *table, *label;
gchar *text;
gint ydays;

    gui_clear_text_buffer (day_desc_text_buffer, &day_desc_iter);

    ydays = get_day_of_the_year (dday, dmonth, dyear);

    anchor = gtk_text_buffer_create_child_anchor (day_desc_text_buffer, &day_desc_iter);

    table = gtk_table_new (3, 3+2, FALSE);
    gtk_widget_show (table);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 8);

    sprintf(tmpbuf, "<b>%s:</b>", _("Day number"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "<b>%s:</b>", _("Week number"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "<b>%s:</b>", _("Marked days"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "<b>%s:</b>", _("Weekend days"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    sprintf(tmpbuf, "<b>%s:</b>", _("Moon phase"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    day_number_label = gtk_label_new (NULL);
    gtk_widget_show (day_number_label);
    gtk_table_attach (GTK_TABLE (table), day_number_label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (day_number_label), 0, 0.5);

    day_number_year_label = gtk_label_new (NULL);
    gtk_widget_show (day_number_year_label);
    gtk_table_attach (GTK_TABLE (table), day_number_year_label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (day_number_year_label), 0, 0.5);

    week_number_label = gtk_label_new (NULL);
    gtk_widget_show (week_number_label);
    gtk_table_attach (GTK_TABLE (table), week_number_label, 1, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (week_number_label), 0, 0.5);

    marked_days_label = gtk_label_new (NULL);
    gtk_widget_show (marked_days_label);
    gtk_table_attach (GTK_TABLE (table), marked_days_label, 1, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (marked_days_label), 0, 0.5);

    weekend_days_label = gtk_label_new (NULL);
    gtk_widget_show (weekend_days_label);
    gtk_table_attach (GTK_TABLE (table), weekend_days_label, 1, 3, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (weekend_days_label), 0, 0.5);

    moon_icon = gtk_image_new();
    gtk_widget_show (moon_icon);
    gtk_table_attach (GTK_TABLE (table), moon_icon, 1, 2, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (moon_icon), 0, 0.5);

    moon_phase_label = gtk_label_new (NULL);
    gtk_widget_show (moon_phase_label);
    gtk_table_attach (GTK_TABLE (table), moon_phase_label, 2, 3, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (moon_phase_label), 0, 0.5);


    sprintf(tmpbuf, "%d", ydays);
    gtk_label_set_text (GTK_LABEL(day_number_label), tmpbuf);
    sprintf(tmpbuf, "(%d %s)", get_days_per_year(dyear)-ydays, _("days till end of year"));
    gtk_label_set_text (GTK_LABEL(day_number_year_label), tmpbuf);
    sprintf(tmpbuf, "%d / %d", get_week_of_the_year (dday, dmonth, dyear), get_week_of_the_year(31, 12-1, dyear));
    gtk_label_set_text (GTK_LABEL(week_number_label), tmpbuf);

    set_moon_icon (calc_moon_phase(dyear, dmonth+1, dday));

    sprintf(tmpbuf, "%d", get_marked_days(dmonth, dyear));
    gtk_label_set_text (GTK_LABEL(marked_days_label), tmpbuf);
    sprintf(tmpbuf, "%d", get_weekend_days(dmonth, dyear));
    gtk_label_set_text (GTK_LABEL(weekend_days_label), tmpbuf);

    gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW(day_desc_textview), table, anchor);
    gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n\n", -1);

    if (config.notes_in_info_panel) {

        text = get_note(dday, dmonth+1, dyear);

        if (text != NULL) {
            sprintf(tmpbuf, "%s:\n", _("Day notes"));
            gtk_text_buffer_insert_with_tags_by_name (day_desc_text_buffer, &day_desc_iter, tmpbuf, -1, "bold", NULL);
            gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, text, -1);
            if (text[strlen(text)-1] != '\n') {
                gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n\n", -1);
            } else {
                gtk_text_buffer_insert (day_desc_text_buffer, &day_desc_iter, "\n", -1);
            }
        }
    }

    check_tasks_and_contacts(date_to_julian (day, month, year));

}

/*------------------------------------------------------------------------------*/

void
set_note (guint uday, guint umonth, guint uyear) {

GtkTextBuffer *textbuffer;
GtkTextIter iter_start, iter_end;
gchar *t;

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_start, 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_end, -1);

    t = get_note(uday, umonth+1, uyear);

    if (t != NULL) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), t, -1);
    } else {
        gtk_text_buffer_delete (GTK_TEXT_BUFFER(textbuffer), &iter_start, &iter_end);
    }
}

/*------------------------------------------------------------------------------*/

void        
day_notes_toggled_cb (GtkToggleToolButton *togglebutton, gpointer user_data) {

    config.day_notes_visible = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(notes_button));

    if (!config.day_notes_visible) {
        calendar_update_note (day, month, year);
        mark_days_with_notes (day, month, year);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
        gtk_widget_hide(notes_vbox);
        gtk_widget_show(day_info_vbox);
    } else {
        gtk_widget_show(notes_vbox);
        gtk_widget_hide(day_info_vbox);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), TRUE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(textview));
    }
}

/*------------------------------------------------------------------------------*/

void
jump_to_date (guint jday, guint jmonth, guint jyear) {

    dont_update = TRUE;

    gui_calendar_select_day(GUI_CALENDAR(calendar), 1);     /* Trick: always select valid day number */

    day = jday;
    month = jmonth;
    year = jyear;
    gui_calendar_select_month(GUI_CALENDAR(calendar), month, year);
    gui_calendar_select_day(GUI_CALENDAR(calendar), day);
    gtk_label_set_text (GTK_LABEL(date_label), get_date_name(day, month, year));

    if (calendar_only == FALSE) {
        mark_days_with_notes (day, month, year);
        set_note (day, month, year);
        set_day_info (day, month, year);
    }
  
    dont_update = FALSE;
}

/*------------------------------------------------------------------------------*/

void
set_today (gboolean update) {

struct tm   *timer;
time_t      tmm;

    tmm = time(NULL); 
    timer = localtime(&tmm);
    day = timer->tm_mday;
    month = timer->tm_mon;
    year = timer->tm_year + 1900;

    if (update == TRUE) {
        jump_to_date (day, month, year);
    }
}

/*------------------------------------------------------------------------------*/

void 
calendar_close_text_cb (GtkWidget *widget, gpointer data) {

    gtk_widget_hide (notes_vbox);
    gtk_widget_show (day_info_vbox);
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(notes_button), FALSE);
    gtk_widget_grab_focus(GTK_WIDGET(calendar));
}

/*------------------------------------------------------------------------------*/

void 
calendar_clear_text_cb (GtkWidget *widget, gpointer data) {

GtkTextBuffer *textbuffer;

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), "", -1);
    calendar_update_note (day, month, year);
    mark_days_with_notes (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_insert_timeline_cb (GtkWidget *widget, gpointer data) {

gint ts, te, m;
GtkTextBuffer *text_buffer;
GtkTextIter titer;

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer), &titer, -1);

    ts = config.timeline_start;
    te = config.timeline_end + config.timeline_step;

    do {

        m = ts / 60;
        sprintf(tmpbuf, "%02d:%02d - \n", m, ts-m*60);
        gtk_text_buffer_insert(text_buffer, &titer, tmpbuf, -1);

        ts += config.timeline_step;
    
    } while (ts < te);
}

/*------------------------------------------------------------------------------*/

void
calendar_update_date (guint day, guint month, guint year) {

guint max_day;

    max_day = get_number_of_days_in_month(month, year);

    if (day > max_day) 
            day = max_day;

    jump_to_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_prev_day (void) {

    gui_calendar_get_date (GUI_CALENDAR(calendar), &year, &month, &day);
    if (day > 1) day--;
	else if (!(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)) {
		if (month > 0) {
			month--;
			day = get_number_of_days_in_month (month, year);
		} else if (year > JG_YEAR) {
			year--;
			month = 11;
			day = 31;
		}
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_next_day (void) {

    gui_calendar_get_date (GUI_CALENDAR(calendar), &year, &month, &day);
    if (day < get_number_of_days_in_month(month, year)) day++;
	else if (!(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)) {
		if (month < 11) {
			month++;
			day = 1;
		} else {
			year++;
			month = 0;
			day = 1;
		}
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_today (void) {
    set_today (FALSE);
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_prev_week (void) {

    gui_calendar_get_date (GUI_CALENDAR(calendar), &year, &month, &day);
    if (day > 7) day-=7;
	else if (!(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)) {
		if (month > 0) {
			month--;
			day = day + get_number_of_days_in_month(month, year) - 7;
		} else if (year > JG_YEAR) {
			year--;
			month = 11;
			day = day + get_number_of_days_in_month(month, year) - 7;
		}
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_next_week (void) {

    gui_calendar_get_date (GUI_CALENDAR(calendar), &year, &month, &day);
    if (day <= get_number_of_days_in_month(month, year)-7) day+=7;
	else if (!(config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE)) {
		if (month < 11) {
			day = 7 - (get_number_of_days_in_month(month, year) - day);
			month++;
		} else {
			day = 7 - (get_number_of_days_in_month(month, year) - day);
			year++;
			month = 0;
		}
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_prev_month (void) {

    gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);
    if (month > 0) month--;
	else if (year > JG_YEAR) {
		year--;
		month = 11;
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_next_month (void) {

    gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);
    if (month < 11) month++;
	else {
		year++;
		month = 0;
	}
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_prev_year (void) {

    gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);
    if (year > JG_YEAR) year--;
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void 
calendar_btn_next_year (void) {

    gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);
    year++;
    calendar_update_date (day, month, year);
}

/*------------------------------------------------------------------------------*/

void
calendar_day_selected_cb (GuiCalendar *calendar, gpointer user_data) {

    prev_day = day;
    prev_month = month;
    prev_year = year;

    if (dont_update == FALSE)
        gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);

    gtk_label_set_text (GTK_LABEL(date_label), get_date_name(day, month, year));

    if (dont_update == FALSE) {
        if (calendar_only == FALSE) {
            calendar_update_note (prev_day, prev_month, prev_year);
            mark_days_with_notes (day, month, year);
            set_note (day, month, year);
            set_day_info (day, month, year);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
calendar_dc_day_selected_cb (GuiCalendar *calendar, gpointer user_data) {

    if (calendar_only == FALSE) {
        if (!config.day_notes_visible) {
                gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(notes_button), 
                                             !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(notes_button)));
        }
    }
}

/*------------------------------------------------------------------------------*/

gchar *
get_mark_symbol (gint idx) {

gchar *symbols[] = {
    "", "·", "'", "*", "~", "|", "°"
};

    return (symbols[idx % 7]);
}

/*------------------------------------------------------------------------------*/

void
add_calendar_toolbar_widget (GtkUIManager *uim_widget, GtkWidget *widget, GtkBox *box) {

GtkWidget *handle_box;

    if (GTK_IS_TOOLBAR (widget)) {

        calendar_toolbar = GTK_TOOLBAR (widget);
        GTK_WIDGET_UNSET_FLAGS(calendar_toolbar, GTK_CAN_FOCUS);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (box, handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", 
                G_CALLBACK (gtk_widget_destroy), handle_box);
  
    } else {
        gtk_box_pack_start (box, widget, FALSE, FALSE, 0);
    }
    
    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

void 
gui_create_calendar(GtkWidget *notebook) {

GtkWidget       *vbox1;
GtkWidget       *vbox2;
GtkWidget       *hbox2;
GtkWidget       *hbox3;
GtkWidget       *hseparator;
GtkWidget       *alignment;
GtkWidget       *label;
GtkWidget       *frame;
GdkColor        calendar_color;
GtkUIManager    *uim_widget = NULL;
GError          *error = NULL;
GtkActionGroup  *action_group = NULL;
GtkWidget       *i_scrolledwindow;

const gchar *ui_info =
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"previous_year\" action=\"previous_year\" />\n"
"    <toolitem name=\"previous_month\" action=\"previous_month\" />\n"
"    <toolitem name=\"previous_day\" action=\"previous_day\" />\n"
"    <toolitem name=\"today\" action=\"today\" />\n"
"    <toolitem name=\"next_day\" action=\"next_day\" />\n"
"    <toolitem name=\"next_month\" action=\"next_month\" />\n"
"    <toolitem name=\"next_year\" action=\"next_year\" />\n"
"    <separator name=\"sep1\" />\n"
"    <toolitem name=\"jump_to_date\" action=\"jump_to_date\" />\n"
"    <toolitem name=\"edit_note\" action=\"edit_note\" />\n"
"    <toolitem name=\"full_year\" action=\"full_year\" />\n"
"  </toolbar>\n";

GtkActionEntry entries[] = {
    { "previous_year", OSMO_STOCK_PREV_YEAR, _("Previous year"), NULL, _("Previous year"), G_CALLBACK(calendar_btn_prev_year)},
    { "previous_month", OSMO_STOCK_PREV_MONTH, _("Previous month"), NULL, _("Previous month"), G_CALLBACK(calendar_btn_prev_month)},
    { "previous_day", OSMO_STOCK_PREV_DAY, _("Previous day"), NULL, _("Previous day"), G_CALLBACK(calendar_btn_prev_day)},
    { "today", OSMO_STOCK_TODAY, _("Today"), NULL, _("Today"), G_CALLBACK(calendar_btn_today)},
    { "next_day", OSMO_STOCK_NEXT_DAY, _("Next day"), NULL, _("Next day"), G_CALLBACK(calendar_btn_next_day)},
    { "next_month", OSMO_STOCK_NEXT_MONTH, _("Next month"), NULL, _("Next month"), G_CALLBACK(calendar_btn_next_month)},
    { "next_year", OSMO_STOCK_NEXT_YEAR, _("Next year"), NULL, _("Next year"), G_CALLBACK(calendar_btn_next_year)},
    { "jump_to_date", OSMO_STOCK_JUMPTO, _("Jump to date"), NULL, _("Jump to date"), G_CALLBACK(calendar_create_jumpto_window)},
    { "full_year", OSMO_STOCK_FULLYEAR, _("Full-year calendar"), NULL, _("Full-year calendar"), G_CALLBACK(calendar_create_fullyear_window)},
};

GtkToggleActionEntry t_entries[] = {
    { "edit_note", OSMO_STOCK_EDIT_NOTE, _("Toggle day note panel"), NULL, _("Toggle day note panel"), G_CALLBACK(day_notes_toggled_cb), FALSE }
};

guint n_entries = G_N_ELEMENTS (entries);
guint n_t_entries = G_N_ELEMENTS (t_entries);


    label = gtk_label_new(NULL);
    gtk_label_set_angle (GTK_LABEL(label), -90.0);
    sprintf(tmpbuf, "<b>%s</b>", _("Calendar"));
    gtk_label_set_markup (GTK_LABEL (label), tmpbuf);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox1, label);

    /*-------------------------------------------------------------------------------------*/

    action_group = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group, entries, n_entries, NULL);
    gtk_action_group_add_toggle_actions (action_group, t_entries, n_t_entries, NULL);
    gtk_action_group_set_sensitive(action_group, TRUE);

    uim_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (uim_widget, action_group, 0);
    g_signal_connect (uim_widget, "add_widget", G_CALLBACK (add_calendar_toolbar_widget), GTK_BOX(vbox1));
    
    if (!gtk_ui_manager_add_ui_from_string (uim_widget, ui_info, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (uim_widget);

    gtk_toolbar_set_style (calendar_toolbar, GTK_TOOLBAR_ICONS);

    notes_button = gtk_ui_manager_get_widget (uim_widget, "/toolbar/edit_note");

    gtk_toolbar_set_tooltips (calendar_toolbar, config.enable_tooltips);
    
    if (calendar_only == TRUE) {
        gtk_widget_hide(notes_button);
        gtk_widget_set_sensitive(notes_button, FALSE);
    }

    /*-------------------------------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, FALSE, 6);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, FALSE, 0);
    
    date_label = gtk_label_new (NULL);
    gtk_widget_show (date_label);
    gtk_box_pack_start (GTK_BOX (hbox2), date_label, TRUE, FALSE, 8);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, FALSE, 6);

    calendar = gui_calendar_new ();
    gtk_widget_show (calendar);
    GTK_WIDGET_UNSET_FLAGS(calendar, GTK_CAN_FOCUS);
    gui_calendar_set_display_options (GUI_CALENDAR (calendar), config.display_options);
    gtk_widget_modify_font (GTK_WIDGET(calendar), fd_calendar);
    gtk_box_pack_start (GTK_BOX (vbox2), calendar, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (calendar), "day-selected", 
                      G_CALLBACK (calendar_day_selected_cb), NULL);
    g_signal_connect (G_OBJECT (calendar), "day-selected-double-click", 
                      G_CALLBACK (calendar_dc_day_selected_cb), NULL);

    gdk_color_parse(config.selection_color, &calendar_color);
    gtk_widget_modify_base(GTK_WIDGET(calendar), GTK_STATE_ACTIVE, &calendar_color);
    gui_calendar_set_header_color(GUI_CALENDAR (calendar), config.header_color);
    gui_calendar_set_weekend_color(GUI_CALENDAR (calendar), config.weekend_color);
    gui_calendar_set_mark_symbol (GUI_CALENDAR (calendar), get_mark_symbol(config.mark_symbol));

    if (calendar_only == FALSE) {

    /*-------------------------------------------------------------------------------------*/
    /* notes */

        notes_vbox = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (notes_vbox);
        gtk_box_pack_start (GTK_BOX (vbox2), notes_vbox, TRUE, TRUE, 0);

        hbox3 = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox3);
        gtk_box_pack_start (GTK_BOX (notes_vbox), hbox3, FALSE, FALSE, 0);

        alignment = gtk_alignment_new (0.0, 0.5, 0, 1);
        gtk_widget_show (alignment);
        gtk_box_pack_start (GTK_BOX (hbox3), alignment, FALSE, FALSE, 4);

        sprintf(tmpbuf, "%s:", _("Notes"));
        note_label = gtk_label_new (tmpbuf);
        gtk_widget_show (note_label);
        gtk_container_add (GTK_CONTAINER (alignment), note_label);

        if (config.default_stock_icons) {
            n_close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
        } else {
            n_close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
        }
        GTK_WIDGET_UNSET_FLAGS(n_close_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(n_close_button), GTK_RELIEF_NONE);
        if(config.enable_tooltips)
            gtk_tooltips_set_tip (osmo_tooltips, n_close_button, _("Close note panel"), NULL);
        gtk_box_pack_end (GTK_BOX (hbox3), n_close_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (n_close_button), "clicked",
                            G_CALLBACK (calendar_close_text_cb), NULL);

        n_clear_button = gui_stock_button(OSMO_STOCK_BUTTON_CLEAR, FALSE);
        gtk_widget_show(n_clear_button);
        GTK_WIDGET_UNSET_FLAGS(n_clear_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(n_clear_button), GTK_RELIEF_NONE);
        if(config.enable_tooltips) {
            gtk_tooltips_set_tip (osmo_tooltips, n_clear_button, _("Clear text"), NULL);
        }
        gtk_box_pack_end (GTK_BOX (hbox3), n_clear_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (n_clear_button), "clicked",
                            G_CALLBACK (calendar_clear_text_cb), NULL);

        n_timeline_button = gui_stock_button(OSMO_STOCK_BUTTON_INSERT_TIMELINE, FALSE);
        gtk_widget_show(n_timeline_button);
        GTK_WIDGET_UNSET_FLAGS(n_timeline_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(n_timeline_button), GTK_RELIEF_NONE);
        if(config.enable_tooltips) {
            gtk_tooltips_set_tip (osmo_tooltips, n_timeline_button, _("Insert timeline"), NULL);
        }
        gtk_box_pack_end (GTK_BOX (hbox3), n_timeline_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (n_timeline_button), "clicked",
                            G_CALLBACK (calendar_insert_timeline_cb), NULL);

        note_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (note_scrolledwindow);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (note_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start (GTK_BOX (notes_vbox), note_scrolledwindow, TRUE, TRUE, 0);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (note_scrolledwindow), GTK_SHADOW_IN);

        textview = gtk_text_view_new ();
        gtk_widget_show (textview);
        gtk_container_add (GTK_CONTAINER (note_scrolledwindow), textview);
        gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (textview), TRUE);
        gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
        gtk_widget_modify_font (GTK_WIDGET(textview), fd_notes);
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(textview), 4);
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textview), 4);
        gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textview), 4);

    /*-------------------------------------------------------------------------------------*/
    /* day info */
 
        day_info_vbox = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (day_info_vbox);
        gtk_box_pack_start (GTK_BOX (vbox2), day_info_vbox, TRUE, TRUE, 0);

        frame = gtk_frame_new (NULL);
        gtk_widget_show (frame);
        gtk_box_pack_start (GTK_BOX (day_info_vbox), frame, TRUE, TRUE, 0);
        gtk_frame_set_label_align (GTK_FRAME (frame), 0.98, 0.5);
        gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);

        alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
        gtk_widget_show (alignment);
        gtk_container_add (GTK_CONTAINER (frame), alignment);
        gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);

        sprintf(tmpbuf, "<b>%s</b>", _("Info"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_frame_set_label_widget (GTK_FRAME (frame), label);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

        i_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (i_scrolledwindow);
        gtk_container_add (GTK_CONTAINER (alignment), i_scrolledwindow);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (i_scrolledwindow), GTK_SHADOW_NONE);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (i_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

        day_desc_textview = gtk_text_view_new ();
        gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (day_desc_textview), GTK_WRAP_WORD);
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(day_desc_textview), 4);
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(day_desc_textview), 4);
        gtk_text_view_set_right_margin(GTK_TEXT_VIEW(day_desc_textview), 4);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(day_desc_textview), FALSE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(day_desc_textview), FALSE);
        gtk_widget_show (day_desc_textview);
        gtk_container_add (GTK_CONTAINER (i_scrolledwindow), day_desc_textview);
        gtk_widget_realize(day_desc_textview);
        gtk_widget_modify_base (day_desc_textview, GTK_STATE_NORMAL, &main_window->style->bg[GTK_STATE_NORMAL]);

        day_desc_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(day_desc_textview));
        gtk_text_buffer_create_tag (day_desc_text_buffer, "bold", "weight", PANGO_WEIGHT_ULTRABOLD, NULL);

    /*-------------------------------------------------------------------------------------*/

    }

    gtk_widget_modify_font (GTK_WIDGET(date_label), fd_day_name);
    gui_calendar_get_date(GUI_CALENDAR(calendar), &year, &month, &day);
    jump_to_date (day, month, year);

    if (calendar_only == FALSE) {
        if (!config.day_notes_visible) {
            gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(notes_button), FALSE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
            gtk_widget_show(day_info_vbox);
            gtk_widget_hide(notes_vbox);
        } else {
            gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(notes_button), TRUE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), TRUE);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), TRUE);
            gtk_widget_hide(day_info_vbox);
            gtk_widget_show(notes_vbox);
        }

        gtk_widget_realize(GTK_WIDGET(textview));
        gtk_widget_grab_focus(GTK_WIDGET(textview));
    }
}

/*------------------------------------------------------------------------------*/

