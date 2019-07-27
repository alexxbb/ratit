#include "taskpanel.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QDir>
#include <QFileInfo>


TaskPanel::TaskPanel(QWidget *parent):
    QWidget(parent)
{
    myParent = qobject_cast<MainWindow*>(parent);
    setLayout(new QVBoxLayout);
    layout()->setAlignment(Qt::AlignTop);
    layout()->setContentsMargins(2,2,2,2);
    setAcceptDrops(true);
}

void TaskPanel::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

void TaskPanel::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void TaskPanel::dropEvent(QDropEvent *event)
{
    auto urls = event->mimeData()->urls();
    QStringList files;
    if (urls.length() == 1)
    {
        QString path = urls[0].toLocalFile();
        QFileInfo fi(path);
        if (fi.isDir())
        {
            files = myParent->getImagesFromFolder(path);
        }
        else{
            files.append(path);
        }
    }
    else{
        std::transform(urls.begin(), urls.end(), std::back_inserter(files), [](QUrl &url){return url.toLocalFile();});
    }
    QString title = QString("Task #%1").arg(myTasks.length() + 1);
    TaskWidget *w = new TaskWidget(title);
    w->addFiles(files);
    addTaskWidget(w);
    event->accept();
}

QList<TaskWidget *> *TaskPanel::getAllTasks()
{
    return &myTasks;
}

void TaskPanel::addTaskWidget(TaskWidget *widget)
{
    layout()->addWidget(widget);
    myTasks.append(widget);

}

void TaskPanel::removeTaskWidget(TaskWidget *widget)
{
    widget->setParent(0);
    myTasks.removeOne(widget);
}

void TaskPanel::removeAllTaskWidgets()
{
    for(QObject *obj: children()){
        TaskWidget *w = qobject_cast<TaskWidget *>(obj);
        if(w) w->setParent(0);
    }
    myTasks.clear();

}
