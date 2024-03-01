#include "creategroup.h"
#include "ui_creategroup.h"

CreateGroup::CreateGroup(QVector<QString>& friends, QTcpSocket* client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateGroup),
    client(client),
    friends_set(new QSet<QCheckBox*>()),
    animation(new QPropertyAnimation(this, "windowOpacity")),
    myself(friends.back())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/img/icon.png"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    for (auto& name: friends) {
        if (name == this->myself) {
            continue;
        }
        auto check_box = new QCheckBox(name);
        check_box->setFont(QFont("Microsoft Yahei UI", 10));
        this->ui->verticalLayout->addWidget(check_box, 0, Qt::AlignLeft);
        this->friends_set->insert(check_box);
    }
    //点×关闭窗口
    connect(this->ui->close, &QPushButton::clicked, [this]() {
        this->animation->setDuration(100);
        this->animation->setStartValue(1);
        this->animation->setEndValue(0);
        this->animation->start();
    });
    connect(this->animation, &QPropertyAnimation::finished, this, &QMainWindow::close);
    connect(this->ui->ok, &QPushButton::clicked, this, &CreateGroup::send_create_group_request);
}

CreateGroup::~CreateGroup()
{
    delete ui;
}

void CreateGroup::send_create_group_request()
{
    if (this->ui->group_name->text() == "") {
        auto pop_up = LoginHint("Empty Group Name!");
        QApplication::beep();
        pop_up.exec();
        return;
    }
    qDebug() << "sent create group request";
    auto friends = QString();
    for (auto& check_box: *this->friends_set) {
        if (check_box->isChecked()) {
            friends += check_box->text() + " ";
        }
    }
    if (friends == "") {
        auto pop_up = LoginHint("At Least Choose one!");
        QApplication::beep();
        pop_up.exec();
        return;
    }
    friends += this->myself;
    auto request = QString("6 %1 %2").arg(this->ui->group_name->text()).arg(friends);
    this->client->write(request.toStdString().c_str());
    this->close();
}

void CreateGroup::mouseMoveEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = y - this->z;
    this->move(x);
}

void CreateGroup::mousePressEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}

void CreateGroup::mouseReleaseEvent(QMouseEvent* event)
{
    this->z = QPoint();
}
