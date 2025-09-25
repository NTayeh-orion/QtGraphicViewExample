#ifndef WIRE_H
#define WIRE_H
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QPen>
#include <QVector>
#include <QPointF>
class Pin; // forward declaration

class Wire : public QGraphicsPathItem
{
public:
    Wire(Pin *startPin, QGraphicsItem *parent = nullptr);

    void setEndPin(Pin *endPin);
    // void updatePosition();
    void updatePath(const QPointF &mousePos = QPointF());

    ~Wire(); // destructor
    void disconnectPins();


private:
    Pin *startPin;
    Pin *endPin = nullptr;
};
#endif // WIRE_H
