#include "networkdesigner.h"

#include <QtGui>
#include <QApplication>
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
    QApplication a(argc, argv);
    LOGD("main: QApplication created");

#ifdef Q_OS_ANDROID
    // The Activity can momentarily close and recreate the Qt window during
    // startup (splash teardown, IME/system focus changes). Because
    // quitOnLastWindowClosed defaults to true, that flicker becomes a silent
    // clean exit — the "window appears then closes" symptom. The Activity owns
    // the process lifetime on Android, so opt out entirely.
    a.setQuitOnLastWindowClosed(false);
    // The platform style on Android is unreliable for Qt Widgets; Fusion is
    // the supported cross-platform choice. Must be set after QApplication so
    // the style plugin paths are initialized.
    QApplication::setStyle("Fusion");
#endif

    NetworkDesigner w;
    LOGD("main: NetworkDesigner constructed");
    w.show();
    LOGD("main: window shown, entering event loop");
    return a.exec();
}
