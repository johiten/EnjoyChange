#include "deviceinfopanel.h"
#include "ui_deviceinfopanel.h"

DeviceInfoPanel::DeviceInfoPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceInfoPanel)
{
    ui->setupUi(this);
}

DeviceInfoPanel::~DeviceInfoPanel()
{
    delete ui;
}
