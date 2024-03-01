#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListView>
#include <friendcard.h>
#include <QListWidget>
#include <QStandardItemModel>
#include <QTcpSocket>
#include <QPropertyAnimation>
#include <creategroup.h>
#include "msgbubble.h"
#include "loginhint.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const QVector<QString>& friends,
                        const QString& acc_name,
                        QTcpSocket* client,
                        const QString& style_sheet,
                        QWidget *parent = nullptr
                        );
    ~ChatWindow();
    void addFriend(const QString& friend_acc);
    void recieve(const QString& who, const QString& what);
    Ui::ChatWindow* get_ui();
private:
    Ui::ChatWindow *ui;
    QListWidget* friends_list;
    QTcpSocket* client;
    QString acc_name;
    QString target_name;
    QListWidget* current_chat;
    QHash<QString, QListWidget*>* list_map;
    QPoint z;
    QPropertyAnimation* animation_close;
    QPropertyAnimation* animation_minimize;
    CreateGroup* create_group;
    QSet<QString>* group_set;
private slots:
    void send_msg();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // CHATWINDOW_H
