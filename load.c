// Copyright 2016, Jeffrey E. Bedard
#include "load.h"
#include "config.h"
#include "font.h"
#include <stdio.h>
#include <stdlib.h>
static uint8_t format(char * restrict buf, const uint8_t sz)
{
	double l;
	getloadavg(&l, 1);
	return snprintf(buf, sz, "%.2f", l);
}
// Returns x offset for next item
uint16_t xstatus_draw_load(xcb_connection_t * xc, const uint16_t offset)
{
	const struct JBDim f = xstatus_get_font_size();
	uint8_t sz=6;
	const int16_t x = offset + XSTATUS_CONST_PAD;
	{ // buf scope
		char buf[sz];
		sz = format(buf, sz);
		xcb_image_text_8(xc, sz, xstatus_get_window(xc),
			xstatus_get_gc(xc), x, f.h, buf);
	}
	return x + f.w * sz + XSTATUS_CONST_PAD;
}
