#include "doublependulum.h"

unsigned short DoublePendulum::m_numberOfPendula = 0;

DoublePendulum::DoublePendulum(const std::array<unsigned short, ArraySize::currentIndex>& indexArray, const std::array<unsigned short, ArraySize::currentUnit>& unitArray, QString name, QColor color): QObject(), m_currentIndex(indexArray), m_currentUnit(unitArray), m_c{0.6756035959798288170238, -0.1756035959798288170238, -0.1756035959798288170238, 0.6756035959798288170238}, m_d{1.351207191959657634048, -1.702414383919315268095, 1.351207191959657634048, 0.0}
{
    m_numberOfPendula++;

    m_name = name;
    m_color = color;

    m_ang1 = convertAngle(2);
    m_ang2 = convertAngle(3);
    m_angVel1 = 0;
    m_angVel2 = 0;
    m_m1 = 2;
    m_m2 = 2;
    m_r1 = 1;
    m_r2 = 1;
    m_time = 0;
    m_dt = 0.00001;
    m_g = UnitConversion::toMps2[1]; //exact g_0 value (~9.81)

    m_integrationMethod = ModEuler;
    m_wasVerlet = false;


    calculateAccelerations();
    m_prevAngAcc1 = m_angAcc1;
    m_prevAngAcc2 = m_angAcc2;

    setPos();

    m_initAng1=m_ang1;
    m_initAng2=m_ang2;
    m_initAngVel1=m_angVel1;
    m_initAngVel2=m_angVel2;
    m_initPosX1=m_posX1;
    m_initPosY1=m_posY1;
    m_initPosX2=m_posX2;
    m_initPosY2=m_posY2;


    m_measuredInitAng1=m_ang1;
    m_measuredInitAng2=m_ang2;
    m_measuredInitAngVel1=m_angVel1;
    m_measuredInitAngVel2=m_angVel2;
    m_measuredM1=m_m1;
    m_measuredM2=m_m2;
    m_measuredR1=m_r1;
    m_measuredR2=m_r2;
    m_measuredG=m_g;


    m_ang1Uncertainty = 0;
    m_ang2Uncertainty = 0;
    m_angVel1Uncertainty = 0;
    m_angVel2Uncertainty = 0;
    m_m1Uncertainty = 0;
    m_m2Uncertainty = 0;
    m_r1Uncertainty = 0;
    m_r2Uncertainty = 0;
    m_gUncertainty = 0;

    m_randGenerator = QRandomGenerator(QRandomGenerator::global()->generate());

    m_disableRestart = false;
}

DoublePendulum::DoublePendulum(const DoublePendulum& pendulum, QString name, QColor color): QObject(), m_currentIndex(pendulum.indexArray()), m_currentUnit(pendulum.unitArray()), m_c{0.6756035959798288170238, -0.1756035959798288170238, -0.1756035959798288170238, 0.6756035959798288170238}, m_d{1.351207191959657634048, -1.702414383919315268095, 1.351207191959657634048, 0.0}
{
    m_numberOfPendula++;

    m_name = name;
    m_color = color;

    m_ang1 = pendulum.measuredInitAng1();
    m_ang2 = pendulum.measuredInitAng2();
    m_angVel1 = pendulum.measuredInitAngVel1();
    m_angVel2 = pendulum.measuredInitAngVel2();
    m_m1 = pendulum.measuredM1();
    m_m2 = pendulum.measuredM2();
    m_r1 = pendulum.measuredR1();
    m_r2 = pendulum.measuredR2();
    m_time = 0;
    m_dt = pendulum.dt();
    m_g = pendulum.measuredG();

    m_integrationMethod = static_cast<IntegrationMethod>(pendulum.integrationMethod());
    m_wasVerlet = false;


    calculateAccelerations();
    m_prevAngAcc1 = m_angAcc1;
    m_prevAngAcc2 = m_angAcc2;

    setPos();

    m_initAng1=m_ang1;
    m_initAng2=m_ang2;
    m_initAngVel1=m_angVel1;
    m_initAngVel2=m_angVel2;
    m_initPosX1=m_posX1;
    m_initPosY1=m_posY1;
    m_initPosX2=m_posX2;
    m_initPosY2=m_posY2;


    m_measuredInitAng1=m_ang1;
    m_measuredInitAng2=m_ang2;
    m_measuredInitAngVel1=m_angVel1;
    m_measuredInitAngVel2=m_angVel2;
    m_measuredM1=m_m1;
    m_measuredM2=m_m2;
    m_measuredR1=m_r1;
    m_measuredR2=m_r2;
    m_measuredG=m_g;

    m_ang1Uncertainty = pendulum.ang1Uncertainty();
    m_ang2Uncertainty = pendulum.ang2Uncertainty();
    m_angVel1Uncertainty = pendulum.angVel2Uncertainty();
    m_angVel2Uncertainty = pendulum.angVel2Uncertainty();
    m_m1Uncertainty = pendulum.m1Uncertainty();
    m_m2Uncertainty = pendulum.m2Uncertainty();
    m_r1Uncertainty = pendulum.r1Uncertainty();
    m_r2Uncertainty = pendulum.r2Uncertainty();
    m_gUncertainty = pendulum.gUncertainty();

    m_randGenerator = QRandomGenerator(QRandomGenerator::global()->generate());

    m_disableRestart = false;

    restart();
}

