#include "FileTreeWidgetItem.hpp"
#include <QDir>
#include <QFileInfo>

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

FileTreeWidgetItem* FileTreeWidgetItem::makeTreeItem(QFileInfo info, QDir master, QDir copy) {
    FileTreeWidgetItem* ftw = new FileTreeWidgetItem(info);
    QFileInfo syncFile = translateName(info, master, copy);
    if (syncFile.exists()) {
	if (syncFile.isSymLink()) {
	    ftw->setCheckState(0, Qt::Checked);
	} else {
	    ftw->setCheckState(0, Qt::PartiallyChecked);
	}
    } else {
	ftw->setCheckState(0, Qt::Unchecked);
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
	    addChild(makeTreeItem(*i, master, copy));
	}
	m_childrenAdded = true;
    }
}
  
