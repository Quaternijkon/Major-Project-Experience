#include<windows.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include <winnt.rh>

#define PATH_SIZE 512
#define BUF_SIZE 1024
using namespace std;
//�����ļ�
void CopyFile(char *OldPath, char *NewPath);

// �ж��Ƿ�Ϊ��Ŀ¼�ļ�
BOOL isChildDir(WIN32_FIND_DATA &lpFindData);

void mycp(char *OldPath, char *NewPath);

// argv�������������е�����������ִ�г��� ���ļ�·�� ���ļ�·��
int main(int argc, char *argv[]) {
    WIN32_FIND_DATA lpFindData, lpNewFindData;
    HANDLE hFindFile;

    if (argc != 3) {
        cout << "Number of Parameters Error!" << endl;
        exit(-1);
    }
    //����Դ�ļ�
    if ((hFindFile = FindFirstFile(argv[1], &lpFindData)) == INVALID_HANDLE_VALUE) {
        cout << "Finding File Error!" << endl;
        exit(-1);
    }
    //������Ŀ¼
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
    //�������ļ�
    HANDLE hNewFile = CreateFile(
            NewPath,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    //��Դ�ļ�
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
        //��Դ�ļ�����
        while (true) {
            ReadFile(
                    hOldFile,
                    buffer,
                    BUF_SIZE,
                    &dwxfer,
                    NULL);
            //�����ļ���д
            WriteFile(
                    hNewFile,
                    buffer,
                    dwxfer,
                    &dwxfer,
                    NULL);
            if (dwxfer != BUF_SIZE) break;
        }
        //�޸��ļ�ʱ������
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
        //������һ���ļ�
        while ((hFound = FindNextFile(hFindData, &lpFindData)) != 0) {
            //·��ƴ��
            lstrcpy(old_path, OldPath);
            lstrcpy(new_path, NewPath);
            lstrcat(old_path, "/");
            lstrcat(new_path, "/");
            lstrcat(old_path, lpFindData.cFileName);
            lstrcat(new_path, lpFindData.cFileName);
            //�ж��ļ��Ƿ�ΪĿ¼�ļ�
            if (isChildDir(lpFindData)) {
                printf("child directory:%s begin copy\n", lpFindData.cFileName);
                CreateDirectory(new_path, NULL);
                mycp(old_path, new_path);
                //�޸���Ŀ¼ʱ������
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
