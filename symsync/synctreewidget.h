#ifndef SYNCTREEWIDGET_H
#define SYNCTREEWIDGET_H

#include "synctreeitem.h"
#include <QTreeWidget>
#include <QDir>


/**
 * Tree widget that handles maintaining a "target" tree full of symlinks that point to selected items in a "source"
 * tree full of actual files.
 *
 * Users can select (by clicking checkboxes) which files they want access to from the target tree, and this class
 * will handle all the required filesystem changes.
 */
class SyncTreeWidget : public QTreeWidget
{
public:
    SyncTreeWidget(QDir sourceTree, QDir targetTree);
    ~SyncTreeWidget();
private:
    QDir sourceTree;
    QDir targetTree;

    void addWidgetsForSubTree(SyncTreeItem* parentWidget, QDir subTree);

private slots:
    void onDataChanged(QTreeWidgetItem* item, int column);
private:
    Q_OBJECT
};

#endif // SYNCTREEWIDGET_H
