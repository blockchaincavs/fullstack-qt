#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include <QObject>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QHttpServerResponder>
#include <QMimeDatabase>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class ApiController : public QObject
{
    Q_OBJECT
public:
    explicit ApiController(QString dirPath = "/", QObject *parent = nullptr);
    ~ApiController() = default;

    // delete coppy constructor and asignment operator
    ApiController(const ApiController &other) = delete;
    ApiController &operator-(const ApiController &other) = delete;

    // Http Request handler for web application
    void webPath(const QHttpServerRequest &request, QHttpServerResponder &responder);
    void authPath(const QHttpServerRequest &request, QHttpServerResponder &responder);
    void apiPath(const QHttpServerRequest &request, QHttpServerResponder &responder);

signals:

private:

    //
    const QString USERNAME = "sgonzalez";
    const QString PASSWORD = "pass123!";

    QString m_dirPath;
    QString getMimeType(const QString &resource);

    QHttpServerResponse createResponse(const QHttpServerResponse::StatusCode &code,
                                       const QString &uri, QString msg = "");
};

#endif // APICONTROLLER_H
