
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
#include "calendar.h"
#include "i18n.h"
#include "options_prefs.h"


/*------------------------------------------------------------------------------*/

#define RUN_FLAG_FILE "pid"

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

GUI *appGUI = NULL;
CALENDAR *cal = NULL;
TASKS *tsk = NULL;
CONTACTS *cnt = NULL;
OPTIONS *opt = NULL;
GtkWidget *info_dialog;
gchar tmpbuf[BUFFER_SIZE];

    appGUI = g_new0 (GUI, 1);
    g_return_val_if_fail (appGUI != NULL, -1);

    cal = g_new0 (CALENDAR, 1);
    g_return_val_if_fail (cal != NULL, -1);
    tsk = g_new0 (TASKS, 1);
    g_return_val_if_fail (tsk != NULL, -1);
    cnt = g_new0 (CONTACTS, 1);
    g_return_val_if_fail (cnt != NULL, -1);

    opt = g_new0 (OPTIONS, 1);
    g_return_val_if_fail (opt != NULL, -1);

    /* register modules */
    appGUI->cal = cal;
    appGUI->tsk = tsk;
    appGUI->cnt = cnt;
    appGUI->opt = opt;

    /* default values */   
    appGUI->hovering_over_link = FALSE;
    appGUI->hand_cursor = NULL;
    appGUI->regular_cursor = NULL;
    appGUI->gui_url_tag = NULL;

    appGUI->cal->prev_day = appGUI->cal->prev_month = appGUI->cal->prev_year = 0;
    appGUI->calendar_only = FALSE;

    appGUI->tsk->tasks_panel_status = FALSE;
    appGUI->tsk->tasks_filter_disabled = FALSE;
    appGUI->tsk->tasks_list_store = NULL;

    appGUI->cnt->photo_image = NULL;
    appGUI->cnt->contacts_uim_widget = NULL;
    appGUI->cnt->contacts_panel_status = FALSE;
    appGUI->cnt->contacts_filter_disabled = TRUE;
    appGUI->cnt->export_button = NULL;
    appGUI->cnt->output_file_entry = NULL;

    appGUI->opt->options_counter = 0;

    if (argc != 1) {
        if (argc == 2 && !strncmp(argv[1], "-cal", 4) && strlen(argv[1]) == 4) {
            appGUI->calendar_only = TRUE;
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

        gui_create_window (appGUI);

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

    g_free(opt);
    g_free(cnt);
    g_free(tsk);
    g_free(cal);
    g_free(appGUI);
    return 0;
}

/*------------------------------------------------------------------------------*/

