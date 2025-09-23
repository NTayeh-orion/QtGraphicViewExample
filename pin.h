#ifndef PIN_H
#define PIN_H
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
class Wire; // forward declaration

class Pin : public QGraphicsEllipseItem
{
public:
    enum Direction
    {
        Input,
        Output
    };

    Pin(Direction dir, const QString &name, int bitIndex = 0, QGraphicsItem *parent = nullptr);

    int bitIndex() const { return m_bitIndex; }

    QPointF connectionPoint() const;
    void addWire(Wire *wire);
    Direction direction() const { return dir; }
    QString name() const { return pinName; }
    void removeWire(Wire *wire);
    QList<Wire *> getWires() const { return wires; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    int m_bitIndex = 0;
    Direction dir;
    QString pinName;
    QList<Wire *> wires;
    Wire *tempWire = nullptr;
};
#endif // PIN_H
