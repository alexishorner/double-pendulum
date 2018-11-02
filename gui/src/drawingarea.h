#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H


#include <QPainter>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QFocusEvent>
#include "globalconstants.h"
#include "doublependulum.h"

class DrawingArea : public QGraphicsView
{
    /* This class displays the double pendula.*/
    Q_OBJECT
public:
    DrawingArea(std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& pendula);
    ~DrawingArea();
    void addPendulum();
    void removePendulum(int pendulumToRemove);
    void changeColor(int i); //changes the display color of the i-th pendulum to the color of the pendulum
    double pixelX1(DoublePendulum* pendulum);
    double pixelY1(DoublePendulum* pendulum);
    double pixelX2(DoublePendulum* pendulum);
    double pixelY2(DoublePendulum* pendulum);
    void clear(int pendulumNumber);
    void clear();
    void adjustSceneRect();
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

    //accessors
    QPointF Origin();
private:
    QGraphicsScene* m_scene;
    std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& m_pendula;
    QPointF m_origin;
    std::vector<QPointF> m_points1;
    std::vector<QPointF> m_points2;
    std::vector<QGraphicsLineItem*> m_rods1;
    std::vector<QGraphicsLineItem*> m_rods2;
    std::vector<QGraphicsEllipseItem*> m_masses1;
    std::vector<QGraphicsEllipseItem*> m_masses2;

    bool m_drawPath;
    QPen m_pen1;
    QPen m_pen2;
    QPen m_defaultPen;
    std::vector<QPainterPath*> m_paths1;
    std::vector<QPainterPath*> m_paths2;
    std::vector<QGraphicsPathItem*> m_pathItems1;
    std::vector<QGraphicsPathItem*> m_pathItems2;

    Quality::Enum m_qualityLevel;
public slots:
    void refresh();
    void adjustViewSize();
    void scale(qreal sx, qreal sy);
    void enterEvent(QEvent* event);
    void wheelEvent(QWheelEvent* event) override;
    void setDrawPath(bool isChecked);
    void setQualityLevel(int qualityLevel);
signals:
#ifndef __linux__
    void viewportUpdated();
#endif // __linux__
};

#endif // DRAWINGAREA_H
