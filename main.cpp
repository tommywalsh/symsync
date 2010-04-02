#include <QApplication>
#include <QDir>
#include "SyncTreeWidget.hpp"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    SyncTreeWidget tw(QDir("/media/mcotp/mcotp"));
    tw.show();

    app.exec();	   
}
