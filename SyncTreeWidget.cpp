#include "SyncTreeWidget.hpp"
#include "FileTreeWidgetItem.hpp"
#include <QDir>

namespace {
    QFileInfo translateName(QFileInfo source, QDir master, QDir copy)
    {
	QString srcPath = source.absoluteFilePath();
	QString dstPath = srcPath;
	dstPath.replace(master.absolutePath(),
			  copy.absolutePath());
	return QFileInfo(dstPath);
    }

}

SyncTreeWidget::SyncTreeWidget(QDir master, QDir copy) :
    m_master(master),
    m_copy(copy)
{
    QFileInfoList fil = master.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    
    for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	FileTreeWidgetItem* ftw = new FileTreeWidgetItem(*i);
	
	QFileInfo syncFile = translateName(*i, master, copy);
	if (syncFile.exists()) {
	    if (syncFile.isSymLink()) {
		ftw->setCheckState(0, Qt::Checked);
	    } else {
		ftw->setCheckState(0, Qt::PartiallyChecked);
	    }
	} else {
	    ftw->setCheckState(0, Qt::Unchecked);
	}
	ftw->loadKids(m_master, m_copy);
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
	    
	     
