#include "drawingarea.h"

DrawingArea::DrawingArea(std::array<DoublePendulum*, Limitations::maxNumberOfPendula>& pendula) : m_pendula(pendula), m_drawPath(false)
{
    setFocusPolicy(Qt::ClickFocus);
    setInteractive(false);
    m_scene = new QGraphicsScene;
    setScene(m_scene);
    setSceneRect(mapToScene(viewport()->rect()).boundingRect());

    m_pen1.setColor(Qt::blue);
    m_pen1.setWidth(1);
    m_pen2.setColor(Qt::red);
    m_pen2.setWidth(1);
    m_defaultPen.setColor(Qt::black);
    m_defaultPen.setWidth(2);

    m_origin = mapToScene(viewport()->rect().center());
    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        addPendulum();
    }

    setRenderHint(QPainter::Antialiasing);
    setOptimizationFlag(QGraphicsView::OptimizationFlag::DontSavePainterState, true);
    setDragMode(QGraphicsView::ScrollHandDrag);
#ifndef __linux__
    QOpenGLWidget* w = new QOpenGLWidget;
    w->setAttribute(Qt::WA_AlwaysStackOnTop);
    w->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
    w->setFormat(QSurfaceFormat::defaultFormat());
    setViewport(w);
    viewport()->raise();
#endif // __linux__
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
}

DrawingArea::~DrawingArea()
{
    for(unsigned int i(0); i < m_rods1.size(); i++)
    {
        delete m_rods1[i];
        delete m_rods2[i];
        delete m_masses1[i];
        delete m_masses2[i];
        delete m_pathItems1[i];
        delete m_pathItems2[i];
        delete m_paths1[i];
        delete m_paths2[i];
    }
}

void DrawingArea::refresh()
{
    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        DoublePendulum* pendulum = m_pendula[i];
        //points
        m_points1[i].setX(pixelX1(pendulum));
        m_points1[i].setY(pixelY1(pendulum));
        m_points2[i].setX(pixelX2(pendulum));
        m_points2[i].setY(pixelY2(pendulum));

        if(m_drawPath)
        {
            if(m_paths1[i] == nullptr)
            {
                m_paths1[i] = new QPainterPath(m_points1[i]);
            }
            else
            {
                m_paths1[i]->lineTo(m_points1[i]);
            }

            if(m_paths2[i] == nullptr)
            {
                m_paths2[i] = new QPainterPath(m_points2[i]);
            }
            else
            {
                m_paths2[i]->lineTo(m_points2[i]);
            }
            m_pathItems1[i]->setPath(*m_paths1[i]);
            m_pathItems2[i]->setPath(*m_paths2[i]);
        }

        //rods
        m_rods1[i]->setLine(m_origin.x(), m_origin.y(), m_points1[i].x(), m_points1[i].y());
        m_rods2[i]->setLine(m_points1[i].x(), m_points1[i].y(), m_points2[i].x(), m_points2[i].y());

        //masses
        m_masses1[i]->setRect(m_points1[i].x() - sqrt(pendulum->m1())*UnitConversion::kgToPixels/2, m_points1[i].y() - sqrt(pendulum->m1())*UnitConversion::kgToPixels/2, sqrt(pendulum->m1())*UnitConversion::kgToPixels, sqrt(pendulum->m1())*UnitConversion::kgToPixels);
        m_masses2[i]->setRect(m_points2[i].x() - sqrt(pendulum->m2())*UnitConversion::kgToPixels/2, m_points2[i].y() - sqrt(pendulum->m2())*UnitConversion::kgToPixels/2, sqrt(pendulum->m2())*UnitConversion::kgToPixels, sqrt(pendulum->m2())*UnitConversion::kgToPixels);
    }
}

void DrawingArea::adjustViewSize()
{
    bool needsAdjustment;
    do
    {
        needsAdjustment = false;
        for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
        {
            if(QLineF(mapFromScene(m_rods1[i]->line().p1()), mapFromScene(m_rods1[i]->line().p2())).length()
              +QLineF(mapFromScene(m_rods2[i]->line().p1()), mapFromScene(m_rods2[i]->line().p2())).length() > viewport()->height()) //makes sure that the pendulum fits in the view when placed in the center
            {
                needsAdjustment = true;
            }
        }
        if(needsAdjustment)
        {
            scale(1-0.1/100.0, 1-0.1/100.0);
        }
    } while(needsAdjustment);
}

