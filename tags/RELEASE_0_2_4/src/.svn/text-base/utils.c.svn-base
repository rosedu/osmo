
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

#include "utils.h"
#include "i18n.h"
#include "options_prefs.h"
#include "stock_icons.h"

static gunichar TAG_CHAR = 0xe000;      /* Unicode chars in the Private Use Area. */


/*------------------------------------------------------------------------------*/

guint
utl_get_current_day (void)
{
GDate *tmpdate;
guint current_day;

	tmpdate = g_date_new ();
	g_return_val_if_fail (tmpdate != NULL, 0);

	g_date_set_time_t (tmpdate, time (NULL));
	current_day = g_date_get_day (tmpdate);
	g_date_free (tmpdate);

	return current_day;
}

/*------------------------------------------------------------------------------*/

guint
utl_get_current_month (void)
{
GDate *tmpdate;
guint current_month;

	tmpdate = g_date_new ();
	g_return_val_if_fail (tmpdate != NULL, 0);

	g_date_set_time_t (tmpdate, time (NULL));
	current_month = g_date_get_month (tmpdate);
	g_date_free (tmpdate);

	return current_month;
}

/*------------------------------------------------------------------------------*/

guint
utl_get_current_year (void)
{
GDate *tmpdate;
guint current_year;

	tmpdate = g_date_new ();
	g_return_val_if_fail (tmpdate != NULL, 0);

	g_date_set_time_t (tmpdate, time (NULL));
	current_year = g_date_get_year (tmpdate);
	g_date_free (tmpdate);

	return current_year;
}

/*------------------------------------------------------------------------------*/

guint32
utl_get_current_julian (void)
{
GDate *tmpdate;
guint32 current_julian;

	tmpdate = g_date_new ();
	g_return_val_if_fail (tmpdate != NULL, 0);

	g_date_set_time_t (tmpdate, time (NULL));
	current_julian = g_date_get_julian (tmpdate);
	g_date_free (tmpdate);

	return current_julian;
}

/*------------------------------------------------------------------------------*/

