//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010,2011 Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

// Own
#include "ServerLayout.h"

#include "GeoSceneTiled.h"
#include "MarbleGlobal.h"
#include "TileId.h"

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#include <math.h>

namespace Marble
{

ServerLayout::ServerLayout( GeoSceneTiled *textureLayer )
    : m_textureLayer( textureLayer )
{
}

ServerLayout::~ServerLayout()
{
}

qint64 ServerLayout::numTilesX( const Marble::TileId& tileId ) const
{
    return ( 1 << tileId.zoomLevel() ) * m_textureLayer->levelZeroColumns();
}

qint64 ServerLayout::numTilesY( const Marble::TileId& tileId ) const
{
    return ( 1 << tileId.zoomLevel() ) * m_textureLayer->levelZeroRows();
}

MarbleServerLayout::MarbleServerLayout( GeoSceneTiled *textureLayer )
    : ServerLayout( textureLayer )
{
}

QUrl MarbleServerLayout::downloadUrl( const QUrl &prototypeUrl, const TileId &id ) const
{
    QUrl url = prototypeUrl;
    url.setPath( url.path() + m_textureLayer->relativeTileFileName( id ) );

    return url;
}

QString MarbleServerLayout::name() const
{
    return "Marble";
}


OsmServerLayout::OsmServerLayout( GeoSceneTiled *textureLayer )
    : ServerLayout( textureLayer )
{
}

QUrl OsmServerLayout::downloadUrl( const QUrl &prototypeUrl, const TileId &id ) const
{
    const QString suffix = m_textureLayer->fileFormat().toLower();
    const QString path = QString( "%1/%2/%3.%4" ).arg( id.zoomLevel() )
                                                 .arg( id.x() )
                                                 .arg( id.y() )
                                                 .arg( suffix );

    QUrl url = prototypeUrl;
    url.setPath( url.path() + path );

    return url;
}

QString OsmServerLayout::name() const
{
    return "OpenStreetMap";
}


CustomServerLayout::CustomServerLayout( GeoSceneTiled *texture )
    : ServerLayout( texture )
{
}

QUrl CustomServerLayout::downloadUrl( const QUrl &prototypeUrl, const TileId &id ) const
{
    QString urlStr = prototypeUrl.toString();

    urlStr.replace( "{zoomLevel}", QString::number( id.zoomLevel() ) );
    urlStr.replace( "{x}", QString::number( id.x() ) );
    urlStr.replace( "{y}", QString::number( id.y() ) );

    return QUrl( urlStr );
}

QString CustomServerLayout::name() const
{
    return "Custom";
}


WmsServerLayout::WmsServerLayout( GeoSceneTiled *texture )
    : ServerLayout( texture )
{
}

QUrl WmsServerLayout::downloadUrl( const QUrl &prototypeUrl, const Marble::TileId &tileId ) const
{
    const qreal radius = numTilesX( tileId ) / 2.0;
    const qint64 x = tileId.x();

    const qreal lonLeft   = ( x - radius ) / radius * 180.0;
    const qreal lonRight  = ( x - radius + 1 ) / radius * 180.0;

#if QT_VERSION < 0x050000
    QUrl urlQuery = prototypeUrl;
#else
    QUrlQuery urlQuery(prototypeUrl.query());
#endif

    urlQuery.addQueryItem( "service", "WMS" );
    urlQuery.addQueryItem( "request", "GetMap" );
    urlQuery.addQueryItem( "version", "1.1.1" );
    if ( !urlQuery.hasQueryItem( "styles" ) )
        urlQuery.addQueryItem( "styles", "" );
    if ( !urlQuery.hasQueryItem( "format" ) ) {
        if ( m_textureLayer->fileFormat().toLower() == "jpg" )
            urlQuery.addQueryItem( "format", "image/jpeg" );
        else
            urlQuery.addQueryItem( "format", "image/" + m_textureLayer->fileFormat().toLower() );
    }
    if ( !urlQuery.hasQueryItem( "srs" ) ) {
        urlQuery.addQueryItem( "srs", epsgCode() );
    }
    if ( !urlQuery.hasQueryItem( "layers" ) )
        urlQuery.addQueryItem( "layers", m_textureLayer->name() );
    urlQuery.addQueryItem( "width", QString::number( m_textureLayer->tileSize().width() ) );
    urlQuery.addQueryItem( "height", QString::number( m_textureLayer->tileSize().height() ) );
    urlQuery.addQueryItem( "bbox", QString( "%1,%2,%3,%4" ).arg( QString::number( lonLeft, 'f', 12 ) )
                                                      .arg( QString::number( latBottom( tileId ), 'f', 12 ) )
                                                      .arg( QString::number( lonRight, 'f', 12 ) )
                                                      .arg( QString::number( latTop( tileId ), 'f', 12 ) ) );

#if QT_VERSION < 0x050000
    QUrl url = urlQuery;
#else
    QUrl url = prototypeUrl;
    url.setQuery(urlQuery);
#endif
    return url;
}

QString WmsServerLayout::name() const
{
    return "WebMapService";
}

qreal WmsServerLayout::latBottom( const Marble::TileId &tileId ) const
{
    const qreal radius = numTilesY( tileId ) / 2.0;

    switch( m_textureLayer->projection() )
    {
    case GeoSceneTiled::Equirectangular:
        return ( radius - tileId.y() - 1 ) / radius *  90.0;
    case GeoSceneTiled::Mercator:
        return atan( sinh( ( radius - tileId.y() - 1 ) / radius * M_PI ) ) * 180.0 / M_PI;
    }

    Q_ASSERT( false ); // not reached
    return 0.0;
}

qreal WmsServerLayout::latTop( const Marble::TileId &tileId ) const
{
    const qreal radius = numTilesY( tileId ) / 2.0;

    switch( m_textureLayer->projection() )
    {
    case GeoSceneTiled::Equirectangular:
        return ( radius - tileId.y() ) / radius *  90.0;
    case GeoSceneTiled::Mercator:
        return atan( sinh( ( radius - tileId.y() ) / radius * M_PI ) ) * 180.0 / M_PI;
    }

    Q_ASSERT( false ); // not reached
    return 0.0;
}

QString WmsServerLayout::epsgCode() const
{
    switch ( m_textureLayer->projection() ) {
        case GeoSceneTiled::Equirectangular:
            return "EPSG:4326";
        case GeoSceneTiled::Mercator:
            return "EPSG:3785";
    }

    Q_ASSERT( false ); // not reached
    return QString();
}

QuadTreeServerLayout::QuadTreeServerLayout( GeoSceneTiled *textureLayer )
    : ServerLayout( textureLayer )
{
}

QUrl QuadTreeServerLayout::downloadUrl( const QUrl &prototypeUrl, const Marble::TileId &id ) const
{
    QString urlStr = prototypeUrl.toString();

    urlStr.replace( "{quadIndex}", encodeQuadTree( id ) );

    return QUrl( urlStr );
}

QString QuadTreeServerLayout::name() const
{
    return "QuadTree";
}

QString QuadTreeServerLayout::encodeQuadTree( const Marble::TileId &id )
{
    QString tileNum;

    for ( int i = id.zoomLevel(); i >= 0; i-- ) {
        const int tileX = (id.x() >> i) % 2;
        const int tileY = (id.y() >> i) % 2;
        const int num = ( 2 * tileY ) + tileX;

        tileNum += QString::number( num );
    }

    return tileNum;
}

TmsServerLayout::TmsServerLayout(GeoSceneTiled *textureLayer )
    : ServerLayout( textureLayer )
{
}

QUrl TmsServerLayout::downloadUrl( const QUrl &prototypeUrl, const TileId &id ) const
{
    const QString suffix = m_textureLayer->fileFormat().toLower();
    // y coordinate in TMS start at the bottom of the map (South) and go upwards,
    // opposed to OSM which start at the top.
    //
    // http://wiki.osgeo.org/wiki/Tile_Map_Service_Specification
    int y_frombottom = ( 1<<id.zoomLevel() ) - id.y() - 1 ;

    const QString path = QString( "%1/%2/%3.%4" ).arg( id.zoomLevel() )
                                                 .arg( id.x() )
                                                 .arg( y_frombottom )
                                                 .arg( suffix );
    QUrl url = prototypeUrl;
    url.setPath( url.path() + path );

    return url;
}

QString TmsServerLayout::name() const
{
    return "TileMapService";
}

}
