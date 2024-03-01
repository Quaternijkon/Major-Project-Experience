#include "variable.h"

Variable::Variable()
{

}

QString r1="ff",g1="ff",b1="ff",r2="ff",g2="ff",b2="ff";
QString file="0";

QString _r1="ff",_g1="ff",_b1="ff",_r2="ff",_g2="ff",_b2="ff";
QString _file="0";

const QString styleStr="QLabel#label{background-image:url(:/new/patterns/patterns/%1.png);"
                       "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #%2,stop:1 #%3);"
                       "background-position:center;border: 1px solid black;border-radius:100px;}";
