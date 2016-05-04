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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef __UNARY_BASE_H__
#define __UNARY_BASE_H__


#include <glib.h>
#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <stdio.h>
#include <string.h>
#include <complex.h>
#include <stdlib.h>
#include <math.h>

G_BEGIN_DECLS
#define UNARY_BASE_TYPE \
	(unary_base_get_type())
#define UNARY_BASE(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), UNARY_BASE_TYPE, UnaryBase))
#define UNARY_BASE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), UNARY_BASE_TYPE, UnaryBaseClass))
#define GST_IS_UNARY_BASE(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), UNARY_BASE_TYPE))
#define GST_IS_UNARY_BASE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), UNARY_BASE_TYPE))
		
typedef struct _UnaryBase UnaryBase;
typedef struct _UnaryBaseClass UnaryBaseClass;

struct _UnaryBase
{
	GstBaseTransform element;

	int is_complex;
	int bits;
	int channels;
	int rate;
};

struct _UnaryBaseClass
{
	GstBaseTransformClass parent_class;
};

GType unary_base_get_type(void);

/*
 * set_caps() is called when the caps are re-negotiated. Can return
 * false if we do not like what we see.
 */

gboolean set_caps(GstBaseTransform *trans, GstCaps *incaps,
	GstCaps *outcaps);


G_END_DECLS
#endif /* __UNARY_BASE_H__ */
