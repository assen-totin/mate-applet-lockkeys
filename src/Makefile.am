# Author: Assen Totin <assen.totin@gmail.com>
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

LOCKKEYS_FLAGS = @LOCKKEYS_FLAGS@
LOCKKEYS_LIBS = @LOCKKEYS_LIBS@

AM_CPPFLAGS = @GTK_CFLAGS@ @LOCKKEYS_FLAGS@
LDADD = @GTK_LIBS@ @LOCKKEYS_LIBS@

libexec_PROGRAMS = lockkeys_applet 

lockkeys_applet_SOURCES = main.c applet.h 

appletdir = $(datadir)/mate-panel/applets
applet_DATA = org.mate.applets.LockkeysApplet.mate-panel-applet

xmluidir = $(datadir)/mate/ui
xmlui_DATA = lockkeys-applet-menu.xml
EXTRA_DIST = lockkeys-applet-menu.xml

servicedir = $(datadir)/dbus-1/services
service_DATA = org.mate.panel.applet.LockkeysApplet.service

gsettingsdir = $(datadir)/glib-2.0/schemas
gsettings_DATA = org.mate.panel.applet.LockkeysApplet.gschema.xml

install-data-hook:
	glib-compile-schemas $(DESTDIR)$(datadir)/glib-2.0/schemas 2>/dev/null
