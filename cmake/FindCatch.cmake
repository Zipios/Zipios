#
# Find the CppUnit includes and library
#
# This module defines
# CATCH_INCLUDE_DIR, where to find tiff.h, etc.
# CATCH_FOUND, If false, do not try to use CppUnit.

SET(CATCH_FOUND "NO")

FIND_PATH(CATCH_INCLUDE_DIR
    catch.hpp
    PATHS
        contrib
        /usr/local/include
        /usr/include
)

IF(CATCH_INCLUDE_DIR)
    IF(CATCH_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find library Catch.")
    ELSE(CATCH_FIND_REQUIRED)
        SET(CATCH_FOUND "YES")
    ENDIF(CATCH_FIND_REQUIRED)
ELSE(CATCH_INCLUDE_DIR)
    IF(CATCH_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find library Catch.")
    ENDIF(CATCH_FIND_REQUIRED)
ENDIF(CATCH_INCLUDE_DIR)

# vim: ts=4 sw=4 et
