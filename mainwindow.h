#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QSysInfo>
#include <qboxlayout.h>

#include "schematicviewer.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_File_triggered();

    void on_actionExit_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionopen_Dir_triggered();


private:
    Ui::MainWindow *ui;
    SchematicViewer *schematicViewer;

    QString dirPath;
    QStringList filesToList;
    QString currentPath;
    QString currentFilePath;

    QVBoxLayout listWidgetLayout;
    QVBoxLayout listEmbededLibLayout;
};
#endif // MAINWINDOW_H
