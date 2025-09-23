#include "pin.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include "wire.h"

// ---------------- Pin ----------------

Pin::Pin(Direction dir, const QString &name, int bitIndex, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), dir(dir), pinName(name), m_bitIndex(bitIndex)
{
    // setRect(-5, -5, 10, 10);  // small circle
    setRect(-7, -7, 14, 14);
    // setZValue(1); // higher than the block

    setBrush(dir == Input ? Qt::red : Qt::green);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    // Label
    QGraphicsTextItem *label = new QGraphicsTextItem(name, this);
    if (dir == Input)
        // label->setPos(10, -8);
        label->setPos(10, -8 + bitIndex * 12); // shift each bit
    else
        // label->setPos(-label->boundingRect().width() - 15, -8);
        label->setPos(-label->boundingRect().width() - 15, -8 + bitIndex * 12);
}

QPointF Pin::connectionPoint() const
{
    return mapToScene(rect().center());
}

void Pin::addWire(Wire *wire)
{
    if (!wires.contains(wire))
    {
        wires.append(wire);
    }
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        tempWire = new Wire(this);
        scene()->addItem(tempWire);
        tempWire->updatePath(mapToScene(rect().center()));

        event->accept();
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}
void Pin::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempWire)
    {
        tempWire->updatePath(event->scenePos());
    }
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void Pin::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempWire && event->button() == Qt::LeftButton)
    {
        QList<QGraphicsItem *> itemsAtPos = scene()->items(event->scenePos());
        for (auto *item : itemsAtPos)
        {
            Pin *otherPin = dynamic_cast<Pin *>(item);
            if (otherPin && otherPin != this)
            {
                tempWire->setEndPin(otherPin);
                this->addWire(tempWire);
                otherPin->addWire(tempWire);
                tempWire = nullptr;
                return;
            }
        }
        scene()->removeItem(tempWire);
        delete tempWire;
        tempWire = nullptr;
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
QVariant Pin::itemChange(GraphicsItemChange change, const QVariant &value)
{
    try
    {
        if ((change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemScenePositionHasChanged) && !wires.isEmpty())
        {
            for (Wire *wire : wires)
            {
                wire->updatePath();
            }
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }
    catch (const std::exception &e)
    {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(nullptr, "Error", e.what());
    }
}
void Pin::removeWire(Wire *wire)
{
    wires.removeAll(wire);
}
