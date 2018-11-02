#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H

#include <QRandomGenerator>
#include <QColor>
#include <cmath>
#include <array>
#include <vector>
#include "globalconstants.h"
#include "error.h"

class DoublePendulum : public QObject
{
    /* This class contains the data of the double pendulum and performs calculations on it.
     * It can receive parameters in different units and converts them to SI units.*/
    Q_OBJECT
public:
    DoublePendulum(const std::array<unsigned short, ArraySize::currentIndex>& indexArray, const std::array<unsigned short, ArraySize::currentUnit>& unitArray, QString name, QColor color = QColor(Qt::black));
    DoublePendulum(const DoublePendulum& pendulum, QString name, QColor color = QColor(Qt::black));
    DoublePendulum(DoublePendulum& pendulum);
    ~DoublePendulum();

    enum IntegrationMethod{ModEuler, Verlet, RK4, ModRK4, Leapfrog, Ruth};

    inline static double convertAngle(double angle) //removes exeeding cycles from the angle and converts it to make it be between -pi and pi instead of the classical [0, pi[
    {
        angle = fmod(angle, 2*pi);
        if (angle > pi)
        {
            angle -= 2*pi;
        }
        else if (angle <= -pi)
        {
            angle += 2*pi;
        }
        return angle;
    }

    void moveOneStep(); //makes the simulation move one step forward in time (time += dt)

    inline double f1(double angle1, double angularVelocity1) //calculates the first angular acceleration as a function of the first angular position and velocity
    {
        double sinAng1(sin(angle1));
        double sinAng2(sin(m_ang2));
        double sinDifAng12(sin(angle1-m_ang2));
        double cosDifAng12(cos(angle1-m_ang2));
        return (-m_m2*m_r2*m_angVel2*m_angVel2*sinDifAng12 - (m_m1+m_m2)*m_g*sinAng1 - m_m2*m_r1*angularVelocity1*angularVelocity1*sinDifAng12*cosDifAng12 + m_m2*m_g*sinAng2*cosDifAng12)/(m_r1*((m_m1+m_m2) - m_m2*cosDifAng12*cosDifAng12));
    }

    inline double f2(double angle2, double angularVelocity2) //caluclates the second angular acceleration as a function of the second angular position and velocity
    {
        double sinAng1(sin(m_ang1));
        double sinAng2(sin(angle2));
        double sinDifAng12(sin(m_ang1-angle2));
        double cosDifAng12(cos(m_ang1-angle2));
        return (-m_m2*m_r2*angularVelocity2*angularVelocity2*sinDifAng12*cosDifAng12 + (m_m1+m_m2)*(-m_g*sinAng1*cosDifAng12 - m_r1*m_angVel1*m_angVel1*sinDifAng12 + m_g*sinAng2))/(m_r2*(m_m2*cosDifAng12*cosDifAng12 - (m_m1+m_m2)));
    }

    inline void calculateAccelerations() //updates the value of each angular acceleration
    {
        double sinAng1(sin(m_ang1));
        double sinAng2(sin(m_ang2));
        double sinDifAng12(sin(m_ang1-m_ang2));
        double cosDifAng12(cos(m_ang1-m_ang2)); //precalculating trigonometric functions which are used multiple times drastically increases performance when compiler optimizations are disabled
        m_angAcc1 = (-m_m2*m_r2*m_angVel2*m_angVel2*sinDifAng12 - (m_m1+m_m2)*m_g*sinAng1 - m_m2*m_r1*m_angVel1*m_angVel1*sinDifAng12*cosDifAng12 + m_m2*m_g*sinAng2*cosDifAng12)/(m_r1*((m_m1+m_m2) - m_m2*cosDifAng12*cosDifAng12));
        m_angAcc2 = (-m_m2*m_r2*m_angVel2*m_angVel2*sinDifAng12*cosDifAng12 + (m_m1+m_m2)*(-m_g*sinAng1*cosDifAng12 - m_r1*m_angVel1*m_angVel1*sinDifAng12 + m_g*sinAng2))/(m_r2*(m_m2*cosDifAng12*cosDifAng12 - (m_m1+m_m2)));
    }

    void setPos(); //calculates the position of the pendulum in cartesian coordinates


    double angVelToSpeed1(double angularVelocity1, double radius1); //returns the module of the first speed
    double angVelToSpeed2(double angle1, double angularVelocity1, double radius1, double angle2, double angularVelocity2, double radius2); //returns the module of the second speed

    double angVelToSpeed2Squared(double angle1, double angularVelocity1, double radius1, double angle2, double angularVelocity2, double radius2); //Returns the square of the second speed. This function allows to perform optimizations.

    //accessors
    static unsigned short numberOfPendula();
    QString name();
    QColor color();

    double ang1() const;
    double ang2() const;
    double prevAng1() const;
    double prevAng2() const;
    double angVel1() const;
    double angVel2() const;
    double angAcc1() const;
    double angAcc2() const;
    double prevAngAcc1() const;
    double prevAngAcc2() const;
    double posX1();
    double posY1();
    double posX2();
    double posY2();
    double v1();
    double v2();
    double v2Squared();
    double acc1() const;
    double acc2( ) const;
    double m1() const;
    double m2() const;
    double r1() const;
    double r2() const;
    double g() const;
    double dt() const;
    int integrationMethod() const;
    bool wasVerlet() const;
    double time() const;
    double kinEnergy();
    double potEnergy();
    double totEnergy();
    double lagrangian();