DoublePendulum::DoublePendulum(DoublePendulum& pendulum): QObject(), m_currentIndex(pendulum.indexArray()), m_currentUnit(pendulum.unitArray()), m_c{0.6756035959798288170238, -0.1756035959798288170238, -0.1756035959798288170238, 0.6756035959798288170238}, m_d{1.351207191959657634048, -1.702414383919315268095, 1.351207191959657634048, 0.0}
{
    m_name = pendulum.name();
    m_color = pendulum.color();
    m_ang1 = pendulum.ang1();
    m_ang2 = pendulum.ang2();
    m_prevAng1 = pendulum.prevAng1();
    m_prevAng2 = pendulum.prevAng2();
    m_angVel1 = pendulum.angVel1();
    m_angVel2 = pendulum.angVel2();
    m_angAcc1 = pendulum.angAcc1();
    m_angAcc2 = pendulum.angAcc2();
    m_prevAngAcc1 = pendulum.prevAngAcc1();
    m_prevAngAcc2 = pendulum.prevAngAcc2();
    m_m1 = pendulum.m1();
    m_m2 = pendulum.m2();
    m_r1 = pendulum.r1();
    m_r2 = pendulum.r2();
    m_time = pendulum.time();
    m_dt = pendulum.dt();
    m_g = pendulum.g();

    m_posX1 = pendulum.posX1();
    m_posY1 = pendulum.posY1();
    m_posX2 = pendulum.posX2();
    m_posY2 = pendulum.posY2();

    m_integrationMethod = static_cast<IntegrationMethod>(pendulum.integrationMethod());
    m_wasVerlet = pendulum.wasVerlet();

    m_initAng1 = pendulum.initAng1();
    m_initAng2 = pendulum.initAng2();
    m_initAngVel1 = pendulum.initAngVel1();
    m_initAngVel2 = pendulum.initAngVel2();
    m_initPosX1 = pendulum.initPosX1();
    m_initPosY1 = pendulum.initPosY1();
    m_initPosX2 = pendulum.initPosX2();
    m_initPosY2 = pendulum.initPosY2();


    m_measuredInitAng1 = pendulum.measuredInitAng1();
    m_measuredInitAng2 = pendulum.measuredInitAng2();
    m_measuredInitAngVel1 = pendulum.measuredInitAngVel1();
    m_measuredInitAngVel2 = pendulum.measuredInitAngVel2();
    m_measuredM1 = pendulum.measuredM1();
    m_measuredM2 = pendulum.measuredM2();
    m_measuredR1 = pendulum.measuredR1();
    m_measuredR2 = pendulum.measuredR2();
    m_measuredG = pendulum.measuredG();

    m_ang1Uncertainty = pendulum.ang1Uncertainty();
    m_ang2Uncertainty = pendulum.ang2Uncertainty();
    m_angVel1Uncertainty = pendulum.angVel2Uncertainty();
    m_angVel2Uncertainty = pendulum.angVel2Uncertainty();
    m_m1Uncertainty = pendulum.m1Uncertainty();
    m_m2Uncertainty = pendulum.m2Uncertainty();
    m_r1Uncertainty = pendulum.r1Uncertainty();
    m_r2Uncertainty = pendulum.r2Uncertainty();
    m_gUncertainty = pendulum.gUncertainty();

    m_randGenerator = pendulum.randGenerator();

    m_disableRestart = pendulum.restartIsDisabled();
}

DoublePendulum::~DoublePendulum()
{
    m_numberOfPendula--;
}

