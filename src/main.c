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

#include <X11/XKBlib.h>
#include <gdk/gdkx.h>
#include <mate-panel-applet.h>
#include <libintl.h>
#include <string.h>

#include "../config.h"
#include "applet.h"

#define _(String) gettext (String)

/* Reorder the icons in the applet, according to the orient and the size 
 * of the panel
 * at the moment this seems ok...
 */
static void applet_reorder_icons(int size, int orient, LedApplet *applet) {
	GtkBox *box;
	int count = 0;
	
	g_assert(applet);

	/* how many buttons do we have?
	 * if TRUE == 1 then we could do this easier,
	 * but who knows...
	 */
	count += applet->show[CAPSLOCK] ? 1 : 0;
	count += applet->show[NUMLOCK] ? 1 : 0;
	count += applet->show[SCROLLLOCK] ? 1 : 0;
	
	g_assert(count > 0);
	
	/* Extract the pixmaps out of the boxes, in case they 
	 * have been put there before...
	 * they have to be ref'ed before doing so
	 */
	if (applet->num_pix->parent) {
		g_object_ref(G_OBJECT(applet->num_pix));
		gtk_container_remove(GTK_CONTAINER(applet->num_pix->parent), applet->num_pix);
	}
	if (applet->caps_pix->parent) {
		g_object_ref(G_OBJECT(applet->caps_pix));
		gtk_container_remove(GTK_CONTAINER(applet->caps_pix->parent), applet->caps_pix);
	}
	if (applet->scroll_pix->parent)	{
		g_object_ref(G_OBJECT(applet->scroll_pix));
		gtk_container_remove(GTK_CONTAINER(applet->scroll_pix->parent), applet->scroll_pix);
	}
	
	/* Do we put the pixmaps into the vbox or the hbox?
	 * this depends on size of the panel and how many pixmaps are shown
	 */
	if ((orient == MATE_PANEL_APPLET_ORIENT_UP) || (orient == MATE_PANEL_APPLET_ORIENT_DOWN)) 
		box = GTK_BOX(size < 48 ? applet->hbox : applet->vbox);
	else
		box = GTK_BOX(size < 48 ? applet->vbox : applet->hbox);
	
	if ((count < 3) || (size < 48))	{
		if (applet->show[CAPSLOCK])
			gtk_box_pack_start(box, applet->caps_pix, TRUE, TRUE, 0);
		if (applet->show[NUMLOCK])
			gtk_box_pack_start(box, applet->num_pix, TRUE, TRUE, 0);
		if (applet->show[SCROLLLOCK])
			gtk_box_pack_start(box, applet->scroll_pix, TRUE, TRUE, 0);
	}
	else {
		gtk_box_pack_start(GTK_BOX(applet->vbox), applet->caps_pix, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(applet->hbox), applet->num_pix, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(applet->hbox), applet->scroll_pix, TRUE, TRUE, 0);
	}
	
	gtk_widget_show_all(GTK_WIDGET(applet->applet));
}

/* panel has changed its size 
 */
static void applet_change_size(MatePanelApplet *applet_widget, int size, LedApplet *applet) {
	applet_reorder_icons(size, mate_panel_applet_get_orient(applet_widget), applet);
}

/* panel has changed its orientation
 */
static void applet_change_orient(MatePanelApplet *applet_widget, int orient, LedApplet *applet) {
	applet_reorder_icons(mate_panel_applet_get_size(applet_widget), orient, applet);
}

/* Set the icons according to the status of the keyboard leds
 */
