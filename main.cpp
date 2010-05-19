#include <QApplication>
#include <QDir>
#include <iostream>
#include "SyncTreeWidget.hpp"




int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    bool success = false;
    if (argc == 3) {
        QDir srcDir = QDir(argv[1]);
        QDir dstDir = QDir(argv[2]);

        if (srcDir.exists()) {
            if(dstDir.exists()) {
                std::cout << "loading...";
                std::cout.flush();
                SyncTreeWidget tw(srcDir, dstDir);
                std::cout << "done.\n";
                tw.show();
                
                app.exec();
                success = true;
            }
        }
    }

    if (!success) {
        std::cout << "Usage: " << argv[0] << " srcdir dstdir\n";
        std::cout << "    Both srcdir and dstdir must exist\n";
        std::cout << "\n";
        std::cout << "Makes/destroys symlinks in dstdir that point to srcdir\n";
    }
        

}
