#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget* welcome_page, QWidget *parent)
    : QWidget{parent}, button{new QPushButton()} , welcome_page{welcome_page}
{
    this->button->setText("Yesy!!!");
    this->button->setParent(this);
    connect(button, &QPushButton::clicked, [this](){
       this->hide();
       this->welcome_page->show();
    });
    const auto ptr = &QPushButton::click;
}
