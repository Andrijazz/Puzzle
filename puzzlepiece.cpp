#include "puzzlepiece.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QRegion>
#include <QApplication>
#include <QBitmap>
#include <QtCore/qmath.h>

#include "mainwindow.h"

qreal PuzzlePiece::mZValue = 1;

PuzzlePiece::PuzzlePiece(QPixmap &pixmap, QPolygonF polygon, PuzzleBoard* puzzleBoard, MainWindow* parent)
    : drawingSource(pixmap), mPuzzleBoard(puzzleBoard), parent(parent)
{
    this->setPolygon(polygon);
    origin = polygon[0];

    mZValue = 1;
    setZValue(mZValue);

}

void PuzzlePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRegion region(polygon().toPolygon());
    QVector<QRect> rects = region.rects();

    /**
     * Draws a pixmap at (x, y) by copying a part of the given pixmap into the paint device.
     * (x, y) specifies the top-left point in the paint device that is to be drawn onto.
     * (sx, sy) specifies the top-left point in pixmap that is to be drawn. The default is (0, 0).
     * (sw, sh) specifies the size of the pixmap that is to be drawn. The default, (0, 0) (and negative) means all the way to the
     * bottom-right of the pixmap.
     * drawPixmap ( int x, int y, const QPixmap & pixmap, int sx, int sy, int sw, int sh )
     */   

    for (int i = 0; i < rects.size(); i++)
    {
        painter->drawPixmap(rects[i].x(), rects[i].y(), drawingSource,
                            rects[i].x(), rects[i].y(), rects[i].width(), rects[i].height());
    }
}

void PuzzlePiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);

    itemPosition = mapToScene(origin);
    startMouseDragPos = mapToScene(event->pos());

    mZValue++;
    setZValue(mZValue);


    QGraphicsItem::mousePressEvent(event);
}

void PuzzlePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);

    QPointF endMouseDragPos = mapToScene(event->pos());

    // translate polygon into scene coordinate system
    QPolygonF piecePolygon = polygon();

    qreal dx = itemPosition.x() - origin.x();
    qreal dy = itemPosition.y() - origin.y();

    piecePolygon.translate(dx, dy);

    // translate polygon for mouse move distance
    dx = endMouseDragPos.x() - startMouseDragPos.x();
    dy = endMouseDragPos.y() - startMouseDragPos.y();

    piecePolygon.translate(dx, dy);

    int pointsInside = pointsInTheBoard(piecePolygon);
    if (pointsInside == 0)
    {
        return;
    }

    if (pointsInside < piecePolygon.size() || isOverlaping(piecePolygon))
    {
        itemPosition.setX(itemPosition.x() - origin.x());
        itemPosition.setY(itemPosition.y() - origin.y());
        setPos(itemPosition);
        return;
    }

    if (pointsInside == piecePolygon.size())
    {
        QPointF closestBoardPoint = findClosestBoardPoint(piecePolygon[0]);

        // translate polygon to match the closestBoardPoint
        dx = closestBoardPoint.x() - piecePolygon[0].x();
        dy = closestBoardPoint.y() - piecePolygon[0].y();

        piecePolygon.translate(dx, dy);

        // set pos takes as a (0, 0) first point in polygon
        piecePolygon.translate(-origin.x(), -origin.y());
        setPos(piecePolygon[0]);
        parent->isPuzzleCompleted();
    }
}

QPointF PuzzlePiece::findClosestBoardPoint(QPointF point)
{
    QVector<QPointF>* boardPoints = mPuzzleBoard->getBoardPoints();
    QVector<qreal> distances;
    for (int i = 0; i < boardPoints->size(); i++)
    {
        qreal distance = qSqrt(qPow(qAbs(boardPoints->at(i).x() - point.x()), 2) + qPow(qAbs(boardPoints->at(i).y() - point.y()), 2));
        distances.push_back(distance);
    }

    qreal minDistance = distances[0];
    int minDistanceIndex = 0;
    for (int i = 1; i < distances.size(); i ++)
    {
        if (distances[i] < minDistance)
        {
            minDistance = distances[i];
            minDistanceIndex = i;
        }
    }

    return boardPoints->at(minDistanceIndex);
}

int PuzzlePiece::pointsInTheBoard(QPolygonF piecePolygon)
{
    int count = 0;
    for (int i = 0; i < piecePolygon.size(); i++)
    {
        if (585 <= piecePolygon[i].x() && piecePolygon[i].x() <= 1015
                && 285 <= piecePolygon[i].y() && piecePolygon[i].y() <= 715)
        {
            count ++;
        }
    }
    return count;
}

QPointF PuzzlePiece::getOrigin()
{
    return origin;
}

void PuzzlePiece::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    QPolygonF piecePolygon = polygon();

    QPointF currentItemPos = mapToScene(piecePolygon[0]);

    QRegion region(piecePolygon.toPolygon());
    QRect boundingItemRect = region.boundingRect();

    QPointF currentBoundingRectPos = mapToScene(QPointF(boundingItemRect.x(), boundingItemRect.y()));

    if (currentBoundingRectPos.x() < 0)
        setX(-origin.x() + currentItemPos.x() - currentBoundingRectPos.x());
    else if (currentBoundingRectPos.x() > scene()->width() - boundingItemRect.width())
        setX(scene()->width() - boundingItemRect.width() - origin.x() + currentItemPos.x() - currentBoundingRectPos.x());

    if (currentBoundingRectPos.y() < 0)
        setY(-origin.y());
    else if (currentBoundingRectPos.y() > scene()->height() - boundingItemRect.height())
        setY(scene()->height() - boundingItemRect.height() - origin.y() + currentItemPos.y() - currentBoundingRectPos.y());

}

bool PuzzlePiece::isOnTheRightPlace()
{
    QPointF A = mapToScene(origin);
    QPointF B = QPointF(polygon()[0].x() + 600, polygon()[0].y() + 300);
    return  A == B;
}

bool PuzzlePiece::isOverlaping(QPolygonF piecePolygon)
{
    for (int i = 0; i < piecePolygon.size(); i++)
    {
        int itemsUnderPoint = scene()->items(piecePolygon[i]).size();
        if (itemsUnderPoint > 2)
        {
            return true;
        }
    }
    return false;
}
