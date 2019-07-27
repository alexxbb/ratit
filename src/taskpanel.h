#ifndef TASKPANEL_H
#define TASKPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include "taskwidget.h"

class MainWindow;

class TaskPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TaskPanel(QWidget *parent = 0);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

private:
    QList<TaskWidget *> myTasks;
    MainWindow *myParent;

public slots:
    QList<TaskWidget *> *getAllTasks();
    void addTaskWidget(TaskWidget *widget);
    void removeTaskWidget(TaskWidget *widget);
    void removeAllTaskWidgets();

signals:
};

#endif // TASKPANEL_H
