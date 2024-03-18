#include "mainwindow.h"
#include <QApplication>
#include "mmsystem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //std::thread inputThread(std::bind(&MainWindow::consoleInputThread, &w));

    int guiResult = a.exec();
    //inputThread.join();

    return guiResult;
}
