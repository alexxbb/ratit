#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include "memory"
#include "convertoptions.h"

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0);
    void convert(std::shared_ptr<QStringList> filelist,
                 std::shared_ptr<ConvertOptions> options);

private:
    std::shared_ptr<QStringList> myFileList;
    int myCounter;
    bool myIsTerminated;
signals:
    int progress(int val);
    void finished();

public slots:
    void workerDone();
    void terminate(){myIsTerminated = true;}

};

QString supportedIMGFormats();

#endif // CONVERTER_H
