#ifndef BRAINPOWERPANEL_H
#define BRAINPOWERPANEL_H

#include <QWidget>

namespace Ui {
class BrainPowerPanel;
}

class BrainPowerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BrainPowerPanel(QWidget *parent = 0);
    ~BrainPowerPanel();

private:
    Ui::BrainPowerPanel *ui;
};

#endif // BRAINPOWERPANEL_H
