// Copyright 2016, Jeffrey E. Bedard
#include "battery.h"
#include "config.h"
#include "font.h"
#include "libjb/log.h"
#include "libjb/util.h"
#include "util.h"
#include "xstatus.h"
//#define XSTATUS_DEBUG_BATTERY
#ifndef XSTATUS_DEBUG_BATTERY
#undef LOG
#define LOG(...)
#endif//!XSTATUS_DEBUG_BATTERY
//#define XSTATUS_BATTERY_TEST
// get percent value, maximum 100, -1 on error
static int8_t get_percent(void)
{
	return JB_MIN(xstatus_system_value(XSTATUS_SYSFILE_BATTERY), 100);
}
// index into gc array, keeps gc array private
enum BATGCs { BATTERY_GC_BACKGROUND, BATTERY_GC_AC, BATTERY_GC_BATTERY,
	BATTERY_GC_CRITICAL, BATTERY_GC_SIZE };
// Selects a gc to use based on ac/battery status
static uint8_t get_gc(const uint8_t pct)
{
	return xstatus_system_value(XSTATUS_SYSFILE_AC) ? BATTERY_GC_AC : pct
		< XSTATUS_CONST_CRITICAL_PERCENT
		? BATTERY_GC_CRITICAL : BATTERY_GC_BATTERY;
}
static uint8_t format(char * buf, const uint8_t sz, const uint8_t pct)
{
	return snprintf(buf, sz, " %d%% ", pct);
}
static void draw_percent(xcb_connection_t * restrict xc, const xcb_gc_t gc,
	const uint8_t pct, const int16_t x)
{
	enum {BUF_SZ = 7};
	char buf[BUF_SZ];
	xcb_image_text_8(xc, format(buf, BUF_SZ, pct), xstatus_get_window(xc),
		gc, x, xstatus_get_font_size().h, buf);
}
static void set_gc(xcb_connection_t * restrict xc, const xcb_window_t w,
	xcb_gc_t * restrict gc, const char * restrict fg)
{
	xstatus_create_gc(xc, *gc = xcb_generate_id(xc), w, fg,
		XSTATUS_BATTERY_BACKGROUND_COLOR);
}
static void initialize_gcs(xcb_connection_t * restrict xc, const xcb_window_t w,
	xcb_gc_t * restrict gc)
{
#define SETGC(color) set_gc(xc, w, gc + BATTERY_GC_##color, \
	XSTATUS_BATTERY_##color##_COLOR);
	SETGC(BACKGROUND); SETGC(AC); SETGC(BATTERY); SETGC(CRITICAL);
#undef SETGC
}
static xcb_rectangle_t get_rectangle(const uint16_t start, const uint16_t end)
{
	return (xcb_rectangle_t){.x=start,
		.y = (XSTATUS_CONST_HEIGHT >> 2) + 1,
		.height = XSTATUS_CONST_HEIGHT >> 1,
		.width = end - start - XSTATUS_CONST_PAD};
}
__attribute__((const))
static uint16_t get_width_for_percent(const uint16_t width, const uint8_t pct)
{
	return width * pct / 100;
}
static void draw_rectangles(xcb_connection_t * restrict xc,
	const xcb_window_t w, const xcb_gc_t gc, const xcb_gc_t bg_gc,
	const uint16_t start, const uint16_t end, const uint8_t pct)
{
	xcb_rectangle_t rect = get_rectangle(start, end);
	// clear:
	xcb_poly_fill_rectangle(xc, w, bg_gc, 1, &rect);
	rect.width = get_width_for_percent(rect.width, pct);
	// fill rectangle per percent full:
	xcb_poly_fill_rectangle(xc, w, gc, 1, &rect);

}
void xstatus_draw_battery(xcb_connection_t * xc, const uint16_t start,
	const uint16_t end)
{
	static xcb_gc_t gc[BATTERY_GC_SIZE];
	const xcb_window_t w = xstatus_get_window(xc);
	if (!*gc)
		initialize_gcs(xc, w, gc);
	{ // pct scope
		const int8_t pct = get_percent();
		if (pct < 0) { // error getting percent
			// likely no battery or non-linux
			LOG("Coult not get percent, returning");
			return;
		}
		{ // gc_index scope
			const uint8_t gc_index = get_gc(pct);
			draw_rectangles(xc, w, gc[gc_index],
				gc[BATTERY_GC_BACKGROUND],
				start, end, pct);
			draw_percent(xc, gc[gc_index], pct,
				start + (end-start) / 2);
		}
	}
	xcb_flush(xc);
}
