#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QRunnable>
#include <PXL/PXL_Raster.h>
#include "memory"
#include "convertoptions.h"

class Worker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Worker(int filenumber,
                    const QString &filepath,
                    const QString &rat_dir,
                    std::shared_ptr<ConvertOptions> options);
    virtual ~Worker();

    void run();
private:
    QString myFilePath;
    QString myRatDir;
    std::shared_ptr<ConvertOptions> myOptions;
    UT_Array<PXL_Raster *> myRasters;
    int myFileNumber;
signals:
    void done();

};

#endif // WORKER_H
