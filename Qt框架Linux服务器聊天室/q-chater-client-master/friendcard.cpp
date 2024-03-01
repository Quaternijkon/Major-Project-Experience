#include "friendcard.h"

FriendCard::FriendCard(const QString& acc, QWidget* parrent): ClickableLabel(parrent)
{
    //this->setGeometry(QRect(10, 20, 131, 51));
    this->setFrameShape(QFrame::Box);
    this->setAlignment(Qt::AlignCenter);
    this->setText(acc);
}