double DrawingArea::pixelX1(DoublePendulum* pendulum)
{
    return pendulum->posX1()*UnitConversion::mToPixels+m_origin.x(); //returns the value of x1 converted to screen coordinates
}
double DrawingArea::pixelY1(DoublePendulum* pendulum)
{
    return pendulum->posY1()*(-1)*UnitConversion::mToPixels+m_origin.y();
}
double DrawingArea::pixelX2(DoublePendulum* pendulum)
{
    return pendulum->posX2()*UnitConversion::mToPixels+m_origin.x();
}
double DrawingArea::pixelY2(DoublePendulum* pendulum)
{
    return pendulum->posY2()*(-1)*UnitConversion::mToPixels+m_origin.y();
}


void DrawingArea::scale(qreal sx, qreal sy)
{
    this->QGraphicsView::scale(sx, sy);
    adjustSceneRect();
}

void DrawingArea::enterEvent(QEvent* event)
{
    setFocus();
    event->ignore();
}

void DrawingArea::wheelEvent(QWheelEvent* event)
{
    qreal angle = qreal(event->angleDelta().y())/8.0/36.0*10.0; //angle in 100th of a turns
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        scale(1+angle/100.0, 1+angle/100.0);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
    event->accept();
}

void DrawingArea::clear(int pendulumNumber)
{
    delete m_paths1[pendulumNumber];
    m_paths1[pendulumNumber] = nullptr;
    m_pathItems1[pendulumNumber]->setPath(QPainterPath());

    delete m_paths2[pendulumNumber];
    m_paths2[pendulumNumber] = nullptr;
    m_pathItems2[pendulumNumber]->setPath(QPainterPath());
}

void DrawingArea::clear()
{
    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        clear(i);
    }

    refresh();
}

void DrawingArea::adjustSceneRect()
{
    double maxLength(0);
    for(unsigned int i(0); i < m_rods1.size(); i++)
    {
        double length(m_rods1[i]->line().length() +  m_rods2[i]->line().length());
        if(length > maxLength)
        {
            maxLength = length;
        }
    }
    QRect rec(m_origin.x()-maxLength-1, m_origin.y()-maxLength-1, 2*maxLength, 2*maxLength);
    QPoint a(mapFromScene(rec.center()));
    QPoint b(viewport()->rect().center());
    setSceneRect(rec);
}

void DrawingArea::showEvent(QShowEvent* event)
{
    for(int i(0); i < DoublePendulum::numberOfPendula(); i++)
    {
        if(!m_scene->items().contains(m_rods1[i]))
        {
            m_scene->addItem(m_rods1[i]);
        }
        if(!m_scene->items().contains(m_rods2[i]))
        {
            m_scene->addItem(m_rods2[i]);
        }
        if(!m_scene->items().contains(m_masses1[i]))
        {
            m_scene->addItem(m_masses1[i]);
        }
        if(!m_scene->items().contains(m_masses2[i]))
        {
            m_scene->addItem(m_masses2[i]);
        }
        if(!m_scene->items().contains(m_pathItems1[i]))
        {
            m_scene->addItem(m_pathItems1[i]);
        }
        if(!m_scene->items().contains(m_pathItems2[i]))
        {
            m_scene->addItem(m_pathItems2[i]);
        }
    }
    event->accept();
}

void DrawingArea::hideEvent(QHideEvent* event)
{
    for(unsigned int i(0); i < DoublePendulum::numberOfPendula(); i++)
    {
        m_scene->removeItem(m_rods1[i]);
        m_scene->removeItem(m_rods2[i]);
        m_scene->removeItem(m_masses1[i]);
        m_scene->removeItem(m_masses2[i]);
        m_scene->removeItem(m_pathItems1[i]);
        m_scene->removeItem(m_pathItems2[i]);
    }
    event->accept();
}

QPointF DrawingArea::Origin()
{
    return m_origin;
}

