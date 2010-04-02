#ifndef JITFILETREEWIDGET_HPP
#define JITFILETREEWIDGET_HPP


#include <QTreeWidgetItem>
#include <QFileInfo>


class JITFileTreeWidget : public QTreeWidgetItem
{
private:
    bool m_childrenAdded;
    QFileInfo m_fileInfo;
public:
    JITFileTreeWidget(const QFileInfo& fi);
    JITFileTreeWidget(const QFileInfo& fi, JITFileTreeWidget* parent);
    void loadKids();    
};


#endif
