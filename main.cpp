#include <QApplication>
#include <QTreeWidget>
#include <QDirIterator>
#include <QFileInfo>
#include "JITFileTreeWidget.hpp"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QTreeWidget tw;

    QDir dir("/media/mcotp/mcotp/Replacements");

    QFileInfoList fil = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    
    for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	JITFileTreeWidget* ftw = new JITFileTreeWidget(*i);
	ftw->loadKids();
	tw.addTopLevelItem(ftw);
    }

    tw.show();
    app.exec();	   
}
