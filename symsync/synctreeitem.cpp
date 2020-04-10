#include "synctreeitem.h"
#include "utils.h"
#include <functional>

SyncTreeItem::SyncTreeItem(SyncTreeItem* parent, QFileInfo sourceFile, QFileInfo targetFile, FileStatus fileStatus) :
    sourceFile(sourceFile), targetFile(targetFile) {
    setText(0, sourceFile.fileName());

    switch (fileStatus) {
    case NONEXISTANT:
        if (parent != nullptr && (parent->linkStatus == DIRECTLY_LINKED || parent->linkStatus == INDIRECTLY_LINKED)) {
            adjustLinkStatus(INDIRECTLY_LINKED);
        } else {
            adjustLinkStatus(UNLINKED);
        }
        break;
    case DIRECTORY:
        adjustLinkStatus(PLACEHOLDER);
        break;
    case SYMLINK:
        adjustLinkStatus(DIRECTLY_LINKED);
        break;
    case OTHER:
        // Is there something better we can do here?
        assert(false);
        break;
    };
}

/**
 * Convenience method for traversing to all parents.
 *
 * TODO: allow for short-circuits to avoid unnecessarily traversing too high after all work is done.
 */
void traverseUpward(SyncTreeItem* start, std::function<void(SyncTreeItem*)> task) {
    auto parent = start->getTypedParent();
    while (parent != nullptr) {
        task(parent);
        parent = parent->getTypedParent();
    }
}

/**
 * Convenience method for traversing to all children.
 */
void traverseDownward(SyncTreeItem* start, std::function<void(SyncTreeItem*)> task) {

    // Do depth-first recursion
    for (auto i = start->childCount() - 1; i >= 0; --i) {
        auto* kid = static_cast<SyncTreeItem*>(start->child(i));
        traverseDownward(kid, task);
        task(kid);
    }
}

void SyncTreeItem::uncheckIfPossible(SyncTreeItem* item) {
    // If all of our children are unlinked, then we can be, too.
    for (auto i = item->childCount() - 1; i >= 0; --i) {
        auto kid = static_cast<SyncTreeItem*>(item->child(i));
        if (kid->linkStatus != UNLINKED) {
            // At least one child is not unlinked. Nothing to do
            return;
        }
    }

    // All children are unlinked, so we can be as well
    item->adjustLinkStatus(UNLINKED);
}

void SyncTreeItem::adjustLinkStatus(LinkStatus newStatus) {
    linkStatus = newStatus;
    switch (linkStatus) {
    case UNLINKED:
        setCheckState(0, Qt::Unchecked);
        setCheckable(true);
        break;
    case DIRECTLY_LINKED:
        setCheckState(0, Qt::Checked);
        setCheckable(true);
        break;
    case INDIRECTLY_LINKED:
    case PLACEHOLDER:
        setCheckState(0, Qt::PartiallyChecked);
        setCheckable(false);
        break;
    }
}

void SyncTreeItem::onUserCheckChange()
{
    if (checkState(0) == Qt::Checked) {
        linkStatus = DIRECTLY_LINKED;

        // Because this item is checked, all of its parents must be placeholders...
        traverseUpward(this, [](SyncTreeItem* p) {p->adjustLinkStatus(PLACEHOLDER);});
        // ... and the children are indirectly linked
        traverseDownward(this, [](SyncTreeItem* p) {p->adjustLinkStatus(INDIRECTLY_LINKED);});

    } else {
        // It should be impossible for a user to change state to partially checked
        assert(checkState(0) == Qt::Unchecked);

        linkStatus = UNLINKED;

        // Now that this item is unlinked, our children need to be unlinked as well
        traverseDownward(this, [](SyncTreeItem* p) {p->adjustLinkStatus(UNLINKED);});

        // We may have parents that are currently placeholders only due to this one item. If so, uncheck them.
        traverseUpward(this, uncheckIfPossible);
    }
}

SyncTreeItem* SyncTreeItem::getTypedParent() const
{
    return static_cast<SyncTreeItem*>(parent());
}

void SyncTreeItem::setCheckable(bool checkable) {
    if (checkable) {
        setFlags(flags() | Qt::ItemIsUserCheckable);
    } else {
        setFlags(flags() & ~Qt::ItemIsUserCheckable);
    }
}
