AM_CPPFLAGS = -DLOCALEDIR=\"$(datadir)/locale\"
bin_PROGRAMS = osmo
osmo_SOURCES = calendar.c calendar.h \
			   calendar_fullyear.c calendar_fullyear.h \
			   calendar_jumpto.c calendar_jumpto.h \
			   calendar_moon.h \
			   calendar_notes.c calendar_notes.h \
			   calendar_utils.c calendar_utils.h \
			   calendar_widget.c calendar_widget.h \
			   contacts.c contacts.h \
			   contacts_export.c contacts_export.h \
			   contacts_items.c contacts_items.h \
			   gui.c gui.h \
			   gui_icon.h gui_logo.h \
			   gui_utils.c gui_utils.h \
			   i18n.h \
			   main.c \
			   options_gui.c options_gui.h \
			   options_prefs.c options_prefs.h \
			   stock_icons.c stock_icons.h stock_icons_data.h \
			   tasks.c tasks.h \
			   tasks_items.c tasks_items.h

osmo_LDADD = $(LIBINTL)

