
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 Tomasz Maka <pasp@users.sourceforge.net>
 *           (C) 2007 Piotr Maka <silloz@users.sourceforge.net>
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

#include "calendar_calc.h"
#include "i18n.h"
#include "calendar.h"
#include "calendar_utils.h"
#include "options_prefs.h"
#include "stock_icons.h"
#include "gui_utils.h"

/*------------------------------------------------------------------------------*/

void
window_date_calculator_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
    gtk_widget_destroy (appGUI->cal->window_date_calculator);
}

/*------------------------------------------------------------------------------*/

void
button_window_date_calculator_close_cb (GtkWidget *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;
    window_date_calculator_close_cb (widget, NULL, appGUI);
}

/*------------------------------------------------------------------------------*/

gint
date_calculator_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (event->keyval == GDK_Escape) {
            window_date_calculator_close_cb (widget, NULL, appGUI);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
set_current_date (GtkWidget *year, GtkWidget *month, GtkWidget *day)
{
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (year), get_current_year ());
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (month), get_current_month () + 1);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (day), get_current_day ());
}

/*------------------------------------------------------------------------------*/

void
set_current_time (GtkWidget *hour, GtkWidget *minute, GtkWidget *second)
{
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (hour), get_current_hour ());
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (minute), get_current_minute ());
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (second), get_current_second ());
}

/*------------------------------------------------------------------------------*/

void
reset_current_time (GtkWidget *hour, GtkWidget *minute, GtkWidget *second)
{
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (hour), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (minute), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (second), 0);
}

/*------------------------------------------------------------------------------*/

void
set_current_date_start_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_date (appGUI->cal->spinbutton_start_year, appGUI->cal->spinbutton_start_month,
                        appGUI->cal->spinbutton_start_day); 
}

/*------------------------------------------------------------------------------*/

void
set_current_date_start2_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_date (appGUI->cal->spinbutton2_start_year, appGUI->cal->spinbutton2_start_month,
                        appGUI->cal->spinbutton2_start_day); 
}

/*------------------------------------------------------------------------------*/

void
set_current_date_end_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_date (appGUI->cal->spinbutton_end_year, appGUI->cal->spinbutton_end_month,
                        appGUI->cal->spinbutton_end_day); 
}

/*------------------------------------------------------------------------------*/

void
set_current_time_start_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_time (appGUI->cal->spinbutton_start_hour, appGUI->cal->spinbutton_start_minute,
                        appGUI->cal->spinbutton_start_second); 
}

/*------------------------------------------------------------------------------*/

void
set_current_time_start2_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_time (appGUI->cal->spinbutton2_start_hour, appGUI->cal->spinbutton2_start_minute,
                        appGUI->cal->spinbutton2_start_second); 
}

/*------------------------------------------------------------------------------*/

void
set_current_time_end_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    set_current_time (appGUI->cal->spinbutton_end_hour, appGUI->cal->spinbutton_end_minute,
                        appGUI->cal->spinbutton_end_second); 
}

/*------------------------------------------------------------------------------*/

void
reset_current_time_start_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    reset_current_time (appGUI->cal->spinbutton_start_hour, appGUI->cal->spinbutton_start_minute,
                        appGUI->cal->spinbutton_start_second); 
}

/*------------------------------------------------------------------------------*/

void
reset_current_time_start2_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    reset_current_time (appGUI->cal->spinbutton2_start_hour, appGUI->cal->spinbutton2_start_minute,
                        appGUI->cal->spinbutton2_start_second); 
}

/*------------------------------------------------------------------------------*/

void
reset_current_time_end_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    reset_current_time (appGUI->cal->spinbutton_end_hour, appGUI->cal->spinbutton_end_minute,
                        appGUI->cal->spinbutton_end_second); 
}

/*------------------------------------------------------------------------------*/

void
reset_addsub_time_cb (GtkWidget *widget, gpointer user_data)
{
    GUI *appGUI = (GUI *)user_data;

    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_year), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_month), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_week), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_day), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_hour), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_minute), 0);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_second), 0);
}

/*------------------------------------------------------------------------------*/

