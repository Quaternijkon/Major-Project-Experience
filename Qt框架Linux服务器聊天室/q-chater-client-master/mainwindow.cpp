#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , can_login(false), button1(new QPushButton()), button2(new QPushButton()), ui(new Ui::MainWindow)
    , log_in(new LoginWidget(this)), client(new QTcpSocket(this)), sign_up(nullptr), chat_window(nullptr)
    , style(new Style()), animation_close(new QPropertyAnimation(this, "windowOpacity"))
    , animation_minimize(new QPropertyAnimation(this, "windowOpacity"))
{

    this->ui->setupUi(this);
    this->setWindowIcon(QIcon("/img/icon.png"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->style->hide();
    client->connectToHost(QHostAddress("127.0.0.1"), 8899);
    //客户端响应服务器
    connect(client, &QTcpSocket::readyRead, [this]() {
        auto recv = this->client->readAll();
        auto recv_list = recv.split('\n');
        for (auto& recv: recv_list) {
            process_response(recv);
        }
    });
    //点击×关闭窗口
    connect(this->animation_close, &QPropertyAnimation::finished, this, &QMainWindow::close);
    connect(this->ui->closeWindow, &QPushButton::clicked, [this]() {
        this->animation_close->setDuration(100);
        this->animation_close->setStartValue(1);
        this->animation_close->setEndValue(0);
        this->animation_close->start();
    });
    //点击-最小化窗口
    connect(this->ui->minimizeWindow, &QPushButton::clicked, [this]() {
        this->animation_minimize->setDuration(100);
        this->animation_minimize->setStartValue(1);
        this->animation_minimize->setEndValue(0);
        this->animation_minimize->start();
    });
    connect(this->animation_minimize, &QPropertyAnimation::finished, [this]() {
        this->showMinimized();
        this->setProperty("windowOpacity", 1);
    });
    //点击登录，同时disable登录按钮
    connect(this->ui->logIn, &QPushButton::clicked, [this](){
       QString acc_psw = Utility::get_string_of_request_type(RequestType::LOGIN) + " " + this->ui->account->text() + " " + this->ui->password->text();
       //将account和psw和请求类型传给服务器
       this->client->write(acc_psw.toStdString().c_str());
       this->ui->logIn->setText("Waiting...");
       this->ui->logIn->setEnabled(false);

    });
    //点击注册label
    connect(this->ui->signUp, &ClickableLabel::clicked, [this]() {
        this->sign_up = new SignUp(this->client, this);
        //ui->signUp->setStyleSheet()
        this->sign_up->get_ui()->label->setStyleSheet(QString(styleStr).arg(file).arg(r1+g1+b1).arg(r2+g2+b2));
        this->sign_up->show();
        //TODO:bug，多次点击sign up label会产生多个注册窗口
    });
    //账号、密码有一个为空，disable登录按钮
    connect(this->ui->account, &QLineEdit::textChanged, [this]() {
        this->can_login = this->ui->account->text() != "" && this->ui->password->text() != "";
        this->ui->logIn->setEnabled(can_login);
    });
    connect(this->ui->password, &QLineEdit::textChanged, [this]() {
        this->can_login = this->ui->account->text() != "" && this->ui->password->text() != "";
        this->ui->logIn->setEnabled(can_login);
    });

    //*dry<
    connect(this->ui->styleButton, &ClickableLabel::clicked, [this]() {
        this->style->show();
    });

    //connect(this->style, &QWidget::close, this, &QMainWindow::show);

    connect(style,&Style::sig,this,[this](){
        this->ui->window_border->setStyleSheet(QString("QLabel#window_border{background-image:url(:/new/patterns/patterns/%1.png);"
                                                 "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #%2,stop:1 #%3);"
                                                 "background-position:center;}").arg(file).arg(r1+g1+b1).arg(r2+g2+b2));

    });
    //*>

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::my_slot()
{
    qDebug() << "Click Button2\n";
    this->close();
}

void MainWindow::recover_login_btn()
{
    this->ui->logIn->setEnabled(true);
    this->ui->logIn->setText("Login");
}

void MainWindow::process_response(QByteArray& bytes)
{
    auto std_str = QString(bytes).toStdString();
    if (std_str.empty()) {
        return;
    }
    std::cout << "got: " + std_str << std::endl;  
    switch (std_str.front()) {
    case 'a':
    {
        auto friends = QVector<QString>();
        std::stringstream in;
        //登录
        int status;
        int acc_exist;
        int correct_psw;
        int is_online;
        in = std::stringstream(std_str.substr(9, std_str.size()));
        sscanf_s(std_str.c_str(), "a %d %d %d %d", &status, &acc_exist, &correct_psw, &is_online);
        if (status == 0) {
            QApplication::beep();
            auto pop_up = LoginHint("Server Error!", this);
            qDebug() << "Server Error";
            pop_up.exec();
            this->recover_login_btn();
            return;
        }
        if (!acc_exist) {
            QApplication::beep();
            auto pop_up = LoginHint("Illegal Account!", this);
            qDebug() << "Illegal Account";
            pop_up.exec();
            this->recover_login_btn();
            return;
        }
        if (!correct_psw) {
            QApplication::beep();
            auto pop_up = LoginHint("Error Password!", this);
            qDebug() << "Error Password";
            pop_up.exec();
            this->recover_login_btn();
            return;
        }
        if (is_online) {
            QApplication::beep();
            auto pop_up = LoginHint("Already Online!", this);
            qDebug() << "Already Online";
            pop_up.exec();
            this->recover_login_btn();
            return;
        }
        while (in) {
            auto friend_acc = std::string();
            in >> friend_acc;
            friends.push_back(friend_acc.c_str());
        }
        friends.pop_back();
        chat_window = new ChatWindow(friends, this->ui->account->text(), this->client, this->ui->window_border->styleSheet() != ""? QString("QLabel#windowBorder{background-image:url(:/new/patterns/patterns/%1.png);"
                                                                                               "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #%2,stop:1 #%3);"
                                                                                               "background-position:center;}").arg(file).arg(r1+g1+b1).arg(r2+g2+b2):"");
        //this->chat_window->get_ui()->centralwidget->setStyleSheet(QString("QWidget#centralWidget{background-image:url(:/new/patterns/patterns/%1.png);"
        //                                                              "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #%2,stop:1 #%3);"
        //                                                             "background-position:center;}").arg(file).arg(r1+g1+b1).arg(r2+g2+b2));
        this->hide();
        chat_window->show();
    }
        break;
    case 'b':
    {
        //注册
        if (std_str == "b 1") {
            qDebug() << "Success!";
            auto pop_up = LoginHint("Success!", this->sign_up);
            pop_up.exec();
        } else if (std_str == "b 0") {
            QApplication::beep();
            qDebug() << "Account Already Exsits!";
            auto pop_up = LoginHint("Account Already Exsits!", this->sign_up);
            pop_up.exec();
        } else {
            QApplication::beep();
            qDebug() << "Server Error!";
            auto pop_up = LoginHint("Server Error!", this->sign_up);
            pop_up.exec();
        }
    }
        break;
    case 'c':
    {
        //添加好友
        if (std_str[2] == '1') {
            this->chat_window->addFriend(std_str.substr(4, std_str.size()).c_str());
            auto pop_up = LoginHint("Added Successfully", this->chat_window);
            pop_up.exec();
        } else {
            QApplication::beep();
            auto pop_up = LoginHint("No Such ID", this->chat_window);
            pop_up.exec();
        }
    }
        break;
    case 'd':     
    {
        //qDebug() << QString::fromUtf8("收到信息，准备发送");
        int first_blank = 0;
        int second_blank = 0;
        for (int i = 0, blank_cnt = 0; i < std_str.size() && blank_cnt < 2; ++i) {
            if (std_str[i] == ' ') {
                if (blank_cnt == 0) {
                    first_blank = i;
                } else {
                    second_blank = i;
                }
                ++blank_cnt;
            }
        }
        auto from = std_str.substr(first_blank + 1, second_blank - first_blank - 1);
        auto msg = std_str.substr(second_blank + 1, std_str.size());
        this->chat_window->recieve(from._Starts_with("__group__")?
                                       QString(from.substr(9, from.size()).c_str()):
                                       QString(from.c_str())
                                   ,QString(msg.c_str()));
        //sscanf_s(std_str.c_str(), "d %s %s", from, msg);
        std::cout << "recv_msg: {from: " << from << ", msg: " << msg << "}" << std::endl;
    }
        break;
    case 'e':
        {
            qDebug() << "ready to join a group";
            int status;
            sscanf_s(std_str.c_str(), "e %d", &status);
            qDebug() << QString("status = %1").arg(status);
            if (!status) {
                //已经存在这个群聊
                auto pop_up = LoginHint("Group Has Been Existed!", this);
                QApplication::beep();
                pop_up.exec();
                break;
            }
            auto group_name = QString::fromStdString(std_str.substr(4, std_str.size()));
            qDebug() << "\"fake\" group name: " << group_name;
            this->chat_window->addFriend(group_name);
            auto pop_up = LoginHint("Created!");
            pop_up.exec();
        }
        break;
    default:
        break;
    }
    //qDebug() << QString(bytes);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = y - this->z;
    this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    this->z = QPoint();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
        if (this->ui->account->text().size() && this->ui->password->text().size()) {
            this->ui->logIn->click();
        }
        break;
    case Qt::Key_Return:
        if (this->ui->account->text().size() && this->ui->password->text().size()) {
            this->ui->logIn->click();
        }
        break;
    default:
        break;
    }
}



