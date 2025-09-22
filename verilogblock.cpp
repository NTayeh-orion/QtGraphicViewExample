#include "verilogblock.h"
#include <QPainter>
#include <cmath>

VerilogBlock::VerilogBlock(const ModuleInfo &info, int gridSize)
    : module(info), gridSize(gridSize) {
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF VerilogBlock::boundingRect() const {
    int height = 60 + module.ports.size() * 20;
    return QRectF(0, 0, 160, height);
}

void VerilogBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();

    // Box
    painter->setPen(Qt::black);
    painter->setBrush(QColor(230, 230, 250));
    painter->drawRect(rect);

    // Module name
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    painter->drawText(rect.topLeft() + QPointF(5, 15), module.name);

    // Ports
    int yOffset = 35;
    for (const Port &p : module.ports) {
        if (p.dir == "input") {
            painter->drawText(QPointF(rect.left() + 5, yOffset), p.name);
            painter->drawLine(rect.left(), yOffset, rect.left() - 10, yOffset);
        } else { // output
            painter->drawText(QPointF(rect.right() - 45, yOffset), p.name);
            painter->drawLine(rect.right(), yOffset, rect.right() + 10, yOffset);
        }
        yOffset += 20;
    }
}

QVariant VerilogBlock::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == QGraphicsItem::ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        qreal x = std::round(newPos.x() / gridSize) * gridSize;
        qreal y = std::round(newPos.y() / gridSize) * gridSize;
        return QPointF(x, y);
    }
    return QGraphicsItem::itemChange(change, value);
}
