#include "doubleslider.h"
DoubleSlider::DoubleSlider(double value, double minValue, double maxValue, unsigned int precision, Qt::Orientation orientation):
    m_minValue(minValue), m_maxValue(maxValue), m_precision(precision), m_multiplier(pow(10, precision))
{
    setOrientation(orientation);
    setRange(doubleToInt(minValue), doubleToInt(maxValue));
    setValue(doubleToInt(value));
    setMinimumWidth(80);
    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(intToDouble(int))); //converts the value to double and then emits a signal with the double value
}

void DoubleSlider::changeRange(double min, double max)
{
    setRange(doubleToInt(min), doubleToInt(max));
}

void DoubleSlider::changeValue(double newValue)
{
    QSignalBlocker b(this);
    setValue(doubleToInt(newValue));
    b.~QSignalBlocker();
}

void DoubleSlider::intToDouble(int a)
{
    emit doubleValueChanged(double(a)/m_multiplier);
}
int DoubleSlider::doubleToInt(double a)
{
    return round(a*m_multiplier);
}
