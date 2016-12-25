#ifndef SIGNALPANEL_H
#define SIGNALPANEL_H

#include <QWidget>

namespace Ui {
class SignalPanel;
}

class SignalPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SignalPanel(QWidget *parent = 0);
    ~SignalPanel();

private:
    Ui::SignalPanel *ui;
};

#endif // SIGNALPANEL_H
