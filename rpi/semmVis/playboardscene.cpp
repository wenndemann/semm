#include "playboardscene.h"
#include <QFile>
#include <assert.h>
#include <iostream>


PlayboardScene::PlayboardScene(uint32_t playboardWidth, uint32_t playboardHeight,
                               uint32_t meepleWidth, uint32_t meepleHeight,
                               QObject *parent) :
    QGraphicsScene(parent),
    _scalePlayboard( 1.0f ),
    _scalePixmapMeeple( 1.0f ),
    _fields({
            {  0, Point2D(1,  4) },
            {  1, Point2D(2,  4) },
            {  2, Point2D(3,  4) },
            {  3, Point2D(4,  4) },
            {  4, Point2D(4,  3) },
            {  5, Point2D(4,  2) },
            {  6, Point2D(4,  1) },
            {  7, Point2D(5,  1) },
            {  8, Point2D(6,  1) },
            {  9, Point2D(6,  2) },
            { 10, Point2D(6,  3) },
            { 11, Point2D(6,  4) },
            { 12, Point2D(7,  4) },
            { 13, Point2D(8,  4) },
            { 14, Point2D(9,  4) },
            { 15, Point2D(9,  5) },
            { 16, Point2D(9,  6) },
            { 17, Point2D(8,  6) },
            { 18, Point2D(7,  6) },
            { 19, Point2D(6,  6) },
            { 20, Point2D(6,  7) },
            { 21, Point2D(6,  8) },
            { 22, Point2D(6,  9) },
            { 23, Point2D(5,  9) },
            { 24, Point2D(4,  9) },
            { 25, Point2D(4,  8) },
            { 26, Point2D(4,  7) },
            { 27, Point2D(4,  6) },
            { 28, Point2D(3,  6) },
            { 29, Point2D(2,  6) },
            { 30, Point2D(1,  6) },
            { 31, Point2D(1,  5) },
            //start area
            { 32, Point2D(  0,  1) },
            { 33, Point2D(  1,  1) },
            { 34, Point2D(  1,  0) },
            { 35, Point2D(  9,  0) },
            { 36, Point2D(  9,  1) },
            { 37, Point2D( 10,  1) },
            { 38, Point2D( 10,  9) },
            { 39, Point2D(  9,  9) },
            { 40, Point2D(  9, 10) },
            { 41, Point2D(  1, 10) },
            { 42, Point2D(  1,  9) },
            { 43, Point2D(  0,  9) },
            //end area
            { 44, Point2D(2,  5) },
            { 45, Point2D(3,  5) },
            { 46, Point2D(4,  5) },
            { 47, Point2D(5,  2) },
            { 48, Point2D(5,  3) },
            { 49, Point2D(5,  4) },
            { 50, Point2D(8,  5) },
            { 51, Point2D(7,  5) },
            { 52, Point2D(6,  5) },
            { 53, Point2D(5,  8) },
            { 54, Point2D(5,  7) },
            { 55, Point2D(5,  6) }
        }) ,
    _offsetX ( 270 ),
    _offsetY ( 70 ),
    _factorX ( 824 ),
    _factorY ( 824 ),
    _pixmapPlayboard( NULL ),
    _itemPlayboard( NULL ),
    _groupPlayboard( NULL ),
    _groupMeeples( NULL )
{
    // semmVis Test version
    /*QFile filePlayboard( "../semmVis/pix/playboard/Feld.png" );
    QFile fileMeeple1( "../semmVis/pix/meeples/blue.png" );
    QFile fileMeeple2( "../semmVis/pix/meeples/pink.png" );
    QFile fileMeeple4( "../semmVis/pix/meeples/lightblue.png" );
    QFile fileMeeple8( "../semmVis/pix/meeples/black.png" );*/

    // semmClientFsm version
    QFile filePlayboard( "../../semmVis/pix/playboard/Feld.png" );
    QFile fileMeeple1( "../../semmVis/pix/meeples/blue.png" );
    QFile fileMeeple2( "../../semmVis/pix/meeples/pink.png" );
    QFile fileMeeple4( "../../semmVis/pix/meeples/lightblue.png" );
    QFile fileMeeple8( "../../semmVis/pix/meeples/black.png" );

    assert( filePlayboard.exists( ) );
    assert( fileMeeple1.exists( ) );
    assert( fileMeeple2.exists( ) );
    assert( fileMeeple4.exists( ) );
    assert( fileMeeple8.exists( ) );

    this->sceneRect().setWidth( playboardWidth );
    this->sceneRect().setHeight( playboardHeight );

    _pixmapPlayboard = new QPixmap( filePlayboard.fileName() );
    _itemPlayboard = new QGraphicsPixmapItem( *_pixmapPlayboard );
    float pixmapPlayboardWidth = static_cast<float>(_pixmapPlayboard->width( ));
    float pixmapPlayboardHeight = static_cast<float>(_pixmapPlayboard->height( ));
    _scalePlayboard = std::min( static_cast<float>( playboardWidth ) / pixmapPlayboardWidth,
                                static_cast<float>( playboardHeight ) / pixmapPlayboardHeight);
    _itemPlayboard->setScale( _scalePlayboard );
    _groupPlayboard = new QGraphicsItemGroup( );
    _groupPlayboard->addToGroup( _itemPlayboard );
    _groupPlayboard->setZValue( Playboard );
    this->addItem( _groupPlayboard );


    // Meeple graphics
    _mapPixmapMeeples[ 1 ] = new QPixmap( fileMeeple1.fileName( ) );
    _mapPixmapMeeples[ 2 ] = new QPixmap( fileMeeple2.fileName( ) );
    _mapPixmapMeeples[ 4 ] = new QPixmap( fileMeeple4.fileName( ) );
    _mapPixmapMeeples[ 8 ] = new QPixmap( fileMeeple8.fileName( ) );
    assert( _mapPixmapMeeples.size( ) == 4 );

    float pixmapMeepleWidth  = static_cast<float>(_mapPixmapMeeples[ 1 ]->width( ));
    float pixmapMeepleHeight = static_cast<float>(_mapPixmapMeeples[ 1 ]->height( ));
    _scalePixmapMeeple = std::min( static_cast<float>( meepleWidth ) / pixmapMeepleWidth,
                                   static_cast<float>( meepleHeight ) / pixmapMeepleHeight);


    _groupMeeples = new QGraphicsItemGroup( );
    _groupMeeples->setZValue( Meeples );
    this->addItem( _groupMeeples );

    // TODO experimental for testing purposes
    /*
    addMeeples( 1, std::map< uint16_t, uint8_t >{ {10, 41}, {11, 42}, {12, 43} } );
    addMeeples( 2, std::map< uint16_t, uint8_t >{ {7, 38}, {8, 39}, {9, 40} } );
    addMeeples( 4, std::map< uint16_t, uint8_t >{ {4, 35}, {5, 36}, {6, 37} } );
    addMeeples( 8, std::map< uint16_t, uint8_t >{ {1, 32}, {2, 33}, {3, 34} } );
    */
}

