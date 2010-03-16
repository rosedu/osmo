
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
#include "calendar_widget.h"
#include "calendar_jumpto.h"
#include "calendar_fullyear.h"
#include "tasks.h"
#include "tasks_items.h"
#include "contacts.h"
#include "gui_utils.h"
#include "options_prefs.h"
#include "options_gui_tasks.h"
#include "calendar_notes.h"
#include "calendar_timeline.h"
#include "calendar_calc.h"
#include "calendar_ical.h"
#include "check_events.h"
#include "calendar_moon.h"
#include "stock_icons.h"

/*------------------------------------------------------------------------------*/

void
set_moon_icon (gint moon_phase, GUI *appGUI) {

const guint8 *icons_table[] = {
    moon_phase_0, moon_phase_1, moon_phase_2, moon_phase_3,
    moon_phase_4, moon_phase_5, moon_phase_6, moon_phase_7
};

GdkPixbuf *icon;
gchar tmpbuf[BUFFER_SIZE];

    icon = gdk_pixbuf_new_from_inline (-1, icons_table[moon_phase], FALSE, NULL);
    gtk_image_set_from_pixbuf (GTK_IMAGE(appGUI->cal->moon_icon), icon);
    g_object_unref (icon);

    g_snprintf (tmpbuf, BUFFER_SIZE, "(%s)", get_moon_phase_name(moon_phase));
    gtk_label_set_text (GTK_LABEL(appGUI->cal->moon_phase_label), tmpbuf);
}

/*------------------------------------------------------------------------------*/

void
mark_days_with_notes (guint day, guint month, guint year, GUI *appGUI) {

gint i;
guint32 julian_day;

    gui_calendar_clear_marks (GUI_CALENDAR(appGUI->cal->calendar));
    gui_calendar_tc_clear_marks (GUI_CALENDAR(appGUI->cal->calendar));

    if (config.enable_day_mark == FALSE) return;

    for (i = 1; i <= g_date_get_days_in_month (month + 1, year); i++) {
        if (check_note(i, month+1, year, appGUI) == TRUE) {
            gui_calendar_mark_day_color (GUI_CALENDAR(appGUI->cal->calendar), i, get_color(i, month+1, year, appGUI));
        }
        julian_day = date_to_julian (i, month, year);
        if (check_tasks_contacts(julian_day, appGUI) == TRUE) {
            gui_calendar_tc_mark_day (GUI_CALENDAR(appGUI->cal->calendar), i);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
calendar_refresh_marks(GUI *appGUI) {
    mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
}

/*------------------------------------------------------------------------------*/

gint
get_marked_days (guint month, guint year, GUI *appGUI) {

gint i, n;

    n = 0;

    for (i = 1; i <= g_date_get_days_in_month (month + 1, year); i++) {
        if (check_note(i, month+1, year, appGUI) == TRUE) {
            n++;
        }
    }

    return n;
}

/*------------------------------------------------------------------------------*/

void
calendar_update_note (guint uday, guint umonth, guint uyear, gchar *color, GUI *appGUI) {

GtkTextBuffer *textbuffer;
GtkTextIter iter_start, iter_end;
gchar *text;

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_start, 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_end, -1);

    text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &iter_start, &iter_end, TRUE);

    if (text != NULL) {
        if (strlen(text)) {
            calendar_refresh_marks(appGUI);
            update_aux_calendars (appGUI);
            add_note (uday, umonth+1, uyear, color, text, appGUI);
        } else {
            remove_note (uday, umonth+1, uyear, appGUI);
        }
    }

    set_day_info (uday, umonth, uyear, appGUI);
}

/*------------------------------------------------------------------------------*/

void
mark_events (GtkWidget *calendar, guint month, guint year, GUI *appGUI)
{
guint32 julian_day;
guint i, days;

	gui_calendar_select_month (GUI_CALENDAR (calendar), month, year);
	gui_calendar_select_day (GUI_CALENDAR (calendar), 1);
	gui_calendar_clear_marks (GUI_CALENDAR (calendar));
	gui_calendar_tc_clear_marks (GUI_CALENDAR (calendar));

	if (config.enable_day_mark == FALSE) return;

	days = g_date_get_days_in_month (month + 1, year);

	for (i = 1; i <= days; i++) {
		if (check_note (i, month + 1, year, appGUI) == TRUE) {
			gui_calendar_mark_day_color (GUI_CALENDAR (calendar), i, get_color (i, month + 1, year, appGUI));
		}
		julian_day = date_to_julian (i, month, year);
		if (check_tasks_contacts(julian_day, appGUI) == TRUE) {
			gui_calendar_tc_mark_day (GUI_CALENDAR (calendar), i);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
update_aux_calendars (GUI *appGUI)
{
gchar buffer[BUFFER_SIZE];
GDate *cdate;

	if (appGUI->calendar_only == TRUE || config.enable_auxilary_calendars == FALSE ||
	    config.auxilary_calendars_state == FALSE) return;

	cdate = g_date_new ();
	g_return_if_fail (cdate != NULL);

	g_date_set_dmy (cdate, 1, appGUI->cal->month + 1, appGUI->cal->year);
	g_date_subtract_months (cdate, 1);
	g_date_strftime (buffer, BUFFER_SIZE, "%B %Y", cdate);
	gtk_label_set_text (GTK_LABEL (appGUI->cal->prev_month_label), buffer);
	mark_events (appGUI->cal->calendar_prev, g_date_get_month (cdate) - 1, g_date_get_year (cdate), appGUI);

	g_date_set_dmy (cdate, 1, appGUI->cal->month + 1, appGUI->cal->year);
	g_date_add_months (cdate, 1);
	g_date_strftime (buffer, BUFFER_SIZE, "%B %Y", cdate);
	gtk_label_set_text (GTK_LABEL(appGUI->cal->next_month_label), buffer);
	mark_events (appGUI->cal->calendar_next, g_date_get_month (cdate) - 1, g_date_get_year (cdate), appGUI);

	g_date_free (cdate);
}

/*------------------------------------------------------------------------------*/

gint
check_add_tasks (guint32 selected_julian, gboolean count, GUI *appGUI)
{
GtkTreePath *path;
GtkTreeIter iter;
GtkTreeModel *model;
guint32 date;
gboolean done;
gint time;
gchar *summary, *category;
gchar tmpbuf[BUFFER_SIZE];
gint i;

	i = 0;
    model = GTK_TREE_MODEL (appGUI->tsk->tasks_list_store);
	g_return_val_if_fail (model != NULL, i);

	path = gtk_tree_path_new_first();

	while (gtk_tree_model_get_iter (model, &iter, path) == TRUE) {
		gtk_tree_model_get (model, &iter, TA_COLUMN_DUE_DATE_JULIAN, &date, TA_COLUMN_CATEGORY, &category, -1);

		if (date == selected_julian && tasks_category_get_state (category, STATE_CALENDAR, appGUI) == TRUE) {
			if (count == FALSE) {
				gtk_tree_model_get (model, &iter, TA_COLUMN_DUE_TIME, &time, TA_COLUMN_DONE, &done,
				                    TA_COLUMN_SUMMARY, &summary, -1);

				if (time >= 0) {
					g_snprintf (tmpbuf, BUFFER_SIZE, "%d. [%02d:%02d] %s", i + 1, time / 3600, time / 60 % 60, summary);
				} else {
					g_snprintf (tmpbuf, BUFFER_SIZE, "%d. %s", i + 1, summary);
				}

				if (done == TRUE) {
					gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer,
					                                          &appGUI->cal->day_desc_iter, tmpbuf, -1, "strike", NULL);
				} else {
					gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1);
				}
				gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n", -1);
				g_free (summary);
			}
			i++;
		}

		g_free (category);
		gtk_tree_path_next (path);
	}
	gtk_tree_path_free (path);

	return i;
}

/*------------------------------------------------------------------------------*/

gint
check_add_contacts (guint32 selected_julian, gboolean count, GUI *appGUI)
{
GtkTreePath *path;
GtkTreeIter iter;
GtkTreeModel *model;
guint32 julian, age;
gchar *first_name, *last_name;
gchar tmpbuf[BUFFER_SIZE];
GDate *cdate;
gint i;

	i = 0;
	model = GTK_TREE_MODEL (appGUI->cnt->contacts_list_store);
	g_return_val_if_fail (model != NULL, i);
	cdate = g_date_new ();
	g_return_val_if_fail (cdate != NULL, i);

	path = gtk_tree_path_new_first ();

	while (gtk_tree_model_get_iter (model, &iter, path) == TRUE) {
		gtk_tree_model_get (model, &iter, COLUMN_BIRTH_DAY_DATE, &julian, -1);

		if (g_date_valid_julian (julian)) {
			g_date_set_julian (cdate, julian);
			age = appGUI->cal->year - g_date_get_year (cdate);

			if (age >= 0) {
				g_date_set_year (cdate, appGUI->cal->year);
				julian = g_date_get_julian (cdate);

				if (julian == selected_julian) {

					if (count == FALSE) {
						gtk_tree_model_get (model, &iter, COLUMN_FIRST_NAME, &first_name, COLUMN_LAST_NAME, &last_name, -1);

						if (age == 0) {
							g_snprintf (tmpbuf, BUFFER_SIZE, "%s %s %s\n", first_name, last_name, _("was born"));
						} else {
							g_snprintf (tmpbuf, BUFFER_SIZE, "%s %s (%d %s)\n", first_name, last_name, age,
							            ngettext ("year old", "years old", age));
						}

						gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer,
						                        &appGUI->cal->day_desc_iter, tmpbuf, -1);

						g_free (first_name);
						g_free (last_name);
					}
					i++;
				}
			}
		}
		gtk_tree_path_next (path);
	}
	gtk_tree_path_free (path);
	g_date_free (cdate);

	return i;
}

/*------------------------------------------------------------------------------*/

gchar *
get_day_category (gint day, gint month, gint year, GUI *appGUI) {

GtkTreeIter iter;
static gchar temp[BUFFER_SIZE];
gint i;
gchar *color_val, *color_name, *color_sel;

    i = 0;

    color_sel = get_color (day, month, year, appGUI);

    temp[0] = '\0';

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, i++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, 1, &color_val, 2, &color_name, -1);

        if (color_sel != NULL) {
            if (!strcmp(color_val, color_sel)) {
                sprintf(temp, "%s", color_name);
                g_free(color_val);
                g_free(color_name);
                break;
            }
        }

        g_free(color_val);
        g_free(color_name);
    }
    
    return temp;
}