void
set_result_cb (GtkWidget *widget, gpointer user_data)
{
GUI *appGUI = (GUI *)user_data;
DATE date_from, date_to, date_diff;
TIME time_from, time_to, time_diff;
gint days, weeks;
guint64 hours, minutes, seconds;
gchar tmpbuf[BUFFER_SIZE];

    date_from.year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_year));
    date_from.month = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_month)) - 1;
    date_from.day = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_day));
    time_from.hour = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_hour));
    time_from.minute = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_minute));
    time_from.second = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_start_second));

    days = g_date_get_days_in_month (date_from.month + 1, date_from.year);
    if (date_from.day > days) {
        date_from.day = days;
        gui_change_bg_widget_state (appGUI->cal->spinbutton_start_day, COLOR_BG_FAIL, appGUI);
    } else {
        gui_change_bg_widget_state (appGUI->cal->spinbutton_start_day, NULL, appGUI);
    }

    date_to.year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_year));
    date_to.month = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_month)) - 1;
    date_to.day = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_day));
    time_to.hour = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_hour));
    time_to.minute = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_minute));
    time_to.second = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton_end_second));

    days = g_date_get_days_in_month (date_to.month + 1, date_to.year);
    if (date_to.day > days) {
        date_to.day = days;
        gui_change_bg_widget_state (appGUI->cal->spinbutton_end_day, COLOR_BG_FAIL, appGUI);
    } else {
        gui_change_bg_widget_state (appGUI->cal->spinbutton_end_day, NULL, appGUI);
    }

    days = calculate_date_time_diff (date_from, time_from, date_to, time_to, &date_diff, &time_diff);

    if (days == -1)
        days = calculate_date_time_diff (date_to, time_to, date_from, time_from, &date_diff, &time_diff);

    g_snprintf (tmpbuf, BUFFER_SIZE, "<big>%d %s, %d %s, %d %s</big>",
        date_diff.year, ngettext ("year", "years", date_diff.year),
        date_diff.month, ngettext ("month", "months", date_diff.month),
        date_diff.day, ngettext ("day", "days", date_diff.day));
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->label_result_1), tmpbuf);

    g_snprintf (tmpbuf, BUFFER_SIZE, "<big>%d %s, %d %s, %d %s</big>",
        time_diff.hour, ngettext ("hour", "hours", time_diff.hour),
        time_diff.minute, ngettext ("minute", "minutes", time_diff.minute),
        time_diff.second, ngettext ("second", "seconds", time_diff.second));
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->label_result_1_2), tmpbuf);

    g_snprintf (tmpbuf, BUFFER_SIZE, "<small>(%s: %d %s, %d %s, %d %s, %d %s)</small>",
        _("or"),
        days, ngettext ("day", "days", days),
        time_diff.hour, ngettext ("hour", "hours", time_diff.hour),
        time_diff.minute, ngettext ("minute", "minutes", time_diff.minute),
        time_diff.second, ngettext ("second", "seconds", time_diff.second));
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->label_result_2), tmpbuf);

    seconds = (guint64)time_diff.second + (guint64)time_diff.minute * 60 +
              (guint64)time_diff.hour * 3600 + (guint64)days * 24 * 3600;
    minutes = seconds / 60;
    hours = minutes / 60;
    weeks = (gint)(hours / (24 * 7));
    sprintf (tmpbuf, "<small>%llu %s\n%llu %s (%s)\n%llu %s (%s)\n%d %s (%s)</small>",
        seconds, ngettext ("second", "seconds", seconds),
        minutes, ngettext ("minute", "minutes", minutes), _("rounded down"),
        hours, ngettext ("hour", "hours", hours), _("rounded down"),
        weeks, ngettext ("week", "weeks", weeks), _("rounded down"));
    gtk_label_set_markup (GTK_LABEL (appGUI->cal->label_result_3), tmpbuf);
}

/*------------------------------------------------------------------------------*/

