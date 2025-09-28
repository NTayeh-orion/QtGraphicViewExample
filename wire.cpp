#include "wire.h"
#include <QPropertyAnimation>
#include "pin.h"
#include <qpainter.h>
Wire::Wire(Pin *startPin, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , startPin(startPin)
{
    // QPen wirePen(QColor(255, 128, 0)); // orange
    // wirePen.setWidth(2);
    // wirePen.setCapStyle(Qt::RoundCap);
    // wirePen.setJoinStyle(Qt::RoundJoin);
    // setPen(wirePen);
    // setPen(QPen(Qt::red, 2));


    m_pen.setColor(QColor(255, 128, 0)); // orange default
    m_pen.setWidth(2);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::RoundJoin);



    setFlag(QGraphicsItem::ItemIsSelectable); // allow selection
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

    updatePath();

}

void Wire::setEndPin(Pin *endPin)
{
    this->endPin = endPin;
    updatePath();
}
Wire::~Wire()
{
    disconnectPins();
}
void Wire::disconnectPins()
{
    if (startPin) {
        startPin->removeWire(this);
        startPin = nullptr;
    }
    if (endPin) {
        endPin->removeWire(this);
        endPin = nullptr;
    }
}
void Wire::updatePath(const QPointF &mousePos)
{
    try {
        QPointF p1 = startPin->connectionPoint();
        QPointF p2 = endPin ? endPin->connectionPoint() : mousePos;

        QPainterPath path(p1);

        // Simple orthogonal routing: horizontal -> vertical -> horizontal
        qreal midX = (p1.x() + p2.x()) / 2;
        path.lineTo(midX, p1.y());
        path.lineTo(midX, p2.y());
        path.lineTo(p2);

        // setPath(path);
        m_path = path;
        prepareGeometryChange();
        update();
    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();

        QMessageBox::information(nullptr, "Error", e.what());
    }
}


void Wire::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // QPen hoverPen = pen();
    // hoverPen.setColor(Qt::yellow);   // highlight color
    // hoverPen.setWidth(4);            // thicker
    // setPen(hoverPen);

    // QGraphicsPathItem::hoverEnterEvent(event);

    QPropertyAnimation *anim = new QPropertyAnimation(this, "penWidth");
    anim->setDuration(150);
    anim->setStartValue(m_penWidth);
    anim->setEndValue(5.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    QGraphicsObject::hoverEnterEvent(event);
}

void Wire::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // QPen normalPen(QColor(255, 128, 0)); // back to orange
    // normalPen.setWidth(2);
    // normalPen.setCapStyle(Qt::RoundCap);
    // normalPen.setJoinStyle(Qt::RoundJoin);
    // setPen(normalPen);

    // QGraphicsPathItem::hoverLeaveEvent(event);


    QPropertyAnimation *anim = new QPropertyAnimation(this, "penWidth");
    anim->setDuration(150);
    anim->setStartValue(m_penWidth);
    anim->setEndValue(2.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    QGraphicsObject::hoverLeaveEvent(event);
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // QPen p(QColor(255, 128, 0));
    // p.setWidthF(m_penWidth);
    // p.setCapStyle(Qt::RoundCap);
    // p.setJoinStyle(Qt::RoundJoin);

    // painter->setPen(p);
    // // painter->drawPath(path());
    // painter->drawPath(m_path);



    // ----------------
    // QPen pen;

    // if (isSelected()) {
    //     // Highlighted look
    //     pen.setColor(QColor(0, 200, 255)); // bright cyan
    //     pen.setWidthF(m_penWidth + 2);

    //     // Optional: dashed outline to emphasize selection
    //     pen.setStyle(Qt::DashLine);

    //     // Soft glow effect
    //     QRadialGradient glowGradient(m_path.pointAtPercent(0.5), 20);
    //     glowGradient.setColorAt(0.0, QColor(0, 200, 255, 120));
    //     glowGradient.setColorAt(1.0, QColor(0, 200, 255, 0));
    //     painter->setBrush(glowGradient);
    // }
    // else {
    //     // Normal (unselected) wire
    //     pen.setColor(QColor(255, 128, 0)); // orange
    //     pen.setWidthF(m_penWidth);
    //     pen.setStyle(Qt::SolidLine);
    //     painter->setBrush(Qt::NoBrush);
    // }

    // pen.setCapStyle(Qt::RoundCap);
    // pen.setJoinStyle(Qt::RoundJoin);

    // painter->setPen(pen);
    // painter->drawPath(m_path);



    QPen drawPen = m_pen;

    // Use the animated pen width
    drawPen.setWidthF(m_penWidth);

    if (isSelected()) {
        drawPen.setColor(QColor(0, 200, 255));
        drawPen.setWidth(3);
        drawPen.setStyle(Qt::DashLine);
    }

    painter->setPen(drawPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(m_path);

    if (isSelected() && m_glowStrength > 0.01) {
        QColor glowColor(0, 200, 255, static_cast<int>(100 * m_glowStrength));
        QPen glowPen(glowColor, 6 * m_glowStrength, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(glowPen);
        painter->drawPath(m_path);
    }
}

QVariant Wire::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // if (change == QGraphicsItem::ItemSelectedChange) {
    //     bool selected = value.toBool();

    //     QPropertyAnimation *anim = new QPropertyAnimation(this, "penWidth");
    //     anim->setDuration(200);
    //     anim->setEasingCurve(QEasingCurve::OutCubic);
    //     anim->setStartValue(m_penWidth);
    //     anim->setEndValue(selected ? 5.0 : 2.0);
    //     anim->start(QAbstractAnimation::DeleteWhenStopped);
    // }

    // return QGraphicsObject::itemChange(change, value);

    if (change == QGraphicsItem::ItemSelectedChange) {
        bool selected = value.toBool();

        QPropertyAnimation *anim = new QPropertyAnimation(this, "glowStrength");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        anim->setStartValue(m_glowStrength);
        anim->setEndValue(selected ? 1.0 : 0.0);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    return QGraphicsObject::itemChange(change, value);
}

void Wire::setGlowStrength(qreal value)
{
    m_glowStrength = value;
    update();
}


QPainterPath Wire::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(m_penWidth + 10); // make it thick enough to hover easily
    return stroker.createStroke(m_path);
}

QRectF Wire::boundingRect() const
{
    // Add some margin so pen width and glow are inside
    qreal extra = m_penWidth / 2 + 6;
    return m_path.boundingRect().adjusted(-extra, -extra, extra, extra);
}
