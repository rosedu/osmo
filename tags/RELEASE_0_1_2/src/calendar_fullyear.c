
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
#include <glib/gprintf.h>
#include <glib.h>
#include <time.h>
#include <ctype.h>

#include "i18n.h"
#include "gui_utils.h"
#include "gui.h"
#include "options_prefs.h"
#include "calendar.h"
#include "calendar_notes.h"
#include "calendar_fullyear.h"
#include "calendar_utils.h"
#include "stock_icons.h"


/*------------------------------------------------------------------------*/

void
display_calendar (gint year, GUI *appGUI) {

gchar calendar_table[MAX_MONTHS*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))];
gint i, j, x, leap, days, fday, idx;
gint cd, cm, cy;
gchar d;
gchar tmpbuf[BUFFER_SIZE];

gint days_in_month[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

    for(i=0; i < MAX_MONTHS*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1)); i++) {
        calendar_table[i] = -1;
    }

    for(j=0; j < MAX_MONTHS; j++) {

        leap = year % ((!(year % 100)) ? 400 : 4);
        days = days_in_month[j] + ((j == 1 && !leap) ? 1 : 0);
        i = (j + 10) % 12 + 1;
        x = year - i / 11;
        fday = (((13*i-1) / 5) + ((x % 100) / 4) + (x / 400) + (x % 100) - 2*(x / 100)) % DAYS_PER_WEEK;
        fday = (fday+DAYS_PER_WEEK) % DAYS_PER_WEEK + 1;

        i = 1;

        while (--days >= 0) {
            calendar_table[j*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1)) - 1 + fday++] = i++;
        }

    }

    cy = get_current_year();
    cm = get_current_month();
    cd = get_current_day();

    for(i=0; i < MAX_MONTHS; i++) {
        for(j=0; j < (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1)); j++) {

            idx = i*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+j;
            gtk_label_set_text(GTK_LABEL(appGUI->cal->calendar_labels[idx]), "");

            d = calendar_table[idx];

            if (d > 0) {

                if (!(year == cy && i == cm && d == cd)) {
                    if(i%2) {
                        sprintf (tmpbuf, "<span foreground='medium blue'>%2d</span>", d);
                    } else {
                        sprintf (tmpbuf, "%2d", d);
                    }

                    if(!((j+1)%7) || !((j+2)%7)) {
                        sprintf (tmpbuf, "<span foreground='firebrick'>%d</span>", d);
                    }
                } else {    /* select current day */
                    if(i%2) {
                        sprintf (tmpbuf, "<span foreground='medium blue' weight='bold'>(%d)</span>", d);
                    } else {
                        sprintf (tmpbuf, "<span weight='bold'>(%d)</span>", d);
                    }

                    if(!((j+1)%7) || !((j+2)%7)) {
                        sprintf (tmpbuf, "<span foreground='firebrick' weight='bold'>(%d)</span>", d);
                    }
                }

                gtk_label_set_markup (GTK_LABEL (appGUI->cal->calendar_labels[idx]), tmpbuf);
            }
        }
    }
}

/*------------------------------------------------------------------------*/

void
change_to_current_year_cb (GtkWidget *widget, gpointer data) {

gint year;

    GUI *appGUI = (GUI *)data;

    year = get_current_year();
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->cal->fy_spinbutton), year);
}

/*------------------------------------------------------------------------*/

void
change_to_next_year_cb (GtkWidget *widget, gpointer data) {

gint year;

    GUI *appGUI = (GUI *)data;

    year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->fy_spinbutton));
    year++;
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->cal->fy_spinbutton), year);
}

/*------------------------------------------------------------------------*/

void
change_to_previous_year_cb (GtkWidget *widget, gpointer data) {

gint year;

    GUI *appGUI = (GUI *)data;

    year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(appGUI->cal->fy_spinbutton));
    if (year > JULIAN_GREGORIAN_YEAR) {
        year--;
    }
    gtk_spin_button_set_value (GTK_SPIN_BUTTON(appGUI->cal->fy_spinbutton), year);
}

/*------------------------------------------------------------------------*/

