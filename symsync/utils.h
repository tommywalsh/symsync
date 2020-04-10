#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

enum FileStatus {
    NONEXISTANT,
    DIRECTORY,
    SYMLINK,
    OTHER
};

FileStatus getStatus(QFileInfo file);

void make_symlink(QFileInfo sourceFile, QFileInfo targetLinkFile);
void remove_file(QFileInfo fileToRemove);

#endif // UTILS_H
