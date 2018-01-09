#include "MainWindow.h"
#include <QApplication>
#include "Global.h"
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(Global::i()->login())
    {
        MainWindow w;
//        QStringList args = a.arguments();
//        QString arg;
//        if(args.count() > 1)
//            arg = args[1];
//        if(arg == "-d")
//            w.hide();
//        else
//            w.show();
        w.show();
        return a.exec();
    }
}
