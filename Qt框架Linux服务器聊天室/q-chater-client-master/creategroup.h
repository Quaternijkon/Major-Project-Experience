#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QCheckBox>
#include <QTcpSocket>
#include <QListWidget>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "loginhint.h"

namespace Ui {
class CreateGroup;
}

class CreateGroup : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateGroup(QVector<QString>& friends, QTcpSocket* client, QWidget *parent = nullptr);
    ~CreateGroup();

private:
    Ui::CreateGroup *ui;
    QTcpSocket* client;
    QSet<QCheckBox*>* friends_set;
    QPropertyAnimation* animation;
    QPoint z;
    QString myself;
private slots:
    void send_create_group_request();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CREATEGROUP_H
