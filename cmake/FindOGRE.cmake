# Find OGRE library
#
# OGRE_INCLUDE_DIR      where to find the include files
# OGRE_LIBRARY_DIR      where to find the libraries
# OGRE_LIBRARIES        list of libraries to link
# OGRE_FOUND            true if OGRE was found

SET( OGRE_LIBRARYDIR / CACHE PATH "Alternative library directory" )
SET( OGRE_INCLUDEDIR / CACHE PATH "Alternative include directory" )
MARK_AS_ADVANCED( OGRE_LIBRARYDIR OGRE_INCLUDEDIR )

FIND_LIBRARY( OGRE_LIBRARY_DIR OgreMain PATHS ${OGRE_LIBRARYDIR} )
FIND_PATH( OGRE_INCLUDE_DIR Ogre.h
           HINTS "/usr/include/OGRE" "/usr/include"
           PATHS ${OGRE_INCLUDEDIR} )

GET_FILENAME_COMPONENT( OGRE_LIBRARY_DIR ${OGRE_LIBRARY_DIR} PATH )

IF( OGRE_INCLUDE_DIR AND OGRE_LIBRARY_DIR )
    SET( OGRE_LIBRARIES OgreMain pthread)
    SET( OGRE_FOUND TRUE )
ELSE( OGRE_INCLUDE_DIR AND OGRE_LIBRARY_DIR )
    SET( OGRE_FOUND FALSE )
ENDIF( OGRE_INCLUDE_DIR AND OGRE_LIBRARY_DIR )