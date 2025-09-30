#ifndef SCHEMATICVIEWER_H
#define SCHEMATICVIEWER_H

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMimeData>

class SchematicViewer : public QGraphicsView
{
    Q_OBJECT

public:
    explicit SchematicViewer(QWidget *parent = nullptr);
    void openFile(const QString &filePath, QPointF dropPos);
    QString currentPath;


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override; // 👈 grid
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
private:
    QGraphicsScene *scene;
    void showContextMenu(const QPoint &globalPos);
};

#endif // SCHEMATICVIEWER_H
