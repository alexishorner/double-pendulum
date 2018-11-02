#include "grapharea.h"

GraphArea::GraphArea(std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& pendula, unsigned short& currentPendulumPlace, QMainWindow* mainWindow, const bool& isPlaying): m_pendula(pendula), m_currentPendulumPlace(currentPendulumPlace), m_mainWindow(*mainWindow), m_isPlaying(isPlaying), m_isRefreshing(false), m_isFirstPoint(true), m_isSecondPoint(false), m_needsCentering(true), m_scrollMultiplier(10), m_xAxis(new QValueAxis), m_yAxis(new QValueAxis)
{
    m_xParameter = Parameter::time;
    m_yParameter = Parameter::ang1;
    m_view = new QChartView;
    m_view->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
    m_view->setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setInteractive(false);
#ifndef __linux__
    QOpenGLWidget* w = new QOpenGLWidget;
    //w->setAttribute(Qt::WA_AlwaysStackOnTop); //bug: black screen
    w->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
    QSurfaceFormat sF(QSurfaceFormat::defaultFormat());
    w->setFormat(sF);
    m_view->setViewport(w);
    m_view->viewport()->raise();
#endif // __linux__
    m_view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    m_hScrollBar = new QScrollBar(Qt::Horizontal);
    m_hScrollBar->setRange(0, 0);
    m_hScrollBar->setSingleStep(m_hScrollBar->singleStep()*m_scrollMultiplier);
    m_hScrollBar->setPageStep(100);
    m_oldHScrollValue = m_hScrollBar->value();
    m_vScrollBar = new QScrollBar(Qt::Vertical);
    m_vScrollBar->setRange(0, 0);
    m_vScrollBar->setSingleStep(m_vScrollBar->singleStep()*m_scrollMultiplier);
    m_vScrollBar->setPageStep(100);
    m_oldVScrollValue = m_vScrollBar->value();
    m_scrollIsEnabled = true;
    m_autoScrollIsEnabled = true;
    m_hScrollBarIsPressed = false;
    m_vScrollBarIsPressed = false;


    QGridLayout* graphLayout = new QGridLayout;
    graphLayout->addWidget(m_view, 0, 0);
    graphLayout->addWidget(m_hScrollBar, 1, 0);
    graphLayout->addWidget(m_vScrollBar, 0, 1, 2, 1);
    setLayout(graphLayout);

    m_chart = new QChart;
    m_chart->legend()->setAlignment(Qt::AlignLeft);
    m_view->setChart(m_chart);
    m_xAxis->setTickCount(1);
    QFont ticksFont;
    ticksFont.setWeight(QFont::Bold);
    ticksFont.setPixelSize(12);
    m_xAxis->setLabelsFont(ticksFont);
    QFont titleFont;
    titleFont.setFamily("Arial");
    titleFont.setWeight(QFont::DemiBold);
    titleFont.setPixelSize(16);
    m_xAxis->setTitleFont(titleFont);
    m_xAxis->setTitleText(Parameter::namesList[m_xParameter] + " [" + UnitList::defaultUnits[m_xParameter] + "]");

    m_yAxis->setTickCount(1);
    m_yAxis->setLabelsFont(ticksFont);
    m_yAxis->setTitleFont(titleFont);
    m_yAxis->setTitleText(Parameter::namesList[m_yParameter] + " [" + UnitList::defaultUnits[m_yParameter] + "]");

    titleFont.setPixelSize(14);
    m_chart->legend()->setFont(titleFont);
    resetAxesRange();
    m_chart->setAxisX(m_xAxis);
    m_chart->setAxisY(m_yAxis);

    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        addPendulum();
    }

    QObject::connect(m_hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollHGraph(int)));
    QObject::connect(m_hScrollBar, SIGNAL(sliderPressed()), this, SLOT(onHScrollBarPression()));
    QObject::connect(m_hScrollBar, SIGNAL(sliderReleased()), this, SLOT(onHScrollBarRelease()));

    QObject::connect(m_vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollVGraph(int)));
    QObject::connect(m_vScrollBar, SIGNAL(sliderPressed()), this, SLOT(onVScrollBarPression()));
    QObject::connect(m_vScrollBar, SIGNAL(sliderReleased()), this, SLOT(onVScrollBarRelease()));
}

