#include "apirouter.h"

ApiRouter::ApiRouter(QObject *parent)
    : QObject{parent}, m_apiController(DIR_PATH)
{
    createApiRoutes();
}

/**
 * @brief ApiRouter::start
 * start tcp server parent it to http server
 *
 * @returns bool true if server has successfully started
 */
bool ApiRouter::start(const QHostAddress &ip, const quint16 &port)
{

    // Handle default route for web interface
    QTcpServer *tcpServer = new QTcpServer();
    if (!tcpServer->listen(ip, port) || !m_httpServer.bind(tcpServer)) {
        delete tcpServer;
        return false;
    }

    return true;

}

/**
 * @brief ApiRouter::createApiRoutes
 *
 */
void ApiRouter::createApiRoutes()
{

    // handle default route for web interface
    m_httpServer.route("/", QHttpServerRequest::Method::Get,
        [=](const QHttpServerRequest &request, QHttpServerResponder &responder){

        m_apiController.webPath(request, responder);
    });

    // This is a "catch all" route. It is intended to serve the static webpage
    m_httpServer.route("/*", QHttpServerRequest::Method::Get,
        [=](const QString &resource, const QHttpServerRequest &request, QHttpServerResponder &responder) {

        m_apiController.webPath(request, responder);
    });

    // Handle api endpoints with one catch All
    m_httpServer.setMissingHandler(this, [](const QHttpServerRequest &request, QHttpServerResponder &responder) {
        QString msg("Resource Not found");
        int code = static_cast<int>(QHttpServerResponse::StatusCode::NotFound);

        QJsonObject responseData{
            {"message", msg},
            {"uri", request.url().path()},
            {"code", code}
        };

        // Cross origin header to indicate response can be shared with any origin
        QHttpHeaders headers;
        headers.append(QHttpHeaders::WellKnownHeader::AccessControlAllowHeaders, "*");

        QHttpServerResponse response(responseData, QHttpServerResponse::StatusCode::NotFound);
        response.setHeaders(headers);
        responder.sendResponse(response);

    });

}
