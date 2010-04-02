#include "SyncTreeWidget.hpp"
#include "FileTreeWidgetItem.hpp"
#include <QDir>


SyncTreeWidget::SyncTreeWidget(QDir master)
{
    QFileInfoList fil = master.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    
    for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	FileTreeWidgetItem* ftw = new FileTreeWidgetItem(*i);
	ftw->loadKids();
	addTopLevelItem(ftw);
    }

}
