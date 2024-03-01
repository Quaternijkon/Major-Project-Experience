#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sqlite3.h>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <unistd.h>

#define PRINT_FONT_GRE  printf("\033[32m");
#define PRINT_FONT_BLA  printf("\033[30m");
#define PRINT_FONT_PUR  printf("\033[35m");

struct sockaddr_in;
struct login_result;
struct signup_result;

extern ssize_t write (int, const void*, size_t);
void* thread_fun(void*);
void process_request(char*, int*);
struct login_result* login(char*, char*);
struct signup_result* sign_up(char*, char*);
int process_login_result(void*, int, char**, char**);
int process_signup_result(void*, int, char**, char**);
int process_get_friends_result(void*, int, char**, char**);
void get_friends(char*, char*);
void add_friend(int*, const char*, const char*);
void send_msg(const char* from, const char* to, const char* msg);
bool is_online(const char* account);
void create_group(const std::string& group_name, const std::vector<std::string>& members, const int* confd_p);

#define MAX_LEN 1024

std::unordered_set<std::string> online_accs;

std::unordered_map<std::string, int> online_groups;

std::unordered_map<std::string, std::unordered_set<std::string>> group_members;

std::unordered_map<std::string, std::queue<std::pair<std::string, char*>>> left_msg;

std::unordered_map<std::string, int*> confd_p_map;

struct login_result {
    /*用户输入的密码*/
    char* psw;
    /*如果status为0，直接报服务器出错*/
    int status;
    /*回调函数调用次数，如果为0，说明没有这个账号*/
    int call_back_times;
    /*密码正确*/
    int correct_psw;
    /*好友信息*/

};

struct signup_result {
    /*是否注册成功, 0: 账号重复， 1：成功， -1：服务器错误*/
    int success;
    /*回调函数调用次数*/
    int call_back_times;
    /*如果账号还没被注册，handler储存了对应db的句柄*/
    sqlite3* handler;
};

void* thread_fun(void* arg) {
    char buffer[MAX_LEN * 2];
    //memset(buffer, 0, sizeof(buffer));
    int confd = (int)(intptr_t)(arg);
    //write(confd, buffer, strlen(buffer));
    memset(buffer, 0, sizeof(buffer));
    while (true) {
        long byte_num = recv(confd, buffer, sizeof(buffer), 0);
        if (byte_num == 0) {
            pthread_exit(nullptr);
        }
        process_request(buffer, &confd);
        //printf("recv: %s\n", buffer);
        //write(confd, buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
    }
}

