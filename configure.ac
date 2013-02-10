# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.66])
AC_INIT([mate-applet-lockkeys], [0.1.5], [assen.totin@gmail.com])

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

# Checks for programs
AC_PROG_CC
MAKEFILE_CC=`which gcc`
AC_SUBST([MAKEFILE_CC])

AC_CHECK_PROG([FOUND_MSGFMT],[msgfmt],["yes"],["no"])
if test $FOUND_MSGFMT = "no"; then
	echo "*** msgfmt not found. Cannot continue."
	exit
else
	MSGFMT=`which msgfmt`
	AC_SUBST([MSGFMT])
fi

# Checks for libraries.

# Check for packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
#PKG_CHECK_MODULES([LIBMATE], [libmate-2.0])
HAVE_GSETTINGS=0
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_4], [libmatepanelapplet-4.0], [FOUND_LMPA_4="yes"], [FOUND_LMPA_4="no"])
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_3], [libmatepanelapplet-3.0], [FOUND_LMPA_3="yes"], [FOUND_LMPA_3="no"])
PKG_CHECK_MODULES([LIBMATEPANELAPPLET_2], [libmatepanelapplet-2.0], [FOUND_LMPA_2="yes"], [FOUND_LMPA_2="no"])
if test $FOUND_LMPA_4 = "yes" ; then
        LIBMATEPANELAPPLET="libmatepanelapplet-4.0"
	HAVE_GSETTINGS=1
elif test $FOUND_LMPA_3 = "yes" ; then
        LIBMATEPANELAPPLET="libmatepanelapplet-3.0"
elif test $FOUND_LMPA_2 = "yes" ; then
        LIBMATEPANELAPPLET="libmatepanelapplet-2.0"
else
        AC_MSG_ERROR(["*** libmatepanelapplet not found."])
fi
AC_DEFINE([HAVE_GSETTINGS],[1],[Define if we have GSettings])

LOCKKEYS_FLAGS=`$PKG_CONFIG --cflags-only-I $LIBMATEPANELAPPLET`
LOCKKEYS_LIBS=`$PKG_CONFIG --libs $LIBMATEPANELAPPLET`
LOCKKEYS_LIBS="$LOCKKEYS_LIBS -lX11"
AC_SUBST([LOCKKEYS_FLAGS])
AC_SUBST([LOCKKEYS_LIBS])

# Makefiles
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
echo "Now, run make."
echo 