/*------------------------------------------------------------------------------*/

void
update_clock (GUI *appGUI) {

gchar tmpbuf[BUFFER_SIZE];

    if (config.time_format == TIME_24) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<tt>%s</tt>", current_time_to_str(TIME_HH_MM_SS));
    } else {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<tt>%s</tt>", current_time_to_str(TIME_HH_MM_SS_AMPM));
    }

    gtk_label_set_markup (GTK_LABEL (appGUI->cal->time_label), tmpbuf);
}

/*------------------------------------------------------------------------------*/

void
set_day_info (guint dday, guint dmonth, guint dyear, GUI *appGUI)
{
GtkTextChildAnchor *anchor;
GtkWidget *table, *label;
gchar *text, *day_category;
gchar tmpbuf[BUFFER_SIZE];
gint ydays, edays;
gboolean current_date;
static guint cmonth;
gint i, rows;
guint32 selected_julian;
GDate *date;

    gui_clear_text_buffer (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter);

	date = g_date_new_dmy (dday, dmonth + 1, dyear);
	g_return_if_fail (date != NULL);

    ydays = g_date_get_day_of_year (date);

    rows = config.di_show_current_time + config.di_show_day_number + config.di_show_marked_days + 
           + config.di_show_week_number + config.di_show_weekend_days + config.di_show_moon_phase +
           config.di_show_zodiac_sign + config.di_show_current_day_distance + config.di_show_day_category;

    if (rows != 0) {
        anchor = gtk_text_buffer_create_child_anchor (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter);

        table = gtk_table_new (rows, 3, FALSE);
        gtk_widget_show (table);
        gtk_table_set_row_spacings (GTK_TABLE (table), 4);
        gtk_table_set_col_spacings (GTK_TABLE (table), 8);
    }

	current_date = (g_date_get_julian (date) == get_gdate_current_julian ());

    day_category = get_day_category (dday, dmonth+1, dyear, appGUI);

    i = 0;

    if (current_date && config.di_show_current_time) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Current time"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_day_number) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Day number"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_current_day_distance) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Today distance"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_week_number) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Week number"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_marked_days) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Marked days"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_weekend_days) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Weekend days"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_day_category && strlen(day_category) && config.enable_day_mark) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Day category"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_moon_phase) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Moon phase"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        i++;
    }

    if (config.di_show_zodiac_sign) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s:</b>", _("Zodiac sign"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
    }

    i = 0;

    appGUI->cal->time_label = gtk_label_new (NULL);
    if (current_date && config.di_show_current_time) {
        update_clock (appGUI);
        gtk_widget_show (appGUI->cal->time_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->time_label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->time_label), 0, 0.5);
        i++;
    }

    if (config.di_show_day_number) {
        appGUI->cal->day_number_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->day_number_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->day_number_label, 1, 2, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->day_number_label), 0, 0.5);

        appGUI->cal->day_number_year_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->day_number_year_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->day_number_year_label, 2, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->day_number_year_label), 0, 0.5);
        i++;
    }

    if (config.di_show_current_day_distance) {
        label = gtk_label_new (NULL);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        gtk_label_set_text (GTK_LABEL(label), get_current_date_distance_str (g_date_get_julian (date)));
        i++;
    }

    if (config.di_show_week_number) {
        appGUI->cal->week_number_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->week_number_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->week_number_label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->week_number_label), 0, 0.5);
        i++;
    }

    if (config.di_show_marked_days) {
        appGUI->cal->marked_days_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->marked_days_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->marked_days_label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->marked_days_label), 0, 0.5);
        i++;
    }

    if (config.di_show_weekend_days) {
        appGUI->cal->weekend_days_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->weekend_days_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->weekend_days_label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->weekend_days_label), 0, 0.5);
        i++;
    }

    if (config.di_show_day_category && strlen(day_category) && config.enable_day_mark) {
        appGUI->cal->day_category_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->day_category_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->day_category_label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->day_category_label), 0, 0.5);
        i++;
    }

    if (config.di_show_moon_phase) {
        appGUI->cal->moon_icon = gtk_image_new();
        gtk_widget_show (appGUI->cal->moon_icon);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->moon_icon, 1, 2, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->moon_icon), 0, 0.5);

        appGUI->cal->moon_phase_label = gtk_label_new (NULL);
        gtk_widget_show (appGUI->cal->moon_phase_label);
        gtk_table_attach (GTK_TABLE (table), appGUI->cal->moon_phase_label, 2, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (appGUI->cal->moon_phase_label), 0, 0.5);

        set_moon_icon (calc_moon_phase (dyear, dmonth+1, dday), appGUI);
        i++;
    }

    if (config.di_show_zodiac_sign) {
        label = gtk_label_new (NULL);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (table), label, 1, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
        gtk_label_set_text (GTK_LABEL(label), get_zodiac_name (dday, dmonth+1));
    }
    
    if (config.di_show_day_number) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "%d", ydays);
        gtk_label_set_text (GTK_LABEL(appGUI->cal->day_number_label), tmpbuf);

        edays = get_days_per_year(dyear)-ydays;
        if (edays) {
            g_snprintf (tmpbuf, BUFFER_SIZE, "(%d %s)", edays,
                ngettext ("day till end of year", "days till end of year", edays));
        } else {
            g_snprintf (tmpbuf, BUFFER_SIZE, "(%s)", _("the last day of the year"));
        }
        gtk_label_set_text (GTK_LABEL(appGUI->cal->day_number_year_label), tmpbuf);
    }

    if (config.di_show_week_number) {
        if (week_number (dyear, dmonth+1, dday) > weeks_in_year (dyear)) {
            sprintf (tmpbuf, "1 / %d", weeks_in_year (dyear+1));
        } else {
            sprintf (tmpbuf, "%d / %d", week_number (dyear, dmonth+1, dday), weeks_in_year (dyear));
        }
        gtk_label_set_text (GTK_LABEL(appGUI->cal->week_number_label), tmpbuf);
    }

    if (config.di_show_marked_days) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "%d", get_marked_days(dmonth, dyear, appGUI));
        gtk_label_set_text (GTK_LABEL(appGUI->cal->marked_days_label), tmpbuf);
    }

    if (config.di_show_weekend_days) {
        g_snprintf (tmpbuf, BUFFER_SIZE, "%d", get_weekend_days(dmonth+1, dyear));
        gtk_label_set_text (GTK_LABEL(appGUI->cal->weekend_days_label), tmpbuf);
    }

    if (config.di_show_day_category && strlen(day_category) && config.enable_day_mark) {
        gtk_label_set_text (GTK_LABEL(appGUI->cal->day_category_label), day_category);
    }

    if (rows != 0) {
        gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), table, anchor);
        gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n\n", -1);
    }

    if (config.di_show_notes && config.enable_day_mark) {

        text = get_note(dday, dmonth+1, dyear, appGUI);

        if (text != NULL) {
            g_snprintf (tmpbuf, BUFFER_SIZE, "%s:\n", _("Day notes"));
            gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1, "bold", NULL);

            if (date_to_julian(dday, dmonth, dyear) < get_julian_for_today() && config.strikethrough_past_notes == TRUE) {
                gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, text, -1, "strike", NULL);
            } else {
                gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, text, -1);
            }

            if (text[strlen(text)-1] != '\n') {
                gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n\n", -1);
            } else {
                gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n", -1);
            }
        }
    
    }

    if (appGUI->calendar_only != TRUE && appGUI->all_pages_added != FALSE && config.enable_day_mark) {

        selected_julian = date_to_julian (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year);

        /* check tasks */

        i = check_add_tasks(selected_julian, TRUE, appGUI);

        if (i) {
            g_snprintf (tmpbuf, BUFFER_SIZE, "%s:\n", _("Day tasks"));
            gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1, "bold", NULL);
            check_add_tasks(selected_julian, FALSE, appGUI);
            gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n", -1);
        }

        /* check contacts */

        i = check_add_contacts(selected_julian, TRUE, appGUI);

        if (i) {
            g_snprintf (tmpbuf, BUFFER_SIZE, "%s:\n", _("Birthday"));
            gtk_text_buffer_insert_with_tags_by_name (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, tmpbuf, -1, "bold", NULL);
            check_add_contacts(selected_julian, FALSE, appGUI);
            gtk_text_buffer_insert (appGUI->cal->day_desc_text_buffer, &appGUI->cal->day_desc_iter, "\n", -1);
        }
    }
    
