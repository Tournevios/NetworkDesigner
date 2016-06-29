#include "networkdesigner.h"

#include <QtGui>
#include <QApplication>
#include "Network.h"
#include "UpdateSchedulingPlan.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkDesigner w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