void DoublePendulum::moveOneStep() //makes the time move one step forward (or backwards if m_dt has a negative value)
{
    switch(m_integrationMethod)
    {
    case ModEuler:
    {
        m_wasVerlet = false;
        m_prevAngAcc1 = m_angAcc1; //possible bug when the anguar acceleration is changed (e.g. when the length of one rod is changed)
        m_prevAngAcc2 = m_angAcc2;

        calculateAccelerations();

        m_ang1 += m_angVel1*m_dt + 0.5*m_angAcc1*m_dt*m_dt;
        m_ang2 += m_angVel2*m_dt + 0.5*m_angAcc2*m_dt*m_dt;

        m_angVel1 += m_angAcc1*m_dt;
        m_angVel2 += m_angAcc2*m_dt;


        if(m_ang1 > pi || m_ang1 <= -pi)
        {
            m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
        }
        if(m_ang2 > pi || m_ang2 <= -pi)
        {
            m_ang2 = convertAngle(m_ang2);
        }
        break;
    }
    case Verlet:
    {
        calculateAccelerations();

        double tempAng1(m_ang1);
        double tempAng2(m_ang2);

        if(!m_wasVerlet) //if first time verlet
        {
            m_prevAng1 = m_ang1;
            m_prevAng2 = m_ang2;
            m_ang1 += m_angVel1*m_dt + 0.5*m_angAcc1*m_dt*m_dt;
            m_ang2 += m_angVel2*m_dt + 0.5*m_angAcc2*m_dt*m_dt;
            m_wasVerlet = true;
        }
        else
        {
            m_ang1 *= 2;
            m_ang1 += -m_prevAng1 + m_angAcc1*m_dt*m_dt;

            m_ang2 *= 2;
            m_ang2 += -m_prevAng2 + m_angAcc2*m_dt*m_dt;
        }

        bool ang1Adjusted(false);
        if(m_ang1 > pi || m_ang1 <= -pi)
        {
            m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
            ang1Adjusted = true;
        }
        bool ang2Adjusted(false);
        if(m_ang2 > pi || m_ang2 <= -pi)
        {
            m_ang2 = convertAngle(m_ang2);
            ang2Adjusted = true;
        }

        double deltaAng1(m_ang1 - m_prevAng1);
        if(ang1Adjusted)
        {
            if(m_ang1 < 0)
            {
                deltaAng1 += 2*pi;
            }
            else
            {
                deltaAng1 -= 2*pi;
            }
        }
        m_angVel1 = deltaAng1/(2*m_dt);
        m_prevAng1 = tempAng1;


        double deltaAng2(m_ang2 - m_prevAng2);
        if(ang2Adjusted)
        {
            if(m_ang2 < 0)
            {
                deltaAng2 += 2*pi;
            }
            else
            {
                deltaAng2 -= 2*pi;
            }
        }
        m_angVel2 = (deltaAng2)/(2*m_dt);
        m_prevAng2 = tempAng2;
        break;
    }
    case RK4:
    {
        m_wasVerlet = false;
        double k1, k2, k3, k4;
        double l1, l2, l3, l4;
        k1=f1(m_ang1, m_angVel1);
        k2=f1(m_ang1+m_dt/2*m_angVel1, m_angVel1+m_dt/2*k1);
        k3=f1(m_ang1+m_dt/2*m_angVel1+m_dt*m_dt/4*k1, m_angVel1+m_dt/2*k2);
        k4=f1(m_ang1+m_dt*m_angVel1+m_dt*m_dt/2*k2, m_angVel1+m_dt*k3);

        l1=f2(m_ang2, m_angVel2);
        l2=f2(m_ang2+m_dt/2*m_angVel2, m_angVel2+m_dt/2*l1);
        l3=f2(m_ang2+m_dt/2*m_angVel2+m_dt*m_dt/4*l1, m_angVel2+m_dt/2*l2);
        l4=f2(m_ang2+m_dt*m_angVel2+m_dt*m_dt/2*l2, m_angVel2+m_dt*l3);


        m_ang1 += m_angVel1*m_dt+m_dt*m_dt/6*(k1+k2+k3);
        if(m_ang1 > pi || m_ang1 <= -pi)
        {
            m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
        }
        m_angVel1 += m_dt/6*(k1+2*k2+2*k3+k4);
        m_angAcc1 = k1;

        m_ang2 += m_angVel2*m_dt+m_dt*m_dt/6*(l1+l2+l3);
        if(m_ang2 > pi || m_ang2 <= -pi)
        {
            m_ang2 = convertAngle(m_ang2);
        }
        m_angVel2 += m_dt/6*(l1+2*l2+2*l3+l4);
        m_angAcc2 = l1;
        break;
    }
    case ModRK4: //This is a modified version of the RK4 method. The difference between the two methods is the order in which the calculations are performed.
    {
        m_wasVerlet = false;
        double k1, k2, k3, k4;
        k1=f1(m_ang1, m_angVel1);
        k2=f1(m_ang1+m_dt/2*m_angVel1, m_angVel1+m_dt/2*k1);
        k3=f1(m_ang1+m_dt/2*m_angVel1+m_dt*m_dt/4*k1, m_angVel1+m_dt/2*k2);
        k4=f1(m_ang1+m_dt*m_angVel1+m_dt*m_dt/2*k2, m_angVel1+m_dt*k3);

        m_ang1 += m_angVel1*m_dt+m_dt*m_dt/6*(k1+k2+k3);
        if(m_ang1 > pi || m_ang1 <= -pi)
        {
            m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
        }
        m_angVel1 += m_dt/6*(k1+2*k2+2*k3+k4);
        m_angAcc1 = k1;

        k1=f2(m_ang2, m_angVel2);                                               //
        k2=f2(m_ang2+m_dt/2*m_angVel2, m_angVel2+m_dt/2*k1);                    //In the original method this block is calculated before setting m_ang1 and m_angVel1.
        k3=f2(m_ang2+m_dt/2*m_angVel2+m_dt*m_dt/4*k1, m_angVel2+m_dt/2*k2);     //
        k4=f2(m_ang2+m_dt*m_angVel2+m_dt*m_dt/2*k2, m_angVel2+m_dt*k3);         //

        m_ang2 += m_angVel2*m_dt+m_dt*m_dt/6*(k1+k2+k3);
        if(m_ang2 > pi || m_ang2 <= -pi)
        {
            m_ang2 = convertAngle(m_ang2);
        }
        m_angVel2 += m_dt/6*(k1+2*k2+2*k3+k4);
        m_angAcc2 = k1;
        break;
    }
    case Leapfrog:
    {
        m_wasVerlet = false;
        m_prevAngAcc1 = m_angAcc1;
        m_prevAngAcc2 = m_angAcc2;

        calculateAccelerations();

        m_ang1 += m_angVel1*m_dt + 0.5*m_prevAngAcc1*m_dt*m_dt;
        m_ang2 += m_angVel2*m_dt + 0.5*m_prevAngAcc2*m_dt*m_dt;

        m_angVel1 += 0.5*(m_prevAngAcc1+m_angAcc1)*m_dt;
        m_angVel2 += 0.5*(m_prevAngAcc2+m_angAcc2)*m_dt;

        if(m_ang1 > pi || m_ang1 <= -pi)
        {
            m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
        }
        if(m_ang2 > pi || m_ang2 <= -pi)
        {
            m_ang2 = convertAngle(m_ang2);
        }
        break;
    }
    case Ruth:
    {
        m_wasVerlet = false;
        for(int i(0); i<4; i++)
        {
            calculateAccelerations();

            m_angVel1 += m_angAcc1*m_c[i]*m_dt;
            m_ang1 += m_angVel1*m_d[i]*m_dt;
            if(m_ang1 > pi || m_ang1 <= -pi)
            {
                m_ang1 = convertAngle(m_ang1); //prevents the angle from being greater than pi or smaller than -pi
            }

            m_angVel2 += m_angAcc2*m_c[i]*m_dt;
            m_ang2 += m_angVel2*m_d[i]*m_dt;
            if(m_ang2 > pi || m_ang2 <= -pi)
            {
                m_ang2 = convertAngle(m_ang2);
            }
        }
        break;
    }
    }
    m_time += m_dt;
}

