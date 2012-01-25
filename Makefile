all:
	gcc tst_keystrokes.c -lX11 -o keystroke

clean keystroke:
	rm keystroke