gchar *
utl_get_day_name (guint day, gboolean short_name)
{
static gchar buffer[BUFFER_SIZE];
GDate *tmpdate;

	tmpdate = g_date_new_dmy (day, 1, 2007);
	g_return_val_if_fail (tmpdate != NULL, buffer);

	g_date_strftime (buffer, BUFFER_SIZE, short_name ? "%a" : "%A", tmpdate);
	g_date_free (tmpdate);

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
utl_get_julian_day_name (guint32 julian)
{
static gchar buffer[BUFFER_SIZE];
GDate *tmpdate;

	buffer[0] = '\0';
	g_return_val_if_fail (g_date_valid_julian (julian) == TRUE, buffer);

	tmpdate = g_date_new_julian (julian);
	g_return_val_if_fail (tmpdate != NULL, buffer);

	g_date_strftime (buffer, BUFFER_SIZE, "%A", tmpdate);
	g_date_free (tmpdate);

	return buffer;
}

/*------------------------------------------------------------------------------*/

gchar *
utl_get_date_name (GDate *date)
{
static gchar buffer[BUFFER_SIZE];

	g_date_strftime (buffer, BUFFER_SIZE, "%e %B %Y", date);     /* e.g. 1 August 1999 */
	return buffer;
}

/*------------------------------------------------------------------------------*/

guint
utl_get_days_in_month (GDate *date)
{
	return g_date_get_days_in_month (g_date_get_month (date), g_date_get_year (date));
}

/*------------------------------------------------------------------------------*/

guint
utl_get_weekend_days_in_month (GDate *date)
{
guint i, day, days, weekend_days;
GDate *tmpdate;

	tmpdate = g_date_new_dmy (1, g_date_get_month (date), g_date_get_year (date));
	g_return_val_if_fail (tmpdate != NULL, 0);

	days = utl_get_days_in_month (tmpdate);
	weekend_days = 0;

	for (i = 1; i <= days; i++) {
		g_date_set_day (tmpdate, i);
		day = g_date_get_weekday (tmpdate);
		if (day == G_DATE_SATURDAY || day == G_DATE_SUNDAY) {
			weekend_days++;
		}
	}

	g_date_free (tmpdate);
	return weekend_days;
}

/*------------------------------------------------------------------------------*/

guint
utl_get_weekend_days_in_month_my (guint month, guint year)
{
guint i, day, days, weekend_days;
GDate *tmpdate;

	g_return_val_if_fail (g_date_valid_dmy (1, month, year) == TRUE, 0);

	tmpdate = g_date_new_dmy (1, month, year);
	g_return_val_if_fail (tmpdate != NULL, 0);

	days = utl_get_days_in_month (tmpdate);
	weekend_days = 0;

	for (i = 1; i <= days; i++) {
		g_date_set_day (tmpdate, i);
		day = g_date_get_weekday (tmpdate);
		if (day == G_DATE_SATURDAY || day == G_DATE_SUNDAY) {
			weekend_days++;
		}
	}

	g_date_free (tmpdate);
	return weekend_days;
}

/*------------------------------------------------------------------------------*/

guint
utl_get_days_per_year (guint year)
{
	return (g_date_is_leap_year (year) ? 366 : 365);
}

/*------------------------------------------------------------------------------*/

void
utl_gdate_to_dmy (GDate *date, gint *day, gint *month, gint *year)
{
	*day = g_date_get_day (date);
	*month = g_date_get_month (date);
	*year = g_date_get_year (date);
}

/*------------------------------------------------------------------------------*/

void
utl_julian_to_dmy (guint32 julian, guint *day, guint *month, guint *year)
{
GDate *tmpdate;

	g_return_if_fail (g_date_valid_julian (julian) == TRUE);

	tmpdate = g_date_new_julian (julian);
	g_return_if_fail (tmpdate != NULL);

	*day = g_date_get_day (tmpdate);
	*month = g_date_get_month (tmpdate);
	*year = g_date_get_year (tmpdate);

	g_date_free (tmpdate);
}

/*------------------------------------------------------------------------------*/

guint32
utl_dmy_to_julian (guint day, guint month, guint year)
{
GDate *tmpdate;
guint32 julian;

	g_return_val_if_fail (g_date_valid_dmy (day, month, year) == TRUE, 0);

	tmpdate = g_date_new_dmy (day, month, year);
	g_return_val_if_fail (tmpdate != NULL, 0);

	julian = g_date_get_julian (tmpdate);
	g_date_free (tmpdate);

	return julian;
}

/*------------------------------------------------------------------------------*/
/*  This routine has been taken from http://www.voidware.com/moon_phase.htm
    calculates the moon phase (0-7), accurate to 1 segment: 0 = > new moon, 4 => full moon.
*/

guint
utl_calc_moon_phase (GDate *date)
{
gdouble jd;
gint day, month, year;
gint b, c, e;

	utl_gdate_to_dmy (date, &day, &month, &year);

	if (month < 3) {
		year--;
		month += 12;
	}
	month++;
	c = 365.25 * year;
	e = 30.6 * month;
	jd = c + e + day - 694039.09;   /* jd is total days elapsed */
	jd /= 29.53;                    /* divide by the moon cycle (29.53 days) */
	b = jd;                         /* int(jd) -> b, take integer part of jd */
	jd -= b;                        /* subtract integer part to leave fractional part of original jd */
	b = jd * 8 + 0.5;               /* scale fraction from 0-8 and round by adding 0.5 */
	b = b & 7;                      /* 0 and 8 are the same so turn 8 into 0 */

	return b;
}

/*------------------------------------------------------------------------------*/

gchar*
utl_get_moon_phase_name (gint phase)
{
const gchar *phase_names[] = {
	N_("New Moon"), N_("Waxing Crescent Moon"), N_("Quarter Moon"), N_("Waxing Gibbous Moon"),
	N_("Full Moon"), N_("Waning Gibbous Moon"), N_("Last Quarter Moon"), N_("Waning Crescent Moon")
};

	return (gchar *) gettext (phase_names[phase]);
}

/*------------------------------------------------------------------------------*/

void
utl_name_strcat (gchar *first, gchar *second, gchar *buffer)
{
gchar tmpbuff[BUFFER_SIZE];
gboolean flag;

	buffer[0] = '\0';
	g_return_if_fail (first != NULL || second != NULL);

	g_snprintf (tmpbuff, BUFFER_SIZE, "(%s)", _("None"));
	flag = FALSE;

	if (first != NULL) {

		if (strcmp (first, tmpbuff) != 0) {
			flag = TRUE;
			g_strlcpy (buffer, first, BUFFER_SIZE);
		}

		g_free (first);
	}

	if (second != NULL) {

		if (strcmp (second, tmpbuff) != 0) {
			if (flag == TRUE) {
				g_strlcat (buffer, " ", BUFFER_SIZE);
				g_strlcat (buffer, second, BUFFER_SIZE);
			} else {
				g_strlcpy (buffer, second, BUFFER_SIZE);
			}
		}

		g_free (second);
	}

	g_return_if_fail (strlen (buffer) > 0);
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_int (gchar *name, gint *iname, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
		key = xmlNodeListGetString (doc, node->xmlChildrenNode, 1);
		if (key != NULL) {
			*iname = atoi ((gchar *) key);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_uint (gchar *name, guint *uname, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
		key = xmlNodeListGetString (doc, node->xmlChildrenNode, 1);
		if (key != NULL) {
			*uname = (guint) atoi ((gchar *) key);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_char (gchar *name, gchar *cname, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
		key = xmlNodeListGetString (doc, node->xmlChildrenNode, 1);
		if (key != NULL) {
			*cname = key[0];
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_str (gchar *name, gchar **sname, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
		key = xmlNodeListGetString (doc, node->xmlChildrenNode, 1);
		if (key != NULL) {
			*sname = g_strdup ((gchar *) key);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_strn (gchar *name, gchar *sname, gint buffer_size, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
		key = xmlNodeListGetString (doc, node->xmlChildrenNode, 1);
		if (key != NULL) {
			g_strlcpy (sname, (gchar *) key, buffer_size);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_int (gchar *name, gint value, xmlNodePtr node)
{
gchar buffer[32];

	g_snprintf (buffer, 32, "%d", value);
	xmlNewChild (node, NULL, (const xmlChar *) name, (xmlChar *) buffer);
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_uint (gchar *name, guint value, xmlNodePtr node)
{
gchar buffer[32];

	g_snprintf (buffer, 32, "%d", value);
	xmlNewChild (node, NULL, (const xmlChar *) name, (xmlChar *) buffer);
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_char (gchar *name, gchar character, xmlNodePtr node)
{
gchar buffer[32];

	g_snprintf (buffer, 32, "%c", character);
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) buffer);
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_str (gchar *name, gchar *string, xmlNodePtr node)
{
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) string);
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_strn (gchar *name, gchar *string, gint buffer_size, xmlNodePtr node)
{
gchar buffer[BUFFER_SIZE];

	if (buffer_size > BUFFER_SIZE) buffer_size = BUFFER_SIZE;
	g_snprintf (buffer, buffer_size, "%s", string);
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) buffer);
}

/*------------------------------------------------------------------------------*/

gboolean
utl_is_valid_command (gchar *command) {

gchar *found_path;

    found_path = g_find_program_in_path (command);

    if (found_path != NULL) {
		g_free (found_path);
		return TRUE;
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
utl_run_command (gchar *command) {

gchar *cmdline[4];

    cmdline[0] = "sh";
    cmdline[1] = "-c";
    cmdline[2] = command;
    cmdline[3] = 0;
    g_spawn_async(NULL, (gchar **)cmdline, NULL, G_SPAWN_SEARCH_PATH | G_SPAWN_STDOUT_TO_DEV_NULL, 
                  NULL, NULL, NULL, FALSE);
}

/*------------------------------------------------------------------------------*/

gboolean
utl_run_helper (gchar *parameter, gint helper) {

gchar command[PATH_MAX];

    if (helper == EMAIL) {
        sprintf(command, config.email_client, parameter);
    } else if (helper == WWW) {
        sprintf(command, config.web_browser, parameter);
    } else {
        return FALSE;
    }

    utl_run_command (command);

    return TRUE;
}

/*------------------------------------------------------------------------------*/

gint
utl_get_link_type(gchar *link) {

gint i, n;

    for(i=n=0; i < strlen(link); i++) {
        if(link[i] == '@') n++;
    }

    if (!strncasecmp(link, "http://", 7) || !strncasecmp(link, "www", 3)) {
        return WWW;
    } else if (n == 1) {
        return EMAIL;
    } else {
        return UNKNOWN;
    }

}

/*------------------------------------------------------------------------------*/

void
utl_cairo_set_color (cairo_t *cr, GdkColor *color, gint alpha)
{
	cairo_set_source_rgba (cr, (double) color->red / 65535.0,
	                           (double) color->green / 65535.0,
	                           (double) color->blue / 65535.0,
	                           (double) alpha / 65535.0);
}

/*------------------------------------------------------------------------------*/

void
utl_cairo_draw (cairo_t *cr, gint stroke)
{
	if (stroke) {
		cairo_set_line_width (cr, stroke);
		cairo_stroke (cr);
	} else {
		cairo_fill (cr);
	}
}

/*------------------------------------------------------------------------------*/

void
utl_draw_rounded_rectangle (cairo_t *cr, gint x, gint y, gint w, gint h, gint a, gint s)
{
	cairo_move_to (cr, x + a + s, y + s);
	cairo_line_to (cr, x + w - a - s, y + s);
	cairo_arc (cr, x + w - a - s, y + a + s, a, 1.5 * M_PI, 2.0 * M_PI);
	cairo_line_to (cr, x + w - s, y + h - a - s);
	cairo_arc (cr, x + w - a - s, y + h - a - s, a, 0.0 * M_PI, 0.5 * M_PI);
	cairo_line_to (cr, x + a + s, y + h - s);
	cairo_arc (cr, x + a + s, y + h - a - s, a, 0.5 * M_PI, 1.0 * M_PI);
	cairo_line_to (cr, x + s, y + a + s);
	cairo_arc (cr, x + a + s, y + a + s, a, 1.0 * M_PI, 1.5 * M_PI);
}

/*------------------------------------------------------------------------------*/

void
utl_draw_left_arrow (cairo_t *cr, gdouble x, gdouble y, gdouble w, gdouble h, gdouble a)
{
	cairo_move_to (cr, x, y);
	cairo_line_to (cr, x + w * a, y + h * 0.50);
	cairo_line_to (cr, x + w * a, y + h * 0.25);
	cairo_line_to (cr, x + w * 1, y + h * 0.25);
	cairo_line_to (cr, x + w * 1, y - h * 0.25);
	cairo_line_to (cr, x + w * a, y - h * 0.25);
	cairo_line_to (cr, x + w * a, y - h * 0.50);
	cairo_close_path (cr);
}

/*------------------------------------------------------------------------------*/

gint
gui_get_combobox_items (GtkComboBox *combo_box) {
    return gtk_tree_model_iter_n_children (gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box)), NULL);
}

/*------------------------------------------------------------------------------*/

void
gui_create_category_combobox (GtkComboBox *combo_box, GtkListStore *store, gboolean none) {

GtkTreeIter iter;
gchar *category;
gint i, n;

    n = gui_get_combobox_items(combo_box);

    gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), -1);

    for (i = n-1; i >= 0; i--) {
        gtk_combo_box_remove_text (GTK_COMBO_BOX (combo_box), i);
    }

    if (none == TRUE) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), _("None"));
    } else {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), _("All items"));
    }

    i = 0;

    while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, i++)) {

        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &category, -1);
        gtk_combo_box_append_text (GTK_COMBO_BOX (combo_box), category);
        g_free(category);
    }

}

