#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QValidator>

class DoubleSpinBox : public QDoubleSpinBox
{
    /* The only purpose of this class is to prevent Qt from using a coma
     * instead of a point as decimal separator.*/
    Q_OBJECT
public:
    DoubleSpinBox(QWidget* parent = nullptr);
    ~DoubleSpinBox();

    QValidator::State validate(QString& input, int& pos) const;
    QString convertToLocale(QString text) const;
    QString textFromValue(double value) const;
    double valueFromText(const QString& text) const;

    /*void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;*/
    /*void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;*/


/*private:
    //bool m_LRArrowIsPressed;
public slots:
    void removeTraillingZeroes();*/
};

#endif // DOUBLESPINBOX_H
