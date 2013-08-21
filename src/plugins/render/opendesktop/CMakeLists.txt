PROJECT( OpenDesktopPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}

  if( NOT QT4_FOUND )
    ${Qt5WebKitWidgets_INCLUDE_DIRS}
  endif()
)
if( QT4_FOUND )
  INCLUDE(${QT_USE_FILE})
endif()

set( my_SRCS
  OpenDesktopPlugin.cpp
  OpenDesktopItem.cpp
  OpenDesktopModel.cpp
)

set( opendesktop_UI OpenDesktopConfigWidget.ui )
qt_wrap_ui( my_SRCS ${opendesktop_UI} )
marble_qt4_automoc( ${my_SRCS} )

marble_add_plugin( OpenDesktopPlugin ${my_SRCS} )
