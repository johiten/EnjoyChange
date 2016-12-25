#ifndef DEVICEINFOPANEL_H
#define DEVICEINFOPANEL_H

#include <QWidget>

namespace Ui {
class DeviceInfoPanel;
}

class DeviceInfoPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceInfoPanel(QWidget *parent = 0);
    ~DeviceInfoPanel();

private:
    Ui::DeviceInfoPanel *ui;
};

#endif // DEVICEINFOPANEL_H