    double initAng1() const;
    double initAng2() const;
    double initAngVel1() const;
    double initAngVel2() const;

    double measuredInitAng1() const;
    double measuredInitAng2() const;
    double measuredInitAngVel1() const;
    double measuredInitAngVel2() const;
    double initPosX1() const;
    double initPosY1() const;
    double initPosX2() const;
    double initPosY2() const;

    double measuredM1() const;
    double measuredM2() const;
    double measuredR1() const;
    double measuredR2() const;
    double measuredG() const;


    double ang1Uncertainty() const;
    double ang2Uncertainty() const;
    double angVel1Uncertainty() const;
    double angVel2Uncertainty() const;
    double m1Uncertainty() const;
    double m2Uncertainty() const;
    double r1Uncertainty() const;
    double r2Uncertainty() const;
    double gUncertainty() const;

    double parameter(int i, bool convertUnit = false); //convenience method which returns the nth parameter of the pendulum,
                                                       //it should be used with the enum Parameter::Enum or
                                                       //in a for loop to get all the parameters in a few lines
    QString unitSymbol(int i);
    const std::array<unsigned short, ArraySize::currentIndex>& indexArray() const;
    const std::array<unsigned short, ArraySize::currentUnit>& unitArray() const;


    QRandomGenerator randGenerator();
    bool restartIsDisabled() const;

    //mutators
    void setName(QString name);

    void setDt(double dt);

    void setDisableRestart(bool disableRestart);

    //other methods
    double absoluteMesurementError(double measuredValue, double relativeUncertainty);
private:
    static unsigned short m_numberOfPendula;
    QString m_name;
    QColor m_color;
//physical attributes
    //real values
    double m_ang1; //first angle
    double m_ang2; //second '    '
    double m_prevAng1;
    double m_prevAng2;
    double m_angVel1; //first angular velocity
    double m_angVel2; //second '              '
    double m_prevAngAcc1;
    double m_prevAngAcc2;
    double m_angAcc1; //first angular acceleration
    double m_angAcc2; //second '                   '
    double m_posX1;
    double m_posY1;
    double m_posX2;
    double m_posY2;
    double m_m1; //mass of the first pendulum
    double m_m2; //'          'second'       '
    double m_r1; //length of the first pendulum
    double m_r2; //'            'second'       '
    double m_time;
    double m_dt;
    double m_g;

    //real initial values
    double m_initAng1;
    double m_initAng2;
    double m_initAngVel1;
    double m_initAngVel2;

    //measured values
    double m_measuredInitAng1;
    double m_measuredInitAng2;
    double m_measuredInitAngVel1;
    double m_measuredInitAngVel2;
    double m_measuredM1;
    double m_measuredM2;
    double m_measuredR1;
    double m_measuredR2;
    double m_measuredG;

    double m_initPosX1;
    double m_initPosY1;
    double m_initPosX2;
    double m_initPosY2;

    //percents uncertainty
    double m_ang1Uncertainty;
    double m_ang2Uncertainty;
    double m_angVel1Uncertainty;
    double m_angVel2Uncertainty;
    double m_m1Uncertainty;
    double m_m2Uncertainty;
    double m_r1Uncertainty;
    double m_r2Uncertainty;
    double m_gUncertainty;

    QRandomGenerator m_randGenerator;

    void addUncertainty(); //private method, adds some randomness to the mesures according to the uncertainties

    //other attributes
    const std::array<unsigned short, ArraySize::currentIndex>& m_currentIndex;
    const std::array<unsigned short, ArraySize::currentUnit>& m_currentUnit;
    bool m_disableRestart;

    IntegrationMethod m_integrationMethod;
    bool m_wasVerlet;

    double const m_c[4];
    double const m_d[4];


public slots:
    //slots related to widgets
    void setColor(QColor color);

    void setInitPosX1(double x);
    void setInitPosY1(double y);
    void setInitPosX2(double x);
    void setInitPosY2(double y);

    void setIntegrationMethod(int integrationMethod);

    void changeAngle(double ang); //changes the value of m_ang1 or m_ang2 depending on the current angle index after converting the new value to radians
    void changeAngularVelocity(double vel); //changes the value of m_angVel1 or m_angVel2 depending on the current angular velocity index after converting the new value to radians per second
    void changeRadius(double radius); //changes the value of m_r1 or m_r2 depending on the current radius index after converting the new value to meters
    void changeMass(double mass); //changes the value of m_m1 or m_m2 depending on the current mass index after converting the new value to kilograms
    void changeGravity(double g); //changes the value of m_g after converting the new value to meters per second squared
    void changeDt(double dt); //changes the value of m_dt after converting the new value to seconds

    void setAngleUncertainty(double percentUncertainty);
    void setAngularVelocityUncertainty(double percentUncertainty);
    void setRadiusUncertainty(double percentUncertainty);
    void setMassUncertainty(double percentUncertainty);
    void setGravityUncertainty(double percentUncertainty);

    void restart(); //restarts the double pendulum


signals:
    void updateSignal(); //signal emitted whenever an initial condition is adjusted because of a change in another condition
    void restarted(); //signal emitted after the double pendulum has been restarted
    void dtValueChanged(double newValue); //signal emitted after the value of m_dt has been changed
    void integrationMethodChanged(int integrationMethod); //signal emitted after the method of integration has been changed
};

#endif // DOUBLEPENDULUM_H
