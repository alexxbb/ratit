#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "taskwidget.h"
#include "taskpanel.h"

TaskWidget::TaskWidget(const QString &title, QWidget *parent) :
    QWidget(parent)
{
    setMinimumHeight(170);
    QHBoxLayout *baseLayout = new QHBoxLayout;
    QVBoxLayout *fileListLayout = new QVBoxLayout;
    QVBoxLayout *optionsLayout = new QVBoxLayout;

    fileList = new QListWidget;
    fileListLayout->addWidget(fileList);
    progress = new QProgressBar;
    fileListLayout->addWidget(progress);
    bitDepth = new QComboBox;
    bitDepth->addItem("byte", QVariant(0x01));
    bitDepth->addItem("short", QVariant(0x02));
    bitDepth->addItem("half", QVariant(0x10));
    bitDepth->addItem("float", QVariant(0x08));
    QHBoxLayout *bitDepthLayout = new QHBoxLayout;
    bitDepthLayout->addWidget(new QLabel("Bit Depth"));
    bitDepthLayout->addWidget(bitDepth);

    imgScale = new QComboBox;
    imgScale->addItem("100%", QVariant(1.0));
    imgScale->addItem("50%", QVariant(0.5));
    imgScale->addItem("25%", QVariant(0.25));
    imgScale->addItem("10%", QVariant(0.1));
    QHBoxLayout *imgScaleLayout = new QHBoxLayout;
    imgScaleLayout->addWidget(new QLabel("Scale"));
    imgScaleLayout->addWidget(imgScale);


    compress = new QCheckBox("Compession");
    compress->setChecked(true);
    makeMips = new QCheckBox("Make MIPs");
    makeMips->setChecked(true);
    removeBtn = new QPushButton("Remove");
    optionsLayout->addWidget(compress);
    optionsLayout->addWidget(makeMips);
    optionsLayout->addLayout(bitDepthLayout);
    optionsLayout->addLayout(imgScaleLayout);
    optionsLayout->addWidget(removeBtn);
    optionsLayout->setAlignment(Qt::AlignTop);

    baseLayout->addLayout(fileListLayout);
    baseLayout->addLayout(optionsLayout);
    QGroupBox *grpbox = new QGroupBox(title);
    grpbox->setStyleSheet("QGroupBox { background-color: \
                          qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E0E0E0, stop: 1 #FFFFFF); \
                          border: 2px solid gray; border-radius: 5px; margin-top: 1ex;} \
                          QGroupBox::title { subcontrol-origin: margin; \
                                subcontrol-position: top left; \
                                padding: 0 3px; }");

    QVBoxLayout *vl = new QVBoxLayout;
    grpbox->setLayout(baseLayout);
    vl->addWidget(grpbox);
    setLayout(vl);

    connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeTask()));
}

void TaskWidget::addFiles(QStringList files)
{
    fileList->clear();
    fileList->addItems(files);

}

const std::shared_ptr<QStringList> TaskWidget::getFiles() const
{
    std::shared_ptr<QStringList> res(new QStringList);
    for (int i=0; i<fileList->count(); i++)
    {
        res->append(fileList->item(i)->text());
    }
    return res;
}

const std::shared_ptr<ConvertOptions> TaskWidget::getOptions() const
{
    std::shared_ptr<ConvertOptions> options(new ConvertOptions);
    options->bitDepth = bitDepth->itemData(bitDepth->currentIndex()).toInt();
    options->compress = compress->isChecked();
    options->makeMips = makeMips->isChecked();
    options->imgScale = imgScale->itemData(imgScale->currentIndex()).toFloat();
    return options;
}


void TaskWidget::removeTask()
{
    TaskPanel *task_panel = qobject_cast<TaskPanel *>(parent());
    task_panel->removeTaskWidget(this);
}
