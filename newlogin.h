#ifndef NEWLOGIN_H
#define NEWLOGIN_H

#include <QDialog>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QLineEdit>
#include <networkmanager.h>
#include "mainwindow.h"

namespace Ui {
class newlogin;
}

class newlogin : public QDialog
{
    Q_OBJECT

public:
    explicit newlogin(QWidget *parent = nullptr);
    ~newlogin();

private slots:
    void onHttpFinished(QNetworkReply *reply);
    void on_pushButton_clicked();

private:
    Ui::newlogin *ui;
    NetworkManager* networkManager;
};

#endif // NEWLOGIN_H
