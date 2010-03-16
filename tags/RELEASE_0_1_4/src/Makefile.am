REVISION := $(shell if test -e .svn; then echo -DREV=\"`LC_ALL=C svn info | sed -n '/^Rev/p'| sed -e 's/^Revision:\ //'`\"; fi;)
AM_CPPFLAGS = -DLOCALEDIR=\"$(datadir)/locale\" $(REVISION)
bin_PROGRAMS = osmo
osmo_SOURCES = calendar.c calendar.h \
			   calendar_fullyear.c calendar_fullyear.h \
			   calendar_jumpto.c calendar_jumpto.h \
			   calendar_timeline.c calendar_timeline.h \
			   calendar_moon.h \
			   calendar_notes.c calendar_notes.h \
			   calendar_widget.c calendar_widget.h \
			   calendar_calc.c calendar_calc.h \
			   contacts.c contacts.h \
			   contacts_birthdays.c contacts_birthdays.h \
			   contacts_import.c contacts_import.h \
			   contacts_export.c contacts_export.h \
			   contacts_items.c contacts_items.h \
			   date_utils.c date_utils.h \
			   gui.c gui.h \
			   gui_icon.h gui_logo.h \
			   gui_utils.c gui_utils.h \
			   i18n.h \
			   main.c \
			   options_gui.c options_gui.h \
			   options_gui_calendar.c options_gui_calendar.h \
			   options_gui_contacts.c options_gui_contacts.h \
			   options_gui_general.c options_gui_general.h \
			   options_gui_tasks.c options_gui_tasks.h \
			   options_prefs.c options_prefs.h \
			   stock_icons.c stock_icons.h stock_icons_data.h \
			   tasks.c tasks.h \
			   tasks_items.c tasks_items.h \
			   time_utils.c time_utils.h

osmo_LDADD = $(LIBINTL)


