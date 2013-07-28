PROJECT( marble-touch )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( my_SRCS
  main.cpp
 )

FILE (GLOB QML_FILES *.qml activities/*.qml plasma/*.qml harmattan/*.qml)
marble_add_project_resources(${QML_FILES})

qt4_add_resources(my_SRCS meego.qrc)
add_executable( marble-touch ${my_SRCS} )
set_target_properties( marble-touch PROPERTIES AUTOMOC ON)
target_link_libraries( marble-touch ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTMAIN_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} marblewidget )
install( TARGETS marble-touch RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
