
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

#ifndef _NOTES_H
#define _NOTES_H

#include <stdio.h>
#include <stdlib.h>

#define NOTES_NAME                  "osmo_notes"
#define NOTES_ENTRIES_NAME          "notes_entries"
#define NOTES_CATEGORY_ENTRIES_NAME "category_entries"
#define NOTES_DIRNAME               "notes"
#define NOTES_ENTRIES_FILENAME      "notes_entries.xml"

enum {                  /* notes columns */
    N_COLUMN_NAME = 0,
    N_COLUMN_CATEGORY,
    N_COLUMN_LAST_CHANGES_DATE,
    N_COLUMN_LAST_CHANGES_DATE_JULIAN,
    N_COLUMN_LAST_CHANGES_TIME,
    N_COLUMN_CREATE_DATE,
    N_COLUMN_CREATE_DATE_JULIAN,
    N_COLUMN_CREATE_TIME,
    N_COLUMN_FILENAME,
    NOTES_NUM_COLUMNS
};

enum {
    SELECTOR = 0,
    EDITOR
};

void    notes_show_selector_editor  (gint mode, GUI *appGUI);
void    gui_create_notes            (GUI *appGUI);
void    update_notes_items          (GUI *appGUI);

#endif /* _NOTES_H */

