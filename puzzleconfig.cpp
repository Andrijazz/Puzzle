#include "puzzleconfig.h"

#include <QFile>
#include <QTextStream>

// TODO: Error handling
QVector<QPolygonF> PuzzleConfig::getPuzzlePolygons()
{
    QVector<QPolygonF> result;

    QFile file(":/config.txt");
    if (!file.open((QIODevice::ReadOnly | QIODevice::Text)))
        return result;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        QPolygonF polygon;

        QRegExp regex("(\\d+)");

        QStringList numbersList;
        int pos = 0;
        while ((pos = regex.indexIn(line, pos)) != -1)
        {
            numbersList << regex.cap(1);
            pos += regex.matchedLength();
        }

        int i = 0;
        while (i < numbersList.size())
        {
            QPointF point(numbersList[i].toInt(),numbersList[i+1].toInt());
            polygon << point;
            i += 2;
        }

        result.push_back(polygon);
    }
    file.close();
    return result;
}
