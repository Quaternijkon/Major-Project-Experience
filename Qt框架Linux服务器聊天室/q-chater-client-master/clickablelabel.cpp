#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent): QLabel(parent)
{
//    this->setStyleSheet("ClickableLabel{"
//                        "border-style: solid;"
//                        "border-top-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgb(215, 215, 215), stop:1 rgb(222, 222, 222));"
//                        "border-right-color: qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgb(217, 217, 217), stop:1 rgb(227, 227, 227));"
//                        "border-left-color: qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgb(227, 227, 227), stop:1 rgb(217, 217, 217));"
//                        "border-bottom-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgb(215, 215, 215), stop:1 rgb(222, 222, 222));"
//                        "border-width: 1px;"
//                        "border-radius: 5px;"
//                        "color: rgb(255,255,255);"
//                        "padding: 2px;"
//                        "background-color:  rgba(255, 255, 255, 0);"
//                        "border-color: rgb(0, 0, 0);"
//                        "color: rgb(0, 0, 0);"
//                    "}"
//                    "ClickableLabel:hover{"
//                        "border-style: solid;"
//                        "border-top-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgb(215, 215, 215), stop:1 rgb(222, 222, 222));"
//                        "border-right-color: qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgb(217, 217, 217), stop:1 rgb(227, 227, 227));"
//                        "border-left-color: qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgb(227, 227, 227), stop:1 rgb(217, 217, 217));"
//                        "border-bottom-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgb(215, 215, 215), stop:1 rgb(222, 222, 222));"
//                        "border-width: 1px;"
//                        "border-radius: 5px;"

//                        "color: rgb(255, 255, 255);"
//                        "padding: 2px;"
//                        "background-color: rgb(0, 0, 0);"
//                    "}");
    this->setStyleSheet("background-color: rgb(0, 0, 0);");
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        emit this->clicked();
        break;
    default:
        break;
    }
    QLabel::mousePressEvent(event);
}
