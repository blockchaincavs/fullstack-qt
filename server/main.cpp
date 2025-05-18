#include <QCoreApplication>
#include "apirouter.h"

const QHostAddress SERVER_IP("127.0.0.1");
const quint16 SERVER_PORT = 8080;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.

    ApiRouter router;

    if (!router.start(SERVER_IP, 8080)) {
        return -1;
    }

    qInfo() << QString("Successfully started HTTP Server on %1:%2")
                   .arg(SERVER_IP.toString()).arg(SERVER_PORT);

    return a.exec();
}
