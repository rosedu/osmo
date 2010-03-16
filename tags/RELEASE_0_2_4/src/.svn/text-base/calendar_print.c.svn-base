
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

#include "i18n.h"
#include "utils.h"

/*------------------------------------------------------------------------------*/

void
calendar_begin_print (GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data)
{
	GtkPageSetup *setup;
	/*gdouble height;*/

	GUI *appGUI = (GUI *) user_data;

	/* FIXME */
	setup = gtk_page_setup_new ();
	gtk_page_setup_set_orientation (setup, GTK_PAGE_ORIENTATION_LANDSCAPE);
	gtk_print_operation_set_default_page_setup (operation, setup);
	g_object_unref (setup);

	appGUI->print_lines_per_page = 1;
	appGUI->print_nlines = 1;
	appGUI->print_npages = 1;
	gtk_print_operation_set_n_pages (operation, appGUI->print_npages);
}

/*------------------------------------------------------------------------------*/

void
calendar_draw_page (GtkPrintOperation *operation, GtkPrintContext *context, gint npage, gpointer user_data)
{
	PangoLayout *layout;
	PangoFontDescription *desc;
	cairo_t *cr;
	gdouble page_width, day_width, day_height;
	gint text_width, text_height, header_height;
	gint day, days, i, j;
	GDate *date;
	gchar buffer[BUFFER_SIZE];

	GUI *appGUI = (GUI *) user_data;

	date = g_date_new_julian (g_date_get_julian (appGUI->cal->date));
	g_return_if_fail (date != NULL);

	cr = gtk_print_context_get_cairo_context (context);
	page_width = gtk_print_context_get_width (context);
	day_width = page_width / 7;
	day_height = day_width * 0.7;

	layout = gtk_print_context_create_pango_layout (context);

	desc = pango_font_description_from_string ("Sans Bold");
	pango_font_description_set_size (desc, 40 * PANGO_SCALE);
	pango_layout_set_font_description (layout, desc);

	g_date_strftime (buffer, BUFFER_SIZE, "%B %Y", date);
	pango_layout_set_markup (layout, buffer, -1);
	pango_layout_get_pixel_size (layout, &text_width, &text_height);
	cairo_move_to (cr, (page_width - text_width) / 2, 0);
	pango_cairo_show_layout (cr, layout);
	pango_font_description_set_size (desc, 10 * PANGO_SCALE);
	pango_layout_set_font_description (layout, desc);

	header_height = text_height * 1.2;
	cairo_set_line_width (cr, 1);

	for (i = 0; i < 7; i++) {
		g_snprintf (buffer, BUFFER_SIZE, "%s", utl_get_day_name (i + 1, 0));
		pango_layout_set_markup (layout, buffer, -1);
		pango_layout_get_pixel_size (layout, &text_width, &text_height);
		cairo_move_to (cr, day_width * i + (day_width - text_width) / 2, header_height);
		pango_cairo_show_layout (cr, layout);
	}

	header_height += text_height * 1.2;

	pango_font_description_set_size (desc, 16 * PANGO_SCALE);
	pango_layout_set_font_description (layout, desc);

	g_date_set_day (date, 1);
	days = utl_get_days_in_month (date);
	day = 0;

	for (i = 0; i < 6; i++) {

		for (j = 1; j <= 7; j++) {

			if (day == 0) {
				if (g_date_get_weekday (date) == j) day++;
			}

			cairo_rectangle (cr, day_width * (j - 1), header_height + day_height * i, day_width, day_height);

			if (day > 0 && day <= days) {
				cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
			} else {
				cairo_set_source_rgb (cr, 0.9, 0.9, 0.9);
			}

			cairo_fill_preserve (cr);
			cairo_set_source_rgb (cr, 0, 0, 0);
			cairo_stroke (cr);

			if (day > 0 && day <= days) {
				if (j == G_DATE_SATURDAY || j == G_DATE_SUNDAY) {
					g_snprintf (buffer, BUFFER_SIZE, "<span color=\"red\">%d</span>", day);
				} else {
					g_snprintf (buffer, BUFFER_SIZE, "%d", day);
				}
				pango_layout_set_markup (layout, buffer, -1);
				cairo_move_to (cr, day_width * (j - 1) + 3, header_height + day_height * i + 2);
				pango_cairo_show_layout (cr, layout);
			}

			if (day > 0) day++;
		}
	}

	g_date_free (date);
	pango_font_description_free (desc);
	g_object_unref (layout);
}

/*------------------------------------------------------------------------------*/

void
calendar_print (GUI *appGUI) {

GtkPrintOperation *print;
GtkPrintOperationResult result;
GError *error = NULL;
gchar buffer[BUFFER_SIZE];

    print = gtk_print_operation_new ();

    appGUI->print_lines_per_page = 0;
    appGUI->print_nlines = 0;
    appGUI->print_npages = 0;

    g_signal_connect (print, "begin_print", G_CALLBACK (calendar_begin_print), appGUI);
    g_signal_connect (print, "draw_page", G_CALLBACK (calendar_draw_page), appGUI);

    result = gtk_print_operation_run (print, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,
                                      GTK_WINDOW (appGUI->main_window), &error);

    if (result == GTK_PRINT_OPERATION_RESULT_ERROR) {
	    g_snprintf (buffer, BUFFER_SIZE, "%s: %s", _("Error printing"), error->message);
        gui_create_dialog(GTK_MESSAGE_ERROR, buffer, GTK_WINDOW (appGUI->main_window));
        g_error_free (error);
    }

    g_object_unref (print);
}

/*------------------------------------------------------------------------------*/