GraphArea::~GraphArea()
{
    for(unsigned i(0); i<m_graphs.size(); i++)
    {
        delete m_graphs[i];
    }
}

void GraphArea::replacePendulumAt(int i)
{
    if(m_graphs[i] != nullptr)
    {
        delete m_graphs[i];
    }
    m_graphs[i] = new QLineSeries;
    if(isVisible())
    {
        m_chart->addSeries(m_graphs[i]);
        m_graphs[i]->attachAxis(m_xAxis);
        m_graphs[i]->attachAxis(m_yAxis);
    }
    m_graphs[i]->setName(m_pendula[i]->name());
    changeColor(i);
}

void GraphArea::refresh()
{
    m_isRefreshing = true;
    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        if(m_graphs[i]==nullptr)
        {
            replacePendulumAt(i);
        }
        setXY(m_pendula[i]);
        m_graphs[i]->append(m_point);

        //TODO: add to the chart only the visible part of each QLineSeries

        if(isVisible() && m_isFirstPoint)
        {
            m_point0 = m_point;
            m_maxXPoint = m_point;
            m_maxYPoint = m_point;
            m_minXPoint = m_point;
            m_minYPoint = m_point;
            m_isFirstPoint = false;
            m_needsCentering = true;
        }


        if(m_point.x() > m_maxXPoint.x())
        {
            m_maxXPoint = m_point;
        }
        if(m_point.y() > m_maxYPoint.y())
        {
            m_maxYPoint = m_point;
        }
        if(m_point.x() < m_minXPoint.x())
        {
            m_minXPoint = m_point;
        }
        if(m_point.y() < m_minYPoint.y())
        {
            m_minYPoint = m_point;
        }

        adjustHScrollBar();
        adjustVScrollBar();


        if(m_needsCentering)
        {
            if(i == m_currentPendulumPlace)
            {
                if(m_isSecondPoint)
                {
                    if(m_xParameter == Parameter::time
                            || m_chart->mapToPosition(m_point, m_graphs[i]).x() < 0                             //
                            || m_chart->mapToPosition(m_point, m_graphs[i]).x() > m_chart->plotArea().width())  //if m_point is not visible (x coordinates)
                    {
                        centerPointHorizontally(i); //repositions m_point so that it is in the middle of the chart
                    }
                    if(m_yParameter == Parameter::time                                                                             //
                            || m_chart->mapToPosition(m_point, m_graphs[i]).y() < 0                                                     //
                            || m_chart->mapToPosition(m_point, m_graphs[i]).y() > m_chart->plotArea().height())                         //same as above, but with y coordinates
                    {                                                                                                              //
                        centerPointVertically(i);                                                                                   //
                    }
                    m_needsCentering = false;
                    m_isSecondPoint = false;
                }
                else
                {
                    m_isSecondPoint = true;
                }
            }
        }

        if(m_autoScrollIsEnabled) //BIG BOTTLENECK: when scrolling data in a graph, the gpu usage drops drastically, probably because it has to wait for data to copy. The problem is that the average fps gets lower and lower as the size of the graph increases.
        {
            if(m_xParameter == Parameter::time && !m_hScrollBarIsPressed && i == m_currentPendulumPlace)
            {
                setHScrollBarValueFromChart(m_point.x());
                centerPointHorizontally(i);
            }
            if(m_yParameter == Parameter::time && !m_vScrollBarIsPressed && i == m_currentPendulumPlace)
            {
                setVScrollBarValueFromChart(m_point.y());
                centerPointVertically(i);
            }
        }
    }
    m_isRefreshing = false;
}

