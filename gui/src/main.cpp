#include <QApplication>
#include <cmath>
#include <chrono>
#include "globalconstants.h"
#include "mainwindow.h"



int main(int argc, char* argv[])
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapInterval(0);
    format.setSamples(0);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);


    QTranslator translator;
    if(translator.load(QLocale(), QString("simulation"), QString("_"), QString("translations")))
        a.installTranslator(&translator);

    TranslatedStrings::pendulum = QObject::tr("pendulum");
    TranslatedStrings::fileName = QObject::tr("data sample");
    TranslatedStrings::chooseFile = QObject::tr("Choose file");
    TranslatedStrings::error = QObject::tr("Error");
    Parameter::namesList = QStringList() << QObject::tr("Time") << "x"+QString(SpecChar::indice1) << "y"+QString(SpecChar::indice1) << "x"+QString(SpecChar::indice2) << "y"+QString(SpecChar::indice2)
                         << QString(SpecChar::theta)+QString(SpecChar::indice1) << QString(SpecChar::theta)+QString(SpecChar::indice2) << "v"+QString(SpecChar::indice1) << "v"+QString(SpecChar::indice2)
                         << QString(SpecChar::omega)+QString(SpecChar::indice1) << QString(SpecChar::omega)+QString(SpecChar::indice2) << "a"+QString(SpecChar::indice1) << "a"+QString(SpecChar::indice2)
                         << QString(SpecChar::alpha)+QString(SpecChar::indice1) << QString(SpecChar::alpha)+QString(SpecChar::indice2) << QObject::tr("Kinetic energy") << QObject::tr("Potential energy")
                         << QObject::tr("Total energy") << QObject::tr("Lagrangian");

    //gets screen resolution and refresh rate
    QRect rec = QApplication::primaryScreen()->geometry();
    Screen::screenWidth = rec.width();
    Screen::screenHeight = rec.height();
    Screen::refreshRate = QApplication::primaryScreen()->refreshRate();


    //gets directory path
    path = QCoreApplication::applicationDirPath();
    path.append("/");

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
