
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 TODO: Change here is needed Tomasz Maka <pasp@users.sourceforge.net>
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

#ifndef _TASKS_EXPORT_GCAL_H
#define _TASKS_EXPORT_GCAL_H
#include <gcalendar.h>
#include "tasks.h"
/**
  * @brief Generates a date compatible with the RFC 3339 standard
  * 
  * I wrote this function to generate a date compatible with
  * the Google Calendar format (RFC 3339).
  * @param julian_day This is the amount of days since the birth of
  * Christ :)
  * @param time This is the number of seconds elapsed from 00:00 that
  * day
  * @return Pointer to a string representing the formated date
  * @author Vlad Bagrin
  * @todo Think of a way to set the UTC time and the DST values. This is
  * tricky because in some cases Linux will not take DST into account,
  * causing problems with the Google synchronization. In this current
  * state the time sent will have the UTC and DST values of the Google 
  * Calendar.
  */
gchar* gcal_julian_to_date (guint32 julian_day, gint time);

void    *tasks_export_gcal     (exportData *data);


#endif /* _TASKS_EXPORT_GCAL_H */