void
set_result2_cb (GtkWidget *widget, gpointer user_data)
{
GUI *appGUI = (GUI *)user_data;
gint year, month, day;
gint hour, minute, second;
gint year2, month2, week2, day2;
gint hour2, minute2, second2;
gint n;
guint32 julian;
gchar timebuf[BUFFER_SIZE];
gchar tmpbuf[BUFFER_SIZE];

    year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_year));
    month = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_month)) - 1;
    day = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_day));
    hour = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_hour));
    minute = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_minute));
    second = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_start_second));

    year2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_year));
    month2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_month));
    week2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_week));
    day2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_day));
    hour2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_hour));
    minute2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_minute));
    second2 = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (appGUI->cal->spinbutton2_end_second));

    if (day > g_date_get_days_in_month (month + 1, year)) {
        gui_change_bg_widget_state (appGUI->cal->spinbutton2_start_day, COLOR_BG_FAIL, appGUI);
    } else {
        gui_change_bg_widget_state (appGUI->cal->spinbutton2_start_day, NULL, appGUI);
    }

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (appGUI->cal->radiobutton_add)) == TRUE) {
    /* add */
        year += year2;
        month += month2;
        year += month / 12;
        month %= 12;

        /* 31 January + one month = 28 or 29 February */
        n = g_date_get_days_in_month (month + 1, year);
        if (day > n) {
            day = n;
        }

        julian = date_to_julian (day, month, year);

        second += second2;
        minute += minute2 + second / 60;
        second %= 60;
        hour += hour2 + minute / 60;
        minute %= 60;
        day2 += hour / 24;
        hour %= 24;

        julian += day2 + week2 * 7;

    } else {
    /* subtract */
        year = year - year2 - (month2 / 12);
        month -= month2 % 12;

        if (month < 0) {
            year--;
            month += 12;
        }

        if (year > 0) {
            /* 31 March - one month = 28 or 29 February */
            n = g_date_get_days_in_month (month + 1, year);
            if (day > n) {
                day = n;
            }

            julian = date_to_julian (day, month, year);

            minute2 += second2 / 60;
            second -= second2 % 60;
            if (second < 0) {
                minute--;
                second += 60;
            }

            hour2 += minute2 / 60;
            minute -= minute2 % 60;
            if (minute < 0) {
                hour--;
                minute += 60;
            }

            day2 += hour2 / 24;
            hour -= hour2 % 24;
            if (hour < 0) {
                julian--;
                hour += 24;
            }

            julian = julian - day2 - week2 * 7;

            if (!(g_date_valid_julian (julian))) {
                year = 0;
            }
        }
    }

	if (year < 1) {

		gtk_label_set_text (GTK_LABEL (appGUI->cal->label2_result), _("This calculator only supports dates after year 1."));

	} else {

		if (hour == 0 && minute == 0 && second == 0) {
			g_snprintf (tmpbuf, BUFFER_SIZE, "<big>%s</big>", julian_to_str (julian, DATE_FULL));
		} else {
			g_snprintf (timebuf, BUFFER_SIZE, "%02d:%02d.%02d", hour, minute, second);
			g_snprintf (tmpbuf, BUFFER_SIZE, "<big>%s, %s</big>", timebuf, julian_to_str (julian, DATE_FULL));
		}
		gtk_label_set_markup (GTK_LABEL (appGUI->cal->label2_result), tmpbuf);

	}

	appGUI->cal->julian_jumpto = julian;
}

/*------------------------------------------------------------------------------*/

void
jumpto_and_window_close_cb (GtkButton *button, gpointer user_data)
{
GDate *cdate;

	GUI *appGUI = (GUI *) user_data;

	cdate = g_date_new_julian (appGUI->cal->julian_jumpto);
	g_return_if_fail (cdate != NULL);

	calendar_jump_to_date (cdate, appGUI);
	update_aux_calendars (appGUI);

	window_date_calculator_close_cb (GTK_WIDGET (button), NULL, user_data);
	g_date_free (cdate);
}

/*------------------------------------------------------------------------------*/

