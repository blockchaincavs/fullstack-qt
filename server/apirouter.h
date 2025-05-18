#ifndef APIROUTER_H
#define APIROUTER_H

#include <QObject>
#include <QHttpServer>
#include <QTcpServer>

#include "apicontroller.h"

class ApiRouter : public QObject
{
    Q_OBJECT
public:
    explicit ApiRouter(QObject *parent = nullptr);
    ~ApiRouter() = default;

    ApiRouter(ApiRouter &other) = delete;
    ApiRouter &operator=(const ApiRouter &other) = delete;

    bool start(const QHostAddress &ip, const quint16 &port);

signals:

private:

    const QString DIR_PATH = "/Users/7skyler/Documents/programming/fullstack-qt/login/build/WebAssembly/Release/";
    QHttpServer m_httpServer;
    ApiController m_apiController;

    void createApiRoutes();

};

#endif // APIROUTER_H