void DoublePendulum::setPos()
{
    m_posX1 = m_r1*sin(m_ang1);
    m_posY1 = -m_r1*cos(m_ang1);
    m_posX2 = m_posX1 + m_r2*sin(m_ang2);
    m_posY2 = m_posY1 - m_r2*cos(m_ang2);
}

double DoublePendulum::angVelToSpeed1(double angularVelocity1, double radius1)
{
    return angularVelocity1*radius1;
}

double DoublePendulum::angVelToSpeed2(double angle1, double angularVelocity1, double radius1, double angle2, double angularVelocity2, double radius2)
{
    return sqrt(angVelToSpeed2Squared(angle1, angularVelocity1, radius1, angle2, angularVelocity2, radius2));
}

double DoublePendulum::angVelToSpeed2Squared(double angle1, double angularVelocity1, double radius1, double angle2, double angularVelocity2, double radius2)
{
    return angularVelocity1*angularVelocity1*radius1*radius1
            +2*angularVelocity1*angularVelocity2*radius1*radius2*cos(angle1-angle2)
            +angularVelocity2*angularVelocity2*radius2*radius2;
}

unsigned short DoublePendulum::numberOfPendula()
{
    return m_numberOfPendula;
}

QString DoublePendulum::name()
{
    return m_name;
}

QColor DoublePendulum::color()
{
    return m_color;
}

void DoublePendulum::restart()
{
    if(!m_disableRestart)
    {
        addUncertainty();

        m_ang1 = m_initAng1;
        m_ang2 = m_initAng2;
        m_angVel1 = m_initAngVel1;
        m_angVel2 = m_initAngVel2;
        calculateAccelerations();
        m_posX1 = m_initPosX1;
        m_posY1 = m_initPosY1;
        m_posX2 = m_initPosX2;
        m_posY2 = m_initPosY2;
        m_time = 0;
        m_wasVerlet = false;
        emit updateSignal();
        emit restarted();
    }
}


//mutators
void DoublePendulum::setDt(double dt)
{
    m_dt = dt;
    m_wasVerlet = false;
    emit dtValueChanged(dt);
}


