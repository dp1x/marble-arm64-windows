//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Daniel Marth <danielmarth@gmx.at>
//

#ifndef MARBLE_QTCOMPONENTS_PLUGIN_H
#define MARBLE_QTCOMPONENTS_PLUGIN_H

#include <qglobal.h>
#include <marble_export.h>

#if QT_VERSION < 0x050000
  #include <QDeclarativeExtensionPlugin>
#else
  #include <QQmlExtensionPlugin>
#endif

/**
 * Dummy class to export plugin.
 */
#if QT_VERSION < 0x050000
class QtComponentsPlugin : public QDeclarativeExtensionPlugin
#else
class QtComponentsPlugin : public QQmlExtensionPlugin
#endif
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.kde.edu.marble.QtComponentsPlugin" )
public:
    /** Overriding QDeclarativeExtensionPlugin to register types */
    virtual void registerTypes( const char *uri );
};

#endif
