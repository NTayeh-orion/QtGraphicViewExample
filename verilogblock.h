
#ifndef VERILOGBLOCK_H
#define VERILOGBLOCK_H
#include <QGraphicsItem>
#include "verilogparser.h"

class VerilogBlock : public QGraphicsItem {
public:
    VerilogBlock(const ModuleInfo &info, int gridSize = 20);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    ModuleInfo module;
    int gridSize;
};
#endif // VERILOGBLOCK_H
