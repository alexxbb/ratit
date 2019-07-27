#include "worker.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <IMG/IMG_File.h>
#include <IMG/IMG_FileFilter.h>
#include <IMG/IMG_FileTypes.h>
#include <IMG/IMG_Format.h>
#include <IMG/IMG_FileParms.h>

Worker::Worker(int filenumber,
               const QString &filepath,
               const QString &rat_dir,
               std::shared_ptr<ConvertOptions> options):
    myFileNumber(filenumber), myFilePath(filepath), myRatDir(rat_dir), myOptions(options)
{
}

Worker::~Worker()
{
    for(int i=0; i < myRasters.entries(); i++)
    {
        delete myRasters(i);
    }
}

void Worker::run()
{
    QDir rat_dir(myRatDir);
    QFileInfo fi(myFilePath);
    QString orig_basename = fi.completeBaseName();
    QString rat_filename = orig_basename + ".rat";
    QString rat_filepath = rat_dir.absoluteFilePath(rat_filename);

    IMG_FileParms parms;
    parms.setDataType((IMG_DataType)myOptions->bitDepth);
    parms.scaleImageBy(myOptions->imgScale, myOptions->imgScale);
    parms.setOption("compression", myOptions->compress ? "gzip" : "none");
    parms.setOption("makemips", myOptions->makeMips ? "1" : "0");
    IMG_File *img = IMG_File::open(myFilePath.toLatin1().data(), &parms);
    img->readImages(myRasters);
    IMG_File::saveRasterAsFile(rat_filepath.toLatin1().data(), myRasters(0), &parms);
    img->close();
    emit done();
}
