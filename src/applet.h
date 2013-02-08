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


#define APPLET_SCHEMA "org.mate.panel.applet.LockkeysApplet"
