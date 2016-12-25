#include "signalpanel.h"
#include "ui_signalpanel.h"

SignalPanel::SignalPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalPanel)
{
    ui->setupUi(this);
}

SignalPanel::~SignalPanel()
{
    delete ui;
}