static void change_icons(LedApplet *applet) {
	char image_file_num[1024], image_file_caps[1024], image_file_scroll[1024];

	if (applet->on[NUMLOCK])
		sprintf(&image_file_num[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_NUM_ON);
	else
		sprintf(&image_file_num[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_NUM_OFF);

	if (applet->on[CAPSLOCK])
		sprintf(&image_file_caps[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_CAPS_ON);
	else
		sprintf(&image_file_caps[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_CAPS_OFF);

	if (applet->on[SCROLLLOCK])
		sprintf(&image_file_scroll[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_SCROLL_ON);
	else
		sprintf(&image_file_scroll[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON_SCROLL_OFF);

	gtk_image_set_from_file(GTK_IMAGE(applet->num_pix), &image_file_num[0]);
	gtk_image_set_from_file(GTK_IMAGE(applet->caps_pix), &image_file_caps[0]);
	gtk_image_set_from_file(GTK_IMAGE(applet->scroll_pix), &image_file_scroll[0]);
}	

/* Free the memory of the applet struct
 * Save the current state of the applet
 */
static void applet_destroy(MatePanelApplet *applet_widget, LedApplet *applet) {
	g_assert(applet);
	g_free(applet);
	return;
}

/* Just a boring about box
 */

static void quitDialogOK( GtkWidget *widget, gpointer data ){
        GtkWidget *quitDialog = data;
        gtk_widget_destroy(quitDialog);
}


static void about_cb (GtkAction *action, LedApplet *applet) {
	char msg1[1024];

	sprintf(&msg1[0], "%s\n\n%s\n\n%s", _("Keyboard Lock Keys"), _("An applet that shows the state of your Capslock-, Numlock-, and Scroll-lock keys"), _("Assen Totin <assen.totin@gmail.com>"));

        GtkWidget *label = gtk_label_new (&msg1[0]);

        GtkWidget *quitDialog = gtk_dialog_new_with_buttons (_("Keyboard Lock Keys"), GTK_WINDOW(applet), GTK_DIALOG_MODAL, NULL);
        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);

        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), label);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

        gtk_widget_show_all (GTK_WIDGET(quitDialog));
}


/* Opens MATE help application
 */
/*
static void help_cb (GtkAction *action, LedApplet *applet) {
	GError *error = NULL;

	mate_help_display(PACKAGE_NAME, NULL, &error);
	if (error) {
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(NULL, 
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
				error->message);
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		g_error_free (error);
		error = NULL;
	}
}
*/

/* Called when one of the applet->show_cb checkbutton is toggled...
 */
static void show_cb_change_cb(GtkToggleButton *togglebutton, LedApplet *applet) {
	gboolean active;
	GtkWidget *others[2];
	
	active = gtk_toggle_button_get_active(togglebutton);

	if (togglebutton == GTK_TOGGLE_BUTTON(applet->show_cb[CAPSLOCK])) {
		others[0] = applet->show_cb[NUMLOCK];
		others[1] = applet->show_cb[SCROLLLOCK];
		applet->show[CAPSLOCK] = active;
	}	
	else if (togglebutton == GTK_TOGGLE_BUTTON(applet->show_cb[NUMLOCK])) {
		others[0] = applet->show_cb[CAPSLOCK];
		others[1] = applet->show_cb[SCROLLLOCK];
		applet->show[NUMLOCK] = active;
	}	
	else if (togglebutton == GTK_TOGGLE_BUTTON(applet->show_cb[SCROLLLOCK])) {
		others[0] = applet->show_cb[CAPSLOCK];
		others[1] = applet->show_cb[NUMLOCK];
		applet->show[SCROLLLOCK] = active;
	}
	else
		g_assert_not_reached();

	/* Shows hides the icons according to the new settings */
	applet_reorder_icons(mate_panel_applet_get_size(applet->applet), 
		mate_panel_applet_get_orient(applet->applet), applet);
	
	if (active) {
		gtk_widget_set_sensitive(others[0], TRUE);
		gtk_widget_set_sensitive(others[1], TRUE);
		return;
	}
	
	if (!active && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(others[0])))
		gtk_widget_set_sensitive(others[1], FALSE);
	
	if (!active && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(others[1])))
		gtk_widget_set_sensitive(others[0], FALSE);
}

static void settings_cb (GtkAction *action, LedApplet *applet) {
	GtkWidget *hbox, *vbox1, *vbox2, *header_lbl, *dummy_lbl;
	int i, sum = 0, lo = 0, answer;
	char* header_str;
	
	g_assert(applet);
	
	if (applet->settings) {
		gtk_window_present(GTK_WINDOW(applet->settings));
		return;
	}

	applet->settings = 
		GTK_DIALOG(gtk_dialog_new_with_buttons(_("Lock Keys Preferences"), 
		NULL, GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
		GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT, 
		//GTK_STOCK_HELP, GTK_RESPONSE_HELP,
		NULL));

	//gtk_container_set_border_width(GTK_CONTAINER(applet->settings), 12);
	//gtk_box_set_spacing(GTK_BOX(applet->settings->vbox), 18);
	//gtk_window_set_default_size(GTK_WINDOW(applet->settings), 240, -1);
	gtk_dialog_set_default_response(GTK_DIALOG(applet->settings), GTK_RESPONSE_ACCEPT);
	gtk_window_set_resizable(GTK_WINDOW(applet->settings), FALSE);
	
	vbox1 = gtk_vbox_new(FALSE, 6);
	gtk_container_set_border_width(GTK_CONTAINER(vbox1), 12);
	
	header_str = g_strconcat("<span weight=\"bold\">", _("Settings"), "</span>", NULL);
	header_lbl = gtk_label_new(header_str);
	gtk_label_set_use_markup(GTK_LABEL(header_lbl), TRUE);
	//gtk_label_set_justify(GTK_LABEL(header_lbl), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment(GTK_MISC(header_lbl), 0, 0.5);
	g_free(header_str);
	gtk_box_pack_start(GTK_BOX(vbox1), header_lbl, TRUE, TRUE, 0);
	
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox, TRUE, TRUE, 0);
	
	dummy_lbl = gtk_label_new("    ");
	vbox2 = gtk_vbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(hbox), dummy_lbl, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
	
	applet->show_cb[CAPSLOCK] = gtk_check_button_new_with_mnemonic(_("Show c_aps-lock"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(applet->show_cb[CAPSLOCK]), applet->show[CAPSLOCK]);
	applet->show_cb[NUMLOCK] = gtk_check_button_new_with_mnemonic(_("Show _num-lock"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(applet->show_cb[NUMLOCK]), applet->show[NUMLOCK]);
	applet->show_cb[SCROLLLOCK] = gtk_check_button_new_with_mnemonic(_("Show _scroll-lock"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(applet->show_cb[SCROLLLOCK]), applet->show[SCROLLLOCK]);
	
	for (i=0;i<3;i++)
		if (applet->show[i]) {
			sum++;
			lo = i;
		}
	
	if (sum < 2)
		gtk_widget_set_sensitive(applet->show_cb[lo], FALSE);
		
	gtk_box_pack_start(GTK_BOX(vbox2), applet->show_cb[CAPSLOCK], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), applet->show_cb[NUMLOCK], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), applet->show_cb[SCROLLLOCK], TRUE, TRUE, 0);
	
	gtk_widget_show_all(vbox1);
	gtk_container_add(GTK_CONTAINER(applet->settings->vbox), vbox1); 
	
	g_signal_connect(G_OBJECT(applet->show_cb[CAPSLOCK]), "toggled", G_CALLBACK(show_cb_change_cb), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet->show_cb[NUMLOCK]), "toggled", G_CALLBACK(show_cb_change_cb), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet->show_cb[SCROLLLOCK]), "toggled", G_CALLBACK(show_cb_change_cb), (gpointer)applet);

	answer = GTK_RESPONSE_HELP;
	while (answer == GTK_RESPONSE_HELP)	{
		answer = gtk_dialog_run(applet->settings);
		
//		if (answer == GTK_RESPONSE_HELP) 
//			help_cb(NULL, NULL);
//		else {
#ifdef HAVE_GSETTINGS
			g_settings_set_boolean (applet->gsettings, "capslock-show", applet->show[CAPSLOCK]);
			g_settings_set_boolean (applet->gsettings, "numlock-show", applet->show[NUMLOCK]);
			g_settings_set_boolean (applet->gsettings, "scrolllock-show", applet->show[SCROLLLOCK]);
#else
			mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "capslock_show", applet->show[CAPSLOCK], NULL);
			mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "numlock_show", applet->show[NUMLOCK], NULL);
			mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "scrolllock_show", applet->show[SCROLLLOCK], NULL);
