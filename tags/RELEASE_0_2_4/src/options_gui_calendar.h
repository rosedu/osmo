
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

#ifndef _OPTIONS_GUI_CALENDAR_H
#define _OPTIONS_GUI_CALENDAR_H

#include "gui.h"

enum {
    ICAL_COLUMN_VALID_ICON = 0,
    ICAL_COLUMN_NAME,
    ICAL_COLUMN_FILENAME,
    ICAL_COLUMN_VALID_FLAG,
    ICAL_COLUMN_ENABLE_DESC,
    ICAL_COLUMN_USE_YEAR
};

void    gui_create_calendar_options_page            (GtkWidget *vbox, GUI *appGUI);
void    calendar_cursor_settings_enable_disable     (GUI *appGUI);

#endif /* _OPTIONS_GUI_CALENDAR_H */

