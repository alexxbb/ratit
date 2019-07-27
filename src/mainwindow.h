#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QKeyEvent>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>
#include "taskpanel.h"
#include "dispatcher.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newTaskFromFileList(const QStringList files);
    void newTaskFromDir(const QString folder);
    QStringList getImagesFromFolder(const QString &folderPath);

private:
    QStatusBar *statusbar;
    QPushButton *btn_addFiles;
    QPushButton *btn_addFromFolder;
    QPushButton *btn_convert;
    QPushButton *btn_close;
    TaskPanel *myTaskPanel;
    QString mySupportedFormatsString;
    QString buttonsStyle;
    TaskDispatcher *myDispatcher;
    QThread *myDispatcherThread;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void cb_addFiles();
    void cb_addFromFolder();
    void cb_startConvert();

    void convertionStarted();
    void convertionFinished();
    void convertionTerminated();


};

#endif // MAINWINDOW_H
