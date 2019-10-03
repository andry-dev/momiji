#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <qcoreapplication.h>
#include <qlocale.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icons/program.ico"));

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QTranslator myTranslator;
    myTranslator.load("translations/momijiqt_" + QLocale::system().name());

    app.installTranslator(&qtTranslator);
    app.installTranslator(&myTranslator);

    MainWindow w;
    w.show();

    return app.exec();
}
