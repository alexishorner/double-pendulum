#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QGraphicsScene>
#include <QOpenGLWidget>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPainterPath>
#include "doublependulum.h"
#include "error.h"
#include <QMainWindow>
#include <QtCharts>

class GraphArea : public QWidget
{
    /* This class displays and stores graphs relative to the simulation.
     * It handles scrolling and zooming, because QChartView does not.*/
    Q_OBJECT
public:
    GraphArea(std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& pendula, unsigned short& currentPendulumPlace, QMainWindow* mainWindow, const bool& isPlaying);
    ~GraphArea();
    void replacePendulumAt(int i);
    inline void addPendulum()
    {
        m_graphs.push_back(nullptr);
        replacePendulumAt(m_graphs.size()-1);
    }
    void removePendulum(int pendulumToRemove);
    void changeColor(int i);

    //mutators
    void setX(double x);
    void setY(double y);
    void setHScrollBarValueFromChart(double valueFromChart);
    void setVScrollBarValueFromChart(double valueFromChart);

    //accessors
    QPointF currentPoint();
    QPointF graphsCenter();
    QChartView* view();
    qreal chartValueWidth();
    qreal chartValueHeight();
    qreal hPixelsPerValue();
    qreal vPixelsPerValue();
    double hScrollBarValueToChart();
    double vScrollBarValueToChart();



    void setXY(DoublePendulum* pendulum);
    void clear(int pendulumNumber);
    void clear();
    void createCustomAxes();
    void resetAxesRange();

    void zoom(qreal factor);
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
private:
    std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& m_pendula;
    unsigned short& m_currentPendulumPlace;
    QMainWindow& m_mainWindow;

    const bool& m_isPlaying;
    bool m_isRefreshing;


    const double m_scrollMultiplier;
    QScrollBar* m_hScrollBar;
    int m_oldHScrollValue;
    void adjustHScrollBar();   //private method
    double hScrollBarToChart(int scrollBarValue); //converts the value from horizontal scroll bar value to chart value
    double chartToHScrollBar(double chartValue);
    void centerPointHorizontally(int i);

    QScrollBar* m_vScrollBar;
    int m_oldVScrollValue;
    void adjustVScrollBar();   //private method
    double vScrollBarToChart(int scrollBarValue); //converts the value from vertical scroll bar value to chart value
    double chartToVScrollBar(double chartValue);
    void centerPointVertically(int i);

    bool m_scrollIsEnabled;
    qreal xValueAtXPos0();     //private method
    qreal yValueAtYPos0();     //private method
    bool m_autoScrollIsEnabled;
    bool m_hScrollBarIsPressed;
    bool m_vScrollBarIsPressed;


    QChartView* m_view;
    QChart* m_chart;
    std::vector<QLineSeries*> m_graphs;
    QPointF m_point;
    QPointF m_point0;
    QPointF m_maxXPoint;
    QPointF m_maxYPoint;
    QPointF m_minXPoint;
    QPointF m_minYPoint;
    bool m_isFirstPoint;
    bool m_isSecondPoint;
    bool m_needsCentering;
    QValueAxis* m_xAxis;
    QValueAxis* m_yAxis;
    enum Parameter::Enum m_xParameter;
    enum Parameter::Enum m_yParameter;

    Quality::Enum m_qualityLevel;
public slots:
    void refresh();
    void wheelEvent(QWheelEvent* event) override;
    void setXParameter(int parameter);
    void setYParameter(int parameter);
    void setAutoScroll(bool isEnabled);
    void onHScrollBarPression();
    void onHScrollBarRelease();
    void onVScrollBarPression();
    void onVScrollBarRelease();
    void setQualityLevel(int qualityLevel);
private slots:
    void scrollHGraph(int value);
    void scrollVGraph(int value);
signals:
#ifndef __linux__
    void viewportUpdated();
#endif // __linux__
};

#endif // GRAPHAREA_H
