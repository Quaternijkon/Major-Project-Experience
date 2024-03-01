#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QTcpSocket* client, QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::SignUp), client(client)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/img/icon.png"));
//    client->connectToHost(QHostAddress("127.0.0.1"), 8899);
    //按×关闭注册窗口
    connect(this->ui->close, &ClickableLabel::clicked, this, &QMainWindow::close);
    //只有三个输入框都有字符时，sign up按钮才是激活的
    connect(this->ui->account, &QLineEdit::textChanged, [this]() {
       this->ui->signUp->setEnabled(this->ui->account->text() != "" &&
                this->ui->password->text() != "" &&
                this->ui->confirm_psw->text() != "");
    });
    connect(this->ui->password, &QLineEdit::textChanged, [this]() {
       this->ui->signUp->setEnabled(this->ui->account->text() != "" &&
                this->ui->password->text() != "" &&
                this->ui->confirm_psw->text() != "");
    });
    connect(this->ui->confirm_psw, &QLineEdit::textChanged, [this]() {
       this->ui->signUp->setEnabled(this->ui->account->text() != "" &&
                this->ui->password->text() != "" &&
                this->ui->confirm_psw->text() != "");
    });
    //点击按钮注册
    connect(this->ui->signUp, &QPushButton::clicked, [this]() {
       auto acc = this->ui->account->text();
       auto psw = this->ui->password->text();
       if (acc.contains(' ')) {
           QApplication::beep();
           auto pop_up = LoginHint("Account cna't contains blank!");
           pop_up.exec();
           return;
       }
       if (psw != this->ui->confirm_psw->text()) {
           QApplication::beep();
           auto pop_up = LoginHint("Passwords are different!", this);
           pop_up.exec();
           return;
       }
       this->sign_up(acc, psw);
    });
}

SignUp::~SignUp()
{
    delete ui;
}

Ui::SignUp* SignUp::get_ui()
{
    return this->ui;
}


void SignUp::sign_up(QString& acc, QString& psw)
{
    auto request = Utility::get_string_of_request_type(RequestType::SIGN_UP) + " " + acc + " " + psw;
    this->client->write(request.toStdString().c_str());
}
