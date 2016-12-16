#include "draweegtest.h"
#include "ui_draweegtest.h"

DrawEEGTest::DrawEEGTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawEEGTest)
{
    ui->setupUi(this);
}

DrawEEGTest::~DrawEEGTest()
{
    delete ui;
}
