# Process this file with cmake to build lightIDL.

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(lightIDL)

SET(LIBIDL_MAJOR_VERSION 0)
SET(LIBIDL_MINOR_VERSION 8)
SET(LIBIDL_MICRO_VERSION 7)
SET(LIBIDL_VERSION ${LIBIDL_MAJOR_VERSION}.${LIBIDL_MINOR_VERSION}.${LIBIDL_MICRO_VERSION})

# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility
# has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed
# interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to
# C:R+1:A
SET(LT_VERSION, "0:0:0")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

INCLUDE(CheckIncludeFile)
INCLUDE(CheckFunctionExists)

FIND_PACKAGE(Flex REQUIRED)
FIND_PACKAGE(Bison REQUIRED)

CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
IF(HAVE_UNISTD_H)
  SET(HAVE_UNISTD_H 1)
ENDIF(HAVE_UNISTD_H)

CHECK_INCLUDE_FILE(wchar.h  HAVE_WCHAR_H)
IF(HAVE_WCHAR_H)
  SET(HAVE_WCHAR_H 1)
ELSE(HAVE_WCHAR_H)
  SET(HAVE_WCHAR_H 0)
ENDIF(HAVE_WCHAR_H)

CHECK_INCLUDE_FILE(wcstr.h  HAVE_WCSTR_H)
IF(HAVE_WCSTR_H)
  SET(HAVE_WCSTR_H 1)
ELSE(HAVE_WCSTR_H)
  SET(HAVE_WCSTR_H 0)
ENDIF(HAVE_WCSTR_H)

CHECK_FUNCTION_EXISTS(popen HAVE_POPEN)
CHECK_FUNCTION_EXISTS(symlink HAVE_SYMLINK)
CHECK_FUNCTION_EXISTS(access HAVE_ACCESS)

# lightIDL precompiler settings
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CPP_PROGRAM "gcc -E")
  SET(HAVE_CPP_PIPE_STDIN 1)
  SET(CPP_NOSTDINC "-nostdinc")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(MSVC60)
  SET(CPP_PROGRAM "CL.EXE /E")
  #SET(HAVE_CPP_PIPE_STDIN 0)
  SET(CPP_NOSTDINC "/X")
ENDIF(MSVC60)

ADD_DEFINITIONS(-DCPP_PROGRAM=\"\\\"${CPP_PROGRAM}\\\"\")
ADD_DEFINITIONS(-DCPP_NOSTDINC=\"\\\"${CPP_NOSTDINC}\\\"\")
IF(HAVE_CPP_PIPE_STDIN)
  ADD_DEFINITIONS(-DHAVE_CPP_PIPE_STDIN)
ENDIF(HAVE_CPP_PIPE_STDIN)

# Disable 'secure' warnings
IF(WIN32)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

# lightIDL requires a 64-bit type
IF(WIN32)
  SET(LONGLONG __int64)
  SET(IDL_LL \"i64\")
ENDIF(WIN32)

IF(UNIX)
  SET(LONGLONG long long)
  SET(IDL_LL \"ll\")
ENDIF(UNIX)

IF(NOT LONGLONG)
  MESSAGE(FATAL_ERROR "lightIDL requires a 64 bit format type.")
ENDIF(NOT LONGLONG)

CONFIGURE_FILE(include/libIDL/IDL.h.in ${CMAKE_SOURCE_DIR}/include/libIDL/IDL.h)

ADD_DEFINITIONS(-DGLIB_PRIVATE -DLIBIDL_VERSION=\\\"${LIBIDL_VERSION}\\\")

# lightIDL build
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/libIDL
  ${CMAKE_BINARY_DIR}
)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_BINARY_DIR}/lexer.c
  COMMAND ${FLEX_EXECUTABLE} -o ${CMAKE_BINARY_DIR}/lexer.c ${CMAKE_SOURCE_DIR}/lexer.l
  DEPENDS ${CMAKE_SOURCE_DIR}/lexer.l
          ${CMAKE_BINARY_DIR}/parser.h
)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_BINARY_DIR}/parser.h
         ${CMAKE_BINARY_DIR}/parser.c
  COMMAND ${BISON_EXECUTABLE} -d -v -o ${CMAKE_BINARY_DIR}/parser.c ${CMAKE_SOURCE_DIR}/parser.y
  DEPENDS ${CMAKE_SOURCE_DIR}/parser.y
)

ADD_LIBRARY(lightIDL SHARED
  ${CMAKE_BINARY_DIR}/parser.c
  ${CMAKE_BINARY_DIR}/parser.h
  ${CMAKE_BINARY_DIR}/lexer.c
  include/libIDL/IDL.h
  include/libIDL/glib.h
  glib_private.h
  gtree.c
  gutils.c
  gstrfuncs.c
  ns.c
  util.c
  util.h
  rename.h
  libchash.h
  libchash.c
  queue.h
)

# Modeline for vim: set tw=79 et ts=4:
