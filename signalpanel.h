#ifndef SIGNALPANEL_H
#define SIGNALPANEL_H

#include <QWidget>
#include <vector>
#include <QPainter>
#include <QTimer>
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

public slots:
    void myUpdate();
};

#endif // SIGNALPANEL_H