void DoublePendulum::setInitPosX1(double x)
{
    m_initPosX1 = x;
    m_measuredInitAng1 = convertAngle(pi/2+atan2(m_initPosY1, x));
    m_measuredR1 = sqrt(x*x+m_initPosY1*m_initPosY1);
    m_initPosX2 = m_initPosX1+m_r2*sin(m_initAng2);

    restart();
}
void DoublePendulum::setInitPosY1(double y)
{
    m_initPosY1 = y;
    m_measuredInitAng1 = convertAngle(pi/2+atan2(y, m_initPosX1));
    m_measuredR1 = sqrt(m_initPosX1*m_initPosX1+y*y);
    m_initPosY2 = m_initPosY1-m_r2*cos(m_initAng2);

    restart();
}
void DoublePendulum::setInitPosX2(double x)
{
    m_initPosX2 = x;
    m_measuredInitAng2 = convertAngle(pi/2+atan2(m_initPosY2-m_initPosY1, x-m_initPosX1));
    m_measuredR2 = sqrt((x-m_initPosX1)*(x-m_initPosX1)+(m_initPosY2-m_initPosY1)*(m_initPosY2-m_initPosY1));

    restart();
}
void DoublePendulum::setInitPosY2(double y)
{
    m_initPosY2 = y;
    m_measuredInitAng2 = convertAngle(pi/2+atan2(y-m_initPosY1, m_initPosX2-m_initPosX1));
    m_measuredR2 = sqrt((m_initPosX2-m_initPosX1)*(m_initPosX2-m_initPosX1)+(y-m_initPosY1)*(y-m_initPosY1));

    restart();
}

void DoublePendulum::setIntegrationMethod(int integrationMethod)
{
    m_integrationMethod = static_cast<IntegrationMethod>(integrationMethod);
    emit integrationMethodChanged(integrationMethod);
}


void DoublePendulum::setDisableRestart(bool disableRestart)
{
    m_disableRestart = disableRestart;
}

double DoublePendulum::absoluteMesurementError(double measuredValue, double relativeUncertainty)
{
    double mesurementError(0);
    double a(pow(10, DefaultParameterValue::uncertaintyDecimals+1));
    if(int(round(relativeUncertainty*a)) != 0)
    {
        mesurementError = (m_randGenerator.generateDouble()*2-1)*relativeUncertainty*measuredValue;
    }
    return mesurementError;
}

void DoublePendulum::addUncertainty()
{
    m_initAng1 = m_measuredInitAng1 + absoluteMesurementError(m_measuredInitAng1, m_ang1Uncertainty);
    m_initAng2 = m_measuredInitAng2 + absoluteMesurementError(m_measuredInitAng2, m_ang2Uncertainty);
    m_r1 = m_measuredR1 + absoluteMesurementError(m_measuredR1, m_r1Uncertainty);
    m_r2 = m_measuredR2 + absoluteMesurementError(m_measuredR2, m_r2Uncertainty);
    m_m1 = m_measuredM1 + absoluteMesurementError(m_measuredM1, m_m1Uncertainty);
    m_m2 = m_measuredM2 + absoluteMesurementError(m_measuredM2, m_m2Uncertainty);
    m_g = m_measuredG + absoluteMesurementError(m_measuredG, m_gUncertainty);

    m_initAngVel1 = m_measuredInitAngVel1 + absoluteMesurementError(m_measuredInitAngVel1, m_angVel1Uncertainty);

    m_initAngVel2 = m_measuredInitAngVel2 + absoluteMesurementError(m_measuredInitAngVel2, m_angVel2Uncertainty);

    m_initPosX1 = m_r1*sin(m_initAng1);
    m_initPosY1 = -m_measuredR1*cos(m_initAng1);
    m_initPosX2 = m_initPosX1 + m_r2*sin(m_initAng2);
    m_initPosY2 = m_initPosY1 - m_r2*cos(m_initAng2);
}




