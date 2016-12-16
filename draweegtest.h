#ifndef DRAWEEGTEST_H
#define DRAWEEGTEST_H

#include <QWidget>

namespace Ui {
class DrawEEGTest;
}

class DrawEEGTest : public QWidget
{
    Q_OBJECT

public:
    explicit DrawEEGTest(QWidget *parent = 0);
    ~DrawEEGTest();

private:
    Ui::DrawEEGTest *ui;
};

#endif // DRAWEEGTEST_H
