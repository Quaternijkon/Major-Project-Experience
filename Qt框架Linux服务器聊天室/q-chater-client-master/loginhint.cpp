#include "loginhint.h"
#include "ui_loginhint.h"

LoginHint::LoginHint(const QString& msg, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::LoginHint)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/img/icon.png"));
    this->ui->info->setText(msg);
    connect(this->ui->ok, &QPushButton::clicked, [this]() {
        //QApplication::beep();
        this->close();
    });
}

LoginHint::~LoginHint()
{
    delete ui;
}

void LoginHint::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
        this->ui->ok->click();
        break;
    case Qt::Key_Return:
        this->ui->ok->click();
        break;
    default:
        break;
    }
}