/*------------------------------------------------------------------------------*/

gchar *
gui_text_buffer_get_text_with_tags (GtkTextBuffer *buffer) {

GtkTextIter start, prev;
GSList *tags = NULL, *i;
gchar tag_char_utf8[7] = {0};
gchar *text = g_strdup (""), *oldtext = NULL, *tmp;
gboolean done = FALSE;

    gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &start);

    g_unichar_to_utf8 (TAG_CHAR, tag_char_utf8);

    prev = start;

    while (!done)
    {
        tmp = gtk_text_buffer_get_text (GTK_TEXT_BUFFER (buffer), &prev, &start, TRUE);
        oldtext = text;
        text = g_strconcat (text, tmp, NULL);
        g_free (oldtext);
        g_free (tmp);

        tags = gtk_text_iter_get_toggled_tags (&start, TRUE);
        for (i = tags; i; i = i->next)
        {
            gchar *name;
            g_object_get (G_OBJECT (i->data), "name", &name, NULL);
            oldtext = text;
            text = g_strconcat (text, tag_char_utf8, name, tag_char_utf8, NULL);
            g_free (oldtext);
            g_free (name);
        }
        g_slist_free (tags);

        tags = gtk_text_iter_get_toggled_tags (&start, FALSE);
        for (i = tags; i; i = i->next)
        {
            gchar *name;
            g_object_get (G_OBJECT (i->data), "name", &name, NULL);
            oldtext = text;
            text = g_strconcat (text, tag_char_utf8, "/", name, tag_char_utf8, NULL);
            g_free (oldtext);
            g_free (name);
        }
        g_slist_free (tags);

        if (gtk_text_iter_is_end (&start))
            done = TRUE;
        prev = start;
        gtk_text_iter_forward_to_tag_toggle (&start, NULL);
    }

    return text;
}

