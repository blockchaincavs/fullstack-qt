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
        msg = "Resource Not Found " + resource;
        responder.sendResponse(
            QHttpServerResponse(msg.toUtf8(), QHttpServerResponse::StatusCode::NotFound));
        return;
    }

    if (!appFile.open(QIODevice::ReadOnly)) {
        msg = "Something went wrong... " + resource;
        responder.sendResponse(
            QHttpServerResponse(msg.toUtf8(), QHttpServerResponse::StatusCode::InternalServerError));
    }

    QByteArray content = appFile.readAll();
    appFile.close();

    QString mimeType = getMimeType(resource);
    responder.write(content, mimeType.toUtf8());

}

/**
 * @brief ApiController::authPath
 * Request handler used to process "/auth" path.
 *
 * @param request
 * @param responder
 */
void ApiController::authPath(const QHttpServerRequest &request, QHttpServerResponder &responder)
{
    QByteArray body = request.body();
    QString uri = request.url().path();
    QString msg; msg.resize(40);

    // Ensure content is json
    QHttpHeaders headers = request.headers();
    QString contentType = QString::fromUtf8(headers.value(QHttpHeaders::WellKnownHeader::ContentType));

    if (contentType != "application/json") {
        msg = "ContentType not application/json.";
        responder.sendResponse(createResponse(QHttpServerResponse::StatusCode::BadRequest, uri, msg));
        return;
    }

    QJsonParseError jsonParseError;
    QJsonDocument doc(QJsonDocument::fromJson(body, &jsonParseError));

    if (jsonParseError.error != QJsonParseError::ParseError::NoError || !doc.isObject()) {
        msg = "Failed to parse request body.";
        responder.sendResponse(createResponse(QHttpServerResponse::StatusCode::BadRequest, uri, msg));
        return;
    }

    // check credentials
    QJsonObject obj = doc.object();
    QJsonValue username = obj.value("username");
    QJsonValue password = obj.value("password");

    if (username.toString("") != USERNAME || password.toString("") != PASSWORD) {
        msg = "Authentication failed.";
        responder.sendResponse(createResponse(QHttpServerResponse::StatusCode::Unauthorized, uri, msg));
        return;
    }

    responder.sendResponse(createResponse(QHttpServerResponse::StatusCode::Ok, uri));
}

/**
 * @brief ApiController::apiPath
 * @param request
 * @param responder
 */
void ApiController::apiPath(const QHttpServerRequest &request, QHttpServerResponder &responder)
{
    QString uri = request.url().path();
    QHttpServerResponse response = createResponse(QHttpServerResponse::StatusCode::NotFound, uri);
    responder.sendResponse(response);
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

/**
 * @brief ApiController::createResponse
 *
 * @param code
 * @param uri
 * @return
 */
QHttpServerResponse ApiController::createResponse(const QHttpServerResponse::StatusCode &code, const QString &uri, QString msg)
{
    switch (code) {
        case QHttpServerResponse::StatusCode::Ok:
            msg = "Success" + msg;
            break;
        case QHttpServerResponse::StatusCode::NotFound:
            msg = "Resource Not Found";
            break;
        case QHttpServerResponse::StatusCode::Unauthorized:
            msg = "Unauthorized: " + msg;
            break;
        case QHttpServerResponse::StatusCode::BadRequest:
            msg = "Bad Request. " + msg;
            break;
        default:
            msg = "Something went wrong ";
    }

    QJsonObject responseData {
        {"message", msg},
        {"uri", uri},
        {"code", int(code)}
    };

    // Cross origin header to indicate response can be shared with any origin
    QHttpHeaders headers;
    headers.append(QHttpHeaders::WellKnownHeader::AccessControlAllowOrigin, "*");

    QHttpServerResponse response(responseData, code);
    response.setHeaders(headers);

    return response;
}