void GraphArea::setXParameter(int parameter)
{
    try
    {
        if(parameter<19) //if in range of the enum
        {
            m_xParameter = static_cast<Parameter::Enum>(parameter);
            m_xAxis->setTitleText(Parameter::namesList[parameter] + " [" + UnitList::defaultUnits[parameter] + "]");
            clear();
        }
        else
        {
            throw Error(1, tr("Parameter not in range of the enum").toStdString(), 0);
        }
    }
    catch(Error const& e)
    {
        e.print();
    }
}

void GraphArea::setYParameter(int parameter)
{
    try
    {
        if(parameter<19) //if in range of the enum
        {
            m_yParameter = static_cast<Parameter::Enum>(parameter);
            m_yAxis->setTitleText(Parameter::namesList[parameter] + " [" + UnitList::defaultUnits[parameter] + "]");
            clear();
        }
        else
        {
            throw Error(1, tr("Parameter not in range of the enum").toStdString(), 0);
        }
    }
    catch(Error const& e)
    {
        e.print();
    }
}

void GraphArea::setAutoScroll(bool isEnabled)
{
    m_autoScrollIsEnabled = isEnabled;
}

void GraphArea::onHScrollBarPression()
{
    m_hScrollBarIsPressed = true;
}

void GraphArea::onHScrollBarRelease()
{
    m_hScrollBarIsPressed = false;
}

void GraphArea::onVScrollBarPression()
{
    m_vScrollBarIsPressed = true;
}

void GraphArea::onVScrollBarRelease()
{
    m_vScrollBarIsPressed = false;
}

void GraphArea::setQualityLevel(int qualityLevel)
{
    m_qualityLevel = static_cast<Quality::Enum>(qualityLevel);
#ifdef __linux__
    switch(mQualityLevel)
    {
    case 0:
        mView->setRenderHint(QPainter::Antialiasing, false);
        mView->setRenderHint(QPainter::SmoothPixmapTransform, false);
        break;
    case 1:
        mView->setRenderHint(QPainter::Antialiasing, true);
        mView->setRenderHint(QPainter::SmoothPixmapTransform, false);
        break;
    case 2:
        mView->setRenderHint(QPainter::Antialiasing, true);
        mView->setRenderHint(QPainter::SmoothPixmapTransform, true);
        break;
    }
#else
    QSurfaceFormat sF(QSurfaceFormat::defaultFormat());
    sF.setSamples(Quality::numberOfSamples[qualityLevel]);
    QOpenGLWidget* w = new QOpenGLWidget;
    //w->setAttribute(Qt::WA_AlwaysStackOnTop);
    w->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
    w->setFormat(sF);
    m_view->setViewport(w);
    emit viewportUpdated();
#endif // __linux__
}

void GraphArea::scrollHGraph(int value)
{
    if(m_scrollIsEnabled)
    {
        if(m_hScrollBar->maximum()-m_hScrollBar->minimum() > 0)
        {
            m_chart->scroll((hScrollBarToChart(value) - hScrollBarToChart(m_oldHScrollValue))*hPixelsPerValue(), 0);
        }
    }
    m_oldHScrollValue = value;
}

void GraphArea::scrollVGraph(int value)
{
    if(m_scrollIsEnabled)
    {
        if(m_vScrollBar->maximum()-m_vScrollBar->minimum() > 0)
        {
            m_chart->scroll(0, (vScrollBarToChart(value) - vScrollBarToChart(m_oldVScrollValue))*vPixelsPerValue());
        }
    }
    m_oldVScrollValue = value;
}

void GraphArea::wheelEvent(QWheelEvent* event)
{
    QPointF angle(event->angleDelta().x()/8.0/360.0, event->angleDelta().y()/8.0/360.0); //angle in turns
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        zoom(1+angle.y());
    }
    else
    {
        if(!(m_autoScrollIsEnabled && m_xParameter == Parameter::time && m_isPlaying))
        {
            m_hScrollBar->setValue(m_oldHScrollValue - int(round(angle.x()*m_hScrollBar->pageStep())));
        }
        if(!(m_autoScrollIsEnabled && m_yParameter == Parameter::time && m_isPlaying))
        {
            m_vScrollBar->setValue(m_oldVScrollValue - int(round(angle.y()*m_vScrollBar->pageStep())));
        }
    }
    event->accept();
}

