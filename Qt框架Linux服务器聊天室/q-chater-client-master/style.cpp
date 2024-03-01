#include "style.h"
#include "ui_style.h"

Style::Style(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Style)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QIntValidator* IntValidator = new QIntValidator;
    IntValidator->setRange(0, 255);

    ui->lineEdit->setValidator(IntValidator);
    ui->lineEdit_2->setValidator(IntValidator);
    ui->lineEdit_3->setValidator(IntValidator);
    ui->lineEdit_4->setValidator(IntValidator);
    ui->lineEdit_5->setValidator(IntValidator);
    ui->lineEdit_6->setValidator(IntValidator);

    //边框线置于最底层
    this->ui->windowBorder->lower();

    connect(this->ui->pattern_1, &ClickableLabel::clicked, [=]() {
        _file="1";
        setLabelStyle();
    });
    connect(this->ui->pattern_2, &ClickableLabel::clicked, [=]() {
        _file="2";
        setLabelStyle();
    });
    connect(this->ui->pattern_3, &ClickableLabel::clicked, [=]() {
        _file="3";
        setLabelStyle();
    });
    connect(this->ui->pattern_4, &ClickableLabel::clicked, [=]() {
        _file="4";
        setLabelStyle();
    });
    connect(this->ui->pattern_5, &ClickableLabel::clicked, [=]() {
        _file="5";
        setLabelStyle();
    });
    connect(this->ui->pattern_6, &ClickableLabel::clicked, [=]() {
        _file="6";
        setLabelStyle();
    });
    connect(this->ui->pattern_7, &ClickableLabel::clicked, [=]() {
        _file="7";
        setLabelStyle();
    });
    connect(this->ui->pattern_8, &ClickableLabel::clicked, [=]() {
        _file="8";
        setLabelStyle();
    });
    connect(this->ui->pattern_9, &ClickableLabel::clicked, [=]() {
        _file="9";
        setLabelStyle();
    });
    connect(this->ui->pattern_10, &ClickableLabel::clicked, [=]() {
        _file="10";
        setLabelStyle();
    });
    connect(this->ui->pattern_11, &ClickableLabel::clicked, [=]() {
        _file="11";
        setLabelStyle();
    });
    connect(this->ui->pattern_12, &ClickableLabel::clicked, [=]() {
        _file="12";
        setLabelStyle();
    });
    connect(this->ui->pattern_13, &ClickableLabel::clicked, [=]() {
        _file="13";
        setLabelStyle();
    });
    connect(this->ui->pattern_14, &ClickableLabel::clicked, [=]() {
        _file="14";
        setLabelStyle();
    });
    connect(this->ui->pattern_15, &ClickableLabel::clicked, [=]() {
        _file="15";
        setLabelStyle();
    });
    connect(this->ui->pattern_16, &ClickableLabel::clicked, [=]() {
        _file="16";
        setLabelStyle();
    });
    connect(this->ui->pattern_17, &ClickableLabel::clicked, [=]() {
        _file="17";
        setLabelStyle();
    });
    connect(this->ui->pattern_18, &ClickableLabel::clicked, [=]() {
        _file="18";
        setLabelStyle();
    });


}

Style::~Style()
{
    delete ui;
}

void Style::setLabelStyle()
{
    //auto base = QString("border-color: black;border-radius:100px;");
    ui->label->setStyleSheet(QString(styleStr).arg(_file).arg(_r1+_g1+_b1).arg(_r2+_g2+_b2));
}

void Style::on_horizontalSlider_valueChanged(int value)
{
    _r1=QString::number(value,16);
    if(_r1.length()==1)
        _r1="0"+_r1;
    ui->lineEdit->setStyleSheet(QString("QLineEdit{background-color:rgb(%1,0,0);}").arg(ui->horizontalSlider->value()));
    ui->lineEdit->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_horizontalSlider_2_valueChanged(int value)
{
    _g1=QString::number(value,16);
    if(_g1.length()==1)
        _g1="0"+_g1;
    ui->lineEdit_2->setStyleSheet(QString("QLineEdit{background-color:rgb(0,%1,0);}").arg(ui->horizontalSlider_2->value()));
    ui->lineEdit_2->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_horizontalSlider_3_valueChanged(int value)
{
    _b1=QString::number(value,16);
    if(_b1.length()==1)
        _b1="0"+_b1;
    ui->lineEdit_3->setStyleSheet(QString("QLineEdit{background-color:rgb(0,0,%1);}").arg(ui->horizontalSlider_3->value()));
    ui->lineEdit_3->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_horizontalSlider_4_valueChanged(int value)
{
    _r2=QString::number(value,16);
    if(_r2.length()==1)
        _r2="0"+_r2;
    ui->lineEdit_4->setStyleSheet(QString("QLineEdit{background-color:rgb(%1,0,0);}").arg(ui->horizontalSlider_4->value()));
    ui->lineEdit_4->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_horizontalSlider_5_valueChanged(int value)
{
    _g2=QString::number(value,16);
    if(_g2.length()==1)
        _g2="0"+_g2;
    ui->lineEdit_5->setStyleSheet(QString("QLineEdit{background-color:rgb(0,%1,0);}").arg(ui->horizontalSlider_5->value()));
    ui->lineEdit_5->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_horizontalSlider_6_valueChanged(int value)
{
    _b2=QString::number(value,16);
    if(_b2.length()==1)
        _b2="0"+_b2;
    ui->lineEdit_6->setStyleSheet(QString("QLineEdit{background-color:rgb(0,0,%1);}").arg(ui->horizontalSlider_6->value()));
    ui->lineEdit_6->setText(QString::number(value));
    setLabelStyle();
}

void Style::on_lineEdit_editingFinished()
{
    ui->horizontalSlider->setValue(ui->lineEdit->text().toInt());
}

void Style::on_lineEdit_2_editingFinished()
{
    ui->horizontalSlider_2->setValue(ui->lineEdit_2->text().toInt());
}

void Style::on_lineEdit_3_editingFinished()
{
    ui->horizontalSlider_3->setValue(ui->lineEdit_3->text().toInt());
}

void Style::on_lineEdit_4_editingFinished()
{
    ui->horizontalSlider_4->setValue(ui->lineEdit_4->text().toInt());
}

void Style::on_lineEdit_5_editingFinished()
{
    ui->horizontalSlider_5->setValue(ui->lineEdit_5->text().toInt());
}

void Style::on_lineEdit_6_editingFinished()
{
    ui->horizontalSlider_6->setValue(ui->lineEdit_6->text().toInt());
}

void Style::on_pushButton_apply_clicked()
{
    r1=_r1;g1=_g1;b1=_b1;
    r2=_r2;g2=_g2;b2=_b2;
    file=_file;
    emit sig();
    this->hide();
}

void Style::on_pushButton_cancel_clicked()
{
    this->hide();
}

void Style::mouseMoveEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = y - this->z;
    this->move(x);
}

void Style::mousePressEvent(QMouseEvent* event)
{
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}

void Style::mouseReleaseEvent(QMouseEvent* event)
{
    this->z = QPoint();
}
