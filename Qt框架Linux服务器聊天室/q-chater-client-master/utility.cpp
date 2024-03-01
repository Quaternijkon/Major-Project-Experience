#include "utility.h"



Utility::Utility(QObject *parent)
    : QObject{parent}
{

}

QString Utility::get_string_of_request_type(RequestType type)
{
    switch (type) {
    case RequestType::LOGIN:
        return "0";
    case RequestType::SIGN_UP:
        return "1";
    default:
        return "-1";
    }
}