/*------------------------------------------------------------------------------*/

void
gui_text_buffer_set_text_with_tags (GtkTextBuffer *buffer, const gchar *text) {

GtkTextIter start, end;
GList *tags = NULL;
gchar **tokens;
gint count;
gchar tag_char_utf8[7] = {0};

    if (!text)
        return;

    gtk_text_buffer_begin_user_action (GTK_TEXT_BUFFER (buffer));

    gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (buffer), &start, &end);
    gtk_text_buffer_delete (GTK_TEXT_BUFFER (buffer), &start, &end);
    gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (buffer), &start, &end);

    g_unichar_to_utf8 (TAG_CHAR, tag_char_utf8);

    tokens = g_strsplit (text, tag_char_utf8, 0);

    for (count = 0; tokens[count]; count++)
    {
        if (count % 2 == 0)
        {
            gint offset;
            GList *j;

            offset = gtk_text_iter_get_offset (&end);
            gtk_text_buffer_insert (GTK_TEXT_BUFFER (buffer), &end, tokens[count], -1);
            gtk_text_buffer_get_iter_at_offset (GTK_TEXT_BUFFER (buffer), &start, offset);

            for (j = tags; j; j = j->next)
            {
                gtk_text_buffer_apply_tag_by_name (GTK_TEXT_BUFFER (buffer), j->data, &start, &end);
            }
        }
        else
        {
            if (tokens[count][0] != '/')
            {
                tags = g_list_prepend (tags, tokens[count]);
            }
            else
            {
                GList *element = g_list_find_custom (tags, &(tokens[count][1]), (GCompareFunc) g_ascii_strcasecmp);

                if (element)
                {
                    tags = g_list_delete_link (tags, element);
                }
            }
        }
    }

    gtk_text_buffer_end_user_action (GTK_TEXT_BUFFER (buffer));

    g_strfreev (tokens);
}

