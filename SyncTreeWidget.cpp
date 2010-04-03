#include "SyncTreeWidget.hpp"
#include "FileTreeWidgetItem.hpp"
#include <QDir>

SyncTreeWidget::SyncTreeWidget(QDir master, QDir copy) :
    m_master(master),
    m_copy(copy)
{
    QFileInfoList fil = master.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    
    for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	FileTreeWidgetItem* ftw = FileTreeWidgetItem::makeTreeItem(*i, master, copy);
	ftw->loadKids(master, copy);
	addTopLevelItem(ftw);
    }

    QObject::connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
		     this, SLOT(onExpanded(QTreeWidgetItem*)));
}

void SyncTreeWidget::onExpanded(QTreeWidgetItem* item)
{
    int numKids = item->childCount();
    for (int i = 0; i < numKids; ++i) {
	QTreeWidgetItem* qwi = item->child(i);
        FileTreeWidgetItem* ftw = dynamic_cast<FileTreeWidgetItem*>(qwi);
	if (ftw != NULL) {
	    ftw->loadKids(m_master, m_copy);
	}
    }
}
	    
	     
