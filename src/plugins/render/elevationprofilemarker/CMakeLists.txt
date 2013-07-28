PROJECT( ElevationProfileMarker )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
)
INCLUDE( ${QT_USE_FILE} )

set( my_SRCS
  ElevationProfileMarker.cpp
 )

qt4_add_resources( my_SRCS elevationprofile.qrc)

marble_add_plugin( ElevationProfileMarker ${my_SRCS} )
set_target_properties( ElevationProfileMarker PROPERTIES AUTOMOC ON )
