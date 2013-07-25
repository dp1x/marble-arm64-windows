if(NOT QT5BUILD)
    add_subdirectory( designer )
    add_subdirectory( declarative )
    add_subdirectory( render )
    add_subdirectory( runner )
    add_subdirectory( templates )
    add_subdirectory( positionprovider )
endif(NOT QT5BUILD)


if( QT_QTDECLARATIVE_FOUND )
    add_subdirectory( qt-components )
endif( QT_QTDECLARATIVE_FOUND )
