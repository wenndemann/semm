#ifndef PLAYBOARDSCENE_H
#define PLAYBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>

class PlayboardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Layer {
        Playboard,
        Meeples,
        Information
    };

    struct Point2D {
        Point2D( ) : x(0),y(0) { }
        Point2D( uint8_t _x, uint8_t _y) : x(_x), y(_y) { }
        uint32_t x, y;
    };

public:
    explicit PlayboardScene(uint32_t playboardWidth, uint32_t playboardHeight,
                            uint32_t meepleWidth, uint32_t meepleHeight,
                            QObject *parent = 0);

private:


signals:

public slots:
    // add meepes of player color with idtags (uint16_t) on fieldIds (uint8_t)
    void addMeeples( uint8_t color, std::map< uint16_t, uint8_t > tagsFields );

    void setMeeplePos( uint8_t color, uint8_t tag, uint8_t toFieldId );

private:

    float _scalePlayboard, _scalePixmapMeeple;

    std::map< uint8_t, Point2D > _fields;
    uint32_t _offsetX,
             _offsetY,
             _factorX,
             _factorY;

    QPixmap *_pixmapPlayboard;
    QGraphicsPixmapItem* _itemPlayboard;

    std::map< uint8_t, QPixmap* > _mapPixmapMeeples;
    std::map< uint8_t, std::map< uint16_t, QGraphicsPixmapItem* > > _mapPlayerToMeepleItems;

    QGraphicsItemGroup *_groupPlayboard,
                       *_groupMeeples;
};

#endif // PLAYBOARDSCENE_H