void
calendar_create_calc_window (GUI *appGUI) {

GtkWidget *vbox;
GtkWidget *label;
GtkWidget *hseparator;
GtkWidget *hbuttonbox;
GtkWidget *frame;
GtkWidget *alignment;
gint      win_xpos, win_ypos;
GtkWidget *notebook;
GtkWidget *vbox2;
GtkWidget *hbox1;
GtkWidget *vbox3;
GtkWidget *table;
GtkObject *spinbutton_adjustment;
GtkWidget *button_start_today;
GtkWidget *vbox4;
GtkWidget *button_start_now;
GtkWidget *button_start_clear;
GtkWidget *hbox2;
GtkWidget *vbox5;
GtkWidget *button_end_today;
GtkWidget *vbox6;
GtkWidget *button_end_now;
GtkWidget *button_end_clear;
GtkWidget *vbox7;
GtkWidget *hbox9;
GtkWidget *vbox8;
GtkWidget *button2_start_today;
GtkWidget *vbox9;
GtkWidget *button2_start_now;
GtkWidget *button2_start_clear;
GtkWidget *button2_end_clear;
GtkWidget *vbox10;
GtkWidget *hbox10;
GSList    *radiobutton_add_group = NULL;
GtkWidget *radiobutton_sub;
GtkWidget *close_button;
GtkWidget *hbox11;
GtkWidget *hbox12;
GtkWidget *hbox13;
GtkWidget *hbox_bst;
GtkWidget *hbox_bet;
GtkWidget *hbox_b2st;
GtkWidget *hbox_b2sn;
GtkWidget *vbox_result;
GtkWidget *jumpto_button;
char      tmpbuf[BUFFER_SIZE];

    appGUI->cal->window_date_calculator = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->cal->window_date_calculator), _("Date calculator"));
    gtk_window_set_position (GTK_WINDOW (appGUI->cal->window_date_calculator), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_modal (GTK_WINDOW (appGUI->cal->window_date_calculator), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cal->window_date_calculator), "delete_event",
                      G_CALLBACK (window_date_calculator_close_cb), appGUI);
    g_signal_connect (G_OBJECT (appGUI->cal->window_date_calculator), "key_press_event",
                        G_CALLBACK (date_calculator_key_press_cb), appGUI);
    gtk_window_set_transient_for (GTK_WINDOW (appGUI->cal->window_date_calculator), GTK_WINDOW (appGUI->main_window));
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cal->window_date_calculator), 8);
    gtk_window_set_resizable (GTK_WINDOW (appGUI->cal->window_date_calculator), FALSE);

    /*---------------------------------------------------------------------------------*/

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (appGUI->cal->window_date_calculator), vbox);

    notebook = gtk_notebook_new ();
    gtk_widget_show (notebook);
    gtk_box_pack_start (GTK_BOX (vbox), notebook, TRUE, TRUE, 0);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (notebook), vbox2);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox2), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (alignment), hbox1);

    vbox3 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox3);
    gtk_box_pack_start (GTK_BOX (hbox1), vbox3, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox3), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Year"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Month"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Day"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 9999, 1, 10, 10);
    appGUI->cal->spinbutton_start_year = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_year);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_year, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
    appGUI->cal->spinbutton_start_month = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_month);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_month, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
    appGUI->cal->spinbutton_start_day = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_day);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_day, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox_bst = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_bst);
    gtk_box_pack_start (GTK_BOX (vbox3), hbox_bst, FALSE, FALSE, 2);

    button_start_today = gtk_button_new_with_mnemonic (_("Current date"));
    gtk_widget_set_size_request (button_start_today, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button_start_today, GTK_CAN_FOCUS);
    gtk_widget_show (button_start_today);
    gtk_box_pack_start (GTK_BOX (hbox_bst), button_start_today, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_start_today, _("Set current date"), NULL);
    g_signal_connect (button_start_today, "clicked", G_CALLBACK (set_current_date_start_cb), appGUI);

    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox4);
    gtk_box_pack_start (GTK_BOX (hbox1), vbox4, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox4), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Hour"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Minute"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Second"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 23, 1, 10, 10);
    appGUI->cal->spinbutton_start_hour = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_hour);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_hour, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton_start_minute = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_minute);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_minute, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton_start_second = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_start_second);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_start_second, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox11 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox11);
    gtk_box_pack_start (GTK_BOX (vbox4), hbox11, FALSE, FALSE, 2);

    button_start_now = gtk_button_new_with_mnemonic (_("Current time"));
    gtk_widget_set_size_request (button_start_now, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button_start_now, GTK_CAN_FOCUS);
    gtk_widget_show (button_start_now);
    gtk_box_pack_start (GTK_BOX (hbox11), button_start_now, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_start_now, _("Set current time"), NULL);
    g_signal_connect (button_start_now, "clicked", G_CALLBACK (set_current_time_start_cb), appGUI);

    if (config.default_stock_icons) {
        button_start_clear = gui_stock_button (GTK_STOCK_CLEAR, FALSE);
    } else {
        button_start_clear = gui_stock_button (OSMO_STOCK_BUTTON_CLEAR, FALSE);
    }

    gtk_widget_show (button_start_clear);
    GTK_WIDGET_UNSET_FLAGS (button_start_clear, GTK_CAN_FOCUS);
    gtk_box_pack_start (GTK_BOX (hbox11), button_start_clear, FALSE, FALSE, 4);
    gtk_button_set_relief (GTK_BUTTON (button_start_clear), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_start_clear, _("Reset time"), NULL);
    g_signal_connect (button_start_clear, "clicked", G_CALLBACK (reset_current_time_start_cb), appGUI);

    sprintf (tmpbuf, "<b>%s:</b>", _("First date and time"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox2), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_container_add (GTK_CONTAINER (alignment), hbox2);

    vbox5 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox5);
    gtk_box_pack_start (GTK_BOX (hbox2), vbox5, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox5), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Year"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Month"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Day"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 9999, 1, 10, 10);
    appGUI->cal->spinbutton_end_year = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_year);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_year, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
    appGUI->cal->spinbutton_end_month = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_month);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_month, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
    appGUI->cal->spinbutton_end_day = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_day);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_day, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox_bet = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_bet);
    gtk_box_pack_start (GTK_BOX (vbox5), hbox_bet, FALSE, FALSE, 2);

    button_end_today = gtk_button_new_with_mnemonic (_("Current date"));
    gtk_widget_set_size_request (button_end_today, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button_end_today, GTK_CAN_FOCUS);
    gtk_widget_show (button_end_today);
    gtk_box_pack_start (GTK_BOX (hbox_bet), button_end_today, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_end_today, _("Set current date"), NULL);
    g_signal_connect (button_end_today, "clicked", G_CALLBACK (set_current_date_end_cb), appGUI);

    vbox6 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox6);
    gtk_box_pack_start (GTK_BOX (hbox2), vbox6, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox6), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Hour"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Minute"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Second"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 23, 1, 10, 10);
    appGUI->cal->spinbutton_end_hour = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_hour);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_hour, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton_end_minute = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_minute);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_minute, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton_end_second = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton_end_second);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton_end_second, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox13 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox13);
    gtk_box_pack_start (GTK_BOX (vbox6), hbox13, FALSE, FALSE, 2);

    button_end_now = gtk_button_new_with_mnemonic (_("Current time"));
    gtk_widget_set_size_request (button_end_now, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button_end_now, GTK_CAN_FOCUS);
    gtk_widget_show (button_end_now);
    gtk_box_pack_start (GTK_BOX (hbox13), button_end_now, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_end_now, _("Set current time"), NULL);
    g_signal_connect (button_end_now, "clicked", G_CALLBACK (set_current_time_end_cb), appGUI);

    if (config.default_stock_icons) {
        button_end_clear = gui_stock_button (GTK_STOCK_CLEAR, FALSE);
    } else {
        button_end_clear = gui_stock_button (OSMO_STOCK_BUTTON_CLEAR, FALSE);
    }

    gtk_widget_show (button_end_clear);
    GTK_WIDGET_UNSET_FLAGS (button_end_clear, GTK_CAN_FOCUS);
    gtk_box_pack_start (GTK_BOX (hbox13), button_end_clear, FALSE, FALSE, 4);
    gtk_button_set_relief (GTK_BUTTON (button_end_clear), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button_end_clear, _("Reset time"), NULL);
    g_signal_connect (button_end_clear, "clicked", G_CALLBACK (reset_current_time_end_cb), appGUI);

    sprintf (tmpbuf, "<b>%s:</b>", _("Second date and time"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox2), hseparator, FALSE, FALSE, 6);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox2), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 8);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    vbox_result = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox_result);
    gtk_container_add (GTK_CONTAINER (alignment), vbox_result);

    appGUI->cal->label_result_1 = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->label_result_1);
    gtk_label_set_selectable (GTK_LABEL (appGUI->cal->label_result_1), 1);
    gtk_box_pack_start (GTK_BOX (vbox_result), appGUI->cal->label_result_1, FALSE, FALSE, 0);

    appGUI->cal->label_result_1_2 = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->label_result_1_2);
    gtk_label_set_selectable (GTK_LABEL (appGUI->cal->label_result_1_2), 1);
    gtk_box_pack_start (GTK_BOX (vbox_result), appGUI->cal->label_result_1_2, FALSE, FALSE, 0);

    appGUI->cal->label_result_2 = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->label_result_2);
    gtk_label_set_selectable (GTK_LABEL (appGUI->cal->label_result_2), 1);
    gtk_box_pack_start (GTK_BOX (vbox_result), appGUI->cal->label_result_2, TRUE, TRUE, 0);

    sprintf (tmpbuf, "<small>\n<i>%s:</i></small>", _("Alternative time units"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (vbox_result), label, FALSE, FALSE, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    appGUI->cal->label_result_3 = gtk_label_new ("");
    gtk_widget_show (appGUI->cal->label_result_3);
    gtk_label_set_selectable (GTK_LABEL (appGUI->cal->label_result_3), 1);
    gtk_box_pack_start (GTK_BOX (vbox_result), appGUI->cal->label_result_3, FALSE, FALSE, 0);
    gtk_misc_set_alignment (GTK_MISC (appGUI->cal->label_result_3), 0.05, 0.5);

    sprintf (tmpbuf, "<b>%s:</b>", _("Result"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    label = gtk_label_new (_("Duration between two dates"));
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label);

    vbox7 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox7);
    gtk_container_add (GTK_CONTAINER (notebook), vbox7);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox7), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    hbox9 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox9);
    gtk_container_add (GTK_CONTAINER (alignment), hbox9);

    vbox8 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox8);
    gtk_box_pack_start (GTK_BOX (hbox9), vbox8, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox8), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Year"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Month"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Day"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 9999, 1, 10, 10);
    appGUI->cal->spinbutton2_start_year = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_year);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_year, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
    appGUI->cal->spinbutton2_start_month = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_month);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_month, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
    appGUI->cal->spinbutton2_start_day = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_day);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_day, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox_b2st = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_b2st);
    gtk_box_pack_start (GTK_BOX (vbox8), hbox_b2st, FALSE, FALSE, 2);

    button2_start_today = gtk_button_new_with_mnemonic (_("Current date"));
    gtk_widget_set_size_request (button2_start_today, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button2_start_today, GTK_CAN_FOCUS);
    gtk_widget_show (button2_start_today);
    gtk_box_pack_start (GTK_BOX (hbox_b2st), button2_start_today, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button2_start_today, _("Set current date"), NULL);
    g_signal_connect (button2_start_today, "clicked", G_CALLBACK (set_current_date_start2_cb), appGUI);

    vbox9 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox9);
    gtk_box_pack_start (GTK_BOX (hbox9), vbox9, TRUE, TRUE, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox9), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Hour"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Minute"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Second"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 23, 1, 10, 10);
    appGUI->cal->spinbutton2_start_hour = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_hour);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_hour, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton2_start_minute = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_minute);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_minute, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
    appGUI->cal->spinbutton2_start_second = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_start_second);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_start_second, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox_b2sn = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_b2sn);
    gtk_box_pack_start (GTK_BOX (vbox9), hbox_b2sn, FALSE, FALSE, 2);

    button2_start_now = gtk_button_new_with_mnemonic (_("Current time"));
    gtk_widget_set_size_request (button2_start_now, -1, 32);
    GTK_WIDGET_UNSET_FLAGS (button2_start_now, GTK_CAN_FOCUS);
    gtk_widget_show (button2_start_now);
    gtk_box_pack_start (GTK_BOX (hbox_b2sn), button2_start_now, TRUE, FALSE, 4);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button2_start_now, _("Set current time"), NULL);
    g_signal_connect (button2_start_now, "clicked", G_CALLBACK (set_current_time_start2_cb), appGUI);

    if (config.default_stock_icons) {
        button2_start_clear = gui_stock_button (GTK_STOCK_CLEAR, FALSE);
    } else {
        button2_start_clear = gui_stock_button (OSMO_STOCK_BUTTON_CLEAR, FALSE);
    }

    gtk_widget_show (button2_start_clear);
    GTK_WIDGET_UNSET_FLAGS (button2_start_clear, GTK_CAN_FOCUS);
    gtk_box_pack_start (GTK_BOX (hbox_b2sn), button2_start_clear, FALSE, FALSE, 4);
    gtk_button_set_relief (GTK_BUTTON (button2_start_clear), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button2_start_clear, _("Reset time"), NULL);
    g_signal_connect (button2_start_clear, "clicked", G_CALLBACK (reset_current_time_start2_cb), appGUI);

    sprintf (tmpbuf, "<b>%s:</b>", _("Date and time to add or subtract from"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox7), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    vbox10 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox10);
    gtk_container_add (GTK_CONTAINER (alignment), vbox10);

    table = gtk_table_new (2, 4, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox10), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Years"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Months"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 999999, 1, 10, 10);
    appGUI->cal->spinbutton2_end_year = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_year);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_year, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 999999, 1, 10, 10);
    appGUI->cal->spinbutton2_end_month = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_month);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_month, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 999999, 1, 10, 10);
    appGUI->cal->spinbutton2_end_day = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_day);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_day, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    label = gtk_label_new (_("Days"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Weeks"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 999999, 1, 10, 10);
    appGUI->cal->spinbutton2_end_week = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_week);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_week, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    table = gtk_table_new (2, 3, FALSE);
    gtk_widget_show (table);
    gtk_box_pack_start (GTK_BOX (vbox10), table, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);

    label = gtk_label_new (_("Hours"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Minutes"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    label = gtk_label_new (_("Seconds"));
    gtk_widget_show (label);
    gtk_table_attach (GTK_TABLE (table), label, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 100000000, 1, 10, 10);
    appGUI->cal->spinbutton2_end_hour = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_hour);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_hour, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 100000000, 1, 10, 10);
    appGUI->cal->spinbutton2_end_minute = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_minute);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_minute, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    spinbutton_adjustment = gtk_adjustment_new (0, 0, 100000000, 1, 10, 10);
    appGUI->cal->spinbutton2_end_second = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adjustment), 1, 0);
    gtk_widget_show (appGUI->cal->spinbutton2_end_second);
    gtk_table_attach (GTK_TABLE (table), appGUI->cal->spinbutton2_end_second, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    hbox10 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox10);
    gtk_box_pack_start (GTK_BOX (vbox10), hbox10, TRUE, TRUE, 4);

    sprintf (tmpbuf, "%s:", _("Operation"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (hbox10), label, FALSE, FALSE, 0);

    appGUI->cal->radiobutton_add = gtk_radio_button_new_with_mnemonic (NULL, _("add"));
    gtk_widget_show (appGUI->cal->radiobutton_add);
    gtk_box_pack_start (GTK_BOX (hbox10), appGUI->cal->radiobutton_add, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (appGUI->cal->radiobutton_add),radiobutton_add_group);
    radiobutton_add_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (appGUI->cal->radiobutton_add));

    radiobutton_sub = gtk_radio_button_new_with_mnemonic (NULL, _("subtract"));
    gtk_widget_show (radiobutton_sub);
    gtk_box_pack_start (GTK_BOX (hbox10), radiobutton_sub, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_sub), radiobutton_add_group);
    radiobutton_add_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_sub));

    if (config.default_stock_icons) {
        button2_end_clear = gui_stock_button (GTK_STOCK_CLEAR, FALSE);
    } else {
        button2_end_clear = gui_stock_button (OSMO_STOCK_BUTTON_CLEAR, FALSE);
    }

    gtk_widget_show (button2_end_clear);
    GTK_WIDGET_UNSET_FLAGS (button2_end_clear, GTK_CAN_FOCUS);
    gtk_box_pack_end (GTK_BOX (hbox10), button2_end_clear, FALSE, FALSE, 4);
    gtk_button_set_relief (GTK_BUTTON (button2_end_clear), GTK_RELIEF_NONE);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, button2_end_clear, _("Reset fields"), NULL);
    g_signal_connect (button2_end_clear, "clicked", G_CALLBACK (reset_addsub_time_cb), appGUI);

    sprintf (tmpbuf, "<b>%s:</b>", _("Time to add or subtract"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox7), hseparator, FALSE, FALSE, 6);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox7), frame, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_container_set_border_width (GTK_CONTAINER (alignment), 8);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    hbox12 = gtk_hbox_new (FALSE, 8);
    gtk_widget_show (hbox12);
    gtk_container_add (GTK_CONTAINER (alignment), hbox12);
