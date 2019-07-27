#include "converter.h"
#include "worker.h"
#include "QThreadPool"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include "taskwidget.h"

#include <IMG/IMG_Format.h>

Converter::Converter(QObject *parent) :
    QObject(parent), myCounter(0), myFileList(nullptr)
{
}

void Converter::convert(std::shared_ptr<QStringList> filelist, std::shared_ptr<ConvertOptions> options)
{
    myCounter = 0;
    myFileList = filelist;
    QDir root_dir = QFileInfo((*filelist)[0]).dir();
    root_dir.mkdir("rat");
    QString rat_dir = root_dir.filePath("rat");
    QThreadPool *pool = QThreadPool::globalInstance();
    for (int i = 0; i < filelist->length(); i ++){
        QString filepath = (*filelist)[i];
        Worker *worker = new Worker(i, filepath, rat_dir, options);
        worker->setAutoDelete(true);
        connect(worker, SIGNAL(done()), this, SLOT(workerDone()), Qt::DirectConnection);
        pool->start(worker);
    }
    pool->waitForDone();
    emit finished();
}

void Converter::workerDone()
{
        myCounter += 1;
        int percent = int((float(myCounter)/float(myFileList->length())) * 100);
        emit progress(percent);
}


QString supportedIMGFormats()
{

    QStringList res;
    for (int i=0; i < IMG_Format::getNFormats(); i++){
        QString s(IMG_Format::getFormat(i)->getDefaultExtension());
        if(res.indexOf(s) == -1 && !s.isEmpty())
            res.append(s);
    }
    res.append("jpeg");
    return res.join(" *.").prepend("*.");
}
