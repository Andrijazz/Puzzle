#ifndef PUZZLECONFIG_H
#define PUZZLECONFIG_H

#include <QVector>
#include <QPolygonF>

class PuzzleConfig
{
public:
    static QVector<QPolygonF> getPuzzlePolygons();
};
#endif // PUZZLECONFIG_H