#endif

			gtk_widget_destroy(GTK_WIDGET(applet->settings));
			applet->settings = NULL;
			break;
//		}
	}
}

/* Get the mask used to set the keyboard leds
 * I "stole" this code from kkeyled
 * the thing with the mask of the CAPS Lock key is 
 * really ugly, but it seems to work
 */
static unsigned int get_lockmask(LedApplet *applet) {
	int i;
	unsigned int mask = 0;
	XkbDescPtr xkb;

	g_assert(applet);
	
	if (!(xkb = XkbGetKeyboard(applet->rootwin, XkbAllComponentsMask, XkbUseCoreKbd)))
		return 0;
	
	if (!xkb->names)
		return 0;
    
	for(i = 0;i <= XkbNumVirtualMods;i++) {                                         
		unsigned int tmp = 0;
		char* name = NULL;
		if (!xkb->names->vmods[i])
			continue;

		name = XGetAtomName(xkb->dpy, xkb->names->vmods[i]);
		if (name) {
			if (!strcmp(name, "Caps Lock") && applet->on[CAPSLOCK]) {
				XkbVirtualModsToReal(xkb, 1 << i, &tmp);
				if (!tmp)
					tmp = 2; /* Hack to make it work. Maybe a bug in xkb? */
			}
			if (!strcmp(name, "NumLock") && applet->on[NUMLOCK])
				XkbVirtualModsToReal(xkb, 1 << i, &tmp);
			if (!strcmp(name, "ScrollLock") && applet->on[SCROLLLOCK])
				XkbVirtualModsToReal(xkb, 1 << i, &tmp);
			mask += tmp;
		}
	}
	XkbFreeKeyboard(xkb, 0, True);
	return mask;
}


