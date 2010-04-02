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

FileTreeWidgetItem::FileTreeWidgetItem(const QFileInfo& fi) :
    m_childrenAdded(!(fi.isDir())),  // not dir => no kids to add
    m_fileInfo(fi)
{
    //    setCheckState(0, Qt::Checked);
    /*    QFileInfo syncFile = translateName(fi, master, copy);
    if (syncFile.exists()) {
	if (syncFile.isSymLink) {
	    setCheckState(0, Qt::Checked);
	} else {
	    setCheckState(0, Qt::PartiallyChecked);
	}
    } else {
	setCheckState(0, Qt::Unchecked);
    }
    */
    setText(0, m_fileInfo.fileName());
}


void FileTreeWidgetItem::loadKids(QDir master, QDir copy)
{
    if (!m_childrenAdded) {

	
	QDir dir(m_fileInfo.absoluteFilePath());
	QFileInfoList fil = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
	
	for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	    FileTreeWidgetItem* newKid = new FileTreeWidgetItem(*i);
	    QFileInfo syncFile = translateName(*i, master, copy);
	    if (syncFile.exists()) {
		if (syncFile.isSymLink()) {
		    newKid->setCheckState(0, Qt::Checked);
		} else {
		    newKid->setCheckState(0, Qt::PartiallyChecked);
		}
	    } else {
		newKid->setCheckState(0, Qt::Unchecked);
	    }
	    addChild(newKid);
	}
	m_childrenAdded = true;
    }
}
  
