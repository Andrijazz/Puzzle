#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsScene>
#include <QVector>

#include "puzzlepiece.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void isPuzzleCompleted();
    void loadImage(const QString &path);

public slots:
    void openImageDialog(const QString &path = QString());
    void setupPuzzle();



private:
    void setupMenus();

    Ui::MainWindow *ui;
    QPixmap puzzleImage;
    QVector<PuzzlePiece*> puzzlePieces;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
