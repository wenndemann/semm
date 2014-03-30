#include "playboardview.h"
#include <QGridLayout>

PlayboardView::PlayboardView(QWidget *parent) :
  QGraphicsView(parent),
  _parent( parent )
{
    this->setRenderHint( QPainter::Antialiasing, true );
    this->setRenderHint( QPainter::HighQualityAntialiasing, true );
    this->setRenderHint( QPainter::SmoothPixmapTransform, true );
    this->setDragMode( ScrollHandDrag );
}

void PlayboardView::initializePlayboard( uint32_t playboardWidth, uint32_t playboardHeight,
                                         uint32_t meepleWidth, uint32_t meepleHeight )
{
    playboardScene = new PlayboardScene( playboardWidth, playboardHeight,
                                         meepleWidth, meepleHeight,
                                         this );

    setGraphicsScene( playboardScene );
}

void PlayboardView::setGraphicsScene( QGraphicsScene* scene )
{
    scene->setParent( this );
    this->setScene( scene );
}


void PlayboardView::keyPressEvent( QKeyEvent *event )
{
    if ( this->isFullScreen( ) )
    {
        if( event->key( ) == Qt::Key_Escape )
        {
            QGridLayout* layout = qobject_cast<QGridLayout*>(_parent->layout( ));
            if ( layout ){ layout->addWidget( this ); }
            this->showNormal( );
        }
    }

}

void PlayboardView::resizeEvent( QResizeEvent *event )
{
    this->fitInView( this->scene( )->sceneRect( ), Qt::KeepAspectRatio );
}

void PlayboardView::fullscreen( )
{
    this->setParent( NULL );
    this->showFullScreen( );
}
