// #include "schematicviewer.h"
// #include <QGraphicsRectItem>
// #include <QGraphicsTextItem>
// #include <QFileInfo>
// #include <QPainter>

// SchematicViewer::SchematicViewer(QWidget *parent)
//     : QGraphicsView(parent) {
//     scene = new QGraphicsScene(this);
//     setScene(scene);

//     setRenderHint(QPainter::Antialiasing);
//     setDragMode(QGraphicsView::ScrollHandDrag);
//     setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

//     setAcceptDrops(true);
// }

// void SchematicViewer::openFile(const QString &filePath) {
//     scene->clear();
//     qDebug() << "Opening schematic file:" << filePath;

//     QFileInfo fileInfo(filePath);
//     QString fileName = fileInfo.fileName();

//     // --- Grid size ---
//     const int gridSize = 20;

//     // Snap position to grid (centered at origin)
//     int x = (0 / gridSize) * gridSize;
//     int y = (0 / gridSize) * gridSize;

//     // --- Draw block ---
//     QGraphicsRectItem *block = scene->addRect(x, y, 120, 60,
//                                               QPen(Qt::black),
//                                               QBrush(Qt::lightGray));

//     // --- Add file name label inside block ---
//     QGraphicsTextItem *label = scene->addText(fileName, QFont("Arial", 12));
//     label->setDefaultTextColor(Qt::blue);
//     QRectF rect = block->rect();
//     label->setPos(rect.center().x() - label->boundingRect().width() / 2,
//                   rect.center().y() - label->boundingRect().height() / 2);

//     label->setParentItem(block); // make label move with block
// }

// void SchematicViewer::dragEnterEvent(QDragEnterEvent *event) {
//     if (event->mimeData()->hasUrls())
//         event->acceptProposedAction();
// }

// void SchematicViewer::dropEvent(QDropEvent *event) {
//     foreach (const QUrl &url, event->mimeData()->urls()) {
//         QString filePath = url.toLocalFile();
//         if (!filePath.isEmpty()) {
//             openFile(filePath);
//         }
//     }
// }

// void SchematicViewer::wheelEvent(QWheelEvent *event) {
//     const double scaleFactor = 1.15;
//     if (event->angleDelta().y() > 0)
//         scale(scaleFactor, scaleFactor);
//     else
//         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
// }

// void SchematicViewer::drawBackground(QPainter *painter, const QRectF &rect) {
//     const int gridSize = 20;
//     const int dotSize = 2; // size of grid points

//     painter->setPen(Qt::NoPen);
//     painter->setBrush(QColor(200, 200, 200)); // light gray dots

//     // Compute starting points aligned to grid
//     qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
//     qreal top  = int(rect.top())  - (int(rect.top())  % gridSize);

//     // Draw points at grid intersections
//     for (qreal x = left; x < rect.right(); x += gridSize) {
//         for (qreal y = top; y < rect.bottom(); y += gridSize) {
//             painter->drawEllipse(QPointF(x, y), dotSize / 2.0, dotSize / 2.0);
//         }
//     }
// }



#include "schematicviewer.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFileInfo>
#include <QPainter>
#include <cmath>

// SchematicViewer::SchematicViewer(QWidget *parent)
//     : QGraphicsView(parent) {
//     scene = new QGraphicsScene(this);
//     setScene(scene);

//     setRenderHint(QPainter::Antialiasing);
//     setDragMode(QGraphicsView::ScrollHandDrag);
//     setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

//     setAcceptDrops(true);
// }

// void SchematicViewer::openFile(const QString &filePath) {
//     scene->clear();
//     qDebug() << "Opening schematic file:" << filePath;

//     QFileInfo fileInfo(filePath);
//     QString fileName = fileInfo.fileName();

//     // --- Grid settings ---
//     const int gridSize = 20;