#ifdef HAVE_LIBICAL
	calendar_display_ics (date, appGUI);
#endif  /* HAVE_LIBICAL */

    if (cmonth != dmonth) {
        cmonth = dmonth;
        update_aux_calendars (appGUI);
    }
}

/*------------------------------------------------------------------------------*/

void
set_note (guint uday, guint umonth, guint uyear, GUI *appGUI) {

GtkTextBuffer *textbuffer;
GtkTextIter iter_start, iter_end;
gchar *t;

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview));
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_start, 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &iter_end, -1);

    t = get_note(uday, umonth+1, uyear, appGUI);

    if (t != NULL) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), t, -1);
    } else {
        gtk_text_buffer_delete (GTK_TEXT_BUFFER(textbuffer), &iter_start, &iter_end);
    }
}

/*------------------------------------------------------------------------------*/

void
day_notes_toggled_cb (GtkToggleToolButton *togglebutton, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    config.day_notes_visible = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button));

    if (!config.day_notes_visible) {
        config.enable_day_mark = TRUE;
        calendar_update_note (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, 
                              get_color(appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, appGUI), appGUI);
        mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), FALSE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), FALSE);
        gtk_widget_hide(appGUI->cal->notes_vbox);
        gtk_widget_show(appGUI->cal->day_info_vbox);
        gui_systray_tooltip_update(appGUI);
	    update_aux_calendars (appGUI);
    } else {
        gtk_widget_show(appGUI->cal->notes_vbox);
        gtk_widget_hide(appGUI->cal->day_info_vbox);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), TRUE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cal->calendar_note_textview));
    }
}

