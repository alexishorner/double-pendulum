#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include <QStringList>
#include <QColor>
#include <QApplication>

/* This file contains data that can be used by any class.
 * The objects created here are not supposed to get modified
 * unless in the main function, at the beginning of execution.*/

extern double const pi;

extern QString path;

namespace Prefix
{
extern double const micro;
extern double const deci;
}

namespace Screen
{
extern int screenWidth;
extern int screenHeight;
extern double refreshRate;
}

namespace DefaultParameterValue
{
extern int const dtDecimals;
extern int const defaultDecimals;
extern double const dtStep;
extern double const uncertaintyStep;
extern double const defaultStep;
extern double const maxAng;
extern double const minRadius;
extern double const maxRadius;
extern double const minDt;
extern double const maxDt;
extern double const defaultMin;
extern double const defaultMax;
extern double const maxRelativeUncertainty;
extern int const uncertaintyDecimals;
}

namespace Quality
{
enum Enum{low, balanced, high};
#ifndef __linux__
extern int const numberOfSamples[3];
#endif
}

namespace ArraySize
{
enum Enum{currentIndex = 4, currentUnit = 6};
}

namespace Index
{
enum Enum{angle, angVel, radius, mass};
}
namespace UnitOf
{
enum Enum{angle, angVel, length, mass, acc, time};
}
namespace Parameter
{
enum Enum{time, x1, y1, x2, y2, ang1, ang2, v1, v2, angVel1, angVel2, acc1, acc2, angAcc1, angAcc2, K, P, totE, L};
extern int const count;
extern QStringList namesList;
}

namespace UnitConversion
{
extern double const toRad[];  //array storing the number of radians required to make one unit of the selected unit                       '
extern double const toRadps[];//'                          'radians per second'                                  '
extern double const toM[];    //'                          'meters'                                              '
extern double const toKg[];   //'                          'kilograms'                                           '
extern double const toMps2[]; //'                          'meters per second squared'                           '
extern double const toS[];    //'                          'seconds'                                             '

extern double const mToPixels;
extern int const kgToPixels;
}

namespace SpecChar
{
extern QChar const plusMinus;
extern QChar const percent;
extern QChar const squared; //special character for ^2 (exponent)
extern QChar const indice0; // special character for _0 (indice)
extern QChar const indice1;
extern QChar const indice2;
extern QChar const alpha; //greek letter alpha
extern QChar const theta; //greek letter theta
extern QChar const mu; //greek letter mu
//if possible add tau
extern QChar const omega; //greek letter omega
}

namespace TranslatedStrings
{
extern QString pendulum;
extern QString fileName;
extern QString chooseFile;
extern QString error;
}

namespace UnitList
{
extern QStringList const unitsOfAngle;
extern QStringList const unitsOfAngularVelocity;
extern QStringList const unitsOfLength;
extern QStringList const unitsOfMass;
extern QStringList const unitsOfAcceleration;
extern QStringList const unitsOfTime;

extern QStringList const defaultUnits;
}

namespace Limitations
{
enum {maxNumberOfPendula = 20};
}

namespace Color
{
extern QList<QColor> const pendulumColor;
}
#endif // GLOBALCONSTANTS_H

