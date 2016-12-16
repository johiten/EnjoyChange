#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ecengine.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startEngine();

    ///////////////////////
    void drawCurves(QPainter &painter);
    void paintEvent(QPaintEvent *);

    int displayDataPos1, displayDataPos2, displayDataTmp;

    QTimer *timer;



    ////////////////////////


private:
    Ui::MainWindow *ui;
    ECEngine *engine;

};

#endif // MAINWINDOW_H