//slots related to widgets
void DoublePendulum::changeAngle(double ang)
{
    switch(m_currentIndex[Index::angle])
    {
    case 0:
        m_measuredInitAng1 = ang*UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
        break;
    case 1:
        m_measuredInitAng2 = ang*UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
        break;
    }

    restart();
}
void DoublePendulum::changeAngularVelocity(double vel)
{
    switch(m_currentIndex[Index::angVel])
    {
    case 0:
        m_measuredInitAngVel1 = vel*UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]];
        break;
    case 1:
        m_measuredInitAngVel2 = vel*UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]];
        break;
    }

    restart();
}
void DoublePendulum::changeRadius(double radius)
{
    switch(m_currentIndex[Index::radius])
    {
    case 0:
        m_measuredR1 = radius*UnitConversion::toM[m_currentUnit[UnitOf::length]];
        m_r1 = m_measuredR1 + absoluteMesurementError(m_measuredR1, m_r1Uncertainty);
        break;
    case 1:
        m_measuredR2 = radius*UnitConversion::toM[m_currentUnit[UnitOf::length]];
        m_r2 = m_measuredR2 + absoluteMesurementError(m_measuredR2, m_r2Uncertainty);
        break;
    }

    m_initPosX1 = m_r1*sin(m_initAng1);
    m_initPosY1 = -m_r1*cos(m_initAng1);
    m_initPosX2 = m_initPosX1 + m_r2*sin(m_initAng2);
    m_initPosY2 = m_initPosY1 - m_r2*cos(m_initAng2);
    setPos();
    calculateAccelerations();

    emit updateSignal();

}
void DoublePendulum::changeMass(double mass)
{
    switch(m_currentIndex[Index::mass])
    {
    case 0:
        m_measuredM1 = mass*UnitConversion::toKg[m_currentUnit[UnitOf::mass]];
        m_m1 = m_measuredM1 + absoluteMesurementError(m_measuredM1, m_m1Uncertainty);
        break;
    case 1:
        m_measuredM2 = mass*UnitConversion::toKg[m_currentUnit[UnitOf::mass]];
        m_m2 = m_measuredM2 + absoluteMesurementError(m_measuredM2, m_m2Uncertainty);
        break;
    }
    calculateAccelerations();
    emit updateSignal();
}
void DoublePendulum::changeGravity(double g)
{
    m_measuredG = g*UnitConversion::toMps2[m_currentUnit[UnitOf::acc]];
    m_g = m_measuredG + absoluteMesurementError(m_measuredG, m_gUncertainty);
    calculateAccelerations();
}
void DoublePendulum::changeDt(double dt)
{
    if(m_dt > 0)
    {
        m_dt = dt*UnitConversion::toS[m_currentUnit[UnitOf::time]];
    }
    else
    {
        m_dt = -dt*UnitConversion::toS[m_currentUnit[UnitOf::time]];
    }
    m_wasVerlet = false;
    emit dtValueChanged(m_dt);
}

void DoublePendulum::setAngleUncertainty(double percentUncertainty) //WARNING: this function takes a percentage as an argument, this implies that a value of 100 means 1, because (100%=1).
{
    switch(m_currentIndex[Index::angle])
    {
    case 0:
        m_ang1Uncertainty = percentUncertainty/100.0;
        break;
    case 1:
        m_ang2Uncertainty = percentUncertainty/100.0;
        break;
    }

    restart();
}

void DoublePendulum::setAngularVelocityUncertainty(double percentUncertainty) //WARNING: this function takes a percentage as an argument, this implies that a value of 100 means 1, because (100%=1).
{
    switch(m_currentIndex[Index::angVel])
    {
    case 0:
        m_angVel1Uncertainty = percentUncertainty/100.0;
        break;
    case 1:
        m_angVel2Uncertainty = percentUncertainty/100.0;
        break;
    }
    restart();
}

void DoublePendulum::setRadiusUncertainty(double percentUncertainty) //WARNING: this function takes a percentage as an argument, this implies that a value of 100 means 1, because (100%=1).
{
    switch(m_currentIndex[Index::radius])
    {
    case 0:
        m_r1Uncertainty = percentUncertainty/100.0;
        m_r1 = m_measuredR1 + absoluteMesurementError(m_measuredR1, m_r1Uncertainty);
        break;
    case 1:
        m_r2Uncertainty = percentUncertainty/100.0;
        m_r2 = m_measuredR2 + absoluteMesurementError(m_measuredR2, m_r2Uncertainty);
        break;
    }

    m_initPosX1 = m_r1*sin(m_initAng1);
    m_initPosY1 = -m_r1*cos(m_initAng1);
    m_initPosX2 = m_initPosX1 + m_r2*sin(m_initAng2);
    m_initPosY2 = m_initPosY1 - m_r2*cos(m_initAng2);

    setPos();
    emit updateSignal();
}

void DoublePendulum::setMassUncertainty(double percentUncertainty) //WARNING: this function takes a percentage as an argument, this implies that a value of 100 means 1, because (100%=1).
{
    switch(m_currentIndex[Index::mass])
    {
    case 0:
        m_m1Uncertainty = percentUncertainty/100.0;
        m_m1 = m_measuredM1 + absoluteMesurementError(m_measuredM1, m_m1Uncertainty);
        break;
    case 1:
        m_m2Uncertainty = percentUncertainty/100.0;
        m_m2 = m_measuredM2 + absoluteMesurementError(m_measuredM2, m_m2Uncertainty);
        break;
    }

    emit updateSignal();
}

void DoublePendulum::setGravityUncertainty(double percentUncertainty) //WARNING: this function takes a percentage as an argument, this implies that a value of 100 means 1, because (100%=1).
{
    m_gUncertainty = percentUncertainty/100.0;
    m_g = m_measuredG + absoluteMesurementError(m_measuredG, m_gUncertainty);
}




//accessors
double DoublePendulum::ang1() const
{
   return m_ang1;
}
double DoublePendulum::ang2() const
{
    return m_ang2;
}

double DoublePendulum::prevAng1() const
{
    return m_prevAng1;
}

double DoublePendulum::prevAng2() const
{
    return m_prevAng2;
}
double DoublePendulum::angVel1() const
{
    return m_angVel1;
}
double DoublePendulum::angVel2() const
{
    return m_angVel2;
}
double DoublePendulum::angAcc1() const
{
    return m_angAcc1;
}
double DoublePendulum::angAcc2() const
{
    return m_angAcc2;
}