void PlayboardScene::addMeeples( uint8_t color, semmVis::mapTagField tagsPoint2Ds )
{
    assert( tagsPoint2Ds.size( ) == 3 );
    assert( color == 1 || color == 2 || color == 4 || color == 8 );

    // clear old meeples
    if ( _mapPlayerToMeepleItems.find( color ) != _mapPlayerToMeepleItems.end( ) )
    {
        // first: tag | second: QGraphicsPixmapItem*
        for ( std::map< uint16_t, QGraphicsPixmapItem* >::iterator it = _mapPlayerToMeepleItems[ color ].begin( );
              it != _mapPlayerToMeepleItems[ color ].end( ); ++it )
        {
            _groupMeeples->removeFromGroup( it->second );
            delete it->second;
        }
        _mapPlayerToMeepleItems[ color ].clear( );
    }

    // first: tag | second: fieldId
    for ( std::map< uint16_t, uint8_t >::iterator it = tagsPoint2Ds.begin( );
          it != tagsPoint2Ds.end( ); ++it )
    {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem( *(_mapPixmapMeeples[ color ]) );

        const Point2D& pos = _fields[ it->second ];
        item->setOffset( _offsetX + _factorX * pos.x, _offsetY + _factorY * pos.y );
        item->setScale( _scalePixmapMeeple );

        _groupMeeples->addToGroup( item );
        _mapPlayerToMeepleItems[ color ][ it->first ] = item;
    }
}

void PlayboardScene::setMeeplePos( uint8_t color, uint16_t tag, uint8_t toFieldId )
{
    assert( color == 1 || color == 2 || color == 4 || color == 8 );
    assert( _fields.find( toFieldId ) != _fields.end( ) );
    assert( _mapPlayerToMeepleItems.find( color ) != _mapPlayerToMeepleItems.end( ) );
    assert( _mapPlayerToMeepleItems.find( color )->second.find( tag ) !=
            _mapPlayerToMeepleItems.find( color )->second.end( ) );

    const Point2D& pos = _fields[ toFieldId ];
    _mapPlayerToMeepleItems[ color ][ tag ]->setOffset( _offsetX + _factorX * pos.x, _offsetY + _factorY * pos.y );
}

