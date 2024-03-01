#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class ClickableLabel: public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent = nullptr);
signals:
    void clicked();
protected:

    void mousePressEvent(QMouseEvent* event) override;
};

#endif // CLICKABLELABEL_H
