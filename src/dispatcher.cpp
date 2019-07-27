#include "dispatcher.h"
#include "converter.h"
#include <QDebug>
#include <QThread>
#include <QRunnable>
#include <QThreadPool>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>

TaskDispatcher::TaskDispatcher(QList<TaskWidget *> *taskwidgets, QObject *parent):
    QObject(parent), myTaskWidgets(taskwidgets)
{
}

void TaskDispatcher::run()
{
    Converter *conv = new Converter();
    for(int i=0; i<myTaskWidgets->length(); i++){
        TaskWidget *task_widget = (*myTaskWidgets)[i];
        auto task_files = task_widget->getFiles();
        auto task_options = task_widget->getOptions();
        connect(conv, SIGNAL(progress(int)), task_widget->progress, SLOT(setValue(int)));
        conv->convert(task_files, task_options);
        disconnect(conv, SIGNAL(progress(int)), task_widget->progress, SLOT(setValue(int)));
        if (isTerminated){
            delete conv;
            emit terminated();
            return;
        }
    }
    delete conv;
    emit finished();
}