/*------------------------------------------------------------------------------*/

void
gui_change_bg_widget_state (GtkWidget *widget, gchar *color_str, GUI *appGUI) {

GdkColor color;

    if (color_str != NULL) {

        gdk_color_parse (color_str, &color);
        gtk_widget_modify_base (widget, GTK_STATE_NORMAL, &color);

    } else {

        gtk_widget_modify_base (widget, GTK_STATE_NORMAL, 
                                &(appGUI->main_window)->style->base[GTK_WIDGET_STATE (appGUI->main_window)]);
    }

}

/*------------------------------------------------------------------------------*/

GdkPixbuf*
gui_create_color_swatch (gchar *color) {

gchar *swatch[] = {
    "32 14 2 1", ".      c #FFFFFF", "-      c #000000",
    "--------------------------------", "-..............................-", "-..............................-",
    "-..............................-", "-..............................-", "-..............................-",
    "-..............................-", "-..............................-", "-..............................-",
    "-..............................-", "-..............................-", "-..............................-",
    "-..............................-", "--------------------------------"
};

gchar color_str[] = ".      c #000000";

    sprintf (color_str, ".      c %s", color);
    swatch[1] = color_str;
    return gdk_pixbuf_new_from_xpm_data ((gchar const **)swatch);
}

/*------------------------------------------------------------------------------*/

void
gui_fill_iconlabel (GtkWidget *dialog, gchar *stock_icon, gchar *message) {

GtkWidget *hbox1;
GtkWidget *image;
GtkWidget *label;

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG(dialog)->vbox), hbox1, TRUE, TRUE, 16);
    image = gtk_image_new_from_stock (stock_icon, GTK_ICON_SIZE_DIALOG);
    gtk_widget_show (image);
    gtk_box_pack_start (GTK_BOX (hbox1), image, FALSE, TRUE, 16);
    label = gtk_label_new (NULL);
    gtk_widget_show (label);
    gtk_label_set_markup (GTK_LABEL(label), message);
    gtk_box_pack_start (GTK_BOX (hbox1), label, TRUE, TRUE, 16);
}

/*------------------------------------------------------------------------------*/

gint
gui_create_dialog (gint dialog_type, gchar *message, GtkWindow *parent) {

GtkWidget *info_dialog = NULL;
gint response = -1;

    switch (dialog_type) {

        case GTK_MESSAGE_QUESTION:
            if (config.default_stock_icons) {
                info_dialog = gtk_message_dialog_new_with_markup (GTK_WINDOW(parent), 
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, message);
                gtk_window_set_title (GTK_WINDOW(info_dialog), _("Question"));
            } else {
                info_dialog = gtk_dialog_new_with_buttons (_("Question"), GTK_WINDOW(parent),
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      OSMO_STOCK_BUTTON_NO, GTK_RESPONSE_NO,
                                                      OSMO_STOCK_BUTTON_YES, GTK_RESPONSE_YES, NULL);
                gui_fill_iconlabel (info_dialog, "gtk-dialog-question", message);
            }
            break;

        case GTK_MESSAGE_INFO:
            if (config.default_stock_icons) {
                info_dialog = gtk_message_dialog_new_with_markup (GTK_WINDOW(parent), 
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, message);
                gtk_window_set_title (GTK_WINDOW(info_dialog), _("Information"));
            } else {
                info_dialog = gtk_dialog_new_with_buttons (_("Information"), GTK_WINDOW(parent),
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      OSMO_STOCK_BUTTON_CLOSE, GTK_RESPONSE_NO, NULL);
                gui_fill_iconlabel (info_dialog, "gtk-dialog-info", message);
            }
            break;

        case GTK_MESSAGE_ERROR:
            if (config.default_stock_icons) {
                info_dialog = gtk_message_dialog_new_with_markup (GTK_WINDOW(parent), 
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, message);
                gtk_window_set_title (GTK_WINDOW(info_dialog), _("Error"));
            } else {
                info_dialog = gtk_dialog_new_with_buttons (_("Error"), GTK_WINDOW(parent),
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      OSMO_STOCK_BUTTON_CLOSE, GTK_RESPONSE_NO, NULL);
                gui_fill_iconlabel (info_dialog, "gtk-dialog-error", message);
            }
            break;

        case GTK_MESSAGE_WARNING:
            if (config.default_stock_icons) {
                info_dialog = gtk_message_dialog_new_with_markup (GTK_WINDOW(parent), 
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, message);
                gtk_window_set_title (GTK_WINDOW(info_dialog), _("Warning"));
            } else {
                info_dialog = gtk_dialog_new_with_buttons (_("Warning"), GTK_WINDOW(parent),
                                                      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                      OSMO_STOCK_BUTTON_CLOSE, GTK_RESPONSE_NO, NULL);
                gui_fill_iconlabel (info_dialog, "gtk-dialog-warning", message);
            }
            break;
    };

    if (info_dialog != NULL) {
        gtk_widget_show (info_dialog);
        response = gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
    }

    return response;
}