void process_request(char* request, int* confid_p) {
    int request_code;
    sscanf(request, "%d", &request_code);
    char account[MAX_LEN / 2];
    char psw[MAX_LEN / 2];
    char response[MAX_LEN * 2];
    char from[MAX_LEN / 2];
    char to[MAX_LEN / 2];
    char content[MAX_LEN * 2];
    PRINT_FONT_GRE
    printf("got: %s\n", request);
    PRINT_FONT_BLA
    switch (request_code)
    {
    case 0:
        {
            /*登录*/
            sscanf(request, "%d %s %s", &request_code, account, psw);
            struct login_result* login_result = login(account, psw);
            strcpy(response, "a ");
            if (login_result->status) {
                strcat(response, "1 ");
            } else {
                strcat(response, "0 ");
            }
            if (login_result->call_back_times) {
                strcat(response, "1 ");
            } else {
                strcat(response, "0 ");
            }
            if (login_result->correct_psw) {
                strcat(response, "1 ");
            } else {
                strcat(response, "0 ");
            }
            if (is_online(account)) {
                //当前在线
                strcat(response, "1 ");
            }
            else {
                strcat(response, "0 ");
            }
            //密码不正确直接返回
            if (!login_result->correct_psw) {
                write(*confid_p, response, strlen(response));
                PRINT_FONT_PUR
                printf("sent: %s\n", response);
                PRINT_FONT_BLA
                free(login_result);
                break;  
            }
            /*获取好友列表*/
            if (login_result->correct_psw) {
                char friends[1024] = "";
                get_friends(account, friends);
                strcat(response, friends);
            }
            //账号登记为在线
            online_accs.emplace(account);
            //一个账号对应一个响应的id
            confd_p_map[account] = confid_p;
            strcat(response, "\n");
            write(*confid_p, response, strlen(response));
            PRINT_FONT_PUR
            printf("sent: %s\n", response);
            PRINT_FONT_BLA
            //发送存在于服务器的消息
            auto& msg_queue = left_msg[account];
            while (!msg_queue.empty()) {
                auto& from_msg = msg_queue.front();
                msg_queue.pop();
                send_msg(from_msg.first.c_str(), account, from_msg.second);
                delete[] from_msg.second;
            }
            free(login_result);
        }
        break;
    case 1:
        {
            /*注册*/
            sscanf(request, "%d %s %s", &request_code, account, psw);
            printf("Staring signing up...\n acc: %s, psw: %s\n", account, psw);
            struct signup_result* signup_result = sign_up(account, psw);
            if (signup_result->success == -1) {
                strcpy(response, "b -1");
                strcat(response, "\n");
                write(*confid_p, response, strlen(response));
                PRINT_FONT_PUR
                printf("sent: b -1\n");
                PRINT_FONT_BLA
                //memset(response, 0, sizeof(response));
                free(signup_result);
                break;
            } else if (signup_result->success == 0) {
                strcpy(response, "b 0");
                strcat(response, "\n");
                write(*confid_p, response, strlen(response));
                PRINT_FONT_PUR
                printf("sent: b 0\n");
                PRINT_FONT_BLA
                //memset(response, 0, sizeof(response));
                free(signup_result);
                break;
            } else {
                char insert_statement[MAX_LEN * 2];
                sprintf(insert_statement, R"(insert into Acc_Psw values('%s', '%s'))", account, psw);
                printf("statement = %s\n", insert_statement);
                char* info = nullptr;
                sqlite3_exec(signup_result->handler, insert_statement, nullptr, nullptr, &info);
                if (info) {
                    printf("%s\n", info);
                    sqlite3_free(info);
                }
                strcpy(response, "b 1");
                strcat(response, "\n");
                write(*confid_p, response, strlen(response));
                PRINT_FONT_PUR
                printf("sent: b 1\n");
                PRINT_FONT_BLA
                free(signup_result);
            }
        }
        break;
    case 3:
        {
            /*添加好友*/
            char friend_acc[MAX_LEN];
            sscanf(request, "%d %s %s", &request_code, account, friend_acc);
            int found = 0;
            add_friend(&found, account, friend_acc);
            /*添加好友成功，返回"c 1"*/
            if (found) {
                sprintf(response, "c 1 %s", friend_acc);
            } else {
                sprintf(response, "c 0 %s", friend_acc);
            }
            strcat(response, "\n");
            write(*confid_p, response, strlen(response));
            PRINT_FONT_PUR
            printf("sent: %s\n", response);
            PRINT_FONT_BLA
            //memset(response, 0, sizeof(response));
        }
        break;
    case 4:
        {
            /*发送消息*/
            sscanf(request, "%d %s %s", &request_code, from, to);
            //寻找消息开始的位置
            int msg_start = 0;
            for (int i = 0, blank_cnt = 0; request[i] != '\0'; ++i) {
                if (request[i] == ' ') {
                    ++blank_cnt;
                }
                if (blank_cnt == 3) {
                    msg_start = i + 1;
                    break;
                }
            }
            strcpy(content, request + msg_start);
            printf("message to be sent: %s\n", content);
            //如果是发送的目标是群聊
            if (online_groups.contains(to)) {
                //就对群聊的每个成员发送对应的消息
                auto friends_set = group_members[to];
                for (auto& f: friends_set) {
                    //当然自己除外
                    if (f != from) {
                        send_msg(to, f.c_str(), content);
                    }
                }
                break;
            }
            send_msg(from, to, content);
            break;
        }
    case 5:
        /*下线*/
        {
            sscanf(request, "%d %s", &request_code, account);
            sqlite3* handler;
            sqlite3_open("./db/data.db", &handler);
            char query[MAX_LEN];
            sprintf(query, R"(select Friend from Usr_Friend where Usr = '%s')", account);
            char* info = nullptr;
            sqlite3_exec(handler, query, [](void* arg, int col, char** col_val, char** col_name) {
                auto cur_friend = col_val[0];
                if (online_groups.contains(cur_friend)) {
                    --online_groups[cur_friend];
                    if (online_groups[cur_friend] == 0) {
                        online_groups.erase(cur_friend);
                        group_members.erase(cur_friend);
                    }
                }
                return 0;
            }, nullptr, &info);
            online_accs.erase(account);
            confd_p_map.erase(account);
            left_msg.erase(account);
        }
        break;
    case 6:
        /*创建群聊*/
        /*格式：6 %s %s %s ...*请求code， 群聊id， 成员名*/
        {
            auto in = std::stringstream(request);
            in >> request_code;
            auto groupid = std::string();
            in >> groupid;
            auto members = std::vector<std::string>();
            while (in) {
                auto member = std::string();
                in >> member;
                if (member.empty() || std::all_of(member.cbegin(), member.cend(), [](char ch) {return ch == ' ';})) {
                    continue;
                }
                members.emplace_back(member);
            }
            create_group(groupid, members, confid_p);
        }
    default:
        break;
    }
}