void DrawingArea::addPendulum()
{
    m_points1.push_back(QPointF());
    m_points2.push_back(QPointF());
    m_rods1.push_back(new QGraphicsLineItem());
    m_rods2.push_back(new QGraphicsLineItem());
    m_masses1.push_back(new QGraphicsEllipseItem());
    m_masses2.push_back(new QGraphicsEllipseItem());
    m_paths1.push_back(nullptr);
    m_paths2.push_back(nullptr);
    m_pathItems1.push_back(new QGraphicsPathItem());
    m_pathItems2.push_back(new QGraphicsPathItem());

    int i(m_rods1.size()-1);
    changeColor(i);

    m_pathItems1[i]->setZValue(-1);
    m_pathItems2[i]->setZValue(-1);


    m_scene->addItem(m_rods1[i]);
    m_scene->addItem(m_rods2[i]);
    m_scene->addItem(m_masses1[i]);
    m_scene->addItem(m_masses2[i]);
    m_scene->addItem(m_pathItems1[i]);
    m_scene->addItem(m_pathItems2[i]);

    refresh();

    adjustSceneRect();
}

void DrawingArea::removePendulum(int pendulumToRemove)
{
    m_scene->removeItem(m_rods1[pendulumToRemove]);
    m_scene->removeItem(m_rods2[pendulumToRemove]);
    m_scene->removeItem(m_masses1[pendulumToRemove]);
    m_scene->removeItem(m_masses2[pendulumToRemove]);
    m_scene->removeItem(m_pathItems1[pendulumToRemove]);
    m_scene->removeItem(m_pathItems2[pendulumToRemove]);

    adjustSceneRect();

    delete m_rods1[pendulumToRemove];
    delete m_rods2[pendulumToRemove];
    delete m_masses1[pendulumToRemove];
    delete m_masses2[pendulumToRemove];
    delete m_paths1[pendulumToRemove];
    delete m_paths2[pendulumToRemove];
    delete m_pathItems1[pendulumToRemove];
    delete m_pathItems2[pendulumToRemove];

    m_points1.erase(m_points1.begin()+pendulumToRemove);
    m_points2.erase(m_points2.begin()+pendulumToRemove);
    m_rods1.erase(m_rods1.begin()+pendulumToRemove);
    m_rods2.erase(m_rods2.begin()+pendulumToRemove);
    m_masses1.erase(m_masses1.begin()+pendulumToRemove);
    m_masses2.erase(m_masses2.begin()+pendulumToRemove);
    m_paths1.erase(m_paths1.begin()+pendulumToRemove);
    m_paths2.erase(m_paths2.begin()+pendulumToRemove);
    m_pathItems1.erase(m_pathItems1.begin()+pendulumToRemove);
    m_pathItems2.erase(m_pathItems2.begin()+pendulumToRemove);
}

void DrawingArea::setDrawPath(bool isChecked)
{
    m_drawPath = isChecked;

    if(!m_drawPath)
    {
        for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
        {
            delete m_paths1[i];
            m_paths1[i] = nullptr;
            delete m_paths2[i];
            m_paths2[i] = nullptr;
        }
    }
}

void DrawingArea::setQualityLevel(int qualityLevel)
{
    m_qualityLevel = static_cast<Quality::Enum>(qualityLevel);
#ifdef __linux__
    switch(qualityLevel)
    {
    case 0:
        setRenderHint(QPainter::Antialiasing, false);
        setRenderHint(QPainter::SmoothPixmapTransform, false);
        break;
    case 1:
        setRenderHint(QPainter::Antialiasing, true);
        setRenderHint(QPainter::SmoothPixmapTransform, false);
        break;
    case 2:
        setRenderHint(QPainter::Antialiasing, true);
        setRenderHint(QPainter::SmoothPixmapTransform, true);
        break;
    }
#else
    QSurfaceFormat sF(QSurfaceFormat::defaultFormat());
    sF.setSamples(Quality::numberOfSamples[qualityLevel]);
    QOpenGLWidget* w = new QOpenGLWidget;
    w->setAttribute(Qt::WA_AlwaysStackOnTop);
    w->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
    w->setFormat(sF);
    setViewport(w);
    emit viewportUpdated();
#endif // __linux__
}

void DrawingArea::changeColor(int i)
{
    QPen pen(m_defaultPen);
    QColor color(m_pendula[i]->color());
    pen.setColor(color);
    m_rods1[i]->setPen(m_defaultPen);
    m_rods2[i]->setPen(m_defaultPen);
    m_masses1[i]->setPen(pen);
    m_masses1[i]->setBrush(QBrush(color));
    m_masses2[i]->setPen(pen);
    m_masses2[i]->setBrush(QBrush(color));
    m_pathItems1[i]->setPen(m_pen1);
    m_pen2.setColor(color);
    m_pathItems2[i]->setPen(m_pen2);
}
