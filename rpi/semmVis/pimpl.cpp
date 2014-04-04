#include "pimpl.h"

#include "defs.h"
#include "mainwindow.h"

#include <QMetaObject>
#include <QApplication>

#include <boost/thread.hpp>

pimpl::pimpl()
{
    std::cout << "Qt wrapper created!" << std::endl;
    _thread = new boost::thread( &pimpl::worker, this );
}

pimpl::~pimpl()
{
    _thread->join( );
    delete _thread;
    std::cout << "Qt wrapper destroyed!" << std::endl;
}

void pimpl::worker( )
{
    std::cout << "Starting Qt worker thread!" << std::endl;
    int argc( 1 );
    char* argv = new char( 'S' );
    QApplication app( argc, &argv );

    _mainWindow = new MainWindow( );
    _mainWindow->initializeVisualization( 800, 800, 67, 67 );

    if ( !_mainWindow->view( ) ) { std::cout << "view is null!" << std::endl; return; }
    if ( !_mainWindow->view( )->scene( ) ) { std::cout << "scene is invalid!" << std::endl; return; }

    qRegisterMetaType< uint8_t >( "uint8_t" );
    qRegisterMetaType< uint16_t >( "uint16_t" );
    qRegisterMetaType< semmVis::mapTagField >( "semmVis::mapTagField" );

    _mainWindow->show( );
    app.exec( );

    std::cout << "Exiting Qt worker thread!" << std::endl;
}

void pimpl::addMeeples( uint8_t color, std::map< uint16_t, uint8_t > tagsFields )
{
    QMetaObject::invokeMethod( _mainWindow->view( )->scene( ), "addMeeples",
                               Qt::QueuedConnection,
                               Q_ARG( uint8_t, color ),
                               Q_ARG( semmVis::mapTagField, tagsFields ) );
}

void pimpl::setMeeplePos( uint8_t color, uint16_t tag, uint8_t toFieldId )
{


    QMetaObject::invokeMethod( _mainWindow->view( )->scene( ), "addMeeples",
                               Qt::QueuedConnection,
                               Q_ARG( uint8_t, color ),
                               Q_ARG( uint16_t, tag ),
                               Q_ARG( uint8_t, toFieldId ) );
}