struct login_result* login(char* account, char* psw) {
    auto* result = (struct login_result*)malloc(sizeof(struct login_result));
    memset(result, 0, sizeof(struct login_result));
    result->psw = psw;
    sqlite3* handler;
    int val = sqlite3_open("./db/data.db", &handler);
    if (val) {
        printf("sqlite3_open(\"./db/data.db\", &handler)\" error!\n");
        result->status = 0;
        return result;
    }
    result->status = 1;
    char* info = nullptr;
    char query[MAX_LEN * 2];
    sprintf(query, "select * from Acc_Psw where Acc = \'%s\'", account);
    printf("query = %s\n", query);
    sqlite3_exec(handler, query, process_login_result, (void*)result, &info);
    if (info) {
        sqlite3_free(info);
    }
    return result;
}

struct signup_result* sign_up(char* account, char* psw) {
    auto* result = (struct signup_result*)malloc(sizeof(struct signup_result));
    memset(result, 0, sizeof(struct signup_result));
    sqlite3* handler;
    int val = sqlite3_open("./db/data.db", &handler);
    if (val) {
        printf("sqlite3_open(\"./db/data.db\", &handler)\" error!\n");
        result->success = -1;
        return result;
    }
    char* info = nullptr;
    char query[MAX_LEN * 2];
    sprintf(query, "select * from Acc_Psw where Acc = \'%s\'", account);
    sqlite3_exec(handler, query, process_signup_result, (void*)result, &info);
    if (info) {
        sqlite3_free(info);
    }
    if (result->call_back_times > 0) {
        result->success = 0;
    } else {
        result->success = 1;
        result->handler = handler;
    }
    return result;
}

int process_login_result(void* arg, int col, char** col_val, char** col_name) {
    auto* result = (struct login_result*)arg;
    result->call_back_times++;
    char* correct_psw = col_val[1];
    //密码正确
    if (!strcmp(result->psw, correct_psw)) {
        result->correct_psw = 1;
    }
    return 0;
}

int process_signup_result(void* arg, int col, char** col_val, char** col_name) {
    struct signup_result* result = (struct signup_result*)arg;
    result->call_back_times++;
    return 0;
}

int process_get_friends_result(void* arg, int col, char** col_val, char** col_name) {
    char* friends = (char*)arg;
    auto cur_friend = col_val[0];
    //printf("%s\n", col_val[0]);
    strcat(friends, cur_friend);
    //判断当前的好友是不是群聊，群聊的特殊密码都是mxy2233@outlook.com
    sqlite3* handler;
    sqlite3_open("./db/data.db", &handler);
    char query[MAX_LEN];
    sprintf(query, "select Psw from Acc_Psw where Acc = \'%s\'", cur_friend);
    char* info = nullptr;
    void* p_arg[] = {&handler, &cur_friend};
    sqlite3_exec(handler, query, [](void* arg, int col, char** col_val, char** col_name) {
        auto psw = col_val[0];
        auto arg_list = (void**)arg;
        auto cur_friend = *(char**)(arg_list[1]);
        auto handler = *(sqlite3**)(arg_list[0]);
        //是群聊
        if (!strcmp(psw, "mxy2233@outlook.com")) {
            char inline_query[MAX_LEN];
            char* inline_info = nullptr;
            //群聊计数+1
            online_groups[cur_friend]++;
            //群聊被加过了，直接退出
            if (online_groups[cur_friend] > 1) {
                return 0;
            }
            //寻找群成员，并加入groups_members
            sprintf(inline_query, R"(select Members from Groupid_Members where Groupid = '%s')", cur_friend);
            sqlite3_exec(handler, inline_query, [](void* arg, int col, char** col_val, char** col_name) {
                auto cur_friend = (char*)arg;
                char* friends_list = col_val[0];
                auto in = std::stringstream(friends_list);
                while (in) {
                    auto friend_acc = std::string();
                    in >> friend_acc;
                    group_members[cur_friend].emplace(friend_acc);
                }
                return 0;
            }, cur_friend, &inline_info);
        }
        return 0;
    }, p_arg, &info);
    strcat(friends, " ");
    return 0;
}

int process_add_friends_result(void* arg, int col, char** col_val, char** col_name) {
    int* found = (int*)arg;
    *found = 1;
    return 0;
}

void get_friends(char* account, char* friends) {
    sqlite3* handler;
    int val = sqlite3_open("./db/data.db", &handler);
    if (val) {
        printf("sqlite3_open(\"./db/data.db\", &handler)\" error!\n");
        return;
    }
    char* info = nullptr;
    char query[MAX_LEN];
    sprintf(query, "select Friend from Usr_Friend where Usr = \'%s\'", account);
    printf("query: %s\n", query);
    sqlite3_exec(handler, query, process_get_friends_result, (void*)friends, &info);
    printf("friends = [%s]\n", friends);
    if (info) {
        sqlite3_free(info);
    }
}

