#ifndef FILETREEWIDGETITEM_HPP
#define FILETREEWIDGETITEM_HPP


#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QDir>

class FileTreeWidgetItem : public QTreeWidgetItem
{

private:
    bool m_childrenAdded;
    QFileInfo m_fileInfo;
public:
    FileTreeWidgetItem(const QFileInfo& fi);
    void loadKids(QDir master, QDir copy);
    static FileTreeWidgetItem* makeTreeItem(QFileInfo info, QDir master, QDir copy, QTreeWidgetItem* parent = NULL);
    QFileInfo fileInfo() const {return m_fileInfo;}

};

#endif
