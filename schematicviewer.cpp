#include "schematicviewer.h"
#include <QFileInfo>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QPainter>
#include "GridBlock.h"
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
    qDebug() << " inside open file " ;
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    qDebug() << "filename = " + fileName;

    // Snap drop position to grid
    const int gridSize = 20;
    qreal x = std::round(dropPos.x() / gridSize) * gridSize;
    qreal y = std::round(dropPos.y() / gridSize) * gridSize;
    qDebug() << fileInfo.suffix().toLower();
    if (fileInfo.suffix().toLower() == "v") {
        ModuleInfo module = parseVerilogModule(filePath);
        qDebug() << module.name ;
        if (!module.name.isEmpty()) {
            QStringList inputs, outputs;
            qDebug() <<  "size ============ " +module.ports.size();
            for (const Port &p : module.ports) {
                if (p.dir == "input")
                    inputs.append(p.name);
                else if (p.dir == "output")
                    outputs.append(p.name);
            }
            GridBlock *block = new GridBlock(module.name, inputs, outputs, gridSize);
            block->setPos(x, y);
            block->blockFilePath = filePath;
            scene->addItem(block);
            return;
        }
    }
    else
    {
        GridBlock *block = new GridBlock(fileName, {}, {}, gridSize);
        block->setPos(x, y);
        block->blockFilePath = filePath;
        scene->addItem(block);

    }
}

void SchematicViewer::dragEnterEvent(QDragEnterEvent *event)
{
    try {
        // const QMimeData *mimeData = event->mimeData();
        // qDebug() << "=== Available MIME Formats ===";
        // qDebug() << event->mimeData()->hasUrls();
        // QStringList formats = mimeData->formats();
        // qDebug() << formats;
        // if (event->mimeData()
        //         ->hasUrls()) // checks if the drag contains file URLs (e.g., the user is dragging C:/schematics/alu.v).
        // {
        //     qDebug() << " entered dragEnterEvent";
        //     event->acceptProposedAction(); // Without calling this, Qt won’t allow dropping.
        // }

        const QMimeData *mimeData = event->mimeData();

        if (mimeData->hasFormat("application/x-qabstractitemmodeldatalist")) {
            event->acceptProposedAction();
            qDebug() << "Accepted Qt internal drag data";
        } else if (mimeData->hasUrls()) {
            event->acceptProposedAction();
            qDebug() << "Accepted standard file URLs";
        } else {
            event->ignore();
        }

    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(this, "Error", e.what());
    }
}

void SchematicViewer::dragMoveEvent(QDragMoveEvent *event) {}

QString extractFilePathFromRoleData(const QMap<int, QVariant> &roleDataMap)
{
    // Priority 1: Check UserRole (most common place for file paths)
    if (roleDataMap.contains(Qt::UserRole)) {
        QString path = roleDataMap[Qt::UserRole].toString();
        if (QFile::exists(path)) {
            return path;
        }
    }

    // Priority 2: Check ToolTipRole (sometimes used for full paths)
    if (roleDataMap.contains(Qt::ToolTipRole)) {
        QString path = roleDataMap[Qt::ToolTipRole].toString();
        if (QFile::exists(path)) {
            return path;
        }
    }

    // Priority 3: Check StatusTipRole
    if (roleDataMap.contains(Qt::StatusTipRole)) {
        QString path = roleDataMap[Qt::StatusTipRole].toString();
        if (QFile::exists(path)) {
            return path;
        }
    }

    // Priority 4: Check if any role contains a valid file path
    QList<int> roles = roleDataMap.keys();
    for (int role : roles) {
        QString potentialPath = roleDataMap[role].toString();
        if (QFile::exists(potentialPath)) {
            return potentialPath;
        }
    }

    // Priority 5: Check if display text is actually a file path
    if (roleDataMap.contains(Qt::DisplayRole)) {
        QString displayText = roleDataMap[Qt::DisplayRole].toString();
        if (QFile::exists(displayText)) {
            return displayText;
        }
    }

    return QString(); // No valid file path found
}

