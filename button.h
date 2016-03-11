#ifndef _BUTTON_H
#define _BUTTON_H

extern uint16_t xstatus_row_x;

typedef struct _Button {
	Display *d;
	Window w, parent;
	XRectangle g; // geometry
	char * label;
	void (*cb) (struct _Button *);
	void *cb_data;
	GC gc;
} Button;

Button * new_Button(Display * restrict d, const Window parent, const GC gc,
	char * restrict label, void (*cb)(Button *), void * restrict cb_data);

void delete_Button(Button * b);

Button * cmd_Button(Display * restrict d, const Window w, const GC gc,
	 char * restrict label, char * restrict cmd);

void draw_Button(Button * restrict b);

#endif//_BUTTON_H
