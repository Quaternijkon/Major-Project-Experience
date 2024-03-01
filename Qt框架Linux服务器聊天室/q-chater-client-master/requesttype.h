#ifndef REQUESTTYPE_H
#define REQUESTTYPE_H
#include <QString>
//枚举请求类型，以便服务器根据不同的请求做出不同的response
enum class RequestType{
    LOGIN,
    SIGN_UP,
};
QString get_string_of_request_type(RequestType type) {
    switch (type) {
    case RequestType::LOGIN:
        return "0";
    case RequestType::SIGN_UP:
        return "1";
    default:
        return "-1";
    }
}
#endif // REQUESTTYPE_H
