## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(KOLUMNZ_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = kolumnz

kolumnz_SOURCES = \
	main.c   \
	EnterNameDlg.c   \
	EnterNameDlg.h   \
	gameData.c   \
	gameData.h   \
	preferences.c   \
	preferences.h   \
	propertiesDlg.c   \
	propertiesDlg.h   \
	scoresDlg.c   \
	scoresDlg.h   \
	StackPackEngine.c   \
	StackPackEngine.h   \
	userData.c   \
	userData.h   \
	view.c   \
	view.h

kolumnz_LDFLAGS = 

kolumnz_LDADD = $(KOLUMNZ_LIBS)