double DoublePendulum::prevAngAcc1() const
{
    return m_prevAngAcc1;
}

double DoublePendulum::prevAngAcc2() const
{
    return m_prevAngAcc2;
}
double DoublePendulum::posX1()
{
    setPos();
    return m_posX1;
}
double DoublePendulum::posY1()
{
    setPos();
    return m_posY1;
}
double DoublePendulum::posX2()
{
    setPos();
    return m_posX2;
}
double DoublePendulum::posY2()
{
    setPos();
    return m_posY2;
}

double DoublePendulum::v1()
{
    return angVelToSpeed1(m_angVel1, m_r1);
}

double DoublePendulum::v2()
{
    return angVelToSpeed2(m_ang1, m_angVel1, m_r1, m_ang2, m_angVel2, m_r2);
}

double DoublePendulum::v2Squared()
{
    return angVelToSpeed2Squared(m_ang1, m_angVel1, m_r1, m_ang2, m_angVel2, m_r2);
}

double DoublePendulum::acc1() const
{
    return m_angAcc1*m_r1;
}

double DoublePendulum::acc2() const
{
    return m_angAcc2*m_r2;
}


double DoublePendulum::m1() const
{
    return m_m1;
}
double DoublePendulum::m2() const
{
    return m_m2;
}
double DoublePendulum::r1() const
{
    return m_r1;
}
double DoublePendulum::r2() const
{
    return m_r2;
}
double DoublePendulum::g() const
{
    return m_g;
}
double DoublePendulum::dt() const
{
    return m_dt;
}

int DoublePendulum::integrationMethod() const
{
    return static_cast<int>(m_integrationMethod);
}

bool DoublePendulum::wasVerlet() const
{
    return m_wasVerlet;
}
double DoublePendulum::time() const
{
    return m_time;
}
double DoublePendulum::kinEnergy()
{
    return 0.5*m_m1*v1()*v1() + 0.5*m_m2*v2Squared();
}
double DoublePendulum::potEnergy()
{
    double height1=m_r1*(1-cos(m_ang1));
    double height2=height1+m_r2*(1-cos(m_ang2));
    return m_g*(m_m1*height1+m_m2*height2);
}
double DoublePendulum::totEnergy()
{
    return kinEnergy()+potEnergy();
}
double DoublePendulum::lagrangian()
{
    return kinEnergy()-potEnergy();
}

double DoublePendulum::initAng1() const
{
    return m_initAng1;
}

double DoublePendulum::initAng2() const
{
    return m_initAng2;
}

double DoublePendulum::initAngVel1() const
{
    return m_initAngVel1;
}

double DoublePendulum::initAngVel2() const
{
    return m_initAngVel2;
}

double DoublePendulum::measuredInitAng1() const
{
    return m_measuredInitAng1;
}
double DoublePendulum::measuredInitAng2() const
{
    return m_measuredInitAng2;
}
double DoublePendulum::measuredInitAngVel1() const
{
    return m_measuredInitAngVel1;
}
double DoublePendulum::measuredInitAngVel2() const
{
    return m_measuredInitAngVel2;
}
double DoublePendulum::initPosX1() const
{
    return m_initPosX1;
}
double DoublePendulum::initPosY1() const
{
    return m_initPosY1;
}
double DoublePendulum::initPosX2() const
{
    return m_initPosX2;
}
double DoublePendulum::initPosY2() const
{
    return m_initPosY2;
}



double DoublePendulum::measuredM1() const
{
    return m_measuredM1;
}

double DoublePendulum::measuredM2() const
{
    return m_measuredM2;
}

double DoublePendulum::measuredR1() const
{
    return m_measuredR1;
}

double DoublePendulum::measuredR2() const
{
    return m_measuredR2;
}

double DoublePendulum::measuredG() const
{
    return m_measuredG;
}

double DoublePendulum::ang1Uncertainty() const
{
    return m_ang1Uncertainty;
}

double DoublePendulum::ang2Uncertainty() const
{
    return m_ang2Uncertainty;
}

double DoublePendulum::angVel1Uncertainty() const
{
    return m_angVel1Uncertainty;
}

double DoublePendulum::angVel2Uncertainty() const
{
    return m_angVel2Uncertainty;
}

double DoublePendulum::m1Uncertainty() const
{
    return m_m1Uncertainty;
}

double DoublePendulum::m2Uncertainty() const
{
    return m_m2Uncertainty;
}

double DoublePendulum::r1Uncertainty() const
{
    return m_r1Uncertainty;
}

double DoublePendulum::r2Uncertainty() const
{
    return m_r2Uncertainty;
}

double DoublePendulum::gUncertainty() const
{
    return m_gUncertainty;
}

