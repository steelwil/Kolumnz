## Process this file with automake to produce Makefile.in
## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

SUBDIRS = po src

stackpackdocdir = ${prefix}/doc/Kolumnz
stackpackdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO

EXTRA_DIST = $(stackpackdoc_DATA)

bin_PROGRAMS = \
	kolumnz

kolumnz_SOURCES = \
	src/gameData.c\
	src/main.c \
	src/EnterNameDlg.c \
	src/EnterNameDlg.h \
	src/gameData.h \
	src/KoulumnzEngine.c \
	src/KoulumnzEngine.h \
	src/preferences.c \
	src/propertiesDlg.c \
	src/propertiesDlg.h \
	src/scoresDlg.c \
	src/scoresDlg.h \
	src/userData.c \
	src/userData.h \
	src/view.c \
	src/view.h \
	src/preferences.h

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