void
change_year_spin_button_cb (GtkSpinButton *spinbutton, gpointer user_data) {

gint year;

    GUI *appGUI = (GUI *)user_data;

    year = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton));

    if (year != get_current_year()) {
        gtk_widget_set_sensitive(appGUI->cal->cyear_button, TRUE);
    } else {
        gtk_widget_set_sensitive(appGUI->cal->cyear_button, FALSE);
    }

    display_calendar(year, appGUI);
}

/*------------------------------------------------------------------------*/

void
fullyear_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gtk_window_get_size (GTK_WINDOW(appGUI->cal->fullyear_window),
                        &config.fy_window_size_x, &config.fy_window_size_y);

    gtk_widget_destroy (appGUI->cal->fullyear_window);
}

/*------------------------------------------------------------------------------*/

void
button_fullyear_window_close_cb (GtkButton *button, gpointer user_data) {

    fullyear_window_close_cb (GTK_WIDGET(button), NULL, user_data);

}

/*------------------------------------------------------------------------------*/

gint 
fullyear_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer data) {

    GUI *appGUI = (GUI *)data;

    switch(event->keyval) {

        case GDK_Escape:
            gtk_widget_destroy(appGUI->cal->fullyear_window);
            return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
calendar_create_fullyear_window (GUI *appGUI) {

GtkWidget   *vbox1;
GtkWidget   *hseparator;
GtkWidget   *hbuttonbox;
GtkWidget   *close_button;
GtkWidget   *hbox1;
GtkWidget   *prev_button;
GtkObject   *fy_spinbutton_adj;
GtkWidget   *next_button;
GtkWidget   *fycal_scrolledwindow;
GtkWidget   *fycal_viewport;
GtkWidget   *vseparator;
GtkWidget   *label;
gchar       tmpbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
gint        i, j, idx;
GDate       *cdate = NULL;

    cdate = g_date_new();
    if (cdate != NULL) {
        g_date_set_day (cdate, get_current_day());
        g_date_set_month (cdate, get_current_month());
        g_date_set_year (cdate, get_current_year());
    }

    appGUI->cal->fullyear_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (appGUI->cal->fullyear_window), _("Full-year calendar"));
    gtk_window_set_position (GTK_WINDOW (appGUI->cal->fullyear_window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_default_size (GTK_WINDOW(appGUI->cal->fullyear_window), config.fy_window_size_x, config.fy_window_size_y);
    gtk_window_set_modal (GTK_WINDOW (appGUI->cal->fullyear_window), TRUE);
    g_signal_connect (G_OBJECT (appGUI->cal->fullyear_window), "delete_event",
                      G_CALLBACK(fullyear_window_close_cb), appGUI);
    gtk_window_set_transient_for(GTK_WINDOW(appGUI->cal->fullyear_window), GTK_WINDOW(appGUI->main_window));
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->cal->fullyear_window), 8);
    g_signal_connect (G_OBJECT (appGUI->cal->fullyear_window), "key_press_event",
                      G_CALLBACK (fullyear_key_press_cb), appGUI);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->cal->fullyear_window), vbox1);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 4);

    sprintf(tmpbuf, "<b>%s:</b>", _("Year"));
    label = gtk_label_new (tmpbuf);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (hbox1), label, FALSE, TRUE, 0);
    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    gtk_misc_set_padding (GTK_MISC (label), 0, 4);

    fy_spinbutton_adj = gtk_adjustment_new (2007, JULIAN_GREGORIAN_YEAR, 9999, 1, 10, 10);
    appGUI->cal->fy_spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (fy_spinbutton_adj), 1, 0);
    gtk_widget_show (appGUI->cal->fy_spinbutton);
    g_signal_connect(appGUI->cal->fy_spinbutton, "value-changed", G_CALLBACK(change_year_spin_button_cb), appGUI);
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->cal->fy_spinbutton, FALSE, FALSE, 8);
    gtk_widget_set_size_request (appGUI->cal->fy_spinbutton, 80, -1);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (appGUI->cal->fy_spinbutton), TRUE);
    gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (appGUI->cal->fy_spinbutton), GTK_UPDATE_IF_VALID);

    if (config.default_stock_icons) {
        prev_button = gui_stock_button(GTK_STOCK_GO_BACK, FALSE);
    } else {
        prev_button = gui_stock_button(OSMO_STOCK_BUTTON_PREV_YEAR, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(prev_button, GTK_CAN_FOCUS);
    gtk_widget_show (prev_button);
    g_signal_connect(prev_button, "clicked", G_CALLBACK(change_to_previous_year_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, prev_button, _("Previous year"), NULL);
    gtk_button_set_relief (GTK_BUTTON (prev_button), GTK_RELIEF_NONE); 
    gtk_box_pack_start (GTK_BOX (hbox1), prev_button, FALSE, FALSE, 2);

    if (config.default_stock_icons) {
        appGUI->cal->cyear_button = gui_stock_button(GTK_STOCK_HOME, FALSE);
    } else {
        appGUI->cal->cyear_button = gui_stock_button(OSMO_STOCK_BUTTON_TODAY, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(appGUI->cal->cyear_button, GTK_CAN_FOCUS);
    gtk_widget_show (appGUI->cal->cyear_button);
    g_signal_connect(appGUI->cal->cyear_button, "clicked", G_CALLBACK(change_to_current_year_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, appGUI->cal->cyear_button, _("Current year"), NULL);
    gtk_button_set_relief (GTK_BUTTON (appGUI->cal->cyear_button), GTK_RELIEF_NONE); 
    gtk_box_pack_start (GTK_BOX (hbox1), appGUI->cal->cyear_button, FALSE, FALSE, 2);

    if (config.default_stock_icons) {
        next_button = gui_stock_button(GTK_STOCK_GO_FORWARD, FALSE);
    } else {
        next_button = gui_stock_button(OSMO_STOCK_BUTTON_NEXT_YEAR, FALSE);
    }
    GTK_WIDGET_UNSET_FLAGS(next_button, GTK_CAN_FOCUS);
    gtk_widget_show (next_button);
    g_signal_connect(next_button, "clicked", G_CALLBACK(change_to_next_year_cb), appGUI);
    gtk_tooltips_set_tip (appGUI->osmo_tooltips, next_button, _("Next year"), NULL);
    gtk_button_set_relief (GTK_BUTTON (next_button), GTK_RELIEF_NONE); 
    gtk_box_pack_start (GTK_BOX (hbox1), next_button, FALSE, FALSE, 2);

    fycal_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (fycal_scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox1), fycal_scrolledwindow, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (fycal_scrolledwindow), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (fycal_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    fycal_viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (fycal_viewport);
    gtk_container_add (GTK_CONTAINER (fycal_scrolledwindow), fycal_viewport);

    if (config.fy_simple_view == FALSE) {
        appGUI->cal->fycal_table = gtk_table_new (MAX_MONTHS+4, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+3, FALSE);
    } else {
        appGUI->cal->fycal_table = gtk_table_new (MAX_MONTHS+2, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+1, FALSE);
    }

    gtk_widget_show (appGUI->cal->fycal_table);
    gtk_container_add (GTK_CONTAINER (fycal_viewport), appGUI->cal->fycal_table);

    vseparator = gtk_vseparator_new ();
    gtk_widget_show (vseparator);
    if (config.fy_simple_view == FALSE) {
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), vseparator, 1, 2, 0, 16,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    } else {
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), vseparator, 1, 2, 0, 14,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
    }

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, 2, 39, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);

    if (config.fy_simple_view == FALSE) {

        vseparator = gtk_vseparator_new ();
        gtk_widget_show (vseparator);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), vseparator, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+2, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+3, 0, 16,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
        hseparator = gtk_hseparator_new ();
        gtk_widget_show (hseparator);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, 2, 39, MAX_MONTHS+2, MAX_MONTHS+3,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 0, 0);
        hseparator = gtk_hseparator_new ();
        gtk_widget_show (hseparator);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, 0, 1, MAX_MONTHS+2, MAX_MONTHS+3,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 0, 0);
        hseparator = gtk_hseparator_new ();
        gtk_widget_show (hseparator);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+3, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+4, 1, 2,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 0, 0);
        hseparator = gtk_hseparator_new ();
        gtk_widget_show (hseparator);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), hseparator, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+3, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+4, MAX_MONTHS+2, MAX_MONTHS+3,
                          (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 0, 0);
    }

    for(i=0;i<MAX_MONTHS; i++) {

        j = config.fy_simple_view;

        if (cdate != NULL) {
            g_date_set_day (cdate, 1);
            g_date_set_month (cdate, i+1);

            if (j) {
                g_date_strftime (buffer, BUFFER_SIZE-1, "%b", cdate);
            } else {
                g_date_strftime (buffer, BUFFER_SIZE-1, "%B", cdate);
            }
        }

        if(i%2) {
            sprintf (tmpbuf, "<span foreground='medium blue'>%s</span>", buffer);
        } else {
            sprintf (tmpbuf, "%s", buffer);
        }

        label = gtk_label_new (NULL);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), label, 0, 1, i+2, i+3,
                         (GtkAttachOptions) (GTK_FILL),
                         (GtkAttachOptions) (0), 0, 0);

        gtk_misc_set_padding (GTK_MISC (label), 8, 0);
        gtk_label_set_markup (GTK_LABEL (label), tmpbuf);

        if (config.fy_simple_view == FALSE) {
            label = gtk_label_new (NULL);
            gtk_widget_show (label);
            gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), label, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+3, (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+4, i+2, i+3,
                             (GtkAttachOptions) (GTK_FILL),
                             (GtkAttachOptions) (0), 0, 0);

            gtk_misc_set_padding (GTK_MISC (label), 8, 0);
            gtk_label_set_markup (GTK_LABEL (label), tmpbuf);
        }
    }

    if (cdate != NULL) {
        g_date_set_day (cdate, 1);
        g_date_set_month (cdate, 1);    /* start with monday */
        g_date_set_year (cdate, 2007);
    }

    for(i=0;i<(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1)); i++) {

        if (cdate != NULL) {
            g_date_set_day (cdate, (i % DAYS_PER_WEEK)+1);
            g_date_strftime (buffer, BUFFER_SIZE-1, "%a", cdate);
        }

        if(!((i+1)%7) || !((i+2)%7)) {
            sprintf (tmpbuf, "<span foreground='firebrick'>%s</span>", buffer);
        } else {
            sprintf (tmpbuf, "%s", buffer);
        }

        label = gtk_label_new (NULL);
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), label, i+2, i+3, 0, 1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (0), 0, 0);
        gtk_misc_set_padding (GTK_MISC (label), 4, 0);
        gtk_label_set_angle (GTK_LABEL (label), 90);
        gtk_label_set_markup (GTK_LABEL (label), tmpbuf);

        if (config.fy_simple_view == FALSE) {
            label = gtk_label_new (NULL);
            gtk_widget_show (label);
            gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), label, i+2, i+3, MAX_MONTHS+3, MAX_MONTHS+4,
                              (GtkAttachOptions) (GTK_FILL),
                              (GtkAttachOptions) (0), 0, 0);
            gtk_misc_set_padding (GTK_MISC (label), 4, 0);
            gtk_label_set_angle (GTK_LABEL (label), 90);
            gtk_label_set_markup (GTK_LABEL (label), tmpbuf);
        }
    }

    for(i=0; i < MAX_MONTHS; i++) {
        for(j=0; j < (MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1)); j++) {

            idx = i*(MAX_WEEKS*DAYS_PER_WEEK-(MAX_WEEKS-1))+j;

                appGUI->cal->calendar_labels[idx] = gtk_label_new (NULL);
                gtk_widget_show (appGUI->cal->calendar_labels[idx]);
                gtk_table_attach (GTK_TABLE (appGUI->cal->fycal_table), appGUI->cal->calendar_labels[idx], j+2, j+3, i+2, i+3,
                                 (GtkAttachOptions) (GTK_FILL),
                                 (GtkAttachOptions) (0), 0, 0);
        }
    }

    display_calendar(get_current_year(), appGUI);
    gtk_widget_set_sensitive(appGUI->cal->cyear_button, FALSE);

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 4);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_set_spacing (GTK_BOX (hbuttonbox), 4);

    if (config.default_stock_icons) {
        close_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    } else {
        close_button = gtk_button_new_from_stock (OSMO_STOCK_BUTTON_CLOSE);
    }
    gtk_widget_show (close_button);
    g_signal_connect(close_button, "clicked", G_CALLBACK(button_fullyear_window_close_cb), appGUI);
    gtk_container_add(GTK_CONTAINER(hbuttonbox), close_button);

    gtk_widget_show(appGUI->cal->fullyear_window);
}

/*------------------------------------------------------------------------*/

