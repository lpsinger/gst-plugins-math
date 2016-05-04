/*
 * Copyright (C) 2010 Leo Singer
 * Copyright (C) 2016 Aaron Viets
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


/*
 * ============================================================================
 *
 *                                  Preamble
 *
 * ============================================================================
 */


/*
 * Stuff from GStreamer
 */


#include <gst/gst.h>


/*
 * Our own stuff
 */


#include <unary_base.h>


/*
 * ============================================================================
 *
 *                              Plugin Entry Point
 *
 * ============================================================================
 */


GType unary_abs_get_type (void);
GType unary_exp_get_type (void);
GType unary_ln_get_type (void);
GType unary_log_get_type (void);
GType unary_log10_get_type (void);
GType unary_pow_get_type (void);


static gboolean
plugin_init (GstPlugin * plugin)
{
	struct
	{
		const gchar *name;
		GType type;
	} *element, elements[] = {
		{
		"unary_base", UNARY_BASE_TYPE}, {
		"abs", unary_abs_get_type ()}, {
		"exp", unary_exp_get_type ()}, {
		"ln", unary_ln_get_type ()}, {
		"log", unary_log_get_type ()}, {
		"log10", unary_log10_get_type ()}, {
		"pow", unary_pow_get_type ()}, {
		NULL, 0},};

	/*
	 * Tell GStreamer about the elements.
	 */

	for (element = elements; element->name; element++)
		if (!gst_element_register (plugin, element->name, GST_RANK_NONE,
			element->type))
			return FALSE;

	/*
	 * Done.
	 */

	return TRUE;
}


/*
 * This is the structure that gst-register looks for.
 */


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR, unary,
	"Unary arithmetic elements", plugin_init, PACKAGE_VERSION, "GPL",
	PACKAGE_NAME, "http://www.lsc-group.phys.uwm.edu/daswg")
