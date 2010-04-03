#include "FileTreeWidgetItem.hpp"
#include <QDir>
#include <QFileInfo>
#include <cassert>

namespace {
    QFileInfo translateName(QFileInfo source, QDir master, QDir copy)
    {
	QString srcPath = source.absoluteFilePath();
	QString dstPath = srcPath;
	dstPath.replace(master.absolutePath(),
			  copy.absolutePath());
	return QFileInfo(dstPath);
    }

    void setCheckStatusBySyncStatus(FileTreeWidgetItem* ftw, QFileInfo syncFile)
    {
	if (syncFile.exists()) {
	    if (syncFile.isSymLink()) {
		ftw->setCheckState(0, Qt::Checked);
	    } else {
		ftw->setCheckState(0, Qt::PartiallyChecked);
	    }
	} else {
	    ftw->setCheckState(0, Qt::Unchecked);
	}
    }

}

FileTreeWidgetItem* FileTreeWidgetItem::makeTreeItem(QFileInfo info, QDir master, QDir copy, QTreeWidgetItem* parent) {
    FileTreeWidgetItem* ftw = new FileTreeWidgetItem(info);

    // We can determine our checked/checkable state by examining our parent
    //
    //   PARENT STATUS                |  NEW ITEM STATUS
    // 1 No parent                    | Checkable (status depends on target area)
    // 2 Uncheckable                  | Uncheckable
    // 3 Checkable, Unchecked         | Checkable, unchecked
    // 4 Partially checked            | Checkable (status depends on target area)
    // 5 Checked                      | Uncheckable
    
    if (!parent) {
	// case 1
	setCheckStatusBySyncStatus(ftw, translateName(info, master, copy));
    } else {
	if (parent->flags() & Qt::ItemIsUserCheckable) {
	    Qt::CheckState ps = parent->checkState(0);
	    if (ps == Qt::Checked) {
		// case 5
		ftw->setFlags(ftw->flags() & !Qt::ItemIsUserCheckable);
	    } else if (ps == Qt::PartiallyChecked) {
		// case 4
		setCheckStatusBySyncStatus(ftw, translateName(info, master, copy));
	    } else {
		// case 3
		assert(ps == Qt::Unchecked);
		ftw->setCheckState(0, Qt::Unchecked);
	    }
	} else {
	    // case 2
	    ftw->setFlags(ftw->flags() & !Qt::ItemIsUserCheckable);
	}
	parent->addChild(ftw);
    }
    return ftw;
}


FileTreeWidgetItem::FileTreeWidgetItem(const QFileInfo& fi) :
    m_childrenAdded(!(fi.isDir())),  // not dir => no kids to add
    m_fileInfo(fi)
{
    setText(0, m_fileInfo.fileName());
}


void FileTreeWidgetItem::loadKids(QDir master, QDir copy)
{
    if (!m_childrenAdded) {

	
	QDir dir(m_fileInfo.absoluteFilePath());
	QFileInfoList fil = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
	
	for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	    makeTreeItem(*i, master, copy, this);
	}
	m_childrenAdded = true;
    }
}
  
