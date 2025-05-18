#include "login.h"
#include "./ui_login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // Connect signals
    QObject::connect(ui->buttonLogin, &QAbstractButton::clicked, this, &Login::onLoginClicked);

}

Login::~Login()
{
    delete ui;
}

/**
 * @brief Login::onLoginClicked
 * Create Http reqeust to login. receive JWT?
 *
 */
void Login::onLoginClicked()
{
    // Create http reqeust
    QUrl authEndpoint("http://127.0.0.1:8080/auth");
    QNetworkRequest request(authEndpoint);

    // set headers
    request.setRawHeader("Content-Type", "application/json");

    // body
    QJsonObject body{
        {"username", ui->lineEditUsername->text()},
        {"password", ui->lineEditPassword->text()}
    };

    QJsonDocument data(body);

    // make request
    QNetworkReply *reply = m_manager.post(request, data.toJson(QJsonDocument::Compact));

    // connect signal
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        QByteArray data = reply->readAll();

        qInfo() << data;
    });

}
