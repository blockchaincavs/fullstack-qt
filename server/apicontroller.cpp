#include "apicontroller.h"
#include <QtHttpServer/qhttpserverresponse.h>

ApiController::ApiController(QString dirPath, QObject *parent)
    : QObject{parent}, m_dirPath(dirPath)
{}

/**
 * @brief ApiController::webPath
 * Send html, js, wasm, and other file resources upon reqeust
 *
 * @param request
 * @param responder
 */
void ApiController::webPath(const QHttpServerRequest &request, QHttpServerResponder &responder)
{
    QString resource = request.url().path();

    if (resource == "/") {
        resource = "login.html";
    }

    QFile appFile(m_dirPath + resource);
    QString msg;

    if (!appFile.exists()) {
        msg = "Resource not found " + resource;
        responder.sendResponse(
            QHttpServerResponse(msg.toUtf8(), QHttpServerResponse::StatusCode::NotFound));
        return;
    }

    if (!appFile.open(QIODevice::ReadOnly)) {
        msg = "Something went wrong " + resource;
        responder.sendResponse(
            QHttpServerResponse(msg.toUtf8(), QHttpServerResponse::StatusCode::InternalServerError));
    }

    QByteArray content = appFile.readAll();
    appFile.close();

    QString mimeType = getMimeType(resource);
    responder.write(content, mimeType.toUtf8());

}

void ApiController::apiPath(const QHttpServerRequest &request, QHttpServerResponder &responder)
{

}

/**
 * @brief ApiController::getMimeType
 * Get mime type for requested resource. This is essentially the content type
 *
 * @param resource
 * @return
 */
QString ApiController::getMimeType(const QString &resource)
{
    if (resource.split(".").at(1) == "wasm") {
        return "application/wasm";
    }

    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForName(resource);
    return mimeType.name();
}
