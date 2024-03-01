#ifndef STYLE_H
#define STYLE_H

#include <QWidget>
#include <variable.h>
#include <QIntValidator>

namespace Ui {
class Style;
}

class Style : public QWidget
{
    Q_OBJECT

public:
    explicit Style(QWidget *parent = 0);
    ~Style();
    void setLabelStyle();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_horizontalSlider_6_valueChanged(int value);

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_lineEdit_5_editingFinished();

    void on_lineEdit_6_editingFinished();

    void on_pushButton_apply_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Style *ui;
    QPoint z;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void sig();
};

#endif // STYLE_H