/*------------------------------------------------------------------------------*/

void
gui_clear_text_buffer (GtkTextBuffer *buffer, GtkTextIter *iter) {

GtkTextIter iter_s, iter_e;

    gtk_text_buffer_get_bounds(buffer, &iter_s, &iter_e);
    gtk_text_buffer_delete(buffer, &iter_s, &iter_e);  
    gtk_text_buffer_get_iter_at_offset(buffer, iter, 0);

}

/*------------------------------------------------------------------------------*/

gint
gui_list_store_get_text_index (GtkListStore *store, gchar *text) {

GtkTreeIter iter;
gint i, f;
gchar *category;

    i = f = 0;

    if (text != NULL) {
        while (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, i++)) {

            gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &category, -1);
            if (category != NULL) {
                if (!strcmp(category, text)) {
                    f = 1;
                    break;
                }
                g_free(category);
            }
        }

        if (f) {
            g_free(category);
        }
    }

    return (f ? i:0);
}

/*------------------------------------------------------------------------------*/

void
gui_scrolled_window_move_position(GtkWidget *sw, gint direction) {

GtkAdjustment *adj;

    adj = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(sw));

    if (direction == SW_MOVE_UP) {

        if (adj->value > 0.0) {
            adj->value -= adj->step_increment;
            adj->value = adj->value < 0.0 ? 0.0 : adj->value;
            gtk_adjustment_value_changed (GTK_ADJUSTMENT(adj));
        }

    } else if (direction == SW_MOVE_DOWN) {

        if (adj->value+adj->page_size < adj->upper) {
            adj->value += adj->step_increment;
            gtk_adjustment_value_changed (GTK_ADJUSTMENT(adj));
        }
    }
}

/*------------------------------------------------------------------------------*/

GtkWidget* 
gui_stock_button(const gchar *bstock, gboolean toggle) {

GtkWidget *button;
GtkWidget *alignment;
GtkWidget *hbox;
GtkWidget *image;

    if(toggle == FALSE) {
        button = g_object_new (GTK_TYPE_BUTTON, "visible", TRUE, NULL);
    } else {
        button = g_object_new (GTK_TYPE_TOGGLE_BUTTON, "visible", TRUE, NULL);
    }

    alignment = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
    hbox = gtk_hbox_new (FALSE, 2);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);

    image = gtk_image_new_from_stock (bstock, GTK_ICON_SIZE_BUTTON);
    if (image)
        gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, TRUE, 0);

    gtk_widget_show_all (alignment);
    gtk_container_add (GTK_CONTAINER (button), alignment);

    return button;
}

/*------------------------------------------------------------------------------*/

GtkWidget* 
gui_image_label_radio_button (gchar *label, const guint8 *pix) {

GtkWidget *button;
GtkWidget *alignment;
GtkWidget *hbox;
GtkWidget *image;
GdkPixbuf *icon;

    button = g_object_new (GTK_TYPE_RADIO_BUTTON, "visible", TRUE, NULL);

    alignment = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
    hbox = gtk_hbox_new (FALSE, 2);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);

    icon = gdk_pixbuf_new_from_inline (-1, pix, FALSE, NULL);
    image = gtk_image_new_from_pixbuf(icon);
    gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, TRUE, 0);

    if (label != NULL && label != (gchar *)-1) {
        gtk_box_pack_start (GTK_BOX (hbox),
        g_object_new (GTK_TYPE_LABEL, "label", label, "use_underline", TRUE, NULL), FALSE, TRUE, 0);
    }

    gtk_widget_show_all (alignment);
    gtk_container_add (GTK_CONTAINER (button), alignment);

    return button;
}

/*------------------------------------------------------------------------------*/

GtkWidget* 
gui_stock_label_radio_button(gchar *label, const gchar *stock, GtkIconSize size) {

GtkWidget *button;
GtkWidget *alignment;
GtkWidget *hbox;
GtkWidget *image;

    button = g_object_new (GTK_TYPE_RADIO_BUTTON, "visible", TRUE, NULL);

    alignment = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
    hbox = gtk_hbox_new (FALSE, 2);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);

    image = gtk_image_new_from_stock (stock, size);

    if (image) {
        gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, TRUE, 0);
    }

    if (label != NULL && label != (gchar *)-1) {
        gtk_box_pack_start (GTK_BOX (hbox),
        g_object_new (GTK_TYPE_LABEL, "label", label, "use_underline", TRUE, NULL), FALSE, TRUE, 0);
    }

    gtk_widget_show_all (alignment);
    gtk_container_add (GTK_CONTAINER (button), alignment);

    return button;
}

