/*
 *  X11 Keystroke Grabber
 *  Copyright (c) 2011 Michael Rosenberg
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
*/

#ifndef KEYSTROKE_C
#define KEYSTROKE_C

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>

#define set_up_mod(mod, sym) if(!_set_up_mod(mod, sym##_L)){_set_up_mod(mod, sym##_R);}


Display *disp;

int Mod_Control;
int Mod_Shift;
int Mod_Alt;
int Mod_Super;

char keysymToChar(int keysym) {
	return XKeysymToString(keysym)[0];
}

static Bool check(Display *dpy, XEvent *event, XPointer x) {
	Display *dpyannoy = dpy;
	XEvent *eventannoy = event;
	XPointer xannoy = x;
	return True;
}

/*********************************************
 *     Checks if keysym is a modifier on     *
 *        the current keyboard layout        *
 *                                           *
 * Returns - 0 if keysym is not a modifier   *
 *                                           *
 *           modifier key (0-7) if keysym is *
 *           a modifier                      *
 *********************************************/
short is_modifier(unsigned int keysym) {
	XModifierKeymap *map = XGetModifierMapping(disp);
	int max = map->max_keypermod; // Probably 5
	short pos = 0;
	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < max; j++) {
			if (XKeycodeToKeysym(disp, map->modifiermap[(i*max) + j], 0) == keysym) {
				pos = i;
			}
		}
	}

	XFreeModifiermap(map);
	return pos;
}

static short _set_up_mod(int *modmask, int keysym) {
	short p = is_modifier(keysym);
	if (p) {
		*modmask = 1 << p;
		return 1;
	} else
		return 0;
}

void init_keystrokes() {
	disp = XOpenDisplay(0);
	Window root = XDefaultRootWindow(disp);
	XSelectInput(disp, root, KeyPressMask);
	/* Later */
		// Check if NumLock is on
		//XkbGetNamedIndicator(disp, NULL, NULL, NULL, &num_set, NULL, NULL);
		// Now find the NumLock modifier and add it to the mask
	
	Mod_Control = ControlMask;
	Mod_Shift 	= ShiftMask;
	set_up_mod(&Mod_Alt, XK_Alt);
	set_up_mod(&Mod_Super, XK_Super);
}

short reg_keystroke(int mods, int keysym) {
	Window root = XDefaultRootWindow(disp);
	XSelectInput(disp, root, KeyPressMask);

	if (is_modifier(keysym)) // Can't catch a keystroke only containing mods
		return 0;

	int code = XKeysymToKeycode(disp, keysym);
	XGrabKey(disp, code, mods, root, False, GrabModeAsync, GrabModeAsync);
	printf("Grabbing mods: %i\n", mods);
	return 1;
}

short get_keystroke(int *state, int *keysym) {
	XEvent event;

	// Asynchronous call to get event
	if (XCheckIfEvent(disp, &event, check, NULL) &&
		event.xkey.type == KeyPress) {
		if (state != NULL)
			*state = event.xkey.state;
		if (keysym != NULL)
			*keysym = XKeycodeToKeysym(disp, event.xkey.keycode, 0);

		return 1;
	}
	return 0;
}

// Nothing yet...
void kill_keystrokes() {}

#endif /*KEYSTROKE_C*/
