
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
GDate *tmpdate = NULL;

	g_return_val_if_fail (day > 0 && day <= 31, buffer);

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

void
utl_subtract_from_date (guint32 date, gint time, gint days, gint seconds, guint32 *new_date, gint *new_time)
{
	*new_date = date - days;

	if (time >= 0) {
		*new_time = time - seconds;

		if (*new_time < 0) {
			*new_time = (*new_time) + 24 * 3600;
			*new_date = (*new_date) - 1;
		}
	} else {
		*new_time = -1;
	}
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
utl_xml_get_int (gchar *name, gint *iname, xmlNodePtr node)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
        key = xmlNodeGetContent(node->xmlChildrenNode);
		if (key != NULL) {
			*iname = atoi ((gchar *) key);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_uint (gchar *name, guint *uname, xmlNodePtr node)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
        key = xmlNodeGetContent(node->xmlChildrenNode);
		if (key != NULL) {
			*uname = (guint) atoi ((gchar *) key);
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_char (gchar *name, gchar *cname, xmlNodePtr node)
{
xmlChar *key;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {
        key = xmlNodeGetContent(node->xmlChildrenNode);
		if (key != NULL) {
			*cname = key[0];
			xmlFree (key);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_str (gchar *name, gchar **sname, xmlNodePtr node)
{
xmlChar *key, *out;
xmlParserCtxtPtr context;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {

        key = xmlNodeGetContent(node->xmlChildrenNode);
	    context = xmlCreateDocParserCtxt(key);
	    out = (xmlChar*)xmlStringDecodeEntities(context, key, XML_SUBSTITUTE_REF, 0, 0, 0);
	    xmlFreeParserCtxt (context);

		if (out != NULL) {
			*sname = g_strdup ((gchar *) out);
			xmlFree (out);
		}
	}
}

/*------------------------------------------------------------------------------*/

void
utl_xml_get_strn (gchar *name, gchar *sname, gint buffer_size, xmlNodePtr node)
{
xmlChar *key, *out;
xmlParserCtxtPtr context;

	if ((xmlStrcmp (node->name, (const xmlChar *) name)) == 0) {

        key = xmlNodeGetContent(node->xmlChildrenNode);
        context = xmlCreateDocParserCtxt(key);
	    out = (xmlChar*) xmlStringDecodeEntities(context, key, XML_SUBSTITUTE_REF, 0, 0, 0);
	    xmlFreeParserCtxt (context);

		if (out != NULL) {
			g_strlcpy (sname, (gchar *) out, buffer_size);
			xmlFree (out);
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
utl_xml_put_char (gchar *name, gchar character, xmlNodePtr node, xmlDocPtr doc)
{
gchar buffer[32];
xmlChar *escaped;

	g_snprintf (buffer, 32, "%c", character);
	escaped = xmlEncodeSpecialChars(doc, (const xmlChar *) buffer);
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) escaped);
    xmlFree (escaped);    
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_str (gchar *name, gchar *string, xmlNodePtr node, xmlDocPtr doc)
{
xmlChar *escaped;

	escaped = xmlEncodeSpecialChars(doc, (const xmlChar *) string);
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) escaped);
    xmlFree (escaped); 
}

/*------------------------------------------------------------------------------*/

void
utl_xml_put_strn (gchar *name, gchar *string, gint buffer_size, xmlNodePtr node, xmlDocPtr doc)
{
gchar buffer[BUFFER_SIZE];
xmlChar *escaped;

	if (buffer_size > BUFFER_SIZE) buffer_size = BUFFER_SIZE;
	g_snprintf (buffer, buffer_size, "%s", string);
	escaped = xmlEncodeSpecialChars(doc, (const xmlChar *) buffer);
	xmlNewTextChild (node, NULL, (const xmlChar *) name, (xmlChar *) escaped);
    xmlFree (escaped); 
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

