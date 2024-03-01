#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_LEN 1024
#define PORT 7721

extern void* thread_fun(void*);
extern void process_request(char*, int*);
extern struct login_result* login(char* account, char* psw);
extern struct signup_result* sign_up(char* account, char* psw);
//登录查询后，检查有无对应账号或者密码是否正确，并修改struct login_result
extern int process_login_result(void*, int, char**, char**);
//注册回调函数，只是用来检测是否被调用过
extern int process_signup_result(void*, int, char**, char**);


int main() {
    pthread_t tid;
    struct sockaddr_in my_addr{};
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int lisfd = socket(AF_INET, SOCK_STREAM, 0);

    if (lisfd == -1) {
        perror("socket");
        return -1;
    }
    int vel = 1;
    setsockopt(lisfd, SOL_SOCKET, SO_REUSEADDR, (void*)&vel, sizeof(vel));
    if (bind(lisfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        return -1;
    }
    if (listen(lisfd, 10)) {
        perror("listen");
        return -1;
    }
    while (true) {
        int confd = accept(lisfd, nullptr, nullptr);
        if (confd == -1) {
            perror("accept");
            return -1;
        }
        pthread_create(&tid, nullptr, thread_fun, (void*)(long long)confd);
    }
    return 0;
}
