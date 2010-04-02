#include "JITFileTreeWidget.hpp"
#include <QDirIterator>


JITFileTreeWidget::JITFileTreeWidget(const QFileInfo& fi) :
    m_childrenAdded(!(fi.isDir())),  // not dir => no kids to add
    m_fileInfo(fi)
{
    setCheckState(0, Qt::Checked);
    setText(0, m_fileInfo.fileName());
}


JITFileTreeWidget::JITFileTreeWidget(const QFileInfo& fi, JITFileTreeWidget* parent) :
    QTreeWidgetItem(parent, 1049),
    m_childrenAdded(false),
    m_fileInfo(fi)

{
    setCheckState(0, Qt::Unchecked);
    setText(0, m_fileInfo.fileName());    
}

void JITFileTreeWidget::loadKids()
{
    if (!m_childrenAdded) {

	
	QDir dir(m_fileInfo.absoluteFilePath());
	QFileInfoList fil = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
	
	for (QFileInfoList::const_iterator i = fil.constBegin(); i != fil.constEnd(); ++i) {
	    JITFileTreeWidget* newKid = new JITFileTreeWidget(*i);
	    addChild(newKid);
	}
	m_childrenAdded = true;
    }
}
  
