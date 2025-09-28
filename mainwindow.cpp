#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <csignal>
#include <cstdlib>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Schematic Viewer");

    QFile file(":/QSS/headerStyle.qss"); // or "style.qss" if in local folder
    if (file.open(QFile::ReadOnly)) {
        QString style = file.readAll();
        ui->menubar->setStyleSheet(style);
        ui->frame_12->setStyleSheet(style);
    }

    // configure the GraphicView
    schematicViewer = new SchematicViewer(this);
    ui->graphicViewFrame->setAcceptDrops(false);
    ui->graphicViewFrame->layout()->addWidget(schematicViewer);
    ui->searchLineEdit->setPlaceholderText(" Search files... ");

    // Set the window icones
    QPixmap logoIcon(":/imgs/imgsAndFiles/mainLogo.png");
    QPixmap searchIcon(":/imgs/imgsAndFiles/loupe.png");

    // Set the window icones size
    ui->topLogoIconeLabel->setPixmap(logoIcon.scaled(50, 50, Qt::KeepAspectRatio));
    ui->searchLabel->setPixmap(searchIcon.scaled(30, 30, Qt::KeepAspectRatio));

    // define the tree model and filter
    libExplorerModel = new QStandardItemModel(ui->treeView);
    proxy = new ChildFilterProxyModel(this);
    proxy->setSourceModel(libExplorerModel);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    QDir dir(":/lib/test_lib/");
    QStringList directories = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &dire : directories) {
        addEmbededLib(dire);
    }

    ui->treeView->setModel(proxy);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addEmbededLib(QString path)
{
    QDir dir(":/lib/test_lib/" + path);
    filesToList = dir.entryList(QDir::Files);
    QIcon icon(":/imgs/imgsAndFiles/books.png");
    TreeViewItemWithPath *parentItem = new TreeViewItemWithPath(path, ":/lib/test_lib/" + path);

    parentItem->setIcon(icon);
    parentItem->setFlags(parentItem->flags() & ~Qt::ItemIsDragEnabled);
    libExplorerModel->appendRow(parentItem);

    // Add files to tree
    for (const QString &file : filesToList) {
        QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
        TreeViewItemWithPath *child = new TreeViewItemWithPath(file, ":/lib/test_lib/" + path);
        child->setIcon(icon);
        parentItem->appendRow(child);
    }
}

// slot for open file menu item
void MainWindow::on_actionOpen_File_triggered()
{
    // get the file path for the choosen file
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open Schematic File"),
                                                    QDir::homePath(),
                                                    tr("Schematic Files (*.v *.sv);;All Files (*)"));

    if (!filePath.isEmpty())
    {
        // add the choosen file to the Graphic view
        QPointF center = schematicViewer->mapToScene(schematicViewer->viewport()->rect().center());
        schematicViewer->openFile(filePath, center);
    }
}

// slot for exit menu item
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

// slot for open directory menu item
void MainWindow::on_actionopen_Dir_triggered()
{
    // store the path for the chosen directory
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("Open Schematic File"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly              // option: only show directories
            | QFileDialog::DontResolveSymlinks // optional
    );

    if (!dirPath.isEmpty())
    {
        currentPath = dirPath;
        schematicViewer->currentPath = dirPath;

        QDir dir(dirPath);

        // Get all files (not directories) and add them to the Lib expolorer
        filesToList = dir.entryList(QDir::Files);

        QFileInfo info(dirPath);

        QString dirName = info.fileName();

        QIcon icon(":/imgs/imgsAndFiles/books.png");
        QStandardItem *parentItem = new QStandardItem(dirName);
        parentItem->setIcon(icon);
        parentItem->setFlags(parentItem->flags() & ~Qt::ItemIsDragEnabled);
        libExplorerModel->appendRow(parentItem);

        // Add files to tree
        for (const QString &file : filesToList)
        {
            QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
            QStandardItem *child = new QStandardItem(file);
            child->setIcon(icon);
            parentItem->appendRow(child);
        }
    }
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}

void MainWindow::on_treeView_pressed(const QModelIndex &index)
{
    QAbstractItemModel *model = ui->treeView->model();
    if (!model) {
        return;
    }

    // Get custom data using UserRole
    QString path = model->data(index, Qt::UserRole).toString();

    if (index.parent().isValid()) { // child item check

        schematicViewer->currentPath = path;
    }
}
