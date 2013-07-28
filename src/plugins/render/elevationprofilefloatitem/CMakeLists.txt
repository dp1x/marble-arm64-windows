PROJECT( ElevationProfileFloatItem )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
)
INCLUDE( ${QT_USE_FILE} )

set( my_SRCS
  ElevationProfileFloatItem.cpp
  ElevationProfilePlotAxis.cpp
 )
set( elevationprofile_UI ElevationProfileConfigWidget.ui )

qt4_wrap_ui( my_SRCS ${elevationprofile_UI})

marble_add_plugin( ElevationProfileFloatItem ${my_SRCS} )
set_target_properties( ElevationProfileFloatItem PROPERTIES AUTOMOC ON)