double DoublePendulum::parameter(int i, bool convertUnit)
{
    try
    {
        if(i >= 0 && i < Parameter::count)
        {
            Parameter::Enum param = static_cast<Parameter::Enum>(i);
            switch(param)
            {
            case Parameter::time:
                return m_time;
                break;
            case Parameter::x1:
                return posX1();
                break;
            case Parameter::y1:
                return posY1();
                break;
            case Parameter::x2:
                return posX2();
                break;
            case Parameter::y2:
                return posY2();
                break;
            case Parameter::ang1:
                if(convertUnit)
                {
                    return m_ang1/UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
                }
                else
                {
                    return m_ang1;
                }
                break;
            case Parameter::ang2:
                if(convertUnit)
                {
                    return m_ang2/UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
                }
                else
                {
                    return m_ang2;
                }
                break;
            case Parameter::v1:
                return v1();
                break;
            case Parameter::v2:
                return v2();
                break;
            case Parameter::angVel1:
                if(convertUnit)
                {
                    return m_angVel1/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]];
                }
                else
                {
                    return m_angVel1;
                }
                break;
            case Parameter::angVel2:
                if(convertUnit)
                {
                    return m_angVel2/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]];
                }
                else
                {
                    return m_angVel2;
                }
                break;
            case Parameter::acc1:
                if(convertUnit)
                {
                    return acc1()/UnitConversion::toMps2[m_currentUnit[UnitOf::acc]];
                }
                else
                {
                    return acc1();
                }
                break;
            case Parameter::acc2:
                if(convertUnit)
                {
                    return acc2()/UnitConversion::toMps2[m_currentUnit[UnitOf::acc]];
                }
                else
                {
                    return acc2();
                }
                break;
            case Parameter::angAcc1:
                if(convertUnit)
                {
                    return m_angAcc1/UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
                }
                else
                {
                    return m_angAcc1;
                }
                break;
            case Parameter::angAcc2:
                if(convertUnit)
                {
                    return m_angAcc2/UnitConversion::toRad[m_currentUnit[UnitOf::angle]];
                }
                else
                {
                    return m_angAcc2;
                }
                break;
            case Parameter::K:
                return kinEnergy();
                break;
            case Parameter::P:
                return potEnergy();
                break;
            case Parameter::totE:
                return totEnergy();
                break;
            case Parameter::L:
                return lagrangian();
                break;
            }
        }
        else
        {
            throw Error(2, "The index value is greater than the number of pendulum parameters minus one", 1);
        }
    }catch(Error e)
    {
        e.print();
        return 1;
    }
}

QString DoublePendulum::unitSymbol(int i)
{
    try
    {
        if(i >= 0 && i < Parameter::count)
        {
            Parameter::Enum param = static_cast<Parameter::Enum>(i);
            switch(param)
            {
            case Parameter::ang1:
                return UnitList::unitsOfAngle[m_currentUnit[UnitOf::angle]];
                break;
            case Parameter::ang2:
                return UnitList::unitsOfAngle[m_currentUnit[UnitOf::angle]];
                break;
            case Parameter::v1:
                return QString("m/s");
                break;
            case Parameter::v2:
                return QString("m/s");
                break;
            case Parameter::angVel1:
                return UnitList::unitsOfAngularVelocity[m_currentUnit[UnitOf::angVel]];
                break;
            case Parameter::angVel2:
                return UnitList::unitsOfAngularVelocity[m_currentUnit[UnitOf::angVel]];
                break;
            case Parameter::acc1:
                return UnitList::unitsOfAcceleration[m_currentUnit[UnitOf::acc]];
                break;
            case Parameter::acc2:
                return UnitList::unitsOfAcceleration[m_currentUnit[UnitOf::acc]];
                break;
            case Parameter::angAcc1:
                return UnitList::unitsOfAngle[m_currentUnit[UnitOf::angle]]+QString("/s")+QString(SpecChar::squared);
                break;
            case Parameter::angAcc2:
                return UnitList::unitsOfAngle[m_currentUnit[UnitOf::angle]]+QString("/s")+QString(SpecChar::squared);
                break;
            default:
                return UnitList::defaultUnits[i];
            }
        }
        else
        {
            throw Error(2, "The index value is greater than the number of pendulum parameters minus one", 1);
        }
    }catch(Error e)
    {
        e.print();
        return QString();
    }
}

const std::array<unsigned short, ArraySize::currentIndex>& DoublePendulum::indexArray() const
{
    return m_currentIndex;
}

const std::array<unsigned short, ArraySize::currentUnit>& DoublePendulum::unitArray() const
{
    return m_currentUnit;
}

QRandomGenerator DoublePendulum::randGenerator()
{
    return m_randGenerator;
}

bool DoublePendulum::restartIsDisabled() const
{
    return m_disableRestart;
}

void DoublePendulum::setName(QString name)
{
    m_name = name;
}

void DoublePendulum::setColor(QColor color)
{
    m_color = color;
}
