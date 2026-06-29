#include "networkdesigner.h"

#include <QtGui>
#include <QApplication>
#include <QStyleFactory>
#include "Network.h"
#include "UpdateSchedulingPlan.h"

#ifdef Q_OS_ANDROID
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "NetworkDesigner", __VA_ARGS__)
#else
#define LOGD(...) qDebug(__VA_ARGS__)
#endif

int main(int argc, char *argv[])
{
    LOGD("main: starting");

#ifdef Q_OS_ANDROID
    // Force the cross-platform Fusion style; the Android style can be
    // unreliable with Qt Widgets and has been known to trigger crashes.
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

    QApplication a(argc, argv);
    LOGD("main: QApplication created");

    NetworkDesigner w;
    LOGD("main: NetworkDesigner constructed");

    w.show();
    LOGD("main: window shown");

#ifndef Q_OS_ANDROID
    // On Android the Activity manages the lifecycle; connecting lastWindowClosed
    // to quit() causes the app to exit immediately when the system briefly
    // takes focus during startup.
    a.connect(&a, &QApplication::lastWindowClosed, &a, &QApplication::quit);
#endif
    return a.exec();
}
