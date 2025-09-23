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

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override; // 👈 grid
    void keyPressEvent(QKeyEvent *event) override; // override the key press event

private:
    QGraphicsScene *scene;
};

#endif // SCHEMATICVIEWER_H
