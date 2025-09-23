#include "schematicviewer.h"
#include <QFileInfo>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QPainter>
#include "GridBlock.h"
#include "gridblock.h"
#include "verilogparser.h"
#include "wire.h"
#include <cmath>
SchematicViewer::SchematicViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    this->viewport()->setAcceptDrops(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setAcceptDrops(true);
}

void SchematicViewer::openFile(const QString &filePath, QPointF dropPos)
{
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    // Snap drop position to grid
    const int gridSize = 20;
    qreal x = std::round(dropPos.x() / gridSize) * gridSize;
    qreal y = std::round(dropPos.y() / gridSize) * gridSize;

    if (fileInfo.suffix().toLower() == "v")
    {
        ModuleInfo module = parseVerilogModule(filePath);
        if (!module.name.isEmpty())
        {
            QStringList inputs, outputs;
            for (const Port &p : module.ports)
            {
                if (p.dir == "input")
                    inputs.append(p.name);
                else if (p.dir == "output")
                    outputs.append(p.name);
            }
            GridBlock *block = new GridBlock(module.name, inputs, outputs, gridSize);
            block->setPos(x, y);
            scene->addItem(block);
            return;
        }
    }
    GridBlock *block = new GridBlock(fileName, {}, {}, gridSize);
    block->setPos(x, y);

    scene->addItem(block);
}

void SchematicViewer::dragEnterEvent(QDragEnterEvent *event)
{
    try
    {
        if (event->mimeData()->hasUrls())  // checks if the drag contains file URLs (e.g., the user is dragging C:/schematics/alu.v).
            event->acceptProposedAction(); // Without calling this, Qt won’t allow dropping.
    }
    catch (const std::exception &e)
    {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(this, "Error", e.what());
    }
}

void SchematicViewer::dropEvent(QDropEvent *event)
{
    try
    {
        foreach (const QUrl &url, event->mimeData()->urls())
        {
            QString filePath = url.toLocalFile();
            if (!filePath.isEmpty())
            {
                // Map drop position to scene coordinates
                QPointF scenePos = mapToScene(event->position().toPoint());
                openFile(filePath, scenePos);
            }
        }
    }
    catch (const std::exception &e)
    {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(this, "Error", e.what());
    }
}

void SchematicViewer::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void SchematicViewer::drawBackground(QPainter *painter, const QRectF &rect)
{
    const int gridSize = 20;
    const int dotSize = 2;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(180, 180, 180));

    qreal left = std::floor(rect.left() / gridSize) * gridSize;
    qreal top = std::floor(rect.top() / gridSize) * gridSize;

    for (qreal x = left; x < rect.right(); x += gridSize)
    {
        for (qreal y = top; y < rect.bottom(); y += gridSize)
        {
            painter->drawEllipse(QPointF(x, y), dotSize / 2.0, dotSize / 2.0);
        }
    }
}
void SchematicViewer::keyPressEvent(QKeyEvent *event)
{
    try
    {
        if (event->key() == Qt::Key_Delete)
        {
            QList<QGraphicsItem *> selectedItems = scene->selectedItems();
            for (QGraphicsItem *item : selectedItems)
            {
                Wire *wire = dynamic_cast<Wire *>(item);
                if (wire)
                {
                    scene->removeItem(wire);
                    delete wire;
                }
                else if (GridBlock *block = dynamic_cast<GridBlock *>(item))
                {
                    scene->removeItem(block);
                    delete block; // this will clean up its wires too
                }
            }
        }
        else
        {
            // Call base class for other keys
            QGraphicsView::keyPressEvent(event);
        }
    }
    catch (const std::exception &e)
    {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(this, "Error", e.what());
    }
}
