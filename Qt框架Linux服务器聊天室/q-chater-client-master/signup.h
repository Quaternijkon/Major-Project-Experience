#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QMainWindow>
#include <QTcpSocket>
#include "loginhint.h"
#include "utility.h"
#include <QHostAddress>

namespace Ui {
class SignUp;
}

class SignUp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp(QTcpSocket* client, QWidget *parent = nullptr);
    ~SignUp();
    Ui::SignUp* get_ui();
private:
    Ui::SignUp *ui;
    QTcpSocket* client;
    void sign_up(QString& acc, QString& psw);
};

#endif // SIGNUP_H
