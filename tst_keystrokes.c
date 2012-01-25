#include <stdio.h>

#include "keystroke.h"
#include <X11/Xlib.h> // Not used here, but whatever

int main() {
	init_keystrokes();
	reg_keystroke(Mod_Super | Mod_Shift, XK_a);
	reg_keystroke(Mod_Control | Mod_Alt, XK_b);
	int keysym, state;
	//while (!get_keystroke(&state, &keysym)){}
	printf("Looking for (Super + Shift + a) or (Control + Alt + b)...\n");
	while(1){
		if (get_keystroke(&state, &keysym)) {
			if (keysym == XK_b) {
				printf("You killed me!\n");
				break;
			} else {
				printf("Got Keystroke!\n");
			}
		}
	}
	char *str = XKeysymToString(keysym);
	printf("%s\n", str);
	kill_keystrokes();
	return 0;
}
