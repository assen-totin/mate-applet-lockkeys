# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.66])
AC_INIT([mate-applet-lockkeys], [0.3.0], [assen.totin@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AM_CONFIG_HEADER([config.h])

# prefix
if test "x$prefix" = "xNONE" ; then
	prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
	exec_prefix=$prefix
fi
PREFIX=$prefix
AC_SUBST([PREFIX])
BINDIR=$exec_prefix/bin
AC_SUBST([BINDIR])
SBINDIR=$exec_prefix/sbin
AC_SUBST([SBINDIR])
DATAROOTDIR=$prefix/share
AC_SUBST([DATAROOTDIR])
LIBEXECDIR=$prefix/libexec
if test "$libexecdir" != '${exec_prefix}/libexec'; then
        LIBEXECDIR=$libexecdir
fi
AC_SUBST([LIBEXECDIR])
PIXMAPDIR=$prefix/share/pixmaps
AC_SUBST([PIXMAPDIR])
ICONSDIR=$prefix/share/icons
AC_SUBST([ICONSDIR])
LOCALEDIR=$prefix/share/locale
AC_SUBST([LOCALEDIR])
AC_DEFINE_UNQUOTED([LOCALEDIR],["$LOCALEDIR"],[Locale files])
TOPLEVELDIR=`pwd`
AC_SUBST([TOPLEVELDIR])

# gettext
AM_GNU_GETTEXT_VERSION(0.18.1)
AM_GNU_GETTEXT([external])
if test $USE_NLS != "yes" ; then
        echo "*** gettext not found. You're losing internationalization."
fi

### Checks for programs

# GCC
AC_PROG_CC
MAKEFILE_CC=`which gcc`
AC_SUBST([MAKEFILE_CC])

# msgfmt - for NSL
AC_CHECK_PROG([FOUND_MSGFMT],[msgfmt],["yes"],["no"])
if test $FOUND_MSGFMT = "no"; then
	echo "*** msgfmt not found. Cannot continue."
	exit
fi

# glib-compile-schemas - for GSettings
PRINT_SETTINGS_BACKEND="GConf"
FORCE_SETTINGS="no"
AC_CHECK_PROG([FOUND_GCS],[glib-compile-schemas],["yes"],["no"])
AC_ARG_ENABLE([settings],[AS_HELP_STRING([--enable-settings],[settings backend to use, gsettings or gconf (default: check GSettings, if missing, use GConf))])],[FORCE_SETTINGS=$enableval])
if test $FORCE_SETTINGS = "gsettings" ; then
	if test $FOUND_GCS = "yes" ; then
        	AC_DEFINE([HAVE_GSETTINGS],[1],[Define if we have GSettings])
		PRINT_SETTINGS_BACKEND="GSettings"
	else
		AC_MSG_ERROR(["*** GSettings forced, but not found."])
	fi
elif test $FORCE_SETTINGS = "gconf" ; then
	# Do nothing, this is the fall-back
	EMPTY="1"
else
	if test $FOUND_GCS = "yes" ; then
		AC_DEFINE([HAVE_GSETTINGS],[1],[Define if we have GSettings])
		PRINT_SETTINGS_BACKEND="GSettings"
	fi
fi


### Checks for libraries.
LIBMATEPANELAPPLET="no"
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_4], [libmatepanelapplet-4.0], [LIBMATEPANELAPPLET="libmatepanelapplet-4.0"], [
        PKG_CHECK_MODULES([LIBMATEPANELAPPLET_3], [libmatepanelapplet-3.0], [LIBMATEPANELAPPLET="libmatepanelapplet-3.0"], [
                PKG_CHECK_MODULES([LIBMATEPANELAPPLET_2], [libmatepanelapplet-2.0], [LIBMATEPANELAPPLET="libmatepanelapplet-2.0"], [
                        AC_MSG_ERROR(["*** libmatepanelapplet not found."])
                ])
        ])
])

### Check for GLib
PKG_CHECK_MODULES([GLIB], [glib-2.0])

# Check for GTK and determine version
FORCE_GTK="no"
RPM_REQUIRE_GTK=" "
USE_GTK="0"
AC_ARG_ENABLE([gtk],[AS_HELP_STRING([--enable-gtk],[compile only for the specified GTK version, 2 or 3 (default: check both, use GTK-3, if not found, use GTK-2))])],[FORCE_GTK=$enableval])

