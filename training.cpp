#include "training.h"
#include "ui_training.h"

Training::Training(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Training)
{
    ui->setupUi(this);
}

Training::~Training()
{
    delete ui;
}
