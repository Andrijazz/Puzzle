#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "puzzleconfig.h"
#include "puzzleboard.h"

#include <QFileDialog>
#include <QMessageBox>

#include <cstdlib>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    scene = new QGraphicsScene(0, 0, 1600, 900, this);

    ui->graphicsView->setScene(scene);

    setWindowTitle(tr("Puzzle"));

    setupMenus();

    loadImage(":/example.jpg");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(const QString& fileName)
{
    if (!fileName.isEmpty()) {
        QPixmap newImage;
        if (!newImage.load(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                  tr("The image file could not be loaded."),
                                  QMessageBox::Cancel);
            return;
        }
        puzzleImage = newImage;

        // Scale image to 400 x 400
        int size = qMin(puzzleImage.width(), puzzleImage.height());
        puzzleImage = puzzleImage.copy((puzzleImage.width() - size)/2,
            (puzzleImage.height() - size)/2, size, size).scaled(400, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setupPuzzle();
    }
}

void MainWindow::openImageDialog(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", "Image Files (*.png *.jpg *.bmp)");

    loadImage(fileName);

}
void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    QAction *restartAction = gameMenu->addAction(tr("&Restart"));

    connect(openAction, SIGNAL(triggered()), this, SLOT(openImageDialog()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(restartAction, SIGNAL(triggered()), this, SLOT(setupPuzzle()));
}

void MainWindow::setupPuzzle()
{
    scene->clear();
    puzzlePieces.clear();

    PuzzleBoard* puzzleBoard = new PuzzleBoard();
    scene->addItem(puzzleBoard);

    // QVector<QPointF>* boardPoints = puzzleBoard->getBoardPoints();
    srand(time(NULL));

    // create puzzle pieces
    QVector<QPolygonF> polygons = PuzzleConfig::getPuzzlePolygons();
    for (int i = 0; i < polygons.size(); i++)
    {
        PuzzlePiece* pzp = new PuzzlePiece(puzzleImage, polygons[i], puzzleBoard, this);
        pzp->setFlag(QGraphicsItem::ItemIsMovable);
        int xpos, ypos;
        QPointF origin = pzp->getOrigin();

        if (i > polygons.size() / 2 - 1)
        {
            xpos = 20 + rand() % 360 - origin.x();
            ypos = 20 + rand() % 500 - origin.y();
            pzp->setPos(QPoint(xpos, ypos));
        }
        else {
            xpos = 1120 + rand() % 160 - origin.x();
            ypos = 20 + rand() % 500 - origin.y();
            pzp->setPos(QPoint(xpos, ypos));

        }

        puzzlePieces.push_back(pzp);

        scene->addItem(pzp);
    }


}
// observer pattern
void MainWindow::isPuzzleCompleted()
{
    for (int i = 0; i < puzzlePieces.size(); i++)
    {
        if (!puzzlePieces[i]->isOnTheRightPlace())
            return;
    }

    QMessageBox::information(this, tr("Puzzle Completed"),
         tr("Congratulations! You have completed the puzzle!\n"
            "Click OK to start again."),
         QMessageBox::Ok);

     setupPuzzle();
}
