#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
enum class RequestType{
    LOGIN,
    SIGN_UP,
};
class Utility : public QObject
{
    Q_OBJECT
public:
    explicit Utility(QObject *parent = nullptr);
    static QString get_string_of_request_type(RequestType type);
signals:

};

#endif // UTILITY_H
