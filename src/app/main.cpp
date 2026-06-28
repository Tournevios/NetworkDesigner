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
#ifndef Q_OS_ANDROID
    // On Android the Activity manages the lifecycle; connecting lastWindowClosed
    // to quit() causes the app to exit immediately when the system briefly
    // takes focus during startup.
    a.connect(&a, &QApplication::lastWindowClosed, &a, &QApplication::quit);
#endif
    return a.exec();
}
