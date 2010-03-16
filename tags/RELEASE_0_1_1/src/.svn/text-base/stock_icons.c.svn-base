
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

#include <gtk/gtk.h>
#include <glib.h>

#include "i18n.h"
#include "stock_icons.h"
#include "stock_icons_data.h"

/*------------------------------------------------------------------------------*/

void
osmo_register_stock_icons(void) {

GdkPixbuf *pixbuf;
gint i;
GtkIconFactory *factory;
GtkIconSet *icon_set;
GtkIconSource *icon_source;

GtkStockItem calendar_stock_items[] = {

    { (gchar*) OSMO_STOCK_BUTTON_ADD,              (gchar*) N_("Add"),                         0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_CLEAR,            (gchar*) N_("Clear"),                       0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_CLOSE,            (gchar*) N_("Close"),                       0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_CANCEL,           (gchar*) N_("Cancel"),                      0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_JUMPTO,           (gchar*) N_("Jump to"),                     0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_CONTACTS_EXPORT,  (gchar*) N_("Export"),                      0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_CONTACTS_IMPORT,  (gchar*) N_("Import"),                      0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_INSERT_TIMELINE,  (gchar*) N_("Insert timeline"),             0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_NEXT_YEAR,        (gchar*) N_("Next year"),                   0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_NO,               (gchar*) N_("No"),                          0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_OK,               (gchar*) N_("OK"),                          0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_OPEN,             (gchar*) N_("Browse"),                      0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_PREV_YEAR,        (gchar*) N_("Previous year"),               0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_REMOVE,           (gchar*) N_("Remove"),                      0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_SELECT_DATE,      (gchar*) N_("Select date"),                 0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_SELECT_FONT,      (gchar*) N_("Select font"),                 0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_TODAY,            (gchar*) N_("Today"),                       0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_BUTTON_YES,              (gchar*) N_("Yes"),                         0, 0, TRANSLATION_DOMAIN },

    { (gchar*) OSMO_STOCK_ABOUT,                   (gchar*) N_("About"),                       0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CALENDAR,                (gchar*) N_("Calendar"),                    0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS_ADD,            (gchar*) N_("Add contact"),                 0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS_EDIT,           (gchar*) N_("Edit contact"),                0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS_EXPORT,         (gchar*) N_("Export contact"),              0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS_IMPORT,         (gchar*) N_("Import contact"),              0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS_REMOVE,         (gchar*) N_("Remove contact"),              0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_CONTACTS,                (gchar*) N_("Contacts"),                    0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_EDIT_NOTE,               (gchar*) N_("Edit day note"),               0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_FULLYEAR,                (gchar*) N_("Display full-year calendar"),  0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_HELP,                    (gchar*) N_("Help"),                        0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_JUMPTO,                  (gchar*) N_("Jump to date"),                0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_LICENSE,                 (gchar*) N_("License"),                     0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_NEXT_DAY,                (gchar*) N_("Next day"),                    0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_NEXT_MONTH,              (gchar*) N_("Next month"),                  0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_NEXT_YEAR,               (gchar*) N_("Next year"),                   0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_PREFERENCES,             (gchar*) N_("Preferences"),                 0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_PREV_DAY,                (gchar*) N_("Previous day"),                0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_PREV_MONTH,              (gchar*) N_("Previous month"),              0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_PREV_YEAR,               (gchar*) N_("Previous year"),               0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_TASKS_ADD,               (gchar*) N_("Add task"),                    0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_TASKS_EDIT,              (gchar*) N_("Edit task"),                   0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_TASKS_REMOVE,            (gchar*) N_("Remove task"),                 0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_TASKS,                   (gchar*) N_("Tasks"),                       0, 0, TRANSLATION_DOMAIN },
    { (gchar*) OSMO_STOCK_TODAY,                   (gchar*) N_("Today"),                       0, 0, TRANSLATION_DOMAIN },
};

const guint8* calendar_stock_item_data[] = {
 
    (const guint8*) osmo_stock_button_add,               
    (const guint8*) osmo_stock_button_clear,               
    (const guint8*) osmo_stock_button_close,       
    (const guint8*) osmo_stock_button_cancel,       
    (const guint8*) osmo_stock_button_jumpto,
    (const guint8*) osmo_stock_button_contacts_export,
    (const guint8*) osmo_stock_button_contacts_import,
    (const guint8*) osmo_stock_button_insert_timeline,     
    (const guint8*) osmo_stock_button_next_year,     
    (const guint8*) osmo_stock_button_no,     
    (const guint8*) osmo_stock_button_ok,     
    (const guint8*) osmo_stock_button_open,     
    (const guint8*) osmo_stock_button_prev_year,     
    (const guint8*) osmo_stock_button_remove,     
    (const guint8*) osmo_stock_button_select_date,     
    (const guint8*) osmo_stock_button_select_font,     
    (const guint8*) osmo_stock_button_today,     
    (const guint8*) osmo_stock_button_yes,     

    (const guint8*) osmo_stock_about,               
    (const guint8*) osmo_stock_calendar,    
    (const guint8*) osmo_stock_contacts_add,            
    (const guint8*) osmo_stock_contacts_edit,            
    (const guint8*) osmo_stock_contacts_export,            
    (const guint8*) osmo_stock_contacts_import,            
    (const guint8*) osmo_stock_contacts_remove,            
    (const guint8*) osmo_stock_contacts,            
    (const guint8*) osmo_stock_edit_note,
    (const guint8*) osmo_stock_fullyear,            
    (const guint8*) osmo_stock_help,        
    (const guint8*) osmo_stock_jumpto,
    (const guint8*) osmo_stock_license,             
    (const guint8*) osmo_stock_next_day,
    (const guint8*) osmo_stock_next_month,          
    (const guint8*) osmo_stock_next_year,
    (const guint8*) osmo_stock_preferences,         
    (const guint8*) osmo_stock_prev_day,
    (const guint8*) osmo_stock_prev_month,          
    (const guint8*) osmo_stock_prev_year,
    (const guint8*) osmo_stock_tasks_add,               
    (const guint8*) osmo_stock_tasks_edit,               
    (const guint8*) osmo_stock_tasks_remove,               
    (const guint8*) osmo_stock_tasks,               
    (const guint8*) osmo_stock_today,
};

    gtk_stock_add (calendar_stock_items, G_N_ELEMENTS (calendar_stock_items));

    factory = gtk_icon_factory_new ();
    gtk_icon_factory_add_default(factory);

    for(i = 0; i < G_N_ELEMENTS(calendar_stock_item_data); i++) {
        pixbuf = gdk_pixbuf_new_from_inline(-1, calendar_stock_item_data[i], FALSE, NULL);
        icon_source = gtk_icon_source_new ();
        gtk_icon_source_set_pixbuf (icon_source, pixbuf);

        icon_set = gtk_icon_set_new ();
        gtk_icon_set_add_source (icon_set, icon_source);

        gtk_icon_factory_add(factory, calendar_stock_items[i].stock_id, icon_set);
        g_object_unref(pixbuf);
        gtk_icon_source_free (icon_source);
        gtk_icon_set_unref (icon_set);
    }

    g_object_unref (factory);

}

/*------------------------------------------------------------------------------*/