//     // Snap a position to the nearest grid point (here center of scene ~ (0,0))
//     int rawX = 0;
//     int rawY = 0;
//     int snappedX = std::round(double(rawX) / gridSize) * gridSize;
//     int snappedY = std::round(double(rawY) / gridSize) * gridSize;

//     // --- Draw block ---
//     QGraphicsRectItem *block = scene->addRect(0, 0, 120, 60,
//                                               QPen(Qt::black),
//                                               QBrush(Qt::lightGray));
//     block->setPos(snappedX, snappedY);

//     // --- Add file name inside block ---
//     QGraphicsTextItem *label = scene->addText(fileName, QFont("Arial", 12));
//     label->setDefaultTextColor(Qt::blue);

//     QRectF rect = block->rect();
//     label->setPos(rect.center().x() - label->boundingRect().width() / 2,
//                   rect.center().y() - label->boundingRect().height() / 2);

//     label->setParentItem(block); // stick label to block
// }

// void SchematicViewer::dragEnterEvent(QDragEnterEvent *event) {
//     if (event->mimeData()->hasUrls())
//         event->acceptProposedAction();
// }

// void SchematicViewer::dropEvent(QDropEvent *event) {
//     foreach (const QUrl &url, event->mimeData()->urls()) {
//         QString filePath = url.toLocalFile();
//         if (!filePath.isEmpty()) {
//             openFile(filePath);
//         }
//     }
// }

// void SchematicViewer::wheelEvent(QWheelEvent *event) {
//     const double scaleFactor = 1.15;
//     if (event->angleDelta().y() > 0)
//         scale(scaleFactor, scaleFactor);
//     else
//         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
// }

// void SchematicViewer::drawBackground(QPainter *painter, const QRectF &rect) {
//     const int gridSize = 20;
//     const int dotSize = 2;

//     painter->setPen(Qt::NoPen);
//     painter->setBrush(QColor(180, 180, 180)); // darker gray for visibility

//     // Start aligned to grid
//     qreal left = std::floor(rect.left() / gridSize) * gridSize;
//     qreal top  = std::floor(rect.top()  / gridSize) * gridSize;

//     // Draw grid points
//     for (qreal x = left; x < rect.right(); x += gridSize) {
//         for (qreal y = top; y < rect.bottom(); y += gridSize) {
//             painter->drawEllipse(QPointF(x, y), dotSize / 2.0, dotSize / 2.0);
//         }
//     }
// }






#include "schematicviewer.h"
#include "GridBlock.h"
#include <QFileInfo>
#include <QPainter>

SchematicViewer::SchematicViewer(QWidget *parent)
    : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setAcceptDrops(true);
}

void SchematicViewer::openFile(const QString &filePath, QPointF dropPos) {
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    // Snap drop position to grid
    const int gridSize = 20;
    qreal x = std::round(dropPos.x() / gridSize) * gridSize;
    qreal y = std::round(dropPos.y() / gridSize) * gridSize;

    GridBlock *block = new GridBlock(fileName, 120, 60, gridSize);
    block->setPos(x, y);

    scene->addItem(block);
}

void SchematicViewer::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void SchematicViewer::dropEvent(QDropEvent *event) {
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        if (!filePath.isEmpty()) {
            // Map drop position to scene coordinates
            QPointF scenePos = mapToScene(event->position().toPoint());
            openFile(filePath, scenePos);
        }
    }
}

void SchematicViewer::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void SchematicViewer::drawBackground(QPainter *painter, const QRectF &rect) {
    const int gridSize = 20;
    const int dotSize = 2;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(180, 180, 180));

    qreal left = std::floor(rect.left() / gridSize) * gridSize;
    qreal top  = std::floor(rect.top()  / gridSize) * gridSize;

    for (qreal x = left; x < rect.right(); x += gridSize) {
        for (qreal y = top; y < rect.bottom(); y += gridSize) {
            painter->drawEllipse(QPointF(x, y), dotSize / 2.0, dotSize / 2.0);
        }
    }
}
