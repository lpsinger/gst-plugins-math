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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <unary_complex_base.h>

#include <complex.h>

GType unary_complex_pow_get_type (void);

#define UNARY_COMPLEX_POW_TYPE \
	(unary_complex_pow_get_type())
#define UNARY_COMPLEX_POW(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), UNARY_COMPLEX_POW_TYPE, UnaryComplexPow))


typedef struct
{
  GstAudioFilter audiofilter;
  double exponent;
} UnaryComplexPow;



static GstFlowReturn
transform_ip (GstBaseTransform * trans, GstBuffer * buf)
{
  UnaryComplexPow *element = UNARY_COMPLEX_POW (trans);
  GstAudioFilter *audiofilter = GST_AUDIO_FILTER (trans);
  GstAudioFormat format = audiofilter->info.finfo->format;

  GstMapInfo info;
  gst_buffer_map (buf, &info, GST_MAP_READWRITE);
  gpointer data = info.data;
  gpointer data_end = data + info.size;

  const double n = element->exponent;

  if (format >= GST_AUDIO_FORMAT_F64) {
    double complex *ptr, *end = data_end;
    for (ptr = data; ptr < end; ptr++)
      *ptr = cpow (*ptr, n);
  } else if (format >= GST_AUDIO_FORMAT_F32) {
    float complex *ptr, *end = data_end;
    for (ptr = data; ptr < end; ptr++)
      *ptr = cpowf (*ptr, n);
  } else {
    g_assert_not_reached ();
  }

  gst_buffer_unmap (buf, &info);

  return GST_FLOW_OK;
}


/*
 * ============================================================================
 *
 *                                Type Support
 *
 * ============================================================================
 */


enum property
{
  PROP_EXPONENT = 1,
};


static void
set_property (GObject * object, enum property id, const GValue * value,
    GParamSpec * pspec)
{
  UnaryComplexPow *element = UNARY_COMPLEX_POW (object);

  GST_OBJECT_LOCK (element);

  switch (id) {
    case PROP_EXPONENT:
      element->exponent = g_value_get_double (value);
      break;
  }

  GST_OBJECT_UNLOCK (element);
}


static void
get_property (GObject * object, enum property id, GValue * value,
    GParamSpec * pspec)
{
  UnaryComplexPow *element = UNARY_COMPLEX_POW (object);

  GST_OBJECT_LOCK (element);

  switch (id) {
    case PROP_EXPONENT:
      g_value_set_double (value, element->exponent);
      break;
  }

  GST_OBJECT_UNLOCK (element);
}


static void
base_init (gpointer class)
{
  gst_element_class_set_details_simple (GST_ELEMENT_CLASS (class),
      "Raise complex input to a power",
      "Filter/Audio",
      "Calculate complex input raised to the power n, y = x^n",
      "Aaron Viets <aaron.viets@ligo.org>, Leo Singer <leo.singer@ligo.org>");
}


static void
class_init (gpointer class, gpointer class_data)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);
  GstBaseTransformClass *basetransform_class = GST_BASE_TRANSFORM_CLASS (class);

  basetransform_class->transform_ip = GST_DEBUG_FUNCPTR (transform_ip);

  gobject_class->get_property = GST_DEBUG_FUNCPTR (get_property);
  gobject_class->set_property = GST_DEBUG_FUNCPTR (set_property);

  g_object_class_install_property (gobject_class,
      PROP_EXPONENT,
      g_param_spec_double ("exponent",
          "Exponent",
          "Exponent",
          -G_MAXDOUBLE, G_MAXDOUBLE, 2.,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT)
      );
}


GType
unary_complex_pow_get_type (void)
{
  static GType type = 0;

  if (!type) {
    static const GTypeInfo info = {
      .class_size = sizeof (UnaryComplexBaseClass),
      .base_init = base_init,
      .class_init = class_init,
      .instance_size = sizeof (UnaryComplexPow),
    };
    type = g_type_register_static (UNARY_COMPLEX_BASE_TYPE, "UnaryComplexPow", &info, 0);
  }

  return type;
}