void GraphArea::setX(double x)
{
    m_point.setX(x);
}

void GraphArea::setY(double y)
{
    m_point.setY(y);
}

void GraphArea::setHScrollBarValueFromChart(double valueFromChart)
{
    m_hScrollBar->setValue(int(round(chartToHScrollBar(valueFromChart))));
}

void GraphArea::setVScrollBarValueFromChart(double valueFromChart)
{
    m_vScrollBar->setValue(int(round(chartToVScrollBar(valueFromChart))));
}

QPointF GraphArea::currentPoint()
{
    int lastPosition(m_graphs[m_currentPendulumPlace]->count()-1);
    return m_graphs[m_currentPendulumPlace]->at(lastPosition);
}

QPointF GraphArea::graphsCenter()
{
    return QPointF((m_maxXPoint.x()+m_minXPoint.x())/2.0, (m_maxYPoint.y()+m_minYPoint.y())/2.0);
}

QChartView* GraphArea::view()
{
    return m_view;
}

qreal GraphArea::chartValueWidth()
{
    return m_xAxis->max()-m_xAxis->min();
}

qreal GraphArea::chartValueHeight()
{
    return m_yAxis->max()-m_yAxis->min();
}

qreal GraphArea::hPixelsPerValue()
{
    return m_chart->plotArea().width()/chartValueWidth();
}

qreal GraphArea::vPixelsPerValue()
{
    return m_chart->plotArea().height()/chartValueHeight();
}

double GraphArea::hScrollBarValueToChart()
{
    return hScrollBarToChart(m_hScrollBar->value());
}

double GraphArea::vScrollBarValueToChart()
{
    return vScrollBarToChart(m_vScrollBar->value());
}

void GraphArea::setXY(DoublePendulum* pendulum)
{
    m_point.setX(pendulum->parameter(m_xParameter));
    m_point.setY(pendulum->parameter(m_yParameter));
}

void GraphArea::clear(int pendulumNumber)
{
    if(m_graphs[pendulumNumber] != nullptr)
    {
        replacePendulumAt(pendulumNumber);
    }
}

void GraphArea::clear()
{
    for(unsigned i(0); i < m_graphs.size(); i++)
    {
        clear(i);
    }

    qreal xAxisLength(m_xAxis->max()-m_xAxis->min());
    qreal yAxisLength(m_yAxis->max()-m_yAxis->min());
    m_chart->axisX()->setRange(-xAxisLength/2.0, xAxisLength/2.0);
    m_chart->axisY()->setRange(-yAxisLength/2.0, yAxisLength/2.0);
    m_hScrollBar->setRange(0, 0);
    m_vScrollBar->setRange(0, 0);

    m_isFirstPoint = true;
    m_scrollIsEnabled = true;
}

void GraphArea::createCustomAxes()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    resetAxesRange();
}

void GraphArea::resetAxesRange()
{
    m_xAxis->setRange(-5, 5);
    QRectF plotArea(m_chart->plotArea());
    m_yAxis->setRange(m_xAxis->min()*plotArea.height()/plotArea.width(), m_xAxis->max()*plotArea.height()/plotArea.width());
}

void GraphArea::zoom(qreal factor)
{
        int oldHVal(m_hScrollBar->value());
        int oldVVal(m_vScrollBar->value());
        m_chart->zoom(factor);
        adjustHScrollBar();
        adjustVScrollBar();

        m_scrollIsEnabled = false;
        m_hScrollBar->setValue(oldHVal);
        m_vScrollBar->setValue(oldVVal);
        m_scrollIsEnabled = true;
}

