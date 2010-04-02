#ifndef FILETREEWIDGETITEM_HPP
#define FILETREEWIDGETITEM_HPP


#include <QTreeWidgetItem>
#include <QFileInfo>


class FileTreeWidgetItem : public QTreeWidgetItem
{

private:
    bool m_childrenAdded;
    QFileInfo m_fileInfo;
public:
    FileTreeWidgetItem(const QFileInfo& fi);
    void loadKids();    
};

#endif
