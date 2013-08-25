//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#ifndef MARBLE_DECLARATIVE_PLUGIN_H
#define MARBLE_DECLARATIVE_PLUGIN_H

#include "qglobal.h"

#if QT_VERSION < 0x050000
  #include <QDeclarativeExtensionPlugin>
  typedef QDeclarativeExtensionPlugin QQmlExtensionPlugin;
  typedef QDeclarativeEngine QQmlEngine;
#else
  #include <QQmlExtensionPlugin>
#endif

/**
  * Registers MarbleWidget, MarbleRunnerManager and MarbleThemeManager
  * as QQml extensions for use in QML.
  */
class MarbleDeclarativePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
public:
    /** Overriding QQmlExtensionPlugin to register types */
    virtual void registerTypes( const char *uri );

    /** Overriding QQmlExtensionPlugin to register image provider */
    void initializeEngine( QQmlEngine *engine, const char *);
};

#endif
