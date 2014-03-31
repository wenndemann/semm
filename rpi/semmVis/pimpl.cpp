#include "pimpl.h"

#include "mainwindow.h"

#include <QMetaObject>
#include <QApplication>

#include <boost/thread.hpp>

pimpl::pimpl()
{
    _thread = new boost::thread( &pimpl::worker, this );


}

pimpl::~pimpl()
{
    _thread->join( );
    delete _thread;
}

void pimpl::worker( )
{
    int argc( 1 );
    char* argv = new char( 'S' );
    QApplication app( argc, &argv );

    _mainWindow = new MainWindow( );

    PlayboardView* view = qobject_cast< PlayboardView* >( _mainWindow );
    if ( !view ) return;
    PlayboardScene* scene = qobject_cast< PlayboardScene* >( view->scene( ) );
    if ( !scene ) return;

    _mainWindow->show( );

    app.exec( );
}

void pimpl::addMeeples( uint8_t color, std::map< uint16_t, uint8_t > tagsFields )
{
    //qRegisterMetaType< mapTagField >( "mapTagField" );
    QMetaObject::invokeMethod( _scene, SLOT( addMeeples ),
                               Qt::QueuedConnection,
                               Q_ARG( uint8_t, color ),
                               Q_ARG( mapTagField, tagsFields ) );
}

void pimpl::setMeeplePos( uint8_t color, uint16_t tag, uint8_t toFieldId )
{


    QMetaObject::invokeMethod( _scene, SLOT( addMeeples ),
                               Qt::QueuedConnection,
                               Q_ARG( uint8_t, color ),
                               Q_ARG( uint16_t, tag ),
                               Q_ARG( uint8_t, toFieldId ) );
}
