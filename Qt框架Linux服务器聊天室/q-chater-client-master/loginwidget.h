#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget* welcome_page, QWidget *parent = nullptr);
private:
    QPushButton* button;
    QWidget* welcome_page;
signals:

};

#endif // LOGINWIDGET_H
