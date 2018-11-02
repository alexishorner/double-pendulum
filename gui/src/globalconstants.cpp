#include "globalconstants.h"

double const pi(3.1415926536);

QString path("");

namespace Prefix
{
double const micro(1e-6);
double const deci(1e-2);
}

namespace Screen
{
int screenWidth(1920);
int screenHeight(1080);
double refreshRate(60);
}

namespace DefaultParameterValue
{
int const dtDecimals(9);
int const defaultDecimals(6);
double const dtStep(0.000001);
double const uncertaintyStep(0.05);
double const defaultStep(0.01);
double const maxAng(pi);
double const minRadius(0.05);
double const maxRadius(10.0);
double const minDt(0.00000001);
double const maxDt(1.0/100.0);
double const defaultMin(0.01);
double const defaultMax(50.0);
double const maxRelativeUncertainty(0.1);
int const uncertaintyDecimals(6);
}

#ifndef __linux__
namespace Quality
{
int const numberOfSamples[] = {1, 3, 7};
}
#endif

namespace Parameter//Here the strings are initialited with their default value, but are translated in the void main(int argc, char* argv[]) function
{
int const count(19);
QStringList namesList = QStringList() << "Time" << "x"+QString(SpecChar::indice1) << "y"+QString(SpecChar::indice1) << "x"+QString(SpecChar::indice2) << "y"+QString(SpecChar::indice2)
                                      << QString(SpecChar::theta)+QString(SpecChar::indice1) << QString(SpecChar::theta)+QString(SpecChar::indice2) << "v"+QString(SpecChar::indice1) << "v"+QString(SpecChar::indice2)
                                      << QString(SpecChar::omega)+QString(SpecChar::indice1) << QString(SpecChar::omega)+QString(SpecChar::indice2) << "a"+QString(SpecChar::indice1) << "a"+QString(SpecChar::indice2)
                                      << QString(SpecChar::alpha)+QString(SpecChar::indice1) << QString(SpecChar::alpha)+QString(SpecChar::indice2) << "Kinetic energy" << "Potential energy"
                                      << "Total energy" << "Lagrangian";
}

namespace UnitConversion
{
double const toRad[] = {1.0, pi/180.0, pi/200.0, 2*pi};
double const toRadps[] = {1.0, pi/180.0, pi/200.0, 2*pi/60.0, 2*pi};
double const toM[] = {1.0, 0.0254, 0.3048, 0.9144};
double const toKg[] = {1.0, 1.0/1000.0, 0.45359237, 0.28349523125};
double const toMps2[] = {1.0, 9.80665, 0.44704, 0.3048};
double const toS[] = {1.0, 1.0/1000.0, 1.0/1000000.0};

double const mToPixels(200.0);
int const kgToPixels(10.0);
}

namespace SpecChar
{
QChar const plusMinus(0x00B1);
QChar const percent(0x0025);
QChar const squared(0x00B2); //special character for ^2 (exponent)
QChar const indice0(0x2080); //special character for 0 (indice)
QChar const indice1(0x2081);
QChar const indice2(0x2082);
QChar const alpha(0x03B1); //greek letter alpha
QChar const theta(0x03B8); //greek letter theta
QChar const mu(0x03BC); //greek letter mu
//if possible add tau
QChar const omega(0x03C9); //greek letter omega
}

namespace TranslatedStrings //Here the strings are initialited with their default value, but are translated in the void main(int argc, char* argv[]) function
{
QString pendulum("pendulum");
QString fileName("data sample");
QString chooseFile("Choose file");
QString error("Error");
}

//  /!\ WARNING /!\ The units in each list below must be in the same order as in the conversion arrays, otherwhise there will be wrong conversions

namespace UnitList
{
QStringList const unitsOfAngle({"rad", "°", "gon", "tr"});
QStringList const unitsOfAngularVelocity({"rad/s", "°/s", "gon/s", "rpm", "Hz"});
QStringList const unitsOfLength({"m", "in", "ft", "yd"});
QStringList const unitsOfMass({"kg", "g", "lb av", "oz av"});
QStringList const unitsOfAcceleration({"m/s"+QString(SpecChar::squared), "g"+QString(SpecChar::indice0),
                                       "mph/s", "ft/s"+QString(SpecChar::squared)});
QStringList const unitsOfTime({"s", "ms", QString(SpecChar::mu)+"s"});

QStringList const defaultUnits({"s", "m", "m", "m", "m", "rad", "rad", "m/s", "m/s", "rad/s", "rad/s",
                                QString("m/s")+QString(SpecChar::squared), QString("m/s")+QString(SpecChar::squared),
                                QString("rad/s")+QString(SpecChar::squared), QString("rad/s")+QString(SpecChar::squared),
                                "J", "J", "J", "J"});
}

namespace Color 
{
QList<QColor> const pendulumColor({QColor(0, 0, 0), QColor(230, 25, 75), QColor(60, 180, 75), QColor(255, 225, 25), QColor(0, 130, 200),
                             QColor(150, 60, 20), QColor(145, 30, 180), QColor(70, 240, 240), QColor(240, 50, 230), QColor(220, 70, 40),
                             QColor(0, 128, 128), QColor(0, 190, 0), QColor(170, 110, 40), QColor(128, 0, 0), QColor(20, 250, 0),
                             QColor(128, 128, 0), QColor(60, 15, 180), QColor(140, 80, 90), QColor(90, 50, 100), QColor(143, 200, 61)});
}
