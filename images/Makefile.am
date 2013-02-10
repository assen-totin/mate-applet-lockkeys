# Author: Assen Totin <assen.totin@gmail.com>
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = applet_lockkeys_capslock_off.xpm applet_lockkeys_numlock_on.xpm applet_lockkeys_scrolllock_on.xpm applet_lockkeys_numlock_off.xpm  applet_lockkeys_scrolllock_off.xpm applet_lockkeys_capslock_on.xpm
EXTRA_DIST = $(pixmap_DATA)

icondir = $(datadir)/icons
icon_DATA = applet_lockkeys.png
EXTRA_DIST += $(icon_DATA)

icons = applet_lockkeys.16.png applet_lockkeys.22.png applet_lockkeys.24.png applet_lockkeys.32.png
EXTRA_DIST += $(icons)

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons

install-data-local:
	for icon in $(icons); do \
		SIZE=`echo $$icon | cut -d. -f2`; \
		FILE=`echo $$icon | cut -d. -f1,3`; \
		if [ "$$SIZE" = "svg" ]; then \
			SIZE="scalable"; \
			FILE="$$FILE.svg"; \
		else \
			SIZE="$${SIZE}x$${SIZE}"; \
		fi; \
		mkdir -p $(DESTDIR)$(datadir)/icons/hicolor/$$SIZE/apps/; \
		$(INSTALL_DATA) $(srcdir)/$$icon $(DESTDIR)$(datadir)/icons/hicolor/$$SIZE/apps/$$FILE; \
	done
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