/*------------------------------------------------------------------------------*/

void
gui_url_initialize(GUI *appGUI) {

    appGUI->hand_cursor = gdk_cursor_new (GDK_HAND2);
    appGUI->regular_cursor = gdk_cursor_new (GDK_XTERM);

    appGUI->hovering_over_link = FALSE;
    appGUI->gui_url_tag = NULL;
}

/*------------------------------------------------------------------------------*/

void 
gui_url_insert_link (GSList *links_list, gint *link_index, GtkWidget *textview, 
                     GtkTextIter *iter, gchar *color, gchar *font, gchar *text, gboolean center, GUI *appGUI) {

    if (font == NULL && color == NULL) {
        if (config.disable_underline_links) {
            if (center == TRUE) {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", "blue", 
                                                          "justification", GTK_JUSTIFY_CENTER, NULL);
            } else {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", "blue", NULL);
            }
        } else {
            if (center == TRUE) {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", "blue", 
                                                          "underline", PANGO_UNDERLINE_SINGLE,
                                                          "justification", GTK_JUSTIFY_CENTER, NULL);
            } else {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", "blue", 
                                                          "underline", PANGO_UNDERLINE_SINGLE, NULL);
            }
        }
    } else {
        if (config.disable_underline_links) {
            if (center == TRUE) {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", color, 
                                                          "font", font,
                                                          "justification", GTK_JUSTIFY_CENTER, NULL);
            } else {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", color, 
                                                          "font", font, NULL);
            }
        } else {
            if (center == TRUE) {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", color, 
                                                          "underline", PANGO_UNDERLINE_SINGLE, 
                                                          "font", font,
                                                          "justification", GTK_JUSTIFY_CENTER, NULL);
            } else {
                appGUI->gui_url_tag = gtk_text_buffer_create_tag (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),
                                                          NULL, "foreground", color, 
                                                          "underline", PANGO_UNDERLINE_SINGLE, 
                                                          "font", font, NULL);
            }
        }
    }

    g_object_set_data (G_OBJECT (appGUI->gui_url_tag), "link", GINT_TO_POINTER (*link_index));

    links_list = g_slist_append (links_list, g_strdup(text));
    gtk_text_buffer_insert_with_tags (gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)), 
                                      iter, text, -1, appGUI->gui_url_tag, NULL);
    (*link_index)++;
}

/*------------------------------------------------------------------------------*/

void
gui_url_remove_links (GSList *links_list, gint *link_index) {

gint i;
gchar *link;
GSList *lnode;

        *link_index = 1;

        if (links_list != NULL) {

            for (i = 0, lnode = links_list; lnode != NULL; lnode = lnode->next, i++) {
                link = g_slist_nth_data (links_list, i);
                if (link != NULL) {
                    g_free(link);
                }
            }

            for (i = 0, lnode = links_list; lnode != NULL; lnode = lnode->next, i++) {
                links_list = g_slist_remove_link (links_list, g_slist_nth(links_list, i));
            }

        }
}


/*------------------------------------------------------------------------------*/

void
gui_url_set_cursor_if_appropriate (GtkTextView *textview, gint x, gint y, GUI *appGUI) {

GSList *tags = NULL, *tagp = NULL;
GtkTextBuffer *buffer;
GtkTextIter iter;
gboolean hovering = FALSE;
GtkTextTag *tag;
int *slink;

    buffer = gtk_text_view_get_buffer (textview);

    gtk_text_view_get_iter_at_location (textview, &iter, x, y);

    tags = gtk_text_iter_get_tags (&iter);

    for (tagp = tags;  tagp != NULL;  tagp = tagp->next) {
        tag = tagp->data;
        slink = g_object_get_data (G_OBJECT (tag), "link");

        if (slink != 0) {
            hovering = TRUE;
            break;
        }
    }

    if (hovering != appGUI->hovering_over_link) {
        appGUI->hovering_over_link = hovering;

        if (appGUI->hovering_over_link) {
            gdk_window_set_cursor (gtk_text_view_get_window (textview, GTK_TEXT_WINDOW_TEXT), appGUI->hand_cursor);
        } else {
            gdk_window_set_cursor (gtk_text_view_get_window (textview, GTK_TEXT_WINDOW_TEXT), appGUI->regular_cursor);
        }
    }

    if (tags) {
        g_slist_free (tags);
    }
}

/*------------------------------------------------------------------------------*/