/* Retrieve the modifier lockmask and then set the status of the leds
 * according to applet->on[]
 */
void set_ledstates(LedApplet *applet) {
	unsigned int mask = 0;
	
	g_assert(applet);
	
	mask = get_lockmask(applet);
	
	XkbLockModifiers(applet->rootwin, XkbUseCoreKbd, mask, mask);
}

/* If the state of the leds has changed,
 * then the icons are set to the new state,
 * the gconf settings are updated and so is
 * the tooltip.
 */
void ledstates_changed(LedApplet *applet, unsigned int state) {
	int i;
	char *buf, on[32], off[32];
	
	for (i=0; i<3; i++) 	{
		if (state & (1 << i))
			applet->on[i] = 1;
		else
			applet->on[i] = 0;
	}

	change_icons(applet);

#ifdef HAVE_GSETTINGS
        g_settings_set_boolean (applet->gsettings, "capslock-state", applet->on[CAPSLOCK]);
        g_settings_set_boolean (applet->gsettings, "numlock-state", applet->on[NUMLOCK]);
        g_settings_set_boolean (applet->gsettings, "scrolllock-state", applet->on[SCROLLLOCK]);
#else
	mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "capslock_state", applet->on[CAPSLOCK], NULL);
	mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "numlock_state", applet->on[NUMLOCK], NULL);
	mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "scrolllock_state", applet->on[SCROLLLOCK], NULL);
#endif

	strncpy(&on[0], _("On"), sizeof(on));
	strncpy(&off[0], _("Off"), sizeof (off));

	buf = g_strdup_printf(_("Caps: %s Num: %s Scroll: %s"), applet->on[CAPSLOCK] ? &on[0] : &off[0], 
		applet->on[NUMLOCK] ? &on[0] : &off[0], applet->on[SCROLLLOCK] ? &on[0] : &off[0]);

	gtk_widget_set_tooltip_text (GTK_WIDGET (applet->applet), &buf[0]);
	g_free(buf);
}


/* Get the current state of the leds
 * call ledstates_changed();
 */
void get_ledstates(LedApplet *applet) {
	unsigned int state = 0;

	XkbGetIndicatorState(applet->rootwin, XkbUseCoreKbd, &state);

	ledstates_changed(applet, state);
}	

GdkFilterReturn event_filter(GdkXEvent *gdkxevent, GdkEvent *event, LedApplet* applet) {
	XkbEvent ev;
	memcpy(&ev.core, gdkxevent, sizeof(ev.core));
	
	if (ev.core.type == applet->xkbev + XkbEventCode) {
		if (ev.any.xkb_type == XkbIndicatorStateNotify)
			ledstates_changed(applet, ev.indicators.state);
	}
	
	return GDK_FILTER_CONTINUE;
}

/* Just check if it's possible to use the xkb extension
 */
