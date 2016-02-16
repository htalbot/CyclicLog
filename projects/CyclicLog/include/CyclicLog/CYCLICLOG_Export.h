
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CYCLICLOG
// ------------------------------
#ifndef CYCLICLOG_EXPORT_H
#define CYCLICLOG_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CYCLICLOG_HAS_DLL)
#  define CYCLICLOG_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CYCLICLOG_HAS_DLL */

#if !defined (CYCLICLOG_HAS_DLL)
#  define CYCLICLOG_HAS_DLL 1
#endif /* ! CYCLICLOG_HAS_DLL */

#if defined (CYCLICLOG_HAS_DLL) && (CYCLICLOG_HAS_DLL == 1)
#  if defined (CYCLICLOG_BUILD_DLL)
#    define CYCLICLOG_Export ACE_Proper_Export_Flag
#    define CYCLICLOG_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CYCLICLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CYCLICLOG_BUILD_DLL */
#    define CYCLICLOG_Export ACE_Proper_Import_Flag
#    define CYCLICLOG_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CYCLICLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CYCLICLOG_BUILD_DLL */
#else /* CYCLICLOG_HAS_DLL == 1 */
#  define CYCLICLOG_Export
#  define CYCLICLOG_SINGLETON_DECLARATION(T)
#  define CYCLICLOG_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CYCLICLOG_HAS_DLL == 1 */

// Set CYCLICLOG_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CYCLICLOG_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CYCLICLOG_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CYCLICLOG_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CYCLICLOG_NTRACE */

#if (CYCLICLOG_NTRACE == 1)
#  define CYCLICLOG_TRACE(X)
#else /* (CYCLICLOG_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CYCLICLOG_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CYCLICLOG_NTRACE == 1) */

#endif /* CYCLICLOG_EXPORT_H */

// End of auto generated file.