/*------------------------------------------------------------------------------*/
/* FIXME */
void
jump_to_date (guint jday, guint jmonth, guint jyear, GUI *appGUI) {

    appGUI->cal->dont_update = TRUE;
    gui_calendar_select_day(GUI_CALENDAR(appGUI->cal->calendar), 1);     /* Trick: always select valid day number */

    appGUI->cal->day = jday;
    appGUI->cal->month = jmonth;
    appGUI->cal->year = jyear;
    gui_calendar_select_month(GUI_CALENDAR(appGUI->cal->calendar), appGUI->cal->month, appGUI->cal->year);
    gui_calendar_select_day(GUI_CALENDAR(appGUI->cal->calendar), appGUI->cal->day);
    gtk_label_set_text (GTK_LABEL(appGUI->cal->date_label), 
                        get_date_name (appGUI->cal->day, appGUI->cal->month + 1, appGUI->cal->year));

    if (appGUI->calendar_only == FALSE) {
        mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
        set_note (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
        set_day_info (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
        enable_disable_note_buttons(appGUI);
    }

    appGUI->cal->dont_update = FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_jump_to_date (GDate *cdate, GUI *appGUI)
{
	appGUI->cal->dont_update = TRUE;
	gui_calendar_select_day (GUI_CALENDAR (appGUI->cal->calendar), 1);     /* Trick: always select valid day number */

	g_date_set_julian (appGUI->cal->date, g_date_get_julian (cdate));
	gui_calendar_select_month (GUI_CALENDAR (appGUI->cal->calendar),
	                           g_date_get_month (appGUI->cal->date) - 1, g_date_get_year (appGUI->cal->date));
	gui_calendar_select_day (GUI_CALENDAR (appGUI->cal->calendar), g_date_get_day (appGUI->cal->date));
	gtk_label_set_text (GTK_LABEL (appGUI->cal->date_label), calendar_get_date_name (appGUI->cal->date));

	/* FIXME */
	sync_cal_date_with_gdate (appGUI);
	if (appGUI->calendar_only == FALSE) {
		mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
		set_note (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
		set_day_info (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
		enable_disable_note_buttons (appGUI);
	}

	appGUI->cal->dont_update = FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_set_today (GUI *appGUI)
{
	g_date_set_time_t (appGUI->cal->date, time (NULL));
	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_close_text_cb (GtkWidget *widget, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;

	gtk_widget_hide (appGUI->cal->notes_vbox);
	gtk_widget_show (appGUI->cal->day_info_vbox);
	gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON (appGUI->cal->notes_button), FALSE);
	gtk_widget_grab_focus (GTK_WIDGET (appGUI->cal->calendar));
}

/*------------------------------------------------------------------------------*/

gchar *
calendar_get_note_text (GUI *appGUI)
{
GtkTextBuffer *textbuffer;
GtkTextIter iter_start, iter_end;

	if (appGUI->calendar_only == TRUE) return NULL;

	textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->cal->calendar_note_textview));
	gtk_text_buffer_get_iter_at_offset (GTK_TEXT_BUFFER (textbuffer), &iter_start, 0);
	gtk_text_buffer_get_iter_at_offset (GTK_TEXT_BUFFER (textbuffer), &iter_end, -1);

	return gtk_text_buffer_get_text (GTK_TEXT_BUFFER (textbuffer), &iter_start, &iter_end, TRUE);
}

/*------------------------------------------------------------------------------*/

void
calendar_clear_text_cb (GtkWidget *widget, gpointer user_data)
{
gint response;
gchar *text;
gchar tmpbuf[BUFFER_SIZE];

	GUI *appGUI = (GUI *) user_data;
	text = calendar_get_note_text (appGUI);
	if (text == NULL) return;
	if (!strlen (text)) return;

	g_snprintf (tmpbuf, BUFFER_SIZE, "%s\n\n%s", _("Selected day note will be removed."), _("Continue?"));
	response = gui_create_dialog (GTK_MESSAGE_QUESTION, tmpbuf, GTK_WINDOW (appGUI->main_window));

	if (response == GTK_RESPONSE_YES) {
		gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (appGUI->cal->calendar_note_textview))), "", -1);
		enable_disable_note_buttons (appGUI);
		calendar_update_note (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, NULL, appGUI);
		mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
		gui_systray_tooltip_update (appGUI);
	}
}

/*------------------------------------------------------------------------------*/

void
calendar_insert_timeline_cb (GtkWidget *widget, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	calendar_create_insert_timeline_window (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_select_color_cb (GtkWidget *widget, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	calendar_create_color_selector_window (FALSE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_update_date (guint day, guint month, guint year, GUI *appGUI)
{
guint max_day;

	max_day = g_date_get_days_in_month (month + 1, year);

	if (day > max_day)
		day = max_day;

	jump_to_date (day, month, year, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_day (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);
	if (g_date_get_julian (appGUI->cal->date) <= 1) return;

	if ((g_date_get_day (appGUI->cal->date) > 1) || (config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE) == FALSE)
		g_date_subtract_days (appGUI->cal->date, 1);

	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_day (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);

	if ((g_date_get_day (appGUI->cal->date) < get_days_in_selected_month (appGUI) ||
	    (config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE) == FALSE)) {
		g_date_add_days (appGUI->cal->date, 1);
	}
	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_today (GUI *appGUI)
{
	calendar_store_note (appGUI);
	calendar_set_today (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_week (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);
	if (g_date_get_julian (appGUI->cal->date) <= 7) return;

	if ((g_date_get_day (appGUI->cal->date) > 7) || (config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE) == FALSE)
		g_date_subtract_days (appGUI->cal->date, 7);

	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_week (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);

	if ((g_date_get_day (appGUI->cal->date) + 7 <= get_days_in_selected_month (appGUI)) ||
	    (config.display_options & GUI_CALENDAR_NO_MONTH_CHANGE) == FALSE) {
		g_date_add_days (appGUI->cal->date, 7);
	}
	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_month (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);
	if (g_date_get_julian (appGUI->cal->date) <= 31) return;

	g_date_subtract_months (appGUI->cal->date, 1);
	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_month (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);

	g_date_add_months (appGUI->cal->date, 1);
	calendar_jump_to_date (appGUI->cal->date, appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_year (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);
	if (g_date_get_year (appGUI->cal->date) == 1) return;

	g_date_subtract_years (appGUI->cal->date, 1);
	calendar_jump_to_date (appGUI->cal->date, appGUI);
	update_aux_calendars (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_year (GUI *appGUI)
{
	calendar_store_note (appGUI);
	gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);

	g_date_add_years (appGUI->cal->date, 1);
	calendar_jump_to_date (appGUI->cal->date, appGUI);
	update_aux_calendars (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_store_note (GUI *appGUI)
{
gchar *text;

	text = calendar_get_note_text(appGUI);

	if (config.day_notes_visible && strlen(text)) {
		add_note (appGUI->cal->day, appGUI->cal->month + 1, appGUI->cal->year,
		          get_color(appGUI->cal->day, appGUI->cal->month + 1, appGUI->cal->year, appGUI),
		          text, appGUI);
	}
}

/*------------------------------------------------------------------------------*/

void
calendar_day_selected_cb (GuiCalendar *calendar, gpointer user_data)
{
guint prev_day, prev_month, prev_year;

    GUI *appGUI = (GUI *) user_data;

    prev_day = appGUI->cal->day;
    prev_month = appGUI->cal->month;
    prev_year = appGUI->cal->year;

    if (appGUI->cal->dont_update == FALSE)
        gui_calendar_get_date(GUI_CALENDAR(calendar), 
                              &appGUI->cal->year, &appGUI->cal->month, &appGUI->cal->day);

    gtk_label_set_text (GTK_LABEL(appGUI->cal->date_label), 
                        get_date_name (appGUI->cal->day, appGUI->cal->month + 1, appGUI->cal->year));

    if (appGUI->cal->dont_update == FALSE) {
        if (appGUI->calendar_only == FALSE) {
            calendar_update_note (prev_day, prev_month, prev_year, get_color (prev_day, prev_month+1, prev_year, appGUI), appGUI);
            mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
            set_note (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
            set_day_info (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
            enable_disable_note_buttons(appGUI);
        }
    }

    if (appGUI->calendar_only == FALSE) {
        /* enable/disable 'select day color' popup entry */
        if (check_note(appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, appGUI) == TRUE) {
            gtk_widget_show(appGUI->cal->popup_menu_select_day_color_entry);
        } else {
            gtk_widget_hide(appGUI->cal->popup_menu_select_day_color_entry);
        }
    }
}

/*------------------------------------------------------------------------------*/

void
calendar_dc_day_selected_cb (GuiCalendar *calendar, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (appGUI->calendar_only == FALSE) {
        if (!config.day_notes_visible) {
                enable_disable_note_buttons(appGUI);
                gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), 
                                             !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button)));
        }
    }
}

/*------------------------------------------------------------------------------*/

void
add_calendar_toolbar_widget (GtkUIManager *uim_widget, GtkWidget *widget, gpointer user_data) {

GtkWidget *handle_box;

    GUI *appGUI = (GUI *)user_data;

    if (GTK_IS_TOOLBAR (widget)) {

        appGUI->cal->calendar_toolbar = GTK_TOOLBAR (widget);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->calendar_toolbar, GTK_CAN_FOCUS);

        handle_box = gtk_handle_box_new ();
        gtk_widget_show (handle_box);
        gtk_container_add (GTK_CONTAINER (handle_box), widget);
        gtk_box_pack_start (GTK_BOX(appGUI->cal->vbox), handle_box, FALSE, FALSE, 0);
        g_signal_connect_swapped (widget, "destroy", 
                G_CALLBACK (gtk_widget_destroy), handle_box);

    } else {
        gtk_box_pack_start (GTK_BOX(appGUI->cal->vbox), widget, FALSE, FALSE, 0);
    }

    gtk_widget_show (widget);
}

/*------------------------------------------------------------------------------*/

void
enable_disable_note_buttons(GUI *appGUI) {

gboolean state;

    if(strlen(calendar_get_note_text (appGUI))) {
        state = TRUE;
    } else {
        state = FALSE;
    }

    gtk_widget_set_sensitive(appGUI->cal->n_clear_button, state);
    gtk_widget_set_sensitive(appGUI->cal->n_select_color_button, state);
}

/*------------------------------------------------------------------------------*/

gint
calendar_textview_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	enable_disable_note_buttons (appGUI);

	return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
click_handler_cb (GtkWidget * widget, GdkEventButton * event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {    /* RMB */
        gtk_menu_popup(GTK_MENU(appGUI->cal->month_selector_menu), NULL, NULL, NULL, NULL, event->button, event->time);
        return TRUE;
    } else if (event->type == GDK_BUTTON_PRESS && event->button == 2) {     /* MMB */
        calendar_update_date (appGUI->cal->day, get_current_month(), appGUI->cal->year, appGUI);
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_create_jumpto_window_cb (GtkWidget *widget, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	calendar_create_jumpto_window (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_create_fullyear_window_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_create_fullyear_window (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_create_datecalc_window_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_create_calc_window (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_year_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_prev_year (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_year_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_next_year (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_month_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_prev_month (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_month_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_next_month (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_prev_day_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_prev_day (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_next_day_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_next_day (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_btn_today_cb (GtkWidget *widget, gpointer data) {

    GUI *appGUI = (GUI *)data;
    calendar_btn_today (appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_edit_note_cb (GtkToggleToolButton *toggle_tool_button, gpointer data) {

    GUI *appGUI = (GUI *)data;
    day_notes_toggled_cb (toggle_tool_button, appGUI);
}

/*------------------------------------------------------------------------------*/ 

void
aux_cal_expander_cb (GObject *object, GParamSpec *param_spec, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
    config.auxilary_calendars_state = gtk_expander_get_expanded (GTK_EXPANDER(appGUI->cal->aux_cal_expander));
    update_aux_calendars (appGUI);
}

/*------------------------------------------------------------------------------*/ 

gboolean
mouse_button_click_handler_cb (GtkWidget * widget, GdkEventButton * event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    if (event->type == GDK_BUTTON_PRESS && event->button == 3 && appGUI->calendar_only == FALSE) {    /* RMB */
        gtk_menu_popup(GTK_MENU(appGUI->cal->popup_menu), NULL, NULL, NULL, NULL, event->button, event->time);
        return TRUE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
popup_add_task_entry_selected_cb (gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	guint cday, cmonth, cyear;
	guint32 julian_date;

	gui_calendar_get_date (GUI_CALENDAR (appGUI->cal->calendar), &cyear, &cmonth, &cday);
	julian_date = date_to_julian (cday, cmonth, cyear);
	tasks_add_edit_dialog_show (FALSE, julian_date, get_seconds_for_today(), appGUI);
}

/*------------------------------------------------------------------------------*/

void
popup_select_day_color_entry_selected_cb (gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	calendar_create_color_selector_window (TRUE, appGUI);
}

/*------------------------------------------------------------------------------*/

void
popup_browse_notes_cb (gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	calendar_notes_browser (appGUI);
}

/*------------------------------------------------------------------------------*/

void
gui_create_calendar(GtkWidget *notebook, GUI *appGUI) {

GtkWidget       *vbox1;
GtkWidget       *vbox2;
GtkWidget       *hbox2;
GtkWidget       *hbox3;
GtkWidget       *eventbox;
GtkWidget       *hseparator;
GtkWidget       *alignment;
GtkWidget       *label;
GtkWidget       *frame;
GtkWidget       *vseparator;
GtkUIManager    *uim_widget = NULL;
GError          *error = NULL;
GtkActionGroup  *action_group = NULL;
GtkWidget       *note_scrolledwindow;
GtkWidget       *calendars_table;
GtkWidget       *popup_menu_add_task_entry;
GtkWidget       *popup_menu_separator;
GtkWidget       *popup_menu_browse_notes;

gchar tmpbuf[BUFFER_SIZE];

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
"    <toolitem name=\"full_year\" action=\"full_year\" />\n"
"    <toolitem name=\"date_calc\" action=\"date_calc\" />\n"
"    <separator name=\"sep2\" />\n"
"    <toolitem name=\"edit_note\" action=\"edit_note\" />\n"
"  </toolbar>\n";

GtkActionEntry entries[] = {
    { "previous_year", OSMO_STOCK_PREV_YEAR, _("Previous year"), NULL, _("Previous year"), NULL},
    { "previous_month", OSMO_STOCK_PREV_MONTH, _("Previous month"), NULL, _("Previous month"), NULL},
    { "previous_day", OSMO_STOCK_PREV_DAY, _("Previous day"), NULL, _("Previous day"), NULL},
    { "today", OSMO_STOCK_TODAY, _("Today"), NULL, _("Today"), NULL},
    { "next_day", OSMO_STOCK_NEXT_DAY, _("Next day"), NULL, _("Next day"), NULL},
    { "next_month", OSMO_STOCK_NEXT_MONTH, _("Next month"), NULL, _("Next month"), NULL},
    { "next_year", OSMO_STOCK_NEXT_YEAR, _("Next year"), NULL, _("Next year"), NULL},
    { "jump_to_date", OSMO_STOCK_JUMPTO, _("Jump to date"), NULL, _("Jump to date"), NULL},
    { "full_year", OSMO_STOCK_FULLYEAR, _("Full-year calendar"), NULL, _("Full-year calendar"), NULL},   
    { "date_calc", OSMO_STOCK_CALCULATOR, _("Date calculator"), NULL, _("Date calculator"), NULL},
};

GtkToggleActionEntry t_entries[] = {
    { "edit_note", OSMO_STOCK_EDIT_NOTE, _("Toggle day note panel"), NULL, _("Toggle day note panel"), NULL, FALSE }
};

guint n_entries = G_N_ELEMENTS (entries);
guint n_t_entries = G_N_ELEMENTS (t_entries);


    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);
    g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s</b>", _("Calendar"));
    gui_add_to_notebook (vbox1, tmpbuf, appGUI);

    appGUI->cal->vbox = GTK_BOX(vbox1);

    if (config.hide_calendar == TRUE) {
        gtk_widget_hide(GTK_WIDGET(appGUI->cal->vbox));
    }

    /*-------------------------------------------------------------------------------------*/

    action_group = gtk_action_group_new ("_actions");
    gtk_action_group_add_actions (action_group, entries, n_entries, NULL);
    gtk_action_group_add_toggle_actions (action_group, t_entries, n_t_entries, NULL);
    gtk_action_group_set_sensitive(action_group, TRUE);

    uim_widget = gtk_ui_manager_new ();

    gtk_ui_manager_insert_action_group (uim_widget, action_group, 0);
    g_signal_connect (uim_widget, "add_widget", G_CALLBACK (add_calendar_toolbar_widget), appGUI);

    if (!gtk_ui_manager_add_ui_from_string (uim_widget, ui_info, -1, &error)) {
        g_message ("building toolbar failed: %s", error->message);
        g_error_free (error);
    }
    gtk_ui_manager_ensure_update (uim_widget);

    gtk_toolbar_set_style (appGUI->cal->calendar_toolbar, GTK_TOOLBAR_ICONS);

    appGUI->cal->notes_button = gtk_ui_manager_get_widget (uim_widget, "/toolbar/edit_note");

    gtk_toolbar_set_tooltips (appGUI->cal->calendar_toolbar, config.enable_tooltips);

    if (appGUI->calendar_only == TRUE) {
        gtk_widget_hide(appGUI->cal->notes_button);
        gtk_widget_set_sensitive(appGUI->cal->notes_button, FALSE);
    }

    /*-------------------------------------------------------------------------------------*/
    /* assign callbacks */

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/previous_year")), "clicked", 
                      G_CALLBACK(calendar_btn_prev_year_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/next_year")), "clicked", 
                      G_CALLBACK(calendar_btn_next_year_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/previous_month")), "clicked", 
                      G_CALLBACK(calendar_btn_prev_month_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/next_month")), "clicked", 
                      G_CALLBACK(calendar_btn_next_month_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/previous_day")), "clicked", 
                      G_CALLBACK(calendar_btn_prev_day_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/next_day")), "clicked", 
                      G_CALLBACK(calendar_btn_next_day_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/today")), "clicked", 
                      G_CALLBACK(calendar_btn_today_cb), appGUI);

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/jump_to_date")), "clicked", 
                      G_CALLBACK(calendar_create_jumpto_window_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/full_year")), "clicked", 
                      G_CALLBACK(calendar_create_fullyear_window_cb), appGUI);
    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/date_calc")), "clicked", 
                      G_CALLBACK(calendar_create_datecalc_window_cb), appGUI);

    g_signal_connect (G_OBJECT(gtk_ui_manager_get_widget (uim_widget, "/toolbar/edit_note")), "toggled", 
                      G_CALLBACK(calendar_edit_note_cb), appGUI);

    /*-------------------------------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, FALSE, 6);

    eventbox = gtk_event_box_new ();
    gtk_widget_show (eventbox);
    gtk_box_pack_start (GTK_BOX (vbox2), eventbox, FALSE, FALSE, 0);
  	g_signal_connect (G_OBJECT(eventbox), "button_press_event", G_CALLBACK(click_handler_cb), appGUI);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_container_add (GTK_CONTAINER (eventbox), hbox2);

    /* Calendar popup menu */

    if (appGUI->calendar_only == FALSE) {

        appGUI->cal->popup_menu = gtk_menu_new();
        popup_menu_add_task_entry = gtk_menu_item_new_with_label(_("Add task"));
        gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->cal->popup_menu), popup_menu_add_task_entry);
        g_signal_connect_swapped(G_OBJECT(popup_menu_add_task_entry), "activate", 
                                 G_CALLBACK(popup_add_task_entry_selected_cb), appGUI);
        gtk_widget_show(popup_menu_add_task_entry);

        appGUI->cal->popup_menu_select_day_color_entry = gtk_menu_item_new_with_label(_("Select day color"));
        gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->cal->popup_menu), appGUI->cal->popup_menu_select_day_color_entry);
        g_signal_connect_swapped(G_OBJECT(appGUI->cal->popup_menu_select_day_color_entry), "activate", 
                                 G_CALLBACK(popup_select_day_color_entry_selected_cb), appGUI);
        gtk_widget_show(appGUI->cal->popup_menu_select_day_color_entry);

        popup_menu_separator = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->cal->popup_menu), popup_menu_separator);
        gtk_widget_show(popup_menu_separator);

        popup_menu_browse_notes = gtk_menu_item_new_with_label(_("Browse notes"));
        gtk_menu_shell_append(GTK_MENU_SHELL(appGUI->cal->popup_menu), popup_menu_browse_notes);
        g_signal_connect_swapped(G_OBJECT(popup_menu_browse_notes), "activate", 
                                 G_CALLBACK(popup_browse_notes_cb), appGUI);
        gtk_widget_show(popup_menu_browse_notes);

    }

    appGUI->cal->date_label = gtk_label_new (NULL);
    gtk_widget_show (appGUI->cal->date_label);
    gtk_box_pack_start (GTK_BOX (hbox2), appGUI->cal->date_label, TRUE, FALSE, 8);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, FALSE, 6);

    appGUI->cal->calendar = gui_calendar_new ();
    gtk_widget_show (appGUI->cal->calendar);
    gui_calendar_set_cursor_type(GUI_CALENDAR(appGUI->cal->calendar), config.cursor_type);
    GTK_WIDGET_UNSET_FLAGS(appGUI->cal->calendar, GTK_CAN_FOCUS);
    gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar), config.display_options);
    gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->calendar), appGUI->cal->fd_cal_font);
    gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cal->calendar, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (appGUI->cal->calendar), "day-selected", 
                      G_CALLBACK (calendar_day_selected_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->cal->calendar), "day-selected-double-click", 
                      G_CALLBACK (calendar_dc_day_selected_cb), appGUI);
  	g_signal_connect (G_OBJECT(appGUI->cal->calendar), "button_press_event", 
                      G_CALLBACK(mouse_button_click_handler_cb), appGUI);
    gui_calendar_enable_cursor (GUI_CALENDAR (appGUI->cal->calendar), TRUE);

	gui_calendar_set_event_marker_type (GUI_CALENDAR (appGUI->cal->calendar), config.event_marker_type);
    gui_calendar_set_event_marker_color (GUI_CALENDAR (appGUI->cal->calendar), config.mark_color);
    gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar), config.selection_color);
    gui_calendar_set_selector_alpha (GUI_CALENDAR (appGUI->cal->calendar), config.selector_alpha);
    gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar), config.header_color);
    gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar), config.weekend_color);
    gui_calendar_set_day_note_marker_type (GUI_CALENDAR (appGUI->cal->calendar), config.day_note_marker);
    gui_calendar_set_today_marker_color (GUI_CALENDAR (appGUI->cal->calendar), config.mark_current_day_color);
    gui_calendar_set_today_marker_alpha (GUI_CALENDAR (appGUI->cal->calendar), config.mark_current_day_alpha);
	gui_calendar_set_today_marker_type (GUI_CALENDAR (appGUI->cal->calendar), config.today_marker_type);

    appGUI->cal->month_selector_menu = gtk_menu_new();
    calendar_create_popup_menu (appGUI->cal->month_selector_menu, appGUI);

    /*-------------------------------------------------------------------------------------*/

    if (appGUI->calendar_only == FALSE) {

        appGUI->cal->aux_cal_expander = gtk_expander_new (_("Previous and next month"));
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->aux_cal_expander, GTK_CAN_FOCUS);
        gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cal->aux_cal_expander, FALSE, FALSE, 0);

        if (config.enable_auxilary_calendars == TRUE) {
            gtk_widget_show (appGUI->cal->aux_cal_expander);
        }

        calendars_table = gtk_table_new (2, 2, FALSE);
        gtk_widget_show (calendars_table);
        gtk_container_add (GTK_CONTAINER (appGUI->cal->aux_cal_expander), calendars_table);
        gtk_table_set_row_spacings (GTK_TABLE (calendars_table), 4);
        gtk_table_set_col_spacings (GTK_TABLE (calendars_table), 4);

        appGUI->cal->prev_month_label = gtk_label_new ("");
        gtk_widget_show (appGUI->cal->prev_month_label);
        gtk_table_attach (GTK_TABLE (calendars_table), appGUI->cal->prev_month_label, 0, 1, 0, 1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);

        appGUI->cal->next_month_label = gtk_label_new ("");
        gtk_widget_show (appGUI->cal->next_month_label);
        gtk_table_attach (GTK_TABLE (calendars_table), appGUI->cal->next_month_label, 1, 2, 0, 1,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);

        appGUI->cal->calendar_prev = gui_calendar_new ();
        gtk_widget_show (appGUI->cal->calendar_prev);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->calendar_prev, GTK_CAN_FOCUS);
        gtk_table_attach (GTK_TABLE (calendars_table), appGUI->cal->calendar_prev, 0, 1, 1, 2,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
        gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar_prev), 
                                          (config.display_options & (GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY)) | GUI_CALENDAR_NO_MONTH_CHANGE);
        gui_calendar_enable_cursor (GUI_CALENDAR (appGUI->cal->calendar_prev), FALSE);
        gui_calendar_set_event_marker_type (GUI_CALENDAR (appGUI->cal->calendar_prev), config.event_marker_type);
        gui_calendar_set_event_marker_color (GUI_CALENDAR (appGUI->cal->calendar_prev), config.mark_color);
        gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar_prev), config.selection_color);
        gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar_prev), config.header_color);
        gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar_prev), config.weekend_color);
        gui_calendar_set_day_note_marker_type (GUI_CALENDAR (appGUI->cal->calendar_prev), config.day_note_marker);
        gui_calendar_set_today_marker_type (GUI_CALENDAR (appGUI->cal->calendar_prev), config.today_marker_type);
        gui_calendar_set_today_marker_color (GUI_CALENDAR (appGUI->cal->calendar_prev), config.mark_current_day_color);
        gui_calendar_set_today_marker_alpha (GUI_CALENDAR (appGUI->cal->calendar_prev), config.mark_current_day_alpha);

        appGUI->cal->calendar_next = gui_calendar_new ();
        gtk_widget_show (appGUI->cal->calendar_next);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->calendar_next, GTK_CAN_FOCUS);
        gtk_table_attach (GTK_TABLE (calendars_table), appGUI->cal->calendar_next, 1, 2, 1, 2,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 0, 0);
        gui_calendar_set_display_options (GUI_CALENDAR (appGUI->cal->calendar_next), 
                                          (config.display_options & (GUI_CALENDAR_SHOW_DAY_NAMES | GUI_CALENDAR_WEEK_START_MONDAY)) | GUI_CALENDAR_NO_MONTH_CHANGE);
        gui_calendar_enable_cursor (GUI_CALENDAR (appGUI->cal->calendar_next), FALSE);
        gui_calendar_set_event_marker_type (GUI_CALENDAR (appGUI->cal->calendar_next), config.event_marker_type);
        gui_calendar_set_event_marker_color (GUI_CALENDAR (appGUI->cal->calendar_next), config.mark_color);
        gui_calendar_set_selector_color (GUI_CALENDAR (appGUI->cal->calendar_next), config.selection_color);
        gui_calendar_set_header_color(GUI_CALENDAR (appGUI->cal->calendar_next), config.header_color);
        gui_calendar_set_weekend_color(GUI_CALENDAR (appGUI->cal->calendar_next), config.weekend_color);
        gui_calendar_set_day_note_marker_type (GUI_CALENDAR (appGUI->cal->calendar_next), config.day_note_marker);
        gui_calendar_set_today_marker_type (GUI_CALENDAR (appGUI->cal->calendar_next), config.today_marker_type);
        gui_calendar_set_today_marker_color (GUI_CALENDAR (appGUI->cal->calendar_next), config.mark_current_day_color);
        gui_calendar_set_today_marker_alpha (GUI_CALENDAR (appGUI->cal->calendar_next), config.mark_current_day_alpha);

    /*-------------------------------------------------------------------------------------*/
    /* notes */

        appGUI->cal->notes_vbox = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (appGUI->cal->notes_vbox);
        gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cal->notes_vbox, TRUE, TRUE, 0);

        hbox3 = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox3);
        gtk_box_pack_start (GTK_BOX (appGUI->cal->notes_vbox), hbox3, FALSE, FALSE, 0);

        alignment = gtk_alignment_new (0.0, 0.5, 0, 1);
        gtk_widget_show (alignment);
        gtk_box_pack_start (GTK_BOX (hbox3), alignment, FALSE, FALSE, 4);

        g_snprintf (tmpbuf, BUFFER_SIZE, "%s:", _("Notes"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_container_add (GTK_CONTAINER (alignment), label);

        if (config.default_stock_icons) {
            appGUI->cal->n_close_button = gui_stock_button(GTK_STOCK_CLOSE, FALSE);
        } else {
            appGUI->cal->n_close_button = gui_stock_button(OSMO_STOCK_BUTTON_CLOSE, FALSE);
        }
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->n_close_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(appGUI->cal->n_close_button), GTK_RELIEF_NONE);
        gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->cal->n_close_button, _("Close note panel"), NULL);
        gtk_box_pack_end (GTK_BOX (hbox3), appGUI->cal->n_close_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (appGUI->cal->n_close_button), "clicked",
                            G_CALLBACK (calendar_close_text_cb), appGUI);

        vseparator = gtk_vseparator_new ();
        gtk_widget_show (vseparator);
        gtk_box_pack_end (GTK_BOX (hbox3), vseparator, FALSE, TRUE, 0);

        appGUI->cal->n_timeline_button = gui_stock_button(OSMO_STOCK_BUTTON_INSERT_TIMELINE, FALSE);
        gtk_widget_show(appGUI->cal->n_timeline_button);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->n_timeline_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(appGUI->cal->n_timeline_button), GTK_RELIEF_NONE);
        gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->cal->n_timeline_button, _("Insert timeline"), NULL);
        gtk_box_pack_end (GTK_BOX (hbox3), appGUI->cal->n_timeline_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (appGUI->cal->n_timeline_button), "clicked",
                            G_CALLBACK (calendar_insert_timeline_cb), appGUI);

        appGUI->cal->n_clear_button = gui_stock_button(OSMO_STOCK_BUTTON_CLEAR, FALSE);
        gtk_widget_show(appGUI->cal->n_clear_button);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->n_clear_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(appGUI->cal->n_clear_button), GTK_RELIEF_NONE);
        gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->cal->n_clear_button, _("Clear text"), NULL);
        gtk_box_pack_end (GTK_BOX (hbox3), appGUI->cal->n_clear_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (appGUI->cal->n_clear_button), "clicked",
                            G_CALLBACK (calendar_clear_text_cb), appGUI);

        appGUI->cal->n_select_color_button = gui_stock_button(OSMO_STOCK_BUTTON_SELECT_COLOR, FALSE);
        gtk_widget_show(appGUI->cal->n_select_color_button);
        GTK_WIDGET_UNSET_FLAGS(appGUI->cal->n_select_color_button, GTK_CAN_FOCUS);
        gtk_button_set_relief (GTK_BUTTON(appGUI->cal->n_select_color_button), GTK_RELIEF_NONE);
        gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->cal->n_select_color_button, _("Select day color"), NULL);
        gtk_box_pack_end (GTK_BOX (hbox3), appGUI->cal->n_select_color_button, FALSE, FALSE, 1);
        g_signal_connect (G_OBJECT (appGUI->cal->n_select_color_button), "clicked",
                            G_CALLBACK (calendar_select_color_cb), appGUI);

        note_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (note_scrolledwindow);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (note_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start (GTK_BOX (appGUI->cal->notes_vbox), note_scrolledwindow, TRUE, TRUE, 0);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (note_scrolledwindow), GTK_SHADOW_IN);

        appGUI->cal->calendar_note_textview = gtk_text_view_new ();
        gtk_widget_show (appGUI->cal->calendar_note_textview);
        g_signal_connect (G_OBJECT (appGUI->cal->calendar_note_textview), "key_press_event",
                          G_CALLBACK (calendar_textview_key_press_cb), appGUI);
        gtk_container_add (GTK_CONTAINER (note_scrolledwindow), appGUI->cal->calendar_note_textview);
        gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (appGUI->cal->calendar_note_textview), TRUE);
        gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->cal->calendar_note_textview), GTK_WRAP_WORD);
        gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->calendar_note_textview), appGUI->cal->fd_notes_font);
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), 4);
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), 4);
        gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), 4);

    /*-------------------------------------------------------------------------------------*/
    /* day info */

        appGUI->cal->day_info_vbox = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (appGUI->cal->day_info_vbox);
        gtk_box_pack_start (GTK_BOX (vbox2), appGUI->cal->day_info_vbox, TRUE, TRUE, 0);

        frame = gtk_frame_new (NULL);
        gtk_widget_show (frame);
        gtk_box_pack_start (GTK_BOX (appGUI->cal->day_info_vbox), frame, TRUE, TRUE, 0);
        gtk_frame_set_label_align (GTK_FRAME (frame), 0.98, 0.5);
        gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);

        alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
        gtk_widget_show (alignment);
        gtk_container_add (GTK_CONTAINER (frame), alignment);
        gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);

        g_snprintf (tmpbuf, BUFFER_SIZE, "<b>%s</b>", _("Info"));
        label = gtk_label_new (tmpbuf);
        gtk_widget_show (label);
        gtk_frame_set_label_widget (GTK_FRAME (frame), label);
        gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

        appGUI->cal->day_info_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (appGUI->cal->day_info_scrolledwindow);
        gtk_container_add (GTK_CONTAINER (alignment), appGUI->cal->day_info_scrolledwindow);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (appGUI->cal->day_info_scrolledwindow), GTK_SHADOW_NONE);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (appGUI->cal->day_info_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

        appGUI->cal->day_desc_textview = gtk_text_view_new ();
        gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->cal->day_desc_textview), GTK_WRAP_WORD);
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), 4);
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), 4);
        gtk_text_view_set_right_margin(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), 4);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), FALSE);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview), FALSE);
        gtk_widget_show (appGUI->cal->day_desc_textview);
        gtk_container_add (GTK_CONTAINER (appGUI->cal->day_info_scrolledwindow), appGUI->cal->day_desc_textview);
        gtk_widget_realize(appGUI->cal->day_desc_textview);
        gtk_widget_modify_base (appGUI->cal->day_desc_textview, GTK_STATE_NORMAL, &appGUI->main_window->style->bg[GTK_STATE_NORMAL]);

        appGUI->cal->day_desc_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(appGUI->cal->day_desc_textview));
        gtk_text_buffer_create_tag (appGUI->cal->day_desc_text_buffer, "bold", "weight", PANGO_WEIGHT_ULTRABOLD, NULL);
        gtk_text_buffer_create_tag (appGUI->cal->day_desc_text_buffer, "strike", "strikethrough", TRUE, NULL);

    /*-------------------------------------------------------------------------------------*/

    }

    gtk_widget_modify_font (GTK_WIDGET(appGUI->cal->date_label), appGUI->cal->fd_day_name_font);
    gui_calendar_get_date(GUI_CALENDAR(appGUI->cal->calendar), 
                          &appGUI->cal->year, &appGUI->cal->month, &appGUI->cal->day);
    gui_calendar_get_gdate (GUI_CALENDAR (appGUI->cal->calendar), appGUI->cal->date);

    gui_calendar_set_frame_cursor_thickness (GUI_CALENDAR (appGUI->cal->calendar), config.frame_cursor_thickness);

    if (appGUI->calendar_only == FALSE) {
        if (!config.day_notes_visible) {
            gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), FALSE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), FALSE);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), FALSE);
            gtk_widget_show(appGUI->cal->day_info_vbox);
            gtk_widget_hide(appGUI->cal->notes_vbox);
        } else {
            gtk_toggle_tool_button_set_active (GTK_TOGGLE_TOOL_BUTTON(appGUI->cal->notes_button), TRUE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), TRUE);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(appGUI->cal->calendar_note_textview), TRUE);
            gtk_widget_hide(appGUI->cal->day_info_vbox);
            gtk_widget_show(appGUI->cal->notes_vbox);
        }

        gtk_widget_realize(GTK_WIDGET(appGUI->cal->calendar_note_textview));
        gtk_widget_grab_focus(GTK_WIDGET(appGUI->cal->calendar_note_textview));
    }

    if (appGUI->calendar_only == FALSE) {
        g_signal_connect (G_OBJECT(appGUI->cal->aux_cal_expander), "notify::expanded", 
                          G_CALLBACK(aux_cal_expander_cb), appGUI);
        gtk_expander_set_expanded (GTK_EXPANDER(appGUI->cal->aux_cal_expander), config.auxilary_calendars_state);
    }

}