void GraphArea::showEvent(QShowEvent* event)
{
    for(int i(0); i < DoublePendulum::numberOfPendula(); i++)
    {
        if(!m_chart->series().contains(m_graphs[i]))
        {
            m_chart->addSeries(m_graphs[i]);
            m_graphs[i]->attachAxis(m_xAxis);
            m_graphs[i]->attachAxis(m_yAxis);
            changeColor(i);
        }
    }
    m_needsCentering = true;
    event->accept();
}

void GraphArea::hideEvent(QHideEvent* event)
{
    for(int i(0); i < DoublePendulum::numberOfPendula(); i++)
    {
        m_chart->removeSeries(m_graphs[i]); //drastically increases performance
    }
    event->accept();
}

void GraphArea::adjustHScrollBar()
{
    m_scrollIsEnabled = false;
    m_hScrollBar->setRange(int(round(m_minXPoint.x()*m_scrollMultiplier)), int(round(m_maxXPoint.x()*m_scrollMultiplier)));
    m_scrollIsEnabled = true;
    m_hScrollBar->setPageStep(int(round(chartValueWidth()*m_scrollMultiplier)));
}

double GraphArea::hScrollBarToChart(int scrollBarValue)
{
    return scrollBarValue/m_scrollMultiplier;
}

double GraphArea::chartToHScrollBar(double chartValue)
{
    return chartValue*m_scrollMultiplier;
}

void GraphArea::centerPointHorizontally(int i)
{
    m_chart->scroll(m_chart->mapToPosition(m_point, m_graphs[i]).x()-(m_chart->plotArea().center().x()), 0);
}

void GraphArea::adjustVScrollBar()
{
    m_scrollIsEnabled = false;

    double oldVScrollLogicalValue(vScrollBarValueToChart());
    m_vScrollBar->setRange(int((round(m_minYPoint.y())*m_scrollMultiplier)), int((round(m_maxYPoint.y()*m_scrollMultiplier))));
    setVScrollBarValueFromChart(oldVScrollLogicalValue);

    m_scrollIsEnabled = true;

    m_vScrollBar->setPageStep(chartValueHeight()*m_scrollMultiplier);
}

double GraphArea::vScrollBarToChart(int scrollBarValue)
{
    return (-scrollBarValue + m_vScrollBar->maximum() + m_vScrollBar->minimum())/m_scrollMultiplier;
}

double GraphArea::chartToVScrollBar(double chartValue)
{
    return m_vScrollBar->maximum()+m_vScrollBar->minimum()-chartValue*m_scrollMultiplier;
}

void GraphArea::centerPointVertically(int i)
{
    m_chart->scroll(0, -m_chart->mapToPosition(m_point, m_graphs[i]).y()+(m_chart->plotArea().center().y()));
}

qreal GraphArea::xValueAtXPos0()
{
    QLineSeries* currentGraph(m_graphs[m_currentPendulumPlace]);
    return currentGraph->at(0).x() - (currentGraph->at(1).x() - currentGraph->at(0).x()) / (m_chart->mapToPosition(currentGraph->at(1), currentGraph).x() - m_chart->mapToPosition(currentGraph->at(0), currentGraph).x())
            *(m_chart->mapToPosition(currentGraph->at(0), currentGraph).x() - m_chart->plotArea().left());
}

qreal GraphArea::yValueAtYPos0()
{
    QLineSeries* currentGraph(m_graphs[m_currentPendulumPlace]);
    return currentGraph->at(0).y() - (currentGraph->at(1).y() - currentGraph->at(0).y()) / (m_chart->mapToPosition(currentGraph->at(0), currentGraph).y() - m_chart->mapToPosition(currentGraph->at(1), currentGraph).y())
            *(-m_chart->mapToPosition(currentGraph->at(0), currentGraph).y());
}

void GraphArea::removePendulum(int pendulumToRemove)
{
    int i(pendulumToRemove);
    delete m_graphs[i];
    m_graphs.erase(m_graphs.begin()+i);
}

void GraphArea::changeColor(int i)
{
    QColor color(m_pendula[i]->color());
    m_graphs[i]->setPen(QPen(color, 2));
}
