#ifndef GRIDBLOCK_H
#define GRIDBLOCK_H
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>
#include <cmath>

class GridBlock : public QGraphicsRectItem {
public:
    GridBlock(const QString &text, int width=120, int height=60, int gridSize=20)
        : QGraphicsRectItem(0,0,width,height), gridSize(gridSize)
    {
        setBrush(QBrush(Qt::lightGray));
        setPen(QPen(Qt::black));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

        // Add label
        QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
        label->setDefaultTextColor(Qt::blue);
        QRectF rect = this->rect();
        label->setPos(rect.center().x() - label->boundingRect().width()/2,
                      rect.center().y() - label->boundingRect().height()/2);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == QGraphicsItem::ItemPositionChange && scene()) {
            QPointF newPos = value.toPointF();
            // Snap to grid
            qreal x = std::round(newPos.x() / gridSize) * gridSize;
            qreal y = std::round(newPos.y() / gridSize) * gridSize;
            return QPointF(x, y);
        }
        return QGraphicsRectItem::itemChange(change, value);
    }

private:
    int gridSize;
};
#endif // GRIDBLOCK_H
