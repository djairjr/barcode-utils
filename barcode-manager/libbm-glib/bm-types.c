/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * libbm_glib -- Access network status & information from glib applications
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2008 Red Hat, Inc.
 */

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <string.h>
#include "bm-types.h"
#include "bm-types-private.h"
#include "bm-object-private.h"
#include "bm-object-cache.h"
#include "bm-dbus-glib-types.h"

static gpointer
bm_ssid_copy (GByteArray *src)
{
	GByteArray *dest;

	dest = g_byte_array_sized_new (src->len);
	g_byte_array_append (dest, src->data, src->len);
	return dest;
}

static void
bm_ssid_free (GByteArray *ssid)
{
	g_byte_array_free (ssid, TRUE);
}

GType
bm_ssid_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static (g_intern_static_string ("bm-ssid"),
		                                         (GBoxedCopyFunc) bm_ssid_copy,
		                                         (GBoxedFreeFunc) bm_ssid_free);
	return our_type;
}

gboolean
_bm_ssid_demarshal (GValue *value, GByteArray **dest)
{
	GByteArray *array;

	if (!G_VALUE_HOLDS (value, DBUS_TYPE_G_UCHAR_ARRAY))
		return FALSE;

	if (*dest) {
		g_boxed_free (BM_TYPE_SSID, *dest);
		*dest = NULL;
	}

	array = (GByteArray *) g_value_get_boxed (value);
	if (array && (array->len > 0)) {
		*dest = g_byte_array_sized_new (array->len);
		(*dest)->len = array->len;
		memcpy ((*dest)->data, array->data, array->len);
	}

	return TRUE;
}

/*****************************/

static gpointer
bm_uint_array_copy (GArray *src)
{
	GArray *dest;

	dest = g_array_sized_new (FALSE, TRUE, sizeof (guint32), src->len);
	g_array_append_vals (dest, src->data, src->len);
	return dest;
}

static void
bm_uint_array_free (GArray *array)
{
	g_array_free (array, TRUE);
}

GType
bm_uint_array_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static (g_intern_static_string ("bm-uint-array"),
		                                         (GBoxedCopyFunc) bm_uint_array_copy,
		                                         (GBoxedFreeFunc) bm_uint_array_free);
	return our_type;
}

gboolean
_bm_uint_array_demarshal (GValue *value, GArray **dest)
{
	GArray *array;

	if (!G_VALUE_HOLDS (value, DBUS_TYPE_G_UINT_ARRAY))
		return FALSE;

	if (*dest) {
		g_boxed_free (BM_TYPE_UINT_ARRAY, *dest);
		*dest = NULL;
	}

	array = (GArray *) g_value_get_boxed (value);
	if (array && (array->len > 0)) {
		*dest = g_array_sized_new (FALSE, TRUE, sizeof (guint32), array->len);
		g_array_append_vals (*dest, array->data, array->len);
	}

	return TRUE;
}

/*****************************/

static gpointer
bm_string_array_copy (GPtrArray *src)
{
	GPtrArray *dest;
	int i;

	dest = g_ptr_array_sized_new (src->len);
	for (i = 0; i < src->len; i++)
		g_ptr_array_add (dest, g_strdup (g_ptr_array_index (src, i)));
	return dest;
}

static void
bm_string_array_free (GPtrArray *array)
{
	int i;

	for (i = 0; i < array->len; i++)
		g_free (g_ptr_array_index (array, i));
	g_ptr_array_free (array, TRUE);
}

GType
bm_string_array_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static (g_intern_static_string ("bm-string-array"),
		                                         (GBoxedCopyFunc) bm_string_array_copy,
		                                         (GBoxedFreeFunc) bm_string_array_free);
	return our_type;
}

gboolean
_bm_string_array_demarshal (GValue *value, GPtrArray **dest)
{
	GPtrArray *array;

	if (!G_VALUE_HOLDS (value, DBUS_TYPE_G_ARRAY_OF_STRING))
		return FALSE;

	if (*dest) {
		g_boxed_free (BM_TYPE_STRING_ARRAY, *dest);
		*dest = NULL;
	}

	array = (GPtrArray *) g_value_get_boxed (value);
	if (array && array->len) {
		int i;

		*dest = g_ptr_array_sized_new (array->len);
		for (i = 0; i < array->len; i++)
			g_ptr_array_add (*dest, g_strdup (g_ptr_array_index (array, i)));
	}

	return TRUE;
}

/*****************************/

static gpointer
bm_object_array_copy (GPtrArray *src)
{
	GPtrArray *dest;
	int i;

	dest = g_ptr_array_sized_new (src->len);
	for (i = 0; i < src->len; i++)
		g_ptr_array_add (dest, g_object_ref (g_ptr_array_index (src, i)));
	return dest;
}

static void
bm_object_array_free (GPtrArray *array)
{
	int i;

	for (i = 0; i < array->len; i++)
		g_object_unref (g_ptr_array_index (array, i));
	g_ptr_array_free (array, TRUE);
}

GType
bm_object_array_get_type (void)
{
	static GType our_type = 0;

	if (our_type == 0)
		our_type = g_boxed_type_register_static (g_intern_static_string ("bm-object-array"),
		                                         (GBoxedCopyFunc) bm_object_array_copy,
		                                         (GBoxedFreeFunc) bm_object_array_free);
	return our_type;
}

gboolean
_bm_object_array_demarshal (GValue *value,
                           GPtrArray **dest,
                           DBusGConnection *connection,
                           BMObjectCreatorFunc func)
{
	GPtrArray *temp = NULL;
	GPtrArray *array;

	if (!G_VALUE_HOLDS (value, DBUS_TYPE_G_ARRAY_OF_OBJECT_PATH))
		return FALSE;

	array = (GPtrArray *) g_value_get_boxed (value);
	if (array && array->len) {
		int i;

		temp = g_ptr_array_sized_new (array->len);
		for (i = 0; i < array->len; i++) {
			const char *path;
			GObject *object;

			path = g_ptr_array_index (array, i);
			object = G_OBJECT (_bm_object_cache_get (path));
			if (object) {
				g_ptr_array_add (temp, g_object_ref (object));
			} else {
				object = (*func) (connection, path);
				if (object)
					g_ptr_array_add (temp, object);
				else
					g_warning ("%s: couldn't create object for %s", __func__, path);
			}
		}
	} else
		temp = g_ptr_array_new ();

	/* Deallocate after to ensure that an object that might already
	 * be in the array doesn't get destroyed due to refcounting.
	 */
	if (*dest)
		g_boxed_free (BM_TYPE_OBJECT_ARRAY, *dest);
	*dest = temp;

	return TRUE;
}