static gboolean init_xkb_extension(LedApplet *applet) {
    int code;
    int maj = XkbMajorVersion;
    int min = XkbMinorVersion;
    
	if (!XkbLibraryVersion(&maj, &min))
		return FALSE;
	if (!XkbQueryExtension(applet->rootwin, &code, &applet->xkbev, &applet->xkberr, &maj, &min))
		return FALSE;
	return TRUE;
}

static const GtkActionEntry applet_menu_actions [] = {
	//{ "Settings", GTK_STOCK_PROPERTIES, N_("_Settings"), NULL, NULL, G_CALLBACK (settings_cb) },
	//{ "Help", GTK_STOCK_HELP, N_("_Help"), NULL, NULL, G_CALLBACK (help_cb) },
	//{ "About", GTK_STOCK_ABOUT, N_("_About"), NULL, NULL, G_CALLBACK (about_cb) }
	{ "Settings", GTK_STOCK_PROPERTIES, "_Settings", NULL, NULL, G_CALLBACK (settings_cb) },
	//{ "Help", GTK_STOCK_HELP, "_Help", NULL, NULL, G_CALLBACK (help_cb) },
	{ "About", GTK_STOCK_ABOUT, NULL, "_About", NULL, G_CALLBACK (about_cb) }
};


static void applet_back_change (MatePanelApplet *a, MatePanelAppletBackgroundType type, GdkColor *color, GdkPixmap *pixmap, LedApplet *applet) {
        /* taken from the TrashApplet */
        GtkRcStyle *rc_style;
        GtkStyle *style;

        /* reset style */
        gtk_widget_set_style (GTK_WIDGET (applet->applet), NULL);
        rc_style = gtk_rc_style_new ();
        gtk_widget_modify_style (GTK_WIDGET (applet->applet), rc_style);
        g_object_unref (rc_style);

        switch (type) {
                case PANEL_COLOR_BACKGROUND:
                        gtk_widget_modify_bg (GTK_WIDGET (applet->applet), GTK_STATE_NORMAL, color);
                        break;

                case PANEL_PIXMAP_BACKGROUND:
                        style = gtk_style_copy (gtk_widget_get_style (GTK_WIDGET (applet->applet)));
                        if (style->bg_pixmap[GTK_STATE_NORMAL])
                                g_object_unref (style->bg_pixmap[GTK_STATE_NORMAL]);
                        style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
                        gtk_widget_set_style (GTK_WIDGET (applet->applet), style);
                        g_object_unref (style);
                        break;

                case PANEL_NO_BACKGROUND:
                default:
                        break;
        }

}

/* The "main" function
 */
