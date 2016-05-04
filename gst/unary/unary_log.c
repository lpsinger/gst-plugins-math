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


#include <unary_base.h>

#define TYPE_UNARY_LOG \
	(unary_log_get_type())
#define UNARY_LOG(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj),TYPE_UNARY_LOG,UnaryLog))
#define UNARY_LOG_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass),TYPE_UNARY_LOG,UnaryLogClass))
#define IS_PLUGIN_TEMPLATE(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj),TYPE_UNARY_LOG))
#define IS_PLUGIN_TEMPLATE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass),TYPE_UNARY_LOG))

typedef struct _UnaryLog UnaryLog;
typedef struct _UnaryLogClass UnaryLogClass;

GType
unary_log_get_type(void);

struct _UnaryLog
{
	UnaryBase unary_base;
	double base;
};

struct _UnaryLogClass 
{
	UnaryBaseClass parent_class;
};


/* Set the exponent */

enum property
{
	PROP_BASE = 1,
};


static void
set_property(GObject * object, enum property id, const GValue * value,
	GParamSpec * pspec)
{
	UnaryLog *element = UNARY_LOG(object);

	GST_OBJECT_LOCK(element);

	switch(id) {
		case PROP_BASE:
			element->base = g_value_get_double(value);
			break;
	}

	GST_OBJECT_UNLOCK(element);
}


static void
get_property(GObject * object, enum property id, GValue * value,
	GParamSpec * pspec)
{
	UnaryLog *element = UNARY_LOG(object);

	GST_OBJECT_LOCK(element);

	switch(id) {
		case PROP_BASE:
			g_value_set_double(value, element->base);
			break;
	}

	GST_OBJECT_UNLOCK(element);
}

static GstFlowReturn transform_ip(GstBaseTransform *trans, GstBuffer *buf);

/* Initialize the plugin's class */
static void
unary_log_class_init(gpointer klass, gpointer klass_data)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GstBaseTransformClass *basetransform_class = GST_BASE_TRANSFORM_CLASS(klass);

	gst_element_class_set_details_simple(GST_ELEMENT_CLASS(klass),
		"Logarithm with base k",
		"Filter/Audio",
		"Calculate logarithm, y = log_k x",
		"Leo Singer <leo.singer@ligo.org>, Aaron Viets <aaron.viets@ligo.org>");

	basetransform_class -> transform_ip = GST_DEBUG_FUNCPTR(transform_ip);
	basetransform_class -> set_caps = GST_DEBUG_FUNCPTR(set_caps);

	gobject_class->get_property = GST_DEBUG_FUNCPTR(get_property);
	gobject_class->set_property = GST_DEBUG_FUNCPTR(set_property);

	g_object_class_install_property(gobject_class,
		PROP_BASE,
		g_param_spec_double("base",
			"Base",
			"Base of logarithm",
			-G_MAXDOUBLE, G_MAXDOUBLE, 10.,
			G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT));
}

/*
 * GstBaseTransform vmethod implementations
 *
 * An in-place transform
 * really does the same thing as the chain function
 */

static GstFlowReturn transform_ip(GstBaseTransform *trans, GstBuffer *buf)
{
	UnaryLog* element = UNARY_LOG(trans);
	int bits = element -> unary_base.bits;
	int is_complex = element -> unary_base.is_complex;

	/* Debugging
	 *
	 * GstObject* element_gstobj = GST_OBJECT(trans);
	 * int channels = element -> unary_base.channels;
	 * int rate = element -> unary_base.rate;
	 * g_print("[%s]: passing GstBuffer: ", element_gstobj->name);
	 * g_print("%d channels, ", channels);
	 * g_print("%d bits, ", bits);
	 * g_print("rate: %d, ", rate);
	 */

	GstMapInfo info;
	if(!gst_buffer_map(buf, &info, GST_MAP_READWRITE)) {
		GST_ERROR_OBJECT(trans, "gst_buffer_map failed\n");
	}
	gpointer data = info.data;
	gpointer data_end = data + info.size;

	const double n = element -> base;
	const double m = 1. / log(n);

	if(is_complex == 1) {

		if(bits == 128) {
			/* g_print("COMPLEX FLOAT128\n"); */
			double complex *ptr, *end = data_end;
			for(ptr = data; ptr < end; ptr++) {
				*ptr = clog(*ptr) * m;
			}
		} else if(bits == 64) {
			/* g_print("COMPLEX FLOAT64\n"); */
			float complex *ptr, *end = data_end;
			for(ptr = data; ptr < end; ptr++) {
				*ptr = clogf(*ptr) * m;
			}
		} else {
			g_assert_not_reached();
		}
	} else if(is_complex == 0) {

		if(bits == 64) {
			/* g_print("REAL FLOAT64\n"); */
			double *ptr, *end = data_end;
			for(ptr = data; ptr < end; ptr++) {
				*ptr = log(*ptr) * m;
			}
		} else if(bits == 32) {
			/* g_print("REAL FLOAT32\n"); */
			float *ptr, *end = data_end;
			for(ptr = data; ptr < end; ptr++) {
				*ptr = logf(*ptr) * m;
			}
		} else {
			g_assert_not_reached();
		}
	} else {
		g_assert_not_reached();
	}
	gst_buffer_unmap(buf, &info);
	return GST_FLOW_OK;
}


GType
unary_log_get_type(void)
{
	static GType type = 0;

	if(!type) {
		static const GTypeInfo info = {
			.class_size = sizeof(UnaryBaseClass),
			.class_init = unary_log_class_init,
			.instance_size = sizeof(UnaryLog),
		};
		type = g_type_register_static(UNARY_BASE_TYPE, "UnaryLog", &info, 0);
	}

	return type;
}