PKG_CHECK_MODULES([PKG_GTK2], [gtk+-2.0], [FOUND_GTK2="1"], [FOUND_GTK2="0"])
PKG_CHECK_MODULES([PKG_GTK3], [gtk+-3.0], [FOUND_GTK3="1"], [FOUND_GTK3="0"])

if test $FORCE_GTK = "3" ; then
	if test $FOUND_GTK3 = "1" ; then
		AM_PATH_GTK_3_0([3.0.0], [USE_GTK="3"], [AC_MSG_ERROR(["*** GTK3 forced, but not found."])])
	else
		AC_MSG_ERROR(["*** GTK3 forced, but not found."])
	fi
elif test $FORCE_GTK = "2" ; then
	if test $FOUND_GTK2 = "1" ; then
		AM_PATH_GTK_2_0([2.24.0], [USE_GTK="2"], [AC_MSG_ERROR(["*** GTK2 forced, but not found."])])
	else
		AC_MSG_ERROR(["*** GTK2 forced, but not found."])
	fi
else
	# Try to match the GTK version used by mate-panel.
	PANEL_GTK=`$PKG_CONFIG --print-requires "$LIBMATEPANELAPPLET" 2>/dev/null |
		grep -m 1 '^gtk+-'`
	case "$PANEL_GTK" in
	gtk+-3*)
		AM_PATH_GTK_3_0([3.14.0], [USE_GTK="3"], [AC_MSG_ERROR(["*** GTK3 used by mate-panel, but no suitable version found."])]);;
	gtk+-2*)
		AM_PATH_GTK_2_0([2.24.0], [USE_GTK="2"], [AC_MSG_ERROR(["*** GTK2 used by mate-panel, but no suitable version found."])]);;
	esac

	# Only accept GTK3 if at least 3.14; else resort to GTK2
	if test $FOUND_GTK3 = "1" -a $USE_GTK = "0"; then
		AM_PATH_GTK_3_0([3.14.0], [USE_GTK="3"], [USE_GTK="0"])
	fi
	if test $FOUND_GTK2 = "1" -a $USE_GTK = "0" ; then
		AM_PATH_GTK_2_0([2.24.0], [USE_GTK="2"], [USE_GTK="0"])
	fi
fi

if test $USE_GTK = "3" ; then
	AC_DEFINE([HAVE_GTK3], [1], ["Define GTK3"])
	AC_DEFINE([GETTEXT_PACKAGE], ["gtk30"], ["GTK gettext"])
	RPM_REQUIRE_GTK="Requires: gtk3 >= 3.14.0"
	RPM_BUILDREQUIRE_GTK="Requires: gtk3 >= 3.14.0"
	SELECTED_GTK="GTK-3"
elif test $USE_GTK = "2" ; then
	AC_DEFINE([HAVE_GTK2], [1], ["Define GTK2"])
	AC_DEFINE([GETTEXT_PACKAGE], ["gtk20"], ["GTK gettext"])
	RPM_REQUIRE_GTK="Requires: gtk2 >= 2.24.0"
	RPM_BUILDREQUIRE_GTK="Requires: gtk2-devel >= 2.24.0"
	SELECTED_GTK="GTK-2"
else
	AC_MSG_ERROR(["*** GTK not found."])
fi

AC_SUBST([RPM_REQUIRE_GTK])
AC_SUBST([RPM_BUILDREQUIRE_GTK])

# Makefiles
LOCKKEYS_FLAGS=`$PKG_CONFIG --cflags-only-I $LIBMATEPANELAPPLET`
LOCKKEYS_LIBS=`$PKG_CONFIG --libs $LIBMATEPANELAPPLET`
LOCKKEYS_LIBS="$LOCKKEYS_LIBS -lX11"
AC_SUBST([LOCKKEYS_FLAGS])
AC_SUBST([LOCKKEYS_LIBS])

AC_CONFIG_FILES([Makefile src/Makefile images/Makefile po/Makefile.in po/Makefile])

# Service files
AC_CONFIG_FILES([src/org.mate.applets.LockkeysApplet.mate-panel-applet src/org.mate.panel.applet.LockkeysApplet.service mate-applet-lockkeys.spec src/org.mate.panel.applet.LockkeysApplet.gschema.xml])

# Icons
AC_DEFINE_UNQUOTED([APPLET_ICON_PATH], ["$PIXMAPDIR"], [Define icons location])

# Grand finale
AC_OUTPUT

echo
echo "Configuration complete."
echo 
echo "* Selected GTK version: $SELECTED_GTK"
echo "* Selected settings backend: $PRINT_SETTINGS_BACKEND"
echo
echo "Now, run make."
echo 
