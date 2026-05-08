#include <QApplication>
#include <QPixmapCache>
#include "database.h"
#include "layout.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/logo/Certa.dev_clean.png"));

    QPixmapCache::setCacheLimit(65536);

    if (!Database::instance().connect("Certadev.db")) {
        return EXIT_FAILURE;
    }

    Layout layout;
    layout.show();

    return app.exec();
}
