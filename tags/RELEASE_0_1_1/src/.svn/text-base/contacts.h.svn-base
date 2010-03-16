
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

#ifndef _CONTACTS_H
#define _CONTACTS_H

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>


#define CONTACTS_NAME               "osmo_contacts"
#define CONTACTS_ENTRIES_NAME       "contacts_records"
#define CONTACTS_GROUP_ENTRIES_NAME "contacts_groups"
#define CONTACTS_ENTRIES_FILENAME   "contacts_records.xml"

/* photo widths */
#define PHOTO_SMALL     80
#define PHOTO_MEDIUM    120
#define PHOTO_LARGE     160

enum {
    CONTACTS_FF_FIRST_NAME = 0,
    CONTACTS_FF_LAST_NAME,
    CONTACTS_FF_ALL_FIELDS
};

enum {
    COLUMN_GROUP = 0, COLUMN_FIRST_NAME, COLUMN_SECOND_NAME, COLUMN_LAST_NAME, COLUMN_NICK_NAME, COLUMN_BIRTH_DAY_DATE, COLUMN_NAME_DAY_DATE,

    COLUMN_HOME_ADDRESS, COLUMN_HOME_POST_CODE, COLUMN_HOME_CITY, COLUMN_HOME_STATE, COLUMN_HOME_COUNTRY,

    COLUMN_WORK_ORGANIZATION, COLUMN_WORK_DEPARTMENT, 
    
    COLUMN_WORK_ADDRESS, COLUMN_WORK_POST_CODE, COLUMN_WORK_CITY, COLUMN_WORK_STATE, COLUMN_WORK_COUNTRY, 
    
    COLUMN_WORK_FAX,
    
    COLUMN_HOME_PHONE_1, COLUMN_HOME_PHONE_2, COLUMN_HOME_PHONE_3, COLUMN_HOME_PHONE_4,
    COLUMN_WORK_PHONE_1, COLUMN_WORK_PHONE_2, COLUMN_WORK_PHONE_3, COLUMN_WORK_PHONE_4,
    COLUMN_CELL_PHONE_1, COLUMN_CELL_PHONE_2, COLUMN_CELL_PHONE_3, COLUMN_CELL_PHONE_4,
    COLUMN_EMAIL_1, COLUMN_EMAIL_2, COLUMN_EMAIL_3, COLUMN_EMAIL_4,
    COLUMN_WWW_1, COLUMN_WWW_2, COLUMN_WWW_3, COLUMN_WWW_4,

    COLUMN_IM_GG, COLUMN_IM_YAHOO, COLUMN_IM_MSN, COLUMN_IM_ICQ, COLUMN_IM_AOL, COLUMN_IM_JABBER, COLUMN_IM_SKYPE, COLUMN_IM_TLEN,
    COLUMN_BLOG, COLUMN_PHOTO, COLUMN_INFO,
    CONTACTS_NUM_COLUMNS
};

void        gui_create_contacts                     (GtkWidget *notebook);
void        show_contacts_desc_panel                (gboolean enable);
void        insert_photo                            (gchar *photo_filename, GtkTextIter * iter);
void        set_export_active                       (void);
void        contacts_select_first_position_in_list  (void);

#endif /* _CONTACTS_H */


