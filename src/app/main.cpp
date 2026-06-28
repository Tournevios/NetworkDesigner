#include "networkdesigner.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include "Network.h"
#include "UpdateSchedulingPlan.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("NetworkDesigner");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("NetworkDesigner");

    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    NetworkDesigner w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
