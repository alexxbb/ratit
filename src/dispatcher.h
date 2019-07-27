#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <QList>
#include <QWidget>
#include <QStringList>
#include <QMap>
#include "taskwidget.h"

class TaskDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit TaskDispatcher(QList<TaskWidget *> *taskwidgets, QObject *parent = 0);
signals:
    void finished();
    int progress(int val);
    void terminated();

public slots:
    void run();
    void terminate(){isTerminated = true;}

private:
    QList<TaskWidget *> *myTaskWidgets;
    bool isTerminated;
};

#endif // DISPATCHER_H
