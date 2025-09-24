#include "gridblock.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene> // <-- add this
#include "pin.h"
#include "ui_block_view_form.h"
#include "wire.h"
#include <qfileinfo.h>
GridBlock::GridBlock(const QString &text,
                     const QStringList &inputs,
                     const QStringList &outputs,
                     int gridSize = 20)
    : QGraphicsRectItem()
    , gridSize(gridSize)
{
    QLinearGradient gradient(0, 0, 0, rect().height());
    gradient.setColorAt(0, QColor(30, 30, 40));   // dark blue top
    gradient.setColorAt(1, QColor(20, 20, 25));   // darker bottom
    setBrush(QBrush(gradient));
    // setBrush(QBrush(Qt::lightGray));

    QPen pen(QColor(0, 255, 255)); // cyan
    pen.setWidth(2);
    setPen(pen);
    // setPen(QPen(Qt::black));

    QGraphicsDropShadowEffect *glow = new QGraphicsDropShadowEffect;
    glow->setBlurRadius(25);
    glow->setColor(QColor(0, 255, 255, 150));
    glow->setOffset(0, 0);
    setGraphicsEffect(glow);

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
    // QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
    // label->setDefaultTextColor(Qt::blue);
    // label->setPos(width / 2 - label->boundingRect().width() / 2, -20);
    QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
    QFont font("Consolas", 12, QFont::Bold);
    label->setFont(font);
    label->setDefaultTextColor(QColor(0, 200, 255)); // neon blue
    label->setPos(width / 2 - label->boundingRect().width() / 2, -25);
    // ---------- 4. Add input pins (left side) ----------
    addPins(inputs, Pin::Input, pinSpacing, rect().left());

    // ---------- 5. Add output pins (right side) ----------
    addPins(outputs, Pin::Output, pinSpacing, rect().right());

    viewForm = new BlockViewForm();
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

void GridBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QFile file(blockFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Cannot open file: " + blockFilePath);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll(); // read the whole file
    file.close();
    CodeHighlighter *highlighter = new CodeHighlighter(viewForm->ui->textEdit->document());

    viewForm->ui->textEdit->setPlainText(content); // display in QTextEdit
    viewForm->show();
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
                pin->setBrush(QBrush(QColor(10, 10, 15)));  // dark pin background
                QPen pinPen(QColor(0, 255, 180));           // neon green outline
                pinPen.setWidth(2);
                pin->setPen(pinPen);

                pin->setPos(x, 20 + pinIndex * spacing);
                pinIndex++;
            }
        } else {
            Pin *pin = new Pin(dir, port, 0, this);
            pin->setBrush(QBrush(QColor(10, 10, 15)));  // dark pin background
            QPen pinPen(QColor(0, 255, 180));           // neon green outline
            pinPen.setWidth(2);
            pin->setPen(pinPen);
            pin->setPos(x, 20 + pinIndex * spacing);
            pinIndex++;
        }
    }
}