static gboolean led_applet_factory(MatePanelApplet *applet_widget, const gchar *iid, gpointer data) {
	GdkDrawable *drawable;
	char *buf;
	LedApplet *applet;
	GdkPixbuf *icon;
	GList *iconlist = NULL;

	if (strcmp (iid, APPLET_ID) != 0) 
		return FALSE;

	// i18n
        setlocale (LC_ALL, "");
        bindtextdomain (PACKAGE_NAME, LOCALEDIR);
        bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
        textdomain (PACKAGE_NAME);

	/* Set an icon for all windows */
	char image_file[1024];
        sprintf(&image_file[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON);
	icon = gdk_pixbuf_new_from_file(&image_file[0], NULL);
	iconlist = g_list_append(NULL, (gpointer)icon);
	gtk_window_set_default_icon_list(iconlist);
	g_object_unref(icon);
	
	applet = g_malloc0(sizeof(LedApplet));
	applet->applet = applet_widget;
	
	/* I use the root window as display... this is probably
	 * not necessary, but it does not seem to cause problems
	 * either 
	 */

	drawable = gdk_get_default_root_window();
	g_assert(drawable);
	applet->rootwin = gdk_x11_drawable_get_xdisplay(drawable);
	
	applet->vbox = gtk_vbox_new(FALSE, 0);
	applet->hbox = gtk_hbox_new(FALSE, 0);

	applet->num_pix = gtk_image_new();
	applet->caps_pix = gtk_image_new();
	applet->scroll_pix = gtk_image_new();

	if (!init_xkb_extension(applet)) {
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(NULL, 
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
				_("Could not initialize X Keyboard Extension."));
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return FALSE;
	}
		
	/* Only set the state of the leds if we already have entries in gconf
	 * otherwise the applet would always turn off all leds when executed
	 * manually
	 */

        applet->show[CAPSLOCK] = TRUE;
        applet->show[NUMLOCK] = TRUE;
        applet->show[SCROLLLOCK] = TRUE;

#ifdef HAVE_GSETTINGS
	//applet->gsettings = mate_panel_applet_settings_new (MATE_PANEL_APPLET(applet->applet), APPLET_SCHEMA);
	applet->gsettings = g_settings_new_with_path(APPLET_SCHEMA, SCHEMA_PATH);
	applet->on[CAPSLOCK] =  g_settings_get_boolean (applet->gsettings, "capslock-state");
	applet->on[NUMLOCK] =  g_settings_get_boolean (applet->gsettings, "numlock-state");
	applet->on[SCROLLLOCK] =  g_settings_get_boolean (applet->gsettings, "scrolllock-state");
        applet->show[CAPSLOCK] =  g_settings_get_boolean (applet->gsettings, "capslock-show");
        applet->show[NUMLOCK] =  g_settings_get_boolean (applet->gsettings, "numlock-show");
        applet->show[SCROLLLOCK] =  g_settings_get_boolean (applet->gsettings, "scrolllock-show");
        if (!applet->show[CAPSLOCK] && !applet->show[NUMLOCK] && !applet->show[SCROLLLOCK])
	        applet->show[CAPSLOCK] = TRUE;

        //set_ledstates(applet);
#else
	if (mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "have_settings", NULL)) {	
		applet->on[CAPSLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "capslock_state", NULL);
		applet->on[NUMLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "numlock_state", NULL);
		applet->on[SCROLLLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "scrolllock_state", NULL);
		applet->show[CAPSLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "capslock_show", NULL);
		applet->show[NUMLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "numlock_show", NULL);
		applet->show[SCROLLLOCK] = mate_panel_applet_mateconf_get_bool(MATE_PANEL_APPLET(applet->applet), "scrolllock_show", NULL);
		if (!applet->show[CAPSLOCK] && !applet->show[NUMLOCK] && !applet->show[SCROLLLOCK])
			applet->show[CAPSLOCK] = TRUE;
		//set_ledstates(applet);
	}
	mate_panel_applet_mateconf_set_bool(MATE_PANEL_APPLET(applet->applet), "have_settings", TRUE, NULL);
#endif

	change_icons(applet);
	get_ledstates(applet);

	gtk_box_pack_end(GTK_BOX(applet->vbox), applet->hbox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(applet->hbox), applet->caps_pix, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(applet->hbox), applet->num_pix, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(applet->hbox), applet->scroll_pix, TRUE, TRUE, 0);
	
	/* the applet doesnt get a change_size/change_orient signal on startup
	 * so it has to figure these two out manually
	 */

	applet_reorder_icons(mate_panel_applet_get_size(applet_widget), mate_panel_applet_get_orient(applet_widget), applet);
	
	gtk_container_add(GTK_CONTAINER(applet_widget), applet->vbox);

        GtkActionGroup *action_group = gtk_action_group_new ("Lockkeys Applet Actions");
        //gtk_action_group_set_translation_domain (action_group, GETTEXT_PACKAGE);
        gtk_action_group_add_actions (action_group, applet_menu_actions, G_N_ELEMENTS (applet_menu_actions), applet);
	mate_panel_applet_setup_menu_from_file(applet->applet, "/usr/share/mate-2.0/ui/lockkeys-applet-menu.xml", action_group);

	gtk_widget_show_all(GTK_WIDGET(applet_widget));

	g_signal_connect(G_OBJECT(applet_widget), "change_size", G_CALLBACK(applet_change_size), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet_widget), "change_orient", G_CALLBACK(applet_change_orient), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet_widget), "destroy", G_CALLBACK(applet_destroy), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet_widget), "change_background", G_CALLBACK (applet_back_change), (gpointer)applet);

	if (!XkbSelectEvents(applet->rootwin, XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask))
		return FALSE;
	
	gdk_window_add_filter(NULL, (GdkFilterFunc)event_filter, applet);

	return TRUE;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY (APPLET_FACTORY, PANEL_TYPE_APPLET, APPLET_NAME, led_applet_factory, NULL)



