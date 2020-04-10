#include "utils.h"

#include <QFile>

FileStatus getStatus(QFileInfo file) {

    // Check for symlink status first, as some other checks will follow links (e.g. link to a directory is identified as a directory)
    if (file.isSymLink()) {
        return SYMLINK;
    } else if (!file.exists()) {
        return NONEXISTANT;
    } else if (file.isDir()) {
        return DIRECTORY;
    }
    return OTHER;
}

void make_symlink(QFileInfo sourceFile, QFileInfo targetLinkFile) {

    // The directory in which this new link is supposed to live might not exist yet.
    QDir dirWithLink = targetLinkFile.dir();
    dirWithLink.mkpath(dirWithLink.absolutePath());

    // Actually make the link
    QFile::link(sourceFile.filePath(), targetLinkFile.filePath());
}

void remove_file(QFileInfo fileToRemove) {
    fileToRemove.refresh();
    if (fileToRemove.isSymLink()) {
        fileToRemove.dir().remove(fileToRemove.fileName());
    } else {
        fileToRemove.dir().rmdir(fileToRemove.fileName());
    }
}
