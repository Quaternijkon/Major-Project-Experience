#ifndef LOGINHINT_H
#define LOGINHINT_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class LoginHint;
}

class LoginHint : public QDialog
{
    Q_OBJECT

public:
    explicit LoginHint(const QString& msg, QWidget *parent = nullptr);
    ~LoginHint();

private:
    Ui::LoginHint *ui;
protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // LOGINHINT_H