void SchematicViewer::dropEvent(QDropEvent *event)
{
    try {
        // qDebug() << " inside drop event " ;

        const QMimeData *mimeData = event->mimeData();
        //  QByteArray itemData = mimeData->data("application/x-qabstractitemmodeldatalist");
        // qDebug() << itemData ;
        // foreach (const QUrl &url, event->mimeData()->urls()) {
        //     QString filePath = url.toLocalFile();
        //     if (!filePath.isEmpty()) {
        //         // Map drop position to scene coordinates
        //         QPointF scenePos = mapToScene(event->position().toPoint());
        //         openFile(filePath, scenePos);
        //     }
        // }

        qDebug() << "Handling Qt internal drag data";

        QByteArray itemData = mimeData->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&itemData, QIODevice::ReadOnly);

        while (!stream.atEnd()) {
            int row, col;
            QMap<int, QVariant> roleDataMap;

            stream >> row >> col >> roleDataMap;

            qDebug() << "=== Extracted Data ===";
            qDebug() << "Row:" << row << "Col:" << col;

            // Print all available roles for debugging
            QList<int> roles = roleDataMap.keys();
            for (int role : roles) {
                QString roleName;
                switch (role) {
                case Qt::DisplayRole:
                    roleName = "DisplayRole";
                    break;
                case Qt::UserRole:
                    roleName = "UserRole";
                    break;
                case Qt::ToolTipRole:
                    roleName = "ToolTipRole";
                    break;
                case Qt::StatusTipRole:
                    roleName = "StatusTipRole";
                    break;
                case Qt::WhatsThisRole:
                    roleName = "WhatsThisRole";
                    break;
                default:
                    roleName = QString("Role %1").arg(role);
                    break;
                }
                qDebug() << roleName << ":" << roleDataMap[role].toString();
            }

            // Try to get the real file path from different roles
            QString filePath = extractFilePathFromRoleData(roleDataMap);

            // if (!filePath.isEmpty()) {
            // qDebug() << "Found file path:" << filePath; // file name
            // handleDroppedFile(filePath, dropPos);
            // } else {
            // Fallback: use display text and try to find the file
            QString displayText = currentPath + "/" + roleDataMap.value(Qt::DisplayRole).toString();
            QPointF scenePos = mapToScene(event->position().toPoint());

            // connect(scene, &QGraphicsScene::selectionChanged, this, [=]() {
            //     for (QGraphicsItem *item : scene->selectedItems()) {
            //         if (dynamic_cast<QGraphicsRectItem *>(item)) {
            //             QMessageBox::information(this, "Clicked", "Rectangle clicked!");
            //         }
            //     }
            // });

            openFile(displayText, scenePos);
            // qDebug() << "No file path found, using display text:" << displayText;
            // handleDisplayText(displayText, dropPos);
            // }
        }
    } catch (const std::exception &e) {
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

    for (qreal x = left; x < rect.right(); x += gridSize) {
        for (qreal y = top; y < rect.bottom(); y += gridSize) {
            painter->drawEllipse(QPointF(x, y), dotSize / 2.0, dotSize / 2.0);
        }
    }
}
void SchematicViewer::keyPressEvent(QKeyEvent *event)
{
    try {
        if (event->key() == Qt::Key_Delete) {
            qDebug() << " inside key delete " ;
            QList<QGraphicsItem *> selectedItems = scene->selectedItems();
            for (QGraphicsItem *item : selectedItems) {
                Wire *wire = dynamic_cast<Wire *>(item);
                if (wire) {
                    scene->removeItem(wire);
                    delete wire;
                } else if (GridBlock *block = dynamic_cast<GridBlock *>(item)) {

                     // scene->removeItem(block); //cause an exception
                    delete block; // this will clean up its wires too
                }
            }
        } else {
            // Call base class for other keys
            QGraphicsView::keyPressEvent(event);
        }
    } catch (const std::exception &e) {
        qCritical() << "Error:" << e.what();

        QMessageBox::critical(this, "Error", e.what());
    }
}
