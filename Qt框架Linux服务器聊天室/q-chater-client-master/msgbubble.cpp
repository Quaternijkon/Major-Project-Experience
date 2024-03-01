#include "msgbubble.h"

MsgBubble::MsgBubble(QString msg, bool mine): QListWidgetItem(msg)
{
    if (!mine) {
        this->setForeground(QBrush(QColor("white")));
        this->setBackground(QBrush(QColor("black")));
    }
    this->setFont(QFont("Microsoft Yahei UI", 9));

}
