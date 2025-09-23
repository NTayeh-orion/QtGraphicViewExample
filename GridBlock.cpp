#include "gridblock.h"
#include "pin.h"
#include "wire.h"
#include <QGraphicsScene> // <-- add this

GridBlock::GridBlock(const QString &text,
                     const QStringList &inputs,
                     const QStringList &outputs,
                     int gridSize = 20)
    : QGraphicsRectItem()
    , gridSize(gridSize)
{
    setBrush(QBrush(Qt::lightGray));
    setPen(QPen(Qt::black));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
             | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);

    // ---------- 1. Count expanded pins ----------
    int inputCount = expandCount(inputs);
    int outputCount = expandCount(outputs);
    int maxPins = std::max(inputCount, outputCount);

    int pinSpacing = 20; // px spacing between pins
    int minHeight = 60;  // minimum block height
    int height = std::max(minHeight, maxPins * pinSpacing + 20);

    // ---------- 2. Compute width based on label length ----------
    int baseWidth = 120;
    int textWidth = text.size() * 8; // rough estimate (8px per char)
    int width = std::max(baseWidth, textWidth + 40);

    setRect(0, 0, width, height);

    // ---------- 3. Add label ----------
    QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
    label->setDefaultTextColor(Qt::blue);
    label->setPos(width / 2 - label->boundingRect().width() / 2, -20);

    // ---------- 4. Add input pins (left side) ----------
    addPins(inputs, Pin::Input, pinSpacing, rect().left());

    // ---------- 5. Add output pins (right side) ----------
    addPins(outputs, Pin::Output, pinSpacing, rect().right());
}
GridBlock::~GridBlock()
{
    for (QGraphicsItem *child : this->childItems()) {
        Pin *pin = dynamic_cast<Pin *>(child);
        if (pin) {
            // Make a copy of the wire list because it will change as we delete
            QList<Wire *> connectedWires = pin->getWires();
            for (Wire *wire : connectedWires) {
                if (wire->scene()) {
                    wire->scene()->removeItem(wire);
                }
                delete wire; // destructor of Wire calls disconnectPins()
            }
        }
    }
}

QVariant GridBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{
    try {
        if (change == QGraphicsItem::ItemPositionChange && scene()) {
            QPointF newPos = value.toPointF();
            // Snap to grid
            qreal x = std::round(newPos.x() / gridSize) * gridSize;
            qreal y = std::round(newPos.y() / gridSize) * gridSize;
            return QPointF(x, y);
        }
        return QGraphicsRectItem::itemChange(change, value);
    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();
        QMessageBox::information(nullptr, "Error", e.what());
    }
    return value;
}

int GridBlock::expandCount(const QStringList &ports)
{
    int count = 0;
    QRegularExpression busRegex(R"((\w+)\[(\d+):(\d+)\])");
    for (auto &p : ports) {
        QRegularExpressionMatch match = busRegex.match(p);
        if (match.hasMatch()) {
            int msb = match.captured(2).toInt();
            int lsb = match.captured(3).toInt();
            count += std::abs(msb - lsb) + 1;
        } else {
            count++;
        }
    }
    return count;
}

void GridBlock::addPins(const QStringList &ports, Pin::Direction dir, int spacing, qreal x)
{
    int pinIndex = 0;
    QRegularExpression busRegex(R"((\w+)\[(\d+):(\d+)\])");

    for (auto &port : ports) {
        QRegularExpressionMatch match = busRegex.match(port);
        if (match.hasMatch()) {
            QString baseName = match.captured(1);
            int msb = match.captured(2).toInt();
            int lsb = match.captured(3).toInt();
            int step = (msb > lsb) ? -1 : 1;
            for (int bit = msb; bit != lsb + step; bit += step) {
                Pin *pin = new Pin(dir, baseName + QString("[%1]").arg(bit), bit, this);
                pin->setPos(x, 20 + pinIndex * spacing);
                pinIndex++;
            }
        } else {
            Pin *pin = new Pin(dir, port, 0, this);
            pin->setPos(x, 20 + pinIndex * spacing);
            pinIndex++;
        }
    }
}
