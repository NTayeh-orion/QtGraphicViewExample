#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Schematic Viewer");

    //configure the GraphicView
    schematicViewer = new SchematicViewer(this);
    ui->graphicViewFrame->setAcceptDrops(false);
    ui->graphicViewFrame->layout()->addWidget(schematicViewer);
    ui->searchLineEdit->setPlaceholderText(" Search files... ");

    //Set the window icones
    QPixmap logoIcon(":/imgs/imgsAndFiles/mainLogo.png");
    QPixmap nextIcon(":/imgs/imgsAndFiles/next.png");
    QPixmap pouseIcon(":/imgs/imgsAndFiles/pause.png");
    QPixmap stopIcon(":/imgs/imgsAndFiles/pause-button.png");
    QPixmap searchIcon(":/imgs/imgsAndFiles/loupe.png");

    //Set the window icones size
    ui->topLogoIconeLabel->setPixmap(logoIcon.scaled(50, 50, Qt::KeepAspectRatio));
    ui->nextLabel->setPixmap(nextIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->pouseLabel->setPixmap(pouseIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->stopLabel->setPixmap(stopIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->searchLabel->setPixmap(searchIcon.scaled(30, 30, Qt::KeepAspectRatio));

    // define the tree model and filter
    libExplorerModel = new QStandardItemModel(ui->treeView);
    proxy = new ChildFilterProxyModel(this);
    proxy->setSourceModel(libExplorerModel);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // add Embeded files to the lib explorer
    QDir dir(":/lib/test_lib");
    schematicViewer->currentPath = ":/lib/test_lib";
    filesToList = dir.entryList(QDir::Files);

    QIcon icon(":/imgs/imgsAndFiles/books.png");
    QStandardItem *parentItem = new QStandardItem("Embeded");
    parentItem->setIcon(icon);
    parentItem->setFlags(parentItem->flags() & ~Qt::ItemIsDragEnabled);
    libExplorerModel->appendRow(parentItem);

    // Add files to tree
    for (const QString &file : filesToList) {
        QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
        QStandardItem *child = new QStandardItem(file);
        child->setIcon(icon);
        parentItem->appendRow(child);
    }
    ui->treeView->setModel(proxy);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// slot for open file menu item
void MainWindow::on_actionOpen_File_triggered()
{
    // get the file path for the choosen file
    QString filePath
        = QFileDialog::getOpenFileName(this,
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
void MainWindow::on_actionopen_Dir_triggered() {
    //store the path for the chosen directory
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("Open Schematic File"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly              // option: only show directories
            | QFileDialog::DontResolveSymlinks // optional
    );

    if (!dirPath.isEmpty()) {
        currentPath = dirPath;
        schematicViewer->currentPath =dirPath;

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
        for (const QString &file : filesToList) {
            QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
            QStandardItem *child = new QStandardItem(file);
            child->setIcon(icon);
            parentItem->appendRow(child);
        }
    }
}

//slot to the search through the Lib Expolorer
void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}
