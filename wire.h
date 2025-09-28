#ifndef WIRE_H
#define WIRE_H
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QPen>
#include <QPointF>
#include <QVector>
class Pin; // forward declaration

class Wire : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(qreal penWidth READ penWidth WRITE setPenWidth)
    Q_PROPERTY(qreal glowStrength READ glowStrength WRITE setGlowStrength)
public:
    Wire(Pin *startPin, QGraphicsItem *parent = nullptr);

    void setEndPin(Pin *endPin);
    // void updatePosition();
    void updatePath(const QPointF &mousePos = QPointF());

    // Glow animation property
    qreal glowStrength() const { return m_glowStrength; }
    void setGlowStrength(qreal value);

    // ---------------- for view -----------------
    qreal penWidth() const { return m_penWidth; }
    void setPenWidth(qreal w)
    {
        m_penWidth = w;
        update();
    }
    QRectF boundingRect() const override;
    // { return m_path.boundingRect(); }

    ~Wire(); // destructor
    void disconnectPins();
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    QPainterPath shape() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Pin *startPin;
    Pin *endPin = nullptr;
    qreal m_penWidth = 2;
    QPainterPath m_path;
    qreal m_glowStrength = 0.0; // animated value
    QPen m_pen;
};
#endif // WIRE_H
