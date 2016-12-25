#include "brainpowerpanel.h"
#include "ui_brainpowerpanel.h"

BrainPowerPanel::BrainPowerPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrainPowerPanel)
{
    ui->setupUi(this);
}

BrainPowerPanel::~BrainPowerPanel()
{
    delete ui;
}