gboolean
gui_url_event_after (GtkWidget *textview, GdkEvent *ev, GSList *links_list) {

GtkTextIter start, end, iter;
GtkTextBuffer *buffer;
GdkEventButton *event;
gint x, y;
GSList *tags = NULL, *tagp = NULL;
GtkTextTag *tag;
gchar *link;
gint slink;

    if (ev->type != GDK_BUTTON_RELEASE) {
        return FALSE;
    }

    event = (GdkEventButton *)ev;

    if (event->button != 1) {
        return FALSE;
    }

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

    gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
    if (gtk_text_iter_get_offset (&start) != gtk_text_iter_get_offset (&end)) {
        return FALSE;
    }

    gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview), GTK_TEXT_WINDOW_WIDGET, 
                                           event->x, event->y, &x, &y);
    gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (textview), &iter, x, y);

    tags = gtk_text_iter_get_tags (&iter);

    for (tagp = tags;  tagp != NULL;  tagp = tagp->next) {

        tag = tagp->data;
        slink = (gint) g_object_get_data (G_OBJECT (tag), "link");

        if (slink != 0) {
            link = g_slist_nth_data (links_list, slink);
            utl_run_helper(link, utl_get_link_type(link));
            break;
        }
    }

    if (tags) {
        g_slist_free (tags);
    }

    return FALSE;
}

/*------------------------------------------------------------------------------*/


gboolean
gui_url_motion_notify_event (GtkWidget *textview, GdkEventMotion *event, gpointer data) {

gint x, y;

    GUI *appGUI = (GUI *)data;

    gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview), GTK_TEXT_WINDOW_WIDGET, 
                                           event->x, event->y, &x, &y);
    gui_url_set_cursor_if_appropriate (GTK_TEXT_VIEW (textview), x, y, appGUI);
    gdk_window_get_pointer (textview->window, NULL, NULL, NULL);
    return FALSE;
}

/*------------------------------------------------------------------------------*/

gboolean
gui_url_visibility_notify_event (GtkWidget *textview, GdkEventVisibility *event, gpointer data) {

gint wx, wy, bx, by;

    GUI *appGUI = (GUI *)data;

    gdk_window_get_pointer (textview->window, &wx, &wy, NULL);
    gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview), GTK_TEXT_WINDOW_WIDGET, wx, wy, &bx, &by);
    gui_url_set_cursor_if_appropriate (GTK_TEXT_VIEW (textview), bx, by, appGUI);
    return FALSE;
}

/*------------------------------------------------------------------------------*/

void
gui_url_setup (GSList *links_list, gint *link_index, GtkWidget *textview, GUI *appGUI) {

    gui_url_remove_links (links_list, link_index);

    *link_index = 1;
    appGUI->hovering_over_link = FALSE;
    appGUI->gui_url_tag = NULL;

    g_signal_connect (textview, "event-after", G_CALLBACK (gui_url_event_after), links_list);
    g_signal_connect (textview, "motion-notify-event", G_CALLBACK (gui_url_motion_notify_event), appGUI);
    g_signal_connect (textview, "visibility-notify-event", G_CALLBACK (gui_url_visibility_notify_event), appGUI);

}

/*------------------------------------------------------------------------------*/

void
gui_update_command_status (GtkWidget *entry_widget, GtkWidget *icon_widget, GUI *appGUI) {

GdkPixbuf *image;
gchar *cmd;
gint i;

    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry_widget)))) {

        cmd = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_widget)));

        for (i=0; i < strlen(cmd); i++) {
            if (cmd[i] == ' ') cmd[i] = 0;
        }
	    gtk_widget_show (icon_widget);

        if (utl_is_valid_command (cmd) == TRUE) {
            image = gtk_widget_render_icon (icon_widget, OSMO_STOCK_LIST_VALID, GTK_ICON_SIZE_MENU, NULL);
        } else {
            image = gtk_widget_render_icon (icon_widget, OSMO_STOCK_LIST_INVALID, GTK_ICON_SIZE_MENU, NULL);
        }

	    gtk_image_set_from_pixbuf (GTK_IMAGE(icon_widget), image);
	    g_object_unref (image);
        g_free (cmd);

    } else {
	    gtk_widget_hide (icon_widget);
    }
}

/*------------------------------------------------------------------------------*/

gint
gui_check_overwrite_file (gchar *filename, GtkWidget *window, GUI *appGUI) {

gint response;

    if (g_file_test(filename, G_FILE_TEST_IS_REGULAR) == TRUE) {

        response = gui_create_dialog(GTK_MESSAGE_QUESTION, _("Selected file exist! Overwrite?"), 
                                     GTK_WINDOW(window));

        if (response == GTK_RESPONSE_NO || response == GTK_RESPONSE_DELETE_EVENT) {
            return -1;
        }
    }

    return 0;
}

/*------------------------------------------------------------------------------*/

