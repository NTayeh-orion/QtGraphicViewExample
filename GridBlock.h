#ifndef GRIDBLOCK_H
#define GRIDBLOCK_H
#include <QBrush>
#include <QFont>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QPen>
#include <QProcess>
#include <QRegularExpression>
#include "block_view_form.h"
#include "codehighlighter.h"
#include "pin.h"
#include <cmath>
class ResizeHandle;

class GridBlock : public QGraphicsRectItem
{
public:
    GridBlock(const QString &text,
              const QStringList &inputs,
              const QStringList &outputs,
              int gridSize);
    QString blockFilePath;
    ~GridBlock();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int gridSize;
    QStringList inputs;
    QStringList outputs;
    QList<Pin*> pins;
    ResizeHandle* handle;
    BlockViewForm *viewForm;
    // Count total pins (expanding buses like a[3:0] into 4 pins)
    int expandCount(const QStringList &ports);

    // Add pins on one side
    void addPins(const QStringList &ports, Pin::Direction dir, int spacing, qreal x);
};

#endif // GRIDBLOCK_H
