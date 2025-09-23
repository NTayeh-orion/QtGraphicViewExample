#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    schematicViewer = new SchematicViewer(this);
    ui->graphicViewFrame->layout()->addWidget(schematicViewer);
    ui->lineEdit->setPlaceholderText(" Search files... ");

    // resize(800, 600);
    setWindowTitle("Schematic Viewer");

    QPixmap logoIcon(":/imgs/imgsAndFiles/shortcut.png");
    QPixmap nextIcon(":/imgs/imgsAndFiles/next.png");
    QPixmap pouseIcon(":/imgs/imgsAndFiles/pause.png");
    QPixmap stopIcon(":/imgs/imgsAndFiles/pause-button.png");
    QPixmap searchIcon(":/imgs/imgsAndFiles/loupe.png");

    ui->topLogoIconeLabel->setPixmap(logoIcon.scaled(40, 40, Qt::KeepAspectRatio));
    ui->nextLabel->setPixmap(nextIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->pouseLabel->setPixmap(pouseIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->stopLabel->setPixmap(stopIcon.scaled(30, 30, Qt::KeepAspectRatio));
    ui->searchLabel->setPixmap(searchIcon.scaled(30, 30, Qt::KeepAspectRatio));

    terminalTab = new QWidget();

    QProcess *proc = new QProcess(terminalTab);

#ifdef Q_OS_WIN
    // Windows → launch cmd.exe (but no -into support)
    proc->start("cmd.exe");
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    // Linux/Mac → embed xterm into the widget
    proc->start("xterm", {"-into", QString::number(container->winId())});
#endif

    ui->terminalTabWidget->addTab(terminalTab, "Terminal");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("Open Schematic File"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly              // option: only show directories
            | QFileDialog::DontResolveSymlinks // optional
    );

    if (!dirPath.isEmpty())
    {

        QPointF center = schematicViewer->mapToScene(schematicViewer->viewport()->rect().center());
        schematicViewer->openFile(dirPath, center);

        QDir dir(dirPath);

        // Get all files (not directories)
        filesToList = dir.entryList(QDir::Files);

        // Clear previous items
        ui->listWidget->clear();

        // Add files to QListWidget
        for (const QString &file : filesToList)
        {

            QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
            QListWidgetItem *item = new QListWidgetItem(icon, file);

            ui->listWidget->addItem(item); // only name
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->listWidget->clear();
    if (!filesToList.empty())
    {
        for (const QString &file : filesToList)
        {
            if (file.contains(arg1, Qt::CaseInsensitive))
            {
                QIcon icon(":/imgs/imgsAndFiles/google-docs.png"); // your icon resource or file path
                QListWidgetItem *item = new QListWidgetItem(icon, file);

                ui->listWidget->addItem(item); // only name
            }
        }
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
}

void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    QMessageBox::information(this, "ffd", item->text());
}
