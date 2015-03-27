#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include "puzzleboard.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>


class MainWindow;

class PuzzlePiece : public QGraphicsPolygonItem
{
public:
    PuzzlePiece(QPixmap &pixmap, QPolygonF polygon, PuzzleBoard* puzzleBoard, MainWindow* parent);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QPointF getOrigin();
    bool isOnTheRightPlace();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:

    bool isOverlaping(QPolygonF piecePolygon);
    int pointsInTheBoard(QPolygonF piecePolygon);
    QPointF findClosestBoardPoint(QPointF point);

    QPixmap &drawingSource;

    QPointF itemPosition;
    QPointF origin;
    QPointF currentItemPositionOnTheBoard;
    QPointF itemPositionOnTheBoard;

    QPointF startMouseDragPos;

    PuzzleBoard* mPuzzleBoard;
    MainWindow* parent;

    static qreal mZValue;
};

#endif // PUZZLEPIECE_H
