#include "wire.h"
#include "pin.h"
Wire::Wire(Pin *startPin, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , startPin(startPin)
{
    QPen wirePen(QColor(255, 128, 0)); // orange
    wirePen.setWidth(2);
    wirePen.setCapStyle(Qt::RoundCap);
    wirePen.setJoinStyle(Qt::RoundJoin);
    setPen(wirePen);
    // setPen(QPen(Qt::red, 2));
    setFlag(QGraphicsItem::ItemIsSelectable); // allow selection
    setFlag(QGraphicsItem::ItemIsFocusable);
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

        setPath(path);
    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();

        QMessageBox::information(nullptr, "Error", e.what());
    }
}



