#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QList>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QIcon>
#include <QLine>
#include <QThread>
#include <QApplication>
#include "mainwindow.h"
#include "defines.h"
#include "taskpanel.h"
#include "taskwidget.h"
#include "dispatcher.h"
#include "converter.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString("RATit %1").arg(QString("1.0.0")));
    setWindowIcon(QIcon(":/icons/app_icon.gif"));
    setMinimumSize(820, 500);
    move(500,500);
    QWidget *mwidget = new QWidget;
    setCentralWidget(mwidget);

    statusbar = new QStatusBar;
    setStatusBar(statusbar);
    auto *myRootLayout= new QHBoxLayout;
    auto *myTaskPanelLayout = new QVBoxLayout;
    auto *myButtonsLayout = new QVBoxLayout;

    myTaskPanel = new TaskPanel(this);
    auto *scroll = new QScrollArea;
    scroll->setWidget(myTaskPanel);
    scroll->setWidgetResizable(true);
    myTaskPanelLayout->addWidget(scroll);
    myRootLayout->addLayout(myTaskPanelLayout);


    btn_convert = new QPushButton("CONVERT");
    btn_convert->setMinimumHeight(80);
    btn_close = new QPushButton("CLOSE");
    btn_addFiles = new QPushButton("Add Files");
    btn_addFiles->setStatusTip("Add files to convert");
    btn_addFromFolder = new QPushButton("Add From Folder");
    btn_about = new QPushButton("About");
    btn_addFromFolder->setStatusTip("Add all files from folder");

    myButtonsLayout->addWidget(btn_addFiles);
    myButtonsLayout->addWidget(btn_addFromFolder);
    myButtonsLayout->addWidget(btn_about);
    myButtonsLayout->addStretch();
    myButtonsLayout->addWidget(btn_convert);
    myButtonsLayout->addWidget(btn_close);

    myRootLayout->addLayout(myButtonsLayout);
    mwidget->setLayout(myRootLayout);

    // Connect signals

    connect(btn_addFiles, SIGNAL(clicked()), SLOT(cb_addFiles()));
    connect(btn_addFromFolder, SIGNAL(clicked()), SLOT(cb_addFromFolder()));
    connect(btn_about, &QPushButton::clicked, [&](){
        QMessageBox::information(this, "About RATit", "A simple, multithreaded converter to RAT file format\n"
                                                "Alex Rusev<hou.alexx@gmail.com>");
    });
    connect(btn_close, SIGNAL(clicked()), SLOT(close()));
    connect(btn_convert, SIGNAL(clicked()), SLOT(cb_startConvert()));

    mySupportedFormatsString = supportedIMGFormats();
    buttonsStyle = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px;}" \
    "QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #7CACCC, stop: 1 #f6f7fa);}";

    btn_convert->setStyleSheet(buttonsStyle);

    myDispatcher = nullptr;
    myDispatcherThread = nullptr;

}

MainWindow::~MainWindow()
{

}

void MainWindow::newTaskFromDir(const QString folder)
{
    auto files = getImagesFromFolder(folder);
    if (!files.empty())
        newTaskFromFileList(files);
}

void MainWindow::newTaskFromFileList(const QStringList files)
{

    QString title = QString("Task #%1").arg(myTaskPanel->getAllTasks()->length() + 1);
    TaskWidget *tw = new TaskWidget(title);
    tw->addFiles(files);
    myTaskPanel->addTaskWidget(tw);
}

QStringList MainWindow::getImagesFromFolder(const QString &folderPath)
{

    QDir curDir(folderPath);
    QStringList filters = mySupportedFormatsString.split(" ");
    QStringList fileList = curDir.entryList(filters);
    if (!fileList.isEmpty())
    {
        std::transform(fileList.begin(), fileList.end(), fileList.begin(),
                       [curDir](QString &f){return curDir.filePath(f);});
    }
    return fileList;
}

void MainWindow::cb_addFiles()
{
    auto selected_files = QFileDialog::getOpenFileNames(this,
                                                        "Select Files",
                                                        "",
                                                        mySupportedFormatsString);
    if (selected_files.isEmpty())
        return;
    newTaskFromFileList(selected_files);
}

void MainWindow::cb_addFromFolder()
{

    auto selected_dir = QFileDialog::getExistingDirectory(this,
                                                        "Select Files",
                                                        "");
    if (selected_dir.isEmpty())
        return;
    newTaskFromDir(selected_dir);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Escape)
    {
        QMainWindow::keyPressEvent(event);
        return;
    }
    close();

}


void MainWindow::cb_startConvert()
{
    if(myDispatcherThread && myDispatcherThread->isRunning())
    {
        qDebug() << "Terminating dispatcher....";
        myDispatcher->terminate();
        return;
    }

    auto tasks_widgets = myTaskPanel->getAllTasks();
    if(tasks_widgets->empty())
        return;

    myDispatcher = new TaskDispatcher(tasks_widgets);
    myDispatcherThread = new QThread;
    myDispatcher->moveToThread(myDispatcherThread);
    connect(myDispatcherThread, SIGNAL(started()), this, SLOT(convertionStarted()));
    connect(myDispatcherThread, SIGNAL(started()), myDispatcher, SLOT(run()));
    connect(myDispatcherThread, SIGNAL(finished()), this, SLOT(convertionFinished()));
    connect(myDispatcherThread, SIGNAL(finished()), myDispatcher, SLOT(deleteLater()));

    connect(myDispatcher, SIGNAL(terminated()), myDispatcherThread, SLOT(terminate()), Qt::DirectConnection);
    connect(myDispatcher, SIGNAL(finished()), myDispatcherThread, SLOT(quit()));

    connect(myDispatcherThread, SIGNAL(finished()), myDispatcher, SLOT(deleteLater()));

    myDispatcherThread->start();
}

void MainWindow::convertionStarted()
{
    QString btn_style_process = "QPushButton { color: black; background-color: #ED8C8C; border: 2px solid #8f8f91; border-radius: 4px;}" \
                                "QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}";
    btn_convert->setStyleSheet(btn_style_process);
    btn_convert->setText("STOP");

}

void MainWindow::convertionFinished()
{
    btn_convert->setStyleSheet(buttonsStyle);
    btn_convert->setText("Convert");
}

void MainWindow::convertionTerminated()
{
    qDebug() << "TERMINATED";
    convertionFinished();
}



