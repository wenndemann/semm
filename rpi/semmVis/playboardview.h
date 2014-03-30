#ifndef PLAYBOARDVIEW_H
#define PLAYBOARDVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QResizeEvent>
#include "playboardscene.h"

class PlayboardView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PlayboardView(QWidget *parent = 0);

    void initializePlayboard( uint32_t playboardWidth, uint32_t playboardHeight,
                              uint32_t meepleWidth, uint32_t meepleHeight);
    void setGraphicsScene( QGraphicsScene* scene );

protected:
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void resizeEvent( QResizeEvent *event );

signals:

public slots:
    void fullscreen( );

private:
    QWidget* _parent;
    PlayboardScene* playboardScene;

};

#endif // PLAYBOARDVIEW_H
