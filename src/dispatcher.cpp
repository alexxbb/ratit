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
    QObject(parent), myTaskWidgets(taskwidgets), isTerminated(false)
{
}

void TaskDispatcher::run()
{
    Converter *conv = new Converter();
    for(auto task_widget : *myTaskWidgets){
        auto task_files = task_widget->getFiles();
        auto task_options = task_widget->getOptions();
        connect(conv, SIGNAL(progress(int)), task_widget->progress, SLOT(setValue(int)));
        conv->convert(task_files, task_options);
        if (isTerminated){
            delete conv;
            emit terminated();
            return;
        }
    }
    delete conv;
    emit finished();
}
