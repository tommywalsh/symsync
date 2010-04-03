#ifndef SYNCTREEWIDGET_HPP
#define SYNCTREEWIDGET_HPP


#include <QTreeWidget>
#include <QDir>

class SyncTreeWidget : public QTreeWidget
{
private:
    QDir m_master, m_copy;
private slots:
    void onExpanded(QTreeWidgetItem* item);
    void onPressed(QTreeWidgetItem * item, int column);
public:
    SyncTreeWidget(QDir master, QDir copy);
Q_OBJECT
};

#endif
