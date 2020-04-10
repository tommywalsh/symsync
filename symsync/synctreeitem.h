#ifndef SYNCTREEITEM_H
#define SYNCTREEITEM_H

#include "utils.h"
#include <QTreeWidgetItem>
#include <QFileInfo>

/**
 * UI Widget class that represents a file/directory in the source directory tree, and its association (or lack thereof)
 * with a link/directory in the target tree.
 *
 * Each SyncTreeItem has the following invariants (here "parents" and "children" include all ancestors and descendants.
 *
 *   - All parents and children are also SyncTreeItems
 *   - If a SyncTreeItem is checked, then all of its parents and children are partially checked
 *   - If a SyncTreeItem is unchecked, then none of its parents are checked (note: this is different than saying they
 *         all must be unchecked)
 *   - If a SyncTreeItem is unchecked, then all of its children are unchecked
 *   - If a SyncTreeItem is partially checked, then either:
 *          - At least one of its children is checked, or
 *          - Exactly one of its parents is checked  (but not both)
 */
class SyncTreeItem : public QTreeWidgetItem
{
public:
    enum LinkStatus {
        UNLINKED,           // This file/dir should not appear in the target area
        DIRECTLY_LINKED,    // This file/dir should be a symlink (and all children should be indirectly linked)
        INDIRECTLY_LINKED,  // One of this file/dir's parents is a symlink
        PLACEHOLDER         // This directory should be present in the target area because one of its children is linked
    };

    SyncTreeItem(SyncTreeItem *parent, QFileInfo sourceFile, QFileInfo targetFile, FileStatus initialFileStatus);

    // Call this when the user has changed the checkbox value
    void onUserCheckChange();

    // Convenience method for getting parent with correct type
    SyncTreeItem* getTypedParent() const;

    LinkStatus getLinkStatus() const {return linkStatus;}

    QFileInfo getTargetFile() const {return targetFile;}
    QFileInfo getSourceFile() const {return sourceFile;}
private:
    static void uncheckIfPossible(SyncTreeItem* item);
    void setCheckable(bool checkability);
    void adjustLinkStatus(LinkStatus newStatus);

    LinkStatus linkStatus;
    QFileInfo sourceFile;
    QFileInfo targetFile;
};

#endif // SYNCTREEITEM_H
