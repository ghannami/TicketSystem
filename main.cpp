#include "MainWindow.h"
#include <QApplication>
#include "Global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(Global::i()->login())
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
