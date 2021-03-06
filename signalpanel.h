#ifndef SIGNALPANEL_H
#define SIGNALPANEL_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "spppinclude/vector.h"
#include "spppinclude/vectormath.h"
class ECEngine;

namespace Ui {
class SignalPanel;
}

class SignalPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SignalPanel(QWidget *parent = 0);
    ~SignalPanel();
    void linkEngine(ECEngine *);

private:
    Ui::SignalPanel *ui;
    ECEngine *enginePoint;

    void paintEvent(QPaintEvent *event);
    void paintOneChannel(double *, double , int , int , QPainter &painter);

public slots:
};

#endif // SIGNALPANEL_H
