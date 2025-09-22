#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    schematicViewer = new SchematicViewer(this);
    setCentralWidget(schematicViewer);

    createMenus();
    resize(800, 600);
    setWindowTitle("Schematic Viewer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Open Schematic File"),
            QDir::homePath(),
            tr("Schematic Files (*.json *.xml *.netlist *.v *.sv);;All Files (*)")
            );

        if (!filePath.isEmpty()) {
            QPointF center = schematicViewer->mapToScene(schematicViewer->viewport()->rect().center());

            schematicViewer->openFile(filePath,center);
        }
    });

    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
}
