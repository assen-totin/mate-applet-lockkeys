/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 *  USA.
 *
 *  Lock Keys (formerly Led) Applet was writen by Jörgen Scheibengruber <mfcn@gmx.de>
 *  Modifed for MATE by Assen Totin <assen.totin@gmail.com>
 *  
 */

#define APPLET_FACTORY "LockkeysAppletFactory"
#define APPLET_ID "LockkeysApplet"
#define APPLET_NAME "lockkeys"
#define APPLET_ICON "applet_lockkeys.png"
#define APPLET_ICON_CAPS_OFF "applet_lockkeys_capslock_off.xpm"
#define APPLET_ICON_CAPS_ON "applet_lockkeys_capslock_on.xpm"
#define APPLET_ICON_NUM_OFF "applet_lockkeys_numlock_off.xpm"
#define APPLET_ICON_NUM_ON "applet_lockkeys_numlock_on.xpm"
#define APPLET_ICON_SCROLL_OFF "applet_lockkeys_scrolllock_off.xpm"
#define APPLET_ICON_SCROLL_ON "applet_lockkeys_scrolllock_on.xpm"
#define APPLET_VERSION "1"

enum {
        CAPSLOCK  = 0,
        NUMLOCK,
        SCROLLLOCK
} Leds;

typedef struct {
        MatePanelApplet *applet;
        GtkWidget *vbox, *hbox;
        GtkWidget *num_pix, *scroll_pix, *caps_pix;
        GtkWidget *show_cb[3];
        Display *rootwin;
        int xkbev, xkberr;

        gboolean on[3];
        gboolean show[3];

        GtkDialog *about, *settings;
#ifdef HAVE_GSETTINGS
	GSettings *gsettings;
#endif
} LedApplet;


#define SCHEMA_PATH "/org/mate/panel/objects/lockkeys/"
#define APPLET_SCHEMA "org.mate.panel.applet.LockkeysApplet"
