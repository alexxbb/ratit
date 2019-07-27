#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QStringList>
#include <QProgressBar>
#include <QGroupBox>
#include <QMap>
#include "memory"
#include "convertoptions.h"

class TaskPanel;

class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(const QString &title, QWidget *parent = 0);

    QListWidget *fileList;
    QPushButton *removeBtn;
    QCheckBox *compress;
    QCheckBox *makeMips;
    QComboBox *bitDepth;
    QComboBox *imgScale;
    QProgressBar *progress;

    void addFiles(QStringList files);
    const std::shared_ptr<QStringList> getFiles() const;
    const std::shared_ptr<ConvertOptions> getOptions() const;

signals:

public slots:
    void removeTask();

};

#endif // TASKWIDGET_H
