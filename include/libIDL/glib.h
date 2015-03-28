/**************************************************************************

    glib.h (fake GLib header)

    Copyright (C) 2007 Guilherme Balena Versiani

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: glib.h,v 1.1 2008/04/06 14:49:53 balena Exp $

***************************************************************************/

/* This is not the GLib header! */

#ifndef __G_LIB_H__
#define __G_LIB_H__


/* Provide macros to feature the GCC function attribute.
 */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define G_GNUC_PRINTF( format_idx, arg_idx )    \
	__attribute__((format (printf, format_idx, arg_idx)))
#else   /* !__GNUC__ */
#define G_GNUC_PRINTF( format_idx, arg_idx )
#endif  /* !__GNUC__ */


/* Wrap the gcc __PRETTY_FUNCTION__ and __FUNCTION__ variables with
 * macros, so we can refer to them as strings unconditionally.
 */
#ifdef  __GNUC__
#define G_GNUC_PRETTY_FUNCTION  __PRETTY_FUNCTION__
#else   /* !__GNUC__ */
#define G_GNUC_PRETTY_FUNCTION  ""
#endif  /* !__GNUC__ */


/* GLib types */
#if defined(_WIN32) && defined(_MSC_VER)
typedef __int64 gint64;
typedef unsigned __int64 guint64;
#else
typedef long long gint64;
typedef unsigned long long guint64;
#endif

typedef int gint;
typedef unsigned int guint;

typedef long glong;
typedef unsigned long guint32;

typedef void *gpointer;
typedef const void *gconstpointer;

typedef gint gboolean;

typedef char gchar;

/* GLib structures */
typedef struct HashTable GHashTable;

typedef guint (*GHashFunc)(gconstpointer);
typedef gint (*GCompareFunc)(gconstpointer, gconstpointer);
typedef void (*GHFunc)(gpointer key, gpointer value, gpointer user_data);

typedef struct _GSList GSList;
typedef struct _GString GString;
typedef struct _GTree GTree;

#ifdef GLIB_PRIVATE
#include <glib_private.h>
#endif


#endif // __G_LIB_H__

/* Modeline for vim: set tw=79 et ts=4: */

