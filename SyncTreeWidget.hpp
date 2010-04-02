#ifndef SYNCTREEWIDGET_HPP
#define SYNCTREEWIDGET_HPP


#include <QTreeWidget>
#include <QDir>

class SyncTreeWidget : public QTreeWidget
{
public:
    SyncTreeWidget(QDir master/*, QDir copy*/);
};

#endif
