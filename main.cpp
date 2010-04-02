#include <QApplication>
#include <QDir>
#include "SyncTreeWidget.hpp"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    SyncTreeWidget tw(QDir("/media/mcotp/mcotp"), QDir("/media/mcotp/mobile"));
    tw.show();

    app.exec();	   
}
