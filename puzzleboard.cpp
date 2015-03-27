#include "puzzleboard.h"

#include <QBrush>

PuzzleBoard::PuzzleBoard()
{
    setRect(600, 300, 400, 400);
    setBrush(QBrush(QColor(200, 200, 200)));
    setAcceptDrops(true);
    setZValue(-1);

    boardPoints = new QVector<QPointF>;
    boardPoints->push_back(QPointF(600, 300)); boardPoints->push_back(QPointF(700, 300));
    boardPoints->push_back(QPointF(800, 300)); boardPoints->push_back(QPointF(900, 300));

    boardPoints->push_back(QPointF(600, 400)); boardPoints->push_back(QPointF(700, 400));
    boardPoints->push_back(QPointF(800, 400)); boardPoints->push_back(QPointF(900, 400));

    boardPoints->push_back(QPointF(600, 500)); boardPoints->push_back(QPointF(700, 500));
    boardPoints->push_back(QPointF(800, 500)); boardPoints->push_back(QPointF(900, 500));

    boardPoints->push_back(QPointF(600, 600)); boardPoints->push_back(QPointF(700, 600));
    boardPoints->push_back(QPointF(800, 600)); boardPoints->push_back(QPointF(900, 600));

    board = rect();
}

PuzzleBoard::~PuzzleBoard()
{}

QVector<QPointF>* PuzzleBoard::getBoardPoints()
{
    return boardPoints;
}
