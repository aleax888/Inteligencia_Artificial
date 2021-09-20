#include "glut.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sstream>
using namespace std;

/* colores
1.0 0.0 0.0 red, duh
0.0 1.0 0.0 green, duh
0.0 0.0 1.0 blue, duh
1.0 1.0 0.0 yellow
1.0 0.0 1.0 purple
0.0 1.0 1.0 cyan
1.0 1.0 1.0 white
1.0 0.5 0.0 orange
0.5 1.0 0.0 greenish yellow
…
0.5f, 0.35f, 0.05f brown
0.5 1.0 0.5 light green
0.0 0.5 0.0 dark green
*/

std::string to_stringg(int val) {
	std::stringstream ss;
	ss << val;
	return ss.str();
}
struct Mouse { //To change begin
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

typedef struct Mouse Mouse;

Mouse TheMouse = { 0,0,0,0,0 };

int winw = 600;
int winh = 600;

typedef void (*ButtonCallback)();
struct Button {
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
	int id;								/* A unique ID to represent this Button */

	struct Button* next;
};
typedef struct Button Button;

Button* pButtonList = NULL;
int GlobalRef = 0;

void Font(void* font, char* text, int x, int y) {
	glRasterPos2i(x, y);
	while (*text != '\0') {
		glutBitmapCharacter(font, *text);
		++text;
	}
}
int ButtonClickTest(Button* b, int x, int y) {
	if (b) {
		if (x > b->x && x < b->x + b->w && y > b->y && y < b->y + b->h) 
			return 1;
	}
	return 0;
}
void ButtonRelease(int x, int y) {
	Button* b = pButtonList;
	while (b) {
		if (ButtonClickTest(b, TheMouse.xpress, TheMouse.ypress) && ButtonClickTest(b, x, y)) {
			if (b->callbackFunction) b->callbackFunction();
		}
		b->state = 0;
		b = b->next;
	}
}
void ButtonPress(int x, int y) {
	Button* b = pButtonList;
	while (b) {
		if (ButtonClickTest(b, x, y)) b->state = 1;
		b = b->next;
	}
}
void ButtonPassive(int x, int y) {
	int needRedraw = 0;
	Button* b = pButtonList;
	while (b) {
		if (ButtonClickTest(b, x, y)) {
			if (b->highlighted == 0) {
				b->highlighted = 1;
				needRedraw = 1;
			}
		}
		else {
			if (b->highlighted == 1) {
				b->highlighted = 0;
				needRedraw = 1;
			}
		}
		b = b->next;
	}
	if (needRedraw) glutPostRedisplay();
}