/*------------------------------------------------------------------------------*/

void
select_bg_color_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;
	gtk_widget_destroy (appGUI->cal->select_bg_color_window);
}

/*------------------------------------------------------------------------------*/

void
button_select_bg_color_close_cb (GtkWidget *widget, gpointer user_data)
{
	select_bg_color_close_cb (widget, NULL, user_data);
}

/*------------------------------------------------------------------------------*/

void
colors_category_selected (GUI *appGUI) {

GtkTreeIter iter;
GtkTreeModel *model;
gchar *color_val;
gint n;

    if (gtk_tree_selection_get_selected (appGUI->cal->colors_category_select, &model, &iter)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->cal->colors_category_store), &iter, 1, &color_val, 3, &n, -1);
        if (n) {
            add_note (appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, 
                      color_val, calendar_get_note_text(appGUI), appGUI);
        } else {
            add_note (appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, 
                      NULL, calendar_get_note_text(appGUI), appGUI);
        }
        mark_days_with_notes (appGUI->cal->day, appGUI->cal->month, appGUI->cal->year, appGUI);
        g_free(color_val);
        select_bg_color_close_cb (NULL, NULL, appGUI);
        update_aux_calendars (appGUI);
    }

}

/*------------------------------------------------------------------------------*/

void
select_bg_color_apply_cb (GtkWidget *widget, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;

	config.enable_day_mark = TRUE;
	calendar_refresh_marks (appGUI);
	update_aux_calendars (appGUI);
	colors_category_selected (appGUI);
}

