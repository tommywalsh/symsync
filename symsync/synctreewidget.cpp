#include "synctreewidget.h"
#include "synctreeitem.h"
#include "utils.h"

/**
 * For the given file in the given source tree, return its analogue inside the given target tree
 */
QFileInfo getTargetFileInfo(QFileInfo sourceFile, QDir sourceTree, QDir targetTree) {
    auto relativePath = sourceTree.relativeFilePath(sourceFile.filePath());
    auto targetPath = targetTree.filePath(relativePath);
    return QFileInfo(targetPath);
}

/**
 * Analyze the contents of the given subtree (within the source tree), and construct all relevant
 * widgets under the given parent widget.
 */
void SyncTreeWidget::addWidgetsForSubTree(SyncTreeItem* parentWidget, QDir subTree) {

    // Loop over all the subdirectories and fils in the given subtree.
    auto directoryList = subTree.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    for (auto sourceItem : directoryList) {
        QFileInfo targetItem = getTargetFileInfo(sourceItem, sourceTree, targetTree);
        FileStatus status = getStatus(targetItem);

        SyncTreeItem* treeItem = new SyncTreeItem(parentWidget, sourceItem, targetItem, status);
        parentWidget == nullptr ? addTopLevelItem(treeItem) : parentWidget->addChild(treeItem);

        // If we're dealing with a subdirectory, and we haven't symlinked to it already, then load in child widgets by recursing.
        if (getStatus(sourceItem) == DIRECTORY && getStatus(targetItem) != SYMLINK) {
            addWidgetsForSubTree(treeItem, QDir(sourceItem.filePath()));
        }
    }
}


SyncTreeWidget::SyncTreeWidget(QDir sourceTree, QDir targetTree):
    sourceTree(sourceTree), targetTree(targetTree) {
    setHeaderHidden(true);
    addWidgetsForSubTree(nullptr, sourceTree);

    QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
                     this, SLOT(onDataChanged(QTreeWidgetItem*, int)));
}

/*
 * This lockout allows us to detect when Qt signals are being generated by user interaction as opposed to side-effects from our own code.
 * In Qt, there is only one UI thread, so we don't need to worry about thread-local storage.
 */
static bool currentlyProcessing = false;

/*
 * RAII class for maintaining the above lockout boolean. This is not threadsafe or reentrant-safe, but it doesn't need to be.
 * Only construct one of these after you have verified that the above boolean is false.
 */
struct UserInputLockout {
    UserInputLockout() {
        currentlyProcessing = true;
    }
    ~UserInputLockout() {
        currentlyProcessing = false;
    }
};

void SyncTreeWidget::onDataChanged(QTreeWidgetItem* genericItem, int) {
    if (currentlyProcessing) {
        return;
    }
    UserInputLockout lockout;

    auto treeItem = static_cast<SyncTreeItem*>(genericItem);

    // Fix up UI first
    treeItem->onUserCheckChange();

    // Now fix filesystem as necessary, based on what the UI is showing.
    auto linkStatus = treeItem->getLinkStatus();
    if (linkStatus == SyncTreeItem::DIRECTLY_LINKED) {
        // The user has just checked this item, which means we need to create a new symlink on the filesystem
        make_symlink(treeItem->getSourceFile(), treeItem->getTargetFile());
    } else {
        // It should not be possible for the user to transition to anything except unlinked
        assert(linkStatus == SyncTreeItem::UNLINKED);

        // The user has just unchecked this item, which means we need to delete this link.
        remove_file(treeItem->getTargetFile());

        // We may also need to delete any unneeded parents
        auto parent = treeItem->getTypedParent();
        while (parent != nullptr && parent->getLinkStatus() == SyncTreeItem::UNLINKED) {
            remove_file(parent->getTargetFile());
            parent = parent->getTypedParent();
        }
    }
}

SyncTreeWidget::~SyncTreeWidget() {
}
