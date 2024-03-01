#include<windows.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include <winnt.rh>

#define PATH_SIZE 512
#define BUF_SIZE 1024
using namespace std;
//复制文件
void CopyFile(char *OldPath, char *NewPath);

// 判断是否为子目录文件
BOOL isChildDir(WIN32_FIND_DATA &lpFindData);

void mycp(char *OldPath, char *NewPath);

// argv接收来自命令行的三个参数，执行程序 旧文件路径 新文件路径
int main(int argc, char *argv[]) {
    WIN32_FIND_DATA lpFindData, lpNewFindData;
    HANDLE hFindFile;

    if (argc != 3) {
        cout << "Number of Parameters Error!" << endl;
        exit(-1);
    }
    //查找源文件
    if ((hFindFile = FindFirstFile(argv[1], &lpFindData)) == INVALID_HANDLE_VALUE) {
        cout << "Finding File Error!" << endl;
        exit(-1);
    }
    //创建新目录
    if (FindFirstFile(argv[2], &lpNewFindData) == INVALID_HANDLE_VALUE) {
        CreateDirectory(argv[2], NULL);
        cout << "Creating Directory Successfully" << endl;
    }
    mycp(argv[1], argv[2]);
    HANDLE hDirFile = CreateFile(
            argv[2],
            GENERIC_WRITE | GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL);
    SetFileTime(
            hDirFile,
            &lpFindData.ftCreationTime,
            &lpFindData.ftLastAccessTime,
            &lpFindData.ftLastWriteTime);
    SetFileAttributes(argv[2], lpFindData.dwFileAttributes);
    CloseHandle(hDirFile);
    CloseHandle(hFindFile);
    hFindFile = INVALID_HANDLE_VALUE;
    hDirFile = INVALID_HANDLE_VALUE;
    cout << "Copy Successfully!" << endl;
    return 0;
}

void CopyFile(char *OldPath, char *NewPath) {
    WIN32_FIND_DATA lpFindData;
    HANDLE hFind = FindFirstFile(OldPath, &lpFindData);
    //创建新文件
    HANDLE hNewFile = CreateFile(
            NewPath,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    //打开源文件
    HANDLE hOldFile = CreateFile(
            OldPath,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    if (hNewFile != INVALID_HANDLE_VALUE && hOldFile != INVALID_HANDLE_VALUE) {
        char buffer[BUF_SIZE];
        DWORD dwxfer(0);
        //读源文件内容
        while (true) {
            ReadFile(
                    hOldFile,
                    buffer,
                    BUF_SIZE,
                    &dwxfer,
                    NULL);
            //向新文件中写
            WriteFile(
                    hNewFile,
                    buffer,
                    dwxfer,
                    &dwxfer,
                    NULL);
            if (dwxfer != BUF_SIZE) break;
        }
        //修改文件时间属性
        SetFileTime(
                hNewFile,
                &lpFindData.ftCreationTime,
                &lpFindData.ftLastAccessTime,
                &lpFindData.ftLastWriteTime);
        SetFileAttributes(NewPath, lpFindData.dwFileAttributes);
    }
    CloseHandle(hNewFile);
    CloseHandle(hOldFile);
    hOldFile = INVALID_HANDLE_VALUE;
    hNewFile = INVALID_HANDLE_VALUE;
}

BOOL isChildDir(WIN32_FIND_DATA &lpFindData) {
    return (
            ((lpFindData.dwFileAttributes &
              FILE_ATTRIBUTE_DIRECTORY) != 0) &&
            (lstrcmp(lpFindData.cFileName, __TEXT(".")) != 0) &&
            (lstrcmp(lpFindData.cFileName, __TEXT("..")) != 0));
}

void mycp(char *OldPath, char *NewPath) {
    char old_path[PATH_SIZE], new_path[PATH_SIZE];
    WIN32_FIND_DATA lpFindData;
    lstrcpy(old_path, OldPath);
    lstrcpy(new_path, NewPath);
    strcat(old_path, "/*.*");
    HANDLE hFindData;
    BOOL hFound = 0;
    hFindData = FindFirstFile(old_path, &lpFindData);
    if (hFindData != INVALID_HANDLE_VALUE) {
        //查找下一个文件
        while ((hFound = FindNextFile(hFindData, &lpFindData)) != 0) {
            //路径拼接
            lstrcpy(old_path, OldPath);
            lstrcpy(new_path, NewPath);
            lstrcat(old_path, "/");
            lstrcat(new_path, "/");
            lstrcat(old_path, lpFindData.cFileName);
            lstrcat(new_path, lpFindData.cFileName);
            //判断文件是否为目录文件
            if (isChildDir(lpFindData)) {
                printf("child directory:%s begin copy\n", lpFindData.cFileName);
                CreateDirectory(new_path, NULL);
                mycp(old_path, new_path);
                //修改子目录时间属性
                HANDLE hDirFile = CreateFile(
                        new_path,
                        GENERIC_WRITE | GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS,
                        NULL);
                SetFileTime(
                        hDirFile,
                        &lpFindData.ftCreationTime,
                        &lpFindData.ftLastAccessTime,
                        &lpFindData.ftLastWriteTime);
                SetFileAttributes(new_path, lpFindData.dwFileAttributes);
                CloseHandle(hDirFile);
                hDirFile = INVALID_HANDLE_VALUE;
                printf("child directory:%s copy Successfully\n", lpFindData.cFileName);
            } else {
                CopyFile(old_path, new_path);
                cout << "Filename:" << lpFindData.cFileName << "copy Successfully" << endl;
            }
        }
    } else {
        cout << "Find File Error!" << endl;
        exit(-1);
    }
    CloseHandle(hFindData);
    hFindData = INVALID_HANDLE_VALUE;
}
