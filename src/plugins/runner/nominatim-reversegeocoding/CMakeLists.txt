PROJECT( NominatimReverseGeocodingPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}

 if( NOT QT4_FOUND )
  ${Qt5WebKitWidgets_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
 endif()
)
if( QT4_FOUND )
  INCLUDE(${QT_USE_FILE})
endif()

set( nominatim_SRCS OsmNominatimReverseGeocodingRunner.cpp NominatimReverseGeocodingPlugin.cpp )

marble_add_plugin( NominatimReverseGeocodingPlugin ${nominatim_SRCS} )
