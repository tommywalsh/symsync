#include "FileTreeWidgetItem.hpp"
#include <QDir>

FileTreeWidgetItem::FileTreeWidgetItem(const QFileInfo& fi) :
    m_childrenAdded(!(fi.isDir())),  // not dir => no kids to add
    m_fileInfo(fi)
{
    setCheckState(0, Qt::Checked);
    setText(0, m_fileInfo.fileName());
}


void FileTreeWidgetItem::loadKids()
{
    if (!m_childrenAdded) {

	
	QDir dir(m_fileInfo.absoluteFilePath());
	QFileInfoList fil = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
	
	for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	    FileTreeWidgetItem* newKid = new FileTreeWidgetItem(*i);
	    addChild(newKid);
	}
	m_childrenAdded = true;
    }
}
  
