#include "pin.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QToolTip>
#include "wire.h"
#include <qgraphicsview.h>
#include <qpainter.h>
// ---------------- Pin ----------------

Pin::Pin(Direction dir, const QString &name, int bitIndex, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , dir(dir)
    , pinName(name)
    , m_bitIndex(bitIndex)
{
    // setRect(-7, -7, 14, 14);
    m_rect = QRectF(-7, -7, 14, 14);
    // setZValue(1); // higher than the block

    // setBrush(dir == Input ? Qt::red : Qt::green);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    // Label
    QGraphicsTextItem *label = new QGraphicsTextItem(name, this);
    if (dir == Input)
        label->setPos(10, -8 + bitIndex * 12); // shift each bit
    else
        label->setPos(-label->boundingRect().width() - 15, -8 + bitIndex * 12);
}
QRectF Pin::boundingRect() const
{
    return m_rect;
}

QPointF Pin::connectionPoint() const
{
    // return mapToScene(rect().center());
    return mapToScene(m_rect.center());
}

void Pin::addWire(Wire *wire)
{
    if (!wires.contains(wire)) {
        wires.append(wire);
    }
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        tempWire = new Wire(this);
        scene()->addItem(tempWire);
        // tempWire->updatePath(mapToScene(rect().center()));
        tempWire->updatePath(mapToScene(m_rect.center()));

        event->accept();
    }
    // QGraphicsEllipseItem::mousePressEvent(event);
    QGraphicsItem::mousePressEvent(event);
}
void Pin::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempWire) {
        tempWire->updatePath(event->scenePos());
    }
    // QGraphicsEllipseItem::mouseMoveEvent(event);
    QGraphicsItem::mouseMoveEvent(event);
}

void Pin::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempWire && event->button() == Qt::LeftButton) {
        QList<QGraphicsItem *> itemsAtPos = scene()->items(event->scenePos());
        for (auto *item : itemsAtPos) {
            Pin *otherPin = dynamic_cast<Pin *>(item);
            if (otherPin && otherPin != this) {
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
    // QGraphicsEllipseItem::mouseReleaseEvent(event);
    QGraphicsItem::mouseReleaseEvent(event);
}
QVariant Pin::itemChange(GraphicsItemChange change, const QVariant &value)
{
    try {
        if ((change == QGraphicsItem::ItemPositionChange
             || change == QGraphicsItem::ItemScenePositionHasChanged)
            && !wires.isEmpty()) {
            for (Wire *wire : wires) {
                wire->updatePath();
            }
        }
        // return QGraphicsEllipseItem::itemChange(change, value);
        return QGraphicsItem::itemChange(change, value);
    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(nullptr, "Error", e.what());
    }
}
void Pin::removeWire(Wire *wire)
{
    wires.removeAll(wire);
}

void Pin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    update(); // trigger repaint
    // small scale-up animation
    QPropertyAnimation *anim = new QPropertyAnimation(this, "scale");
    anim->setDuration(120);
    anim->setStartValue(scale());
    anim->setEndValue(1.2);  // enlarge 20%
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // -------------------
    // if (tooltip == nullptr)
        tooltip = new CustomToolTip();

    if (scene() && !scene()->views().isEmpty()) {
        QGraphicsView *view = scene()->views().first();
        QPointF scenePos = mapToScene(m_rect.center().x(), m_rect.top());
        tooltip->showText(view, scenePos, QString("Pin: %1\nDirection: %2\nBit: %3")
                                              .arg(pinName)
                                              .arg(dir == Input ? "Input" : "Output")
                                              .arg(32));
    }

    QGraphicsItem::hoverEnterEvent(event);
}

void Pin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    update(); // trigger repaint

    // scale back to normal
    QPropertyAnimation *anim = new QPropertyAnimation(this, "scale");
    anim->setDuration(120);
    anim->setStartValue(scale());
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    // Hide tooltip
    // QToolTip::hideText();
    // if (tooltip)
        tooltip->hideSmooth();
    delete tooltip;
    QGraphicsItem::hoverLeaveEvent(event);
}

void Pin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QColor fillColor;
    QPen dynamicPen;
    qDebug() << "inside paint method ";
    if (dir == Input) {
        // QPen inputPen(QColor(0, 200, 255)); // cyan outline
        dynamicPen.setColor(QColor(0, 200, 255));
        dynamicPen.setWidth(m_hovered ? 3 : 2);
        painter->setBrush(QBrush(QColor(20, 20, 20))); // dark gray fill
        painter->drawRect(m_rect);
        fillColor = m_hovered ? QColor(0, 180, 255)   // bright cyan when hovered
                              : QColor(20, 20, 20);   // dark gray when idle
    } else {
        // draw circle for output
        dynamicPen.setColor(QColor(255, 100, 0));
        dynamicPen.setWidth(m_hovered ? 3 : 2);

        painter->setBrush(QBrush(QColor(30, 20, 30))); // darker fill
        painter->drawEllipse(m_rect);

        fillColor = m_hovered ? QColor(255, 140, 60)  // bright orange when hovered
                              : QColor(30, 20, 30);   // dark purple-gray when idle
    }

    painter->setPen(dynamicPen); // thicker outline on hover
    painter->setBrush(QBrush(fillColor));

    if (dir == Input)
        painter->drawRect(m_rect);   // square for input
    else
        painter->drawEllipse(m_rect); // circle for output



}
