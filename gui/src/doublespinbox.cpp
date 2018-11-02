#include "doublespinbox.h"

DoubleSpinBox::DoubleSpinBox(QWidget* parent): QDoubleSpinBox(parent)//, m_LRArrowIsPressed(false)
{
    setKeyboardTracking(false);
    setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
    //this->lineEdit()->setCursorPosition(text().length()); //insures that the position of the cursor will be changed during the focus in event
    //QObject::connect(this->lineEdit(), SIGNAL(cursorPositionChanged(int,int)), this, SLOT(removeTraillingZeroes()));
}

DoubleSpinBox::~DoubleSpinBox()
{
}

QValidator::State DoubleSpinBox::validate(QString& input, int& pos) const
{
    QString localText(convertToLocale(input));
    return this->QDoubleSpinBox::validate(localText, pos);
}

QString DoubleSpinBox::convertToLocale(QString text) const
{
    if(text.contains('.'))
    {
        text.replace(QLatin1Char('.'), QLocale().decimalPoint());
    }
    else
    {
        text.replace(QLatin1Char(','), QLocale().decimalPoint());
    }
    return text;
}

QString DoubleSpinBox::textFromValue(double value) const
{
    QString text = QDoubleSpinBox::textFromValue(value);
    return text.replace(QLocale().decimalPoint(), QLatin1Char('.'));
}

double DoubleSpinBox::valueFromText(const QString& text) const
{
    bool ok(false);
    QString changedText(convertToLocale(text));
    changedText.remove(prefix());
    changedText.remove(suffix());
    changedText.remove(" "); //removes leading and trailing whitespaces
    double val = QLocale().toDouble(changedText, &ok);

    if(ok) //success
    {
        return val; //changes the value
    }
    else //error
    {
        return value(); //doesn't change the value
    }
}

/*void DoubleSpinBox::focusInEvent(QFocusEvent* event)
{
    //QObject::connect(this->lineEdit(), SIGNAL(cursorPositionChanged(int,int)), this, SLOT(removeTraillingZeroes()));
    this->QDoubleSpinBox::focusInEvent(event);
    event->accept();
}*/

/*void DoubleSpinBox::focusOutEvent(QFocusEvent* event)
{
    setValue(valueFromText(text())); //saves the value and restores the trailling zeroes
    //QSignalBlocker b(this->lineEdit());
    //this->lineEdit()->setCursorPosition(text().length()); //insures that the position of the cursor will be changed during the focus in event
    //b.~QSignalBlocker();
    this->QDoubleSpinBox::focusOutEvent(event);
    event->accept();
}*/

/*void DoubleSpinBox::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_Right:
    {
        m_LRArrowIsPressed = true;
        QString valueText = text();
        valueText.remove(suffix());
        if(valueText.back() == ' ')
        {
            valueText.chop(1);
        }
        valueText = convertToLocale(valueText);
        if(lineEdit()->cursorPosition() == valueText.length())
        {
            if(valueText.contains(QLocale().decimalPoint()))
            {
                int decimals = valueText.split(QLocale().decimalPoint()).at(1).length();
                if(decimals < this->decimals())
                {
                    valueText.append('0');
                }
            }
            else
            {
                valueText.append(QLocale().decimalPoint());
            }
        }

        if(!valueText.contains(suffix()))
        {
            valueText.append(suffix());
        }
        valueText.replace(QLocale().decimalPoint(), QLatin1Char('.'));


        int cursorPostion = lineEdit()->cursorPosition();
        QSignalBlocker b(this->lineEdit()); //blocks signals to prevent an infinite loop
        lineEdit()->setText(valueText);
        lineEdit()->setCursorPosition(cursorPostion); //replaces the cursor to where it was, the base class's keyPressEvent will move it later
        b.~QSignalBlocker(); //unblocks signals
        break;
    }
    case Qt::Key_Left:
        m_LRArrowIsPressed = true;
        break;
    }
    this->QDoubleSpinBox::keyPressEvent(event);
}*/

/*void DoubleSpinBox::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_Right:
        m_LRArrowIsPressed = false;
        break;
    case Qt::Key_Left:
        m_LRArrowIsPressed = false;
        break;
    }
    this->QDoubleSpinBox::keyReleaseEvent(event);
}*/

/*void DoubleSpinBox::removeTraillingZeroes()
{
    //QObject::disconnect(this->lineEdit(), SIGNAL(cursorPositionChanged(int,int)), this, SLOT(removeTraillingZeroes()));
    QString valueText = text();
    valueText.remove(suffix());
    if(valueText.back() == ' ')
    {
        valueText.chop(1);
    }
    valueText = convertToLocale(valueText);

    if(valueText.length() > 0)
    {
        //bool condition = valueText.length() > lineEdit()->cursorPosition()
          //          && (valueText.at(valueText.length()-1) == '0'
            //            || valueText.at(valueText.length()-1) == QLocale().decimalPoint())
              //      && !(m_LRArrowIsPressed && valueText.at(valueText.length()-1) == QLocale().decimalPoint());
        while(valueText.contains(QLocale().decimalPoint())
              && valueText.length() > lineEdit()->cursorPosition()
              && (valueText.at(valueText.length()-1) == '0'
                  || valueText.at(valueText.length()-1) == QLocale().decimalPoint()))  //removes zeroes that are after the decimal point and after the cursor
        {
            valueText.chop(1);
            //condition = valueText.length() > 0;
            //if(condition)
            //{
              //  condition = valueText.length() > lineEdit()->cursorPosition()
                //        && (valueText.at(valueText.length()-1) == '0'
                  //          || valueText.at(valueText.length()-1) == QLocale().decimalPoint())
                    //    && !(m_LRArrowIsPressed && valueText.at(valueText.length()-1) == QLocale().decimalPoint());
            //}
        }
    }

    if(!valueText.contains(suffix()))
    {
        valueText.append(suffix());
    }
    valueText.replace(QLocale().decimalPoint(), QLatin1Char('.'));


    int cursorPostion = lineEdit()->cursorPosition();
    lineEdit()->setText(valueText);
    lineEdit()->setCursorPosition(cursorPostion);
    //QSignalBlocker b(this->lineEdit()); //blocks signals to prevent an infinite loop
    //lineEdit()->setText(valueText);
    //lineEdit()->setCursorPosition(cursorPostion); //replaces the cursor to where it was
    //b.~QSignalBlocker(); //unblocks signals
}*/
