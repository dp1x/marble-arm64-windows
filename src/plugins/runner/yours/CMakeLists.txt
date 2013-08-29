PROJECT( YoursPlugin )

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

set( yours_SRCS YoursRunner.cpp YoursPlugin.cpp )

marble_add_plugin( YoursPlugin ${yours_SRCS} )
