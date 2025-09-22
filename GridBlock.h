#ifndef GRIDBLOCK_H
#define GRIDBLOCK_H
#include "pin.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>
#include <cmath>
#include <QRegularExpression>
class GridBlock : public QGraphicsRectItem {
public:
    GridBlock(const QString &text, const QStringList &inputs ,
              const QStringList &outputs, int width=120, int height=60, int gridSize=20)
        : QGraphicsRectItem(0,0,width,height), gridSize(gridSize)
    {
        setBrush(QBrush(Qt::lightGray));
        setPen(QPen(Qt::black));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
        setAcceptedMouseButtons(Qt::LeftButton);

        // Add label
        QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
        label->setDefaultTextColor(Qt::blue);
        QRectF rect = this->rect();
        label->setPos(rect.center().x() - label->boundingRect().width()/2,
                      rect.center().y() - label->boundingRect().height()/2);
        // Add pins
        int pinSpacing = height / (inputs.size()+1);
        for (int i=0; i<inputs.size(); i++) {
            // Pin *pin = new Pin(Pin::Input, inputs[i], this);
            // pin->setPos(rect.left(), rect.top() + (i+1)*pinSpacing);
            // -0----- new
            QString portName = inputs[i];
            // Check if bus, e.g., "b[3:0]"
            QRegularExpression busRegex(R"((\w+)\[(\d+):(\d+)\])");
            QRegularExpressionMatch match = busRegex.match(portName);
            if (match.hasMatch()) {
                QString baseName = match.captured(1);
                int msb = match.captured(2).toInt();
                int lsb = match.captured(3).toInt();
                for (int bit = msb; bit >= lsb; --bit) {
                    Pin *pin = new Pin(Pin::Input, baseName + QString("[%1]").arg(bit), msb-bit, this);
                    pin->setPos(rect.left(), rect.top() + (i+1)*pinSpacing + (msb-bit)*12);
                }
            } else {
                Pin *pin = new Pin(Pin::Input, portName, 0, this);
                pin->setPos(rect.left(), rect.top() + (i+1)*pinSpacing);
            }
        }
        pinSpacing = height / (outputs.size()+1);
        for (int i=0; i<outputs.size(); i++) {
            // Pin *pin = new Pin(Pin::Output, outputs[i], this);
            // pin->setPos(rect.right(), rect.top() + (i+1)*pinSpacing);
            // --00-------- new -0----------------
            QString portName = outputs[i];
            QRegularExpression busRegex(R"((\w+)\[(\d+):(\d+)\])");
            QRegularExpressionMatch match = busRegex.match(portName);
            if (match.hasMatch()) {
                QString baseName = match.captured(1);
                int msb = match.captured(2).toInt();
                int lsb = match.captured(3).toInt();
                for (int bit = msb; bit >= lsb; --bit) {
                    Pin *pin = new Pin(Pin::Output, baseName + QString("[%1]").arg(bit), msb-bit, this);
                    pin->setPos(rect.right(), rect.top() + (i+1)*pinSpacing + (msb-bit)*12);
                }
            } else {
                Pin *pin = new Pin(Pin::Output, portName, 0, this);
                pin->setPos(rect.right(), rect.top() + (i+1)*pinSpacing);
            }
        }
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
