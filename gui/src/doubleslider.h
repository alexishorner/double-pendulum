#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

/*This class is a lightweight wrapper around a QSlider to enable double values instead of int*/

#include <QSlider>
#include <cmath>
#include "globalconstants.h"

class DoubleSlider : public QSlider
{
    /* This class is used to create sliders that can handle floating point values.
     * It is a wrapper around the class QSlider.*/
    Q_OBJECT
public:
    DoubleSlider(double value, double minValue, double maxValue, unsigned int precision=DefaultParameterValue::defaultDecimals-2, Qt::Orientation orientation=Qt::Horizontal);
    void changeRange(double min, double max); //Changes the range of the slider. Differs from setRange by the fact that it handles floating point values.
private:
    double m_minValue;
    double m_maxValue;
    unsigned int m_precision;
    double m_multiplier;
public slots:
    void changeValue(double newValue); //changes the value of the slider after converting it to an integer
    void intToDouble(int a);
    int doubleToInt(double a);
signals:
    void doubleValueChanged(double newValue);
};

#endif // DOUBLESLIDER_H
