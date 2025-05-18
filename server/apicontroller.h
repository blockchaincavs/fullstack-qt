#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include <QObject>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QHttpServerResponder>
#include <QMimeDatabase>
#include <QFile>

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
    void apiPath(const QHttpServerRequest &request, QHttpServerResponder &responder);

signals:

private:

    QString m_dirPath;
    QString getMimeType(const QString &resource);


};

#endif // APICONTROLLER_H
