
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
#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <config.h>

#include "gui.h"
#include "i18n.h"
#include "options_prefs.h"


#define         RUN_FLAG_FILE   "pid"

extern gchar    tmpbuf[];
gboolean        calendar_only = FALSE;

/*------------------------------------------------------------------------------*/

gboolean
check_if_app_is_running(void) {

FILE *fhandle;

    fhandle = fopen(prefs_get_config_filename (RUN_FLAG_FILE), "r");

    if (fhandle == NULL) {

        fhandle = fopen(prefs_get_config_filename (RUN_FLAG_FILE), "w");

        if (fhandle == NULL) {
            return TRUE;
        } else {
            fprintf(fhandle, "%d", getpid());
            fclose (fhandle);
            return FALSE;
        }

    } else {
        fclose (fhandle);
    }

    return TRUE;
}

/*------------------------------------------------------------------------------*/

int main(int argc, char **argv) {

GtkWidget *info_dialog;

    if (argc != 1) {
        if (argc == 2 && !strncmp(argv[1], "-cal", 4) && strlen(argv[1]) == 4) {
            calendar_only = TRUE;
        } else {
            fprintf(stderr, "OSMO v%s - %s\n", VERSION, _("a handy personal organizer"));
            fprintf(stderr, "Copyright (c) 2007 Tomasz Maka <pasp@users.sourceforge.net>\n\n");
            fprintf(stderr, "%s: %s [%s]\n\n", _("usage"), argv[0], _("options"));
            fprintf(stderr, "%s:\n", _("options"));
            fprintf(stderr, "    -cal : %s\n", _("show small calendar window"));
            exit (-1);
        }
    }

    prefs_read_config ();

    gtk_init (&argc, &argv);

    if(check_if_app_is_running() == FALSE) {

        gtk_set_locale();
        bindtextdomain(PACKAGE, LOCALEDIR);
        textdomain(PACKAGE);

        gui_create_window ();

        gtk_main ();

        prefs_write_config ();
        unlink(prefs_get_config_filename (RUN_FLAG_FILE));

    } else {

        sprintf(tmpbuf, _("Another copy of OSMO is already running or it has been stopped uncleanly (in this case please remove '%s' file and try again)."), prefs_get_config_filename (RUN_FLAG_FILE));

        info_dialog = gtk_message_dialog_new (GTK_WINDOW_TOPLEVEL, GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, tmpbuf);

        gtk_window_set_title(GTK_WINDOW(info_dialog), _("Error"));
        gtk_window_set_position(GTK_WINDOW(info_dialog), GTK_WIN_POS_CENTER);
        gtk_widget_show (info_dialog);
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
    }

    return 0;
}

/*------------------------------------------------------------------------------*/

