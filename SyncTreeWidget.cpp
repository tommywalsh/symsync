#include "SyncTreeWidget.hpp"
#include "FileTreeWidgetItem.hpp"
#include <QDir>
#include <cassert>

#include <iostream>

namespace {
    void propagateCheckabilityDown(QTreeWidgetItem* root, bool checkable) 
    {
	// depth-first recursive propagation
	for (int i = root->childCount() - 1; i >= 0; --i) {
	    QTreeWidgetItem* kid = root->child(i);
	    propagateCheckabilityDown(kid, checkable);
	    if (checkable) {
		kid->setFlags(kid->flags() | Qt::ItemIsUserCheckable);
		kid->setCheckState(0, Qt::Unchecked);
	    } else {
		kid->setFlags(kid->flags() & ~Qt::ItemIsUserCheckable);
		kid->setCheckState(0, Qt::Unchecked);
	    }
	}
    }

    void propagatePartialCheckUp(QTreeWidgetItem* child) 
    {
	QTreeWidgetItem* ancestor = child->parent();
	while (ancestor && ancestor->checkState(0) != Qt::PartiallyChecked) {
	    ancestor->setCheckState(0, Qt::PartiallyChecked);
	    ancestor->setFlags(ancestor->flags() & ~Qt::ItemIsUserCheckable);
	    ancestor = ancestor->parent();
	}
    }

    bool allKidsUncheckedExcept(QTreeWidgetItem* parent, QTreeWidgetItem* changingChild)
    {
	bool kidsUnchecked = true;
	for (int i = parent->childCount() - 1; i >= 0; --i) {
	    QTreeWidgetItem* kid = parent->child(i);
	    if (kid != changingChild && kid->checkState(0) != Qt::Unchecked) {
		kidsUnchecked = false;
		break;
	    }
	}
	return kidsUnchecked;
    }

    void propagateCheckUpdateUp(QTreeWidgetItem* changingChild)
    {
	// Some partially-checked items might now need to switch to unchecked
	// The passed-in item is changing to unchecked.  If all other kids are 
	// also unchecked, then the parent should go back to being unchecked
	QTreeWidgetItem* ancestor = changingChild->parent();
	while (ancestor && ancestor->checkState(0) == Qt::PartiallyChecked) {
	    if (allKidsUncheckedExcept(ancestor, changingChild)) {		
		ancestor->setCheckState(0, Qt::Unchecked);
		ancestor->setFlags(ancestor->flags() | Qt::ItemIsUserCheckable);
		ancestor = ancestor->parent();
	    } else {
		break;
	    }
	}	
    }

    void unsync_file(QFileInfo masterFile, QDir masterRoot, QDir copyRoot)
    {
	QString masterPath = masterFile.absoluteFilePath();
	QString copyPath = masterPath;
	copyPath.replace(masterRoot.absolutePath(), copyRoot.absolutePath());
	QFileInfo copyFile(copyPath);
	assert(copyFile.exists() && copyFile.isSymLink());
	QFile::remove(copyFile.absoluteFilePath());

	// We might still need to remove parent directories, if they are empty
	// This function does exactly what we want
	QDir dummy;
	dummy.rmpath(copyFile.dir().absolutePath());

    }

    void sync_file(QFileInfo masterFile, QDir masterRoot, QDir copyRoot)
    {
	QString masterPath = masterFile.absoluteFilePath();
	QString copyPath = masterPath;
	copyPath.replace(masterRoot.absolutePath(), copyRoot.absolutePath());
	QFileInfo copyFile(copyPath);
	assert(!copyFile.exists());
	
	QDir dummy;
	dummy.mkpath(copyFile.dir().absolutePath());
	
	QFile source(masterPath);
	source.link(copyPath);
    }

}

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

    QObject::connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
		     this, SLOT(onPressed(QTreeWidgetItem*, int)));
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

	     
void SyncTreeWidget::onPressed(QTreeWidgetItem * item, int column)
{
    if (column == 0 && (item->flags() & Qt::ItemIsUserCheckable)) {
	Qt::CheckState ps = item->checkState(0);	
	FileTreeWidgetItem* ftw = dynamic_cast<FileTreeWidgetItem*>(item);
	assert(ftw);
	if (ps == Qt::Checked) {
	    propagateCheckabilityDown(item, true);
	    propagateCheckUpdateUp(item);
	    
	    unsync_file(ftw->fileInfo(), m_master, m_copy);
	    
	} else {
	    assert (ps == Qt::Unchecked);
	    propagateCheckabilityDown(item, false);
	    propagatePartialCheckUp(item);

	    // We used to be unsynced, now we need to sync
	    sync_file(ftw->fileInfo(), m_master, m_copy);
	}
    }
}