/*------------------------------------------------------------------------------*/

gint
select_bg_color_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;

	switch (event->keyval) {

		case GDK_Return:
			config.enable_day_mark = TRUE;
			calendar_refresh_marks (appGUI);
			update_aux_calendars (appGUI);
			colors_category_selected (appGUI);
			return TRUE;

		case GDK_Escape:
			select_bg_color_close_cb (NULL, NULL, appGUI);
			return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------*/

gint
colors_list_dbclick_cb (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	GUI *appGUI = (GUI *) user_data;

	if ((event->type == GDK_2BUTTON_PRESS) && (event->button == 1)) {
		config.enable_day_mark = TRUE;
		calendar_refresh_marks (appGUI);
		update_aux_calendars (appGUI);
		colors_category_selected (appGUI);
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_create_color_selector_window (gboolean window_pos, GUI *appGUI) {

GtkWidget *vbox1;
GtkWidget *scrolledwindow;
GtkWidget *colors_category_treeview;
GtkWidget *hseparator;
GtkWidget *hbuttonbox;
GtkWidget *ok_button;
GtkWidget *cancel_button;
GtkTreeIter iter;
GtkCellRenderer     *renderer;
GtkTreeViewColumn   *column;
GtkTreePath     *path;
GdkPixbuf *image;
gchar *color_val, *color_name, *color_sel;
gchar tmpbuf[BUFFER_SIZE];
gint i;
GdkColor color;

    if (check_note(appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, appGUI) == FALSE && 
        !config.day_notes_visible) {
        return;
    }
	
    config.enable_day_mark = TRUE;
	calendar_refresh_marks (appGUI);
	update_aux_calendars (appGUI);

    appGUI->cal->select_bg_color_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cal->select_bg_color_window), 4);
    gtk_window_set_title (GTK_WINDOW (appGUI->cal->select_bg_color_window), _("Select color"));
    gtk_window_set_default_size (GTK_WINDOW(appGUI->cal->select_bg_color_window), 200, 280);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->cal->select_bg_color_window), GTK_WINDOW(appGUI->main_window));
    if (window_pos == TRUE) {
        gtk_window_set_position(GTK_WINDOW(appGUI->cal->select_bg_color_window), GTK_WIN_POS_CENTER_ON_PARENT);
    } else {
        gtk_window_set_position(GTK_WINDOW(appGUI->cal->select_bg_color_window), GTK_WIN_POS_MOUSE);
    }
    gtk_window_set_modal(GTK_WINDOW(appGUI->cal->select_bg_color_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cal->select_bg_color_window), "delete_event",
                      G_CALLBACK(select_bg_color_close_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->cal->select_bg_color_window), "key_press_event",
                      G_CALLBACK (select_bg_color_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->cal->select_bg_color_window), vbox1);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    appGUI->cal->colors_category_store = gtk_list_store_new(4, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    colors_category_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(appGUI->cal->colors_category_store));
    g_signal_connect(G_OBJECT(colors_category_treeview), "button_press_event",
                     G_CALLBACK(colors_list_dbclick_cb), appGUI);
    appGUI->cal->colors_category_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(colors_category_treeview));
    gtk_widget_show (colors_category_treeview);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), colors_category_treeview);
    gtk_container_set_border_width (GTK_CONTAINER (colors_category_treeview), 4);
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (colors_category_treeview), FALSE);
    gtk_tree_view_set_enable_search (GTK_TREE_VIEW (colors_category_treeview), FALSE);

    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "pixbuf", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(colors_category_treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(colors_category_treeview), column);
    gtk_tree_view_column_set_visible (column, FALSE);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(colors_category_treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(colors_category_treeview), column);
    gtk_tree_view_column_set_visible (column, FALSE);

    color = (appGUI->cal->calendar)->style->base[GTK_WIDGET_STATE (appGUI->cal->calendar)];
    g_snprintf (tmpbuf, BUFFER_SIZE, "#%02X%02X%02X", color.red * 256 / 65536, color.green * 256 / 65536, color.blue * 256 / 65536);
    image = gui_create_color_swatch (tmpbuf);
    gtk_list_store_append(appGUI->cal->colors_category_store, &iter);
    gtk_list_store_set(appGUI->cal->colors_category_store, &iter, 0, image, 1, tmpbuf, 2, _("None"), 3, 0, -1);
    g_object_unref (image);

    i = 0;

    color_sel = get_color (appGUI->cal->day, appGUI->cal->month+1, appGUI->cal->year, appGUI);

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, NULL, i++)) {
        gtk_tree_model_get(GTK_TREE_MODEL(appGUI->opt->calendar_category_store), &iter, 1, &color_val, 2, &color_name, -1);
        image = gui_create_color_swatch (color_val);
        gtk_list_store_append(appGUI->cal->colors_category_store, &iter);
        gtk_list_store_set(appGUI->cal->colors_category_store, &iter, 0, image, 1, color_val, 2, color_name, 3, i, -1);

        if (color_sel != NULL) {
            if (!strcmp(color_val, color_sel)) {
                path = gtk_tree_model_get_path (GTK_TREE_MODEL(appGUI->cal->colors_category_store), &iter);
                if (path != NULL) {
                    gtk_tree_view_set_cursor (GTK_TREE_VIEW (colors_category_treeview), path, NULL, FALSE);
                    gtk_tree_path_free(path);
                }
            }
        }

        g_object_unref (image);
        g_free(color_val);
        g_free(color_name);
    }

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 4);

    if (config.default_stock_icons) {
        cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    } else {
        cancel_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CANCEL);
    }
    GTK_WIDGET_UNSET_FLAGS(cancel_button, GTK_CAN_FOCUS);
    gtk_widget_show (cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), cancel_button);
    g_signal_connect (G_OBJECT (cancel_button), "clicked",
                        G_CALLBACK (button_select_bg_color_close_cb), appGUI);

    if (config.default_stock_icons) {
        ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
    } else {
        ok_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_OK);
    }
    GTK_WIDGET_UNSET_FLAGS(ok_button, GTK_CAN_FOCUS);
    gtk_widget_show (ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), ok_button);
    g_signal_connect (G_OBJECT (ok_button), "clicked",
                        G_CALLBACK (select_bg_color_apply_cb), appGUI);

    gtk_widget_show (appGUI->cal->select_bg_color_window);
}

