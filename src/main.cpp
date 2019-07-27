#include <QApplication>
#include <QDir>
#include <QString>
#include <QDebug>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QIcon>
#include <QStyle>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion("1.0.0");
    MainWindow w;
    w.newTaskFromDir("H:/sequences/gogi_sequence");
    w.show();
    return app.exec();

}
