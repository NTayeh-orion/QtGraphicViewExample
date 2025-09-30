#include "gridblock.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene> // <-- add this
#include "pin.h"
#include "ui_block_view_form.h"
#include "verilogparser.h"
#include "wire.h"
#include <algorithm> // for std::max
#include <qfileinfo.h>
using namespace std;
GridBlock::GridBlock(const QString &text,
                     const QStringList &inputs,
                     const QStringList &outputs,
                     int gridSize = 20)
    : QGraphicsRectItem()
    , gridSize(gridSize)
{
    QLinearGradient gradient(0, 0, 0, rect().height());
    gradient.setColorAt(0, QColor(30, 30, 40)); // dark blue top
    gradient.setColorAt(1, QColor(20, 20, 25)); // darker bottom
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

    // Compute width based on the **longest port name** and block label
    auto maxPortLength = [](const QStringList &ports) {
        int maxLen = 0;
        for (QString p : ports) {
            maxLen = std::max(maxLen,  static_cast<int>(p.length()));
        }
        return maxLen;
    };

    // // ---------- 2. Compute width based on label length ----------
    int maxInputLen = maxPortLength(inputs);
    int maxOutputLen = maxPortLength(outputs);

    // estimate space for pin names (8px per char) + margin
    int pinNameWidth = std::max(maxInputLen, maxOutputLen) * 8 + 20;

    int textWidth = text.size() * 8 + 40; // block label width
    int baseWidth = 120;
    int width = std::max({baseWidth, textWidth, pinNameWidth * 2}); // left + right pins

    setRect(0, 0, width, height);

    // ---------- 3. Add label ----------
    QGraphicsTextItem *label = new QGraphicsTextItem(text, this);
    QFont font("Consolas", 12, QFont::Bold);
    label->setFont(font);
    label->setDefaultTextColor(QColor(0, 200, 255)); // neon blue
    label->setPos(width / 2 - label->boundingRect().width() / 2, -25);
    // ---------- 4. Add input pins (left side) ----------
    addPins(inputs, Pin::Input, pinSpacing, rect().left());

    // ---------- 5. Add output pins (right side) --------
    addPins(outputs, Pin::Output, pinSpacing, rect().right());

    viewForm = new BlockViewForm();
}
GridBlock::~GridBlock()
{
    qDebug() << " inside destructor ";
    qDebug() << this->childItems();
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