/*------------------------------------------------------------------------------*/

void
menu_entry_selected_cb (gpointer user_data)
{
    MESSAGE *msg = (MESSAGE *) user_data;
    calendar_update_date (msg->appGUI->cal->day, (gint) msg->data, msg->appGUI->cal->year, msg->appGUI);
}

/*------------------------------------------------------------------------------*/

void
calendar_create_popup_menu (GtkWidget *menu, GUI *appGUI)
{
static MESSAGE msg_month[MAX_MONTHS];
GtkWidget *menu_entry;
gchar buffer[BUFFER_SIZE];
GDate *cdate;
gint i;

	cdate = g_date_new ();
	g_return_if_fail (cdate != NULL);

	g_date_set_day (cdate, 1);
	g_date_set_year (cdate, get_current_year());

	for (i = 0; i < MAX_MONTHS; i++) {
		g_date_set_month (cdate, i+1);
		g_date_strftime (buffer, BUFFER_SIZE, "%B", cdate);

		menu_entry = gtk_menu_item_new_with_label (buffer);
		gtk_menu_shell_append (GTK_MENU_SHELL (appGUI->cal->month_selector_menu), menu_entry);
		msg_month[i].data = (gpointer) i;
		msg_month[i].appGUI = appGUI;
		g_signal_connect_swapped (G_OBJECT (menu_entry), "activate",
		                          G_CALLBACK (menu_entry_selected_cb), &msg_month[i]);
		gtk_widget_show (menu_entry);
	}

	g_date_free (cdate);
}

/*------------------------------------------------------------------------------*/

