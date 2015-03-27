#ifndef PUZZLEBOARD_H
#define PUZZLEBOARD_H

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsRectItem>
#include <QVector>

class PuzzleBoard : public QGraphicsRectItem
{
public:
    PuzzleBoard();
    ~PuzzleBoard();

    QVector<QPointF>* getBoardPoints();

private:
    QVector<QPointF>* boardPoints;
    QPolygonF board;

};
#endif // PUZZLEBOARD_H