void add_friend(int* found, const char* acc, const char* friend_acc) {
    sqlite3* handler;
    if (sqlite3_open("./db/data.db", &handler)) {
        printf("sqlite3_open(\"./db/data.db\", &handler)\" error!\n");
        *found = 0;
        return;
    }
    char query[MAX_LEN];
    char* info = nullptr;
    if (!*found) {
        sprintf(query, "select * from Acc_Psw where Acc = \'%s\'", friend_acc);
        printf("query = %s\n", query);
        sqlite3_exec(handler, query, process_add_friends_result, (void*)found, &info);
        if (info) {
            printf("%s\n", info);
            sqlite3_free(info);
            info = nullptr;
        }
    }
    if (*found) {
        sprintf(query, R"(insert into Usr_Friend values('%s', '%s'))", acc, friend_acc);
        printf("query = %s\n", query);
        sqlite3_exec(handler, query, nullptr, nullptr, &info);
        if (info) {
            printf("%s\n", info);
            sqlite3_free(info);
            info = NULL;
        }
        sprintf(query, R"(insert into Usr_Friend values('%s', '%s'))", friend_acc, acc);
        printf("query = %s\n", query);
        sqlite3_exec(handler, query, nullptr, nullptr, &info);
        if (info) {
            printf("%s\n", info);
            sqlite3_free(info);
            info = nullptr;
        }
    }
}

bool is_online(const char* account) {
    return online_accs.find(account) != online_accs.end();
}

void send_msg(const char* from, const char* to, const char* msg) {
    if (!is_online(to)) {
        auto msg_p = new char[MAX_LEN];
        strcpy(msg_p, msg);
        //auto pair = std::make_pair<std::string, std::unique_ptr<char[]>>(std::string(from), std::make_unique<char[]>(MAX_LEN));
        //strcpy(pair.second.get(), msg);
        left_msg[to].push({std::string(from), msg_p});
        return;
    }
    char response[MAX_LEN * 2];
    //从哪来的消息，内容是什么
    sprintf(response, "d %s %s", from, msg);
    strcat(response, "\n");
    write(*confd_p_map[to], response, strlen(response));
    PRINT_FONT_PUR
    printf("sent: %s\n", response);
    PRINT_FONT_BLA
}

void create_group(const std::string& group_name, const std::vector<std::string>& members, const int* confd_p) {
    sqlite3* handler;
    sqlite3_open("./db/data.db", &handler);
    char query[MAX_LEN];
    auto groupid = std::string("__group__") + group_name;
    sprintf(query, "select * from Groupid_Members where Groupid = \'%s\'", groupid.c_str());
    char* info = nullptr;
    bool already_exist = false;
    sqlite3_exec(handler, query, [](void* arg, int col, auto col_val, auto col_name){
        *(bool*)arg = true;
        return 0;
    }, &already_exist, &info);
    if (info) {
        printf("%s\n", info);
        sqlite3_free(info);
        return;
    }
    //群聊已经存在
    if (already_exist) {
        write(*confd_p, "e 0", strlen("e 0"));
        PRINT_FONT_PUR
        printf("sent: %s\n", "e 0");
        PRINT_FONT_BLA
        return;
    }
    auto members_str = std::string();
    int n = static_cast<int>(members.size());
    for (int i = 0; i < n - 1; ++i) {
        members_str += (members[i] + " ");
    }
    members_str += members.back();
    //成员列表现在是"a b c d"格式
    sprintf(query, R"(insert into Groupid_Members values('%s', '%s'))", groupid.c_str(), members_str.c_str());
    sqlite3_exec(handler, query, nullptr, nullptr, &info);
    if (info) {
        printf("%s\n", info);
        sqlite3_free(info);
        return;
    }
    char response[MAX_LEN * 2];
    sprintf(response, "e 1 %s", groupid.c_str());
    write(*confd_p, response, strlen(response));
    PRINT_FONT_PUR
    printf("sent: %s\n", response);
    PRINT_FONT_BLA
    //群聊是好友的一种，群聊本身是所有群成员的好友
    sprintf(query, R"(insert into Acc_Psw values('%s', 'mxy2233@outlook.com'))", groupid.c_str());
    sqlite3_exec(handler, query, nullptr, nullptr, &info);
    if (info) {
        printf("%s\n", info);
        sqlite3_free(info);
        return;
    }
    int found = 1;
    //创建成功后，为群聊内所有人添加群聊这一特殊好友
    for (const auto& member: members) {
        add_friend(&found, member.c_str(), groupid.c_str());
    }
}

