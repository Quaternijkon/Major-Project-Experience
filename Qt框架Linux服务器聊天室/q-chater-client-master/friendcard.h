#ifndef FRIENDCARD_H
#define FRIENDCARD_H

#include <QObject>
#include <QLabel>
#include "clickablelabel.h"

class FriendCard: public ClickableLabel
{
    Q_OBJECT
public:
    FriendCard(const QString& acc, QWidget* parrent = nullptr);
};

#endif // FRIENDCARD_H
