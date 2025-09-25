#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QSysInfo>
#include <qboxlayout.h>

#include "childFilterProxyModel.h"
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
    void on_actionOpen_File_triggered(); // slot to handle the open file menue item

    void on_actionExit_triggered(); // slot to handle the exit menue item

    void on_actionopen_Dir_triggered(); // slot to handle the open directory menue item

    void on_searchLineEdit_textChanged(
        const QString &arg1); // slot to handle the text change for the search lineEdit

private:
    Ui::MainWindow *ui;
    SchematicViewer *schematicViewer;

    QString dirPath;
    QStringList filesToList;
    QString currentPath;
    QString currentFilePath;

    QStandardItemModel *libExplorerModel;
    ChildFilterProxyModel *proxy;
};
#endif // MAINWINDOW_H