/*    gtk_box_pack_start (GTK_BOX (alignment), hbox12, TRUE, TRUE, 4);*/

    appGUI->cal->label2_result = gtk_label_new ("-");
    gtk_label_set_selectable (GTK_LABEL (appGUI->cal->label2_result), 1);
    gtk_widget_show (appGUI->cal->label2_result);
/*    gtk_container_add (GTK_CONTAINER (alignment), appGUI->cal->label2_result);*/
/*    gtk_container_add (GTK_BOX (hbox12), appGUI->cal->label2_result);*/
    gtk_box_pack_start (GTK_BOX (hbox12), appGUI->cal->label2_result, TRUE, TRUE, 4);

    if (config.default_stock_icons) {
        jumpto_button = gtk_button_new_from_stock (GTK_STOCK_JUMP_TO);
    } else {
        jumpto_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_JUMPTO);
    }
    gtk_widget_show (jumpto_button);
    g_signal_connect(jumpto_button, "clicked", G_CALLBACK(jumpto_and_window_close_cb), appGUI);
    gtk_box_pack_end (GTK_BOX (hbox12), jumpto_button, FALSE, TRUE, 4);

    sprintf (tmpbuf, "<b>%s:</b>", _("Result"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

    label = gtk_label_new (_("Add to or subtract from a date"));
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox), hseparator, FALSE, FALSE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_end (GTK_BOX (vbox), hbuttonbox, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox), 4);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    /*---------------------------------------------------------------------------------*/

    if (config.default_stock_icons) {
        close_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    } else {
        close_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CLOSE);
    }
    gtk_widget_show (close_button);
    GTK_WIDGET_UNSET_FLAGS (close_button, GTK_CAN_FOCUS);

    g_signal_connect (appGUI->cal->spinbutton_start_year, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_start_month, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_start_day, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_start_hour, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_start_minute, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_start_second, "value-changed", G_CALLBACK (set_result_cb), appGUI);

    g_signal_connect (appGUI->cal->spinbutton_end_year, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_end_month, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_end_day, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_end_hour, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_end_minute, "value-changed", G_CALLBACK (set_result_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton_end_second, "value-changed", G_CALLBACK (set_result_cb), appGUI);

    g_signal_connect (appGUI->cal->spinbutton2_start_year, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_start_month, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_start_day, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_start_hour, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_start_minute, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_start_second, "value-changed", G_CALLBACK (set_result2_cb), appGUI);

    g_signal_connect (appGUI->cal->spinbutton2_end_year, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_month, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_week, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_day, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_hour, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_minute, "value-changed", G_CALLBACK (set_result2_cb), appGUI);
    g_signal_connect (appGUI->cal->spinbutton2_end_second, "value-changed", G_CALLBACK (set_result2_cb), appGUI);

    g_signal_connect (appGUI->cal->radiobutton_add, "toggled", G_CALLBACK (set_result2_cb), appGUI);

    set_current_date_start_cb (NULL, appGUI);
    set_current_date_start2_cb (NULL, appGUI);
    set_current_date_end_cb (NULL, appGUI);

    g_signal_connect (close_button, "clicked", G_CALLBACK (button_window_date_calculator_close_cb), appGUI);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), close_button);

    gtk_window_get_position (GTK_WINDOW (appGUI->cal->window_date_calculator), &win_xpos, &win_ypos);
    gtk_window_move (GTK_WINDOW (appGUI->cal->window_date_calculator), win_xpos-5, win_ypos-40);
    gtk_widget_show (appGUI->cal->window_date_calculator);

}

