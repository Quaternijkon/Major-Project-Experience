#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include<utime.h>

#define READ_SIZE 1024
#define Path_LEN 1024

void CopyFile(char *OldPath, char *NewPath);

void CopyLinkFile(const char *LinkPath, char *NewPath);

void MyCopy(char *OldPath, char *NewPath);

int main(int arcg, char *argv[]) {
    if (arcg != 3) {
        printf("Arcg Error!\n");
        exit(-1);
    }

    ///定义参数
    struct stat statbuf{};
    struct utimbuf uTime;
    struct dirent *DR;
    DIR *dirptr = NULL;
    DIR *dirptr2 = NULL;

    //打开目录
    if ((dirptr = opendir(argv[1])) == NULL) {
        printf("open dir error!\n");
        exit(-1);
    }

    //创建目录,前提是为该名字的目录不存在
    if ((dirptr2 = opendir(argv[2])) == NULL) {
        //得到argv[1]指向的文件信息,存储时间信息
        stat(argv[1], &statbuf);
        uTime.actime = statbuf.st_atime;
        uTime.modtime = statbuf.st_mtime;
        //创建目录
        if (mkdir(argv[2], statbuf.st_mode) < 0) {
            printf("create dir error!\n");
            exit(-1);
        }
    } else {
        printf("Filename duplication!\n");
        exit(-1);
    }

    //复制目录中的内容
    MyCopy(argv[1], argv[2]);
    utime(argv[2], &uTime);//修改目录时间属性
    printf("copy finished!\n");
    closedir(dirptr);
    closedir(dirptr2);
    return 0;
}

void MyCopy(char *OldPath, char *NewPath) {
    struct stat statbuf{};
    struct stat copybuf{};
    struct utimbuf uTime;
    struct dirent *entry = NULL;
    DIR *dirptr = NULL;
    char old_path[128], new_path[128];

    strcpy(old_path, OldPath);
    strcpy(new_path, NewPath);
    dirptr = opendir(old_path);
    while ((entry = readdir(dirptr)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        strcpy(new_path, NewPath);
        strcpy(old_path, OldPath);
        strcat(old_path, "/");
        strcat(new_path, "/");
        strcat(old_path, entry->d_name);
        strcat(new_path, entry->d_name);
        lstat(old_path, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
            printf("Director: %s，正在拷贝...\n", old_path);
            stat(old_path, &copybuf);
            mkdir(new_path, copybuf.st_mode);
            MyCopy(old_path, new_path);
            uTime.actime = copybuf.st_atime;
            uTime.modtime = copybuf.st_mtime;
            utime(new_path, &uTime);
            printf("Directory: %s，拷贝完成\n", old_path);
        }

        else if (S_ISLNK(statbuf.st_mode)) {
            printf("LinkFileName: %s，正在拷贝...\n", old_path);
            CopyLinkFile(old_path, new_path);
        }

        else {
            printf("FileName: %s，正在拷贝...\n", old_path);
            CopyFile(old_path, new_path);
            printf("FileName: %s，拷贝完成\n", old_path);
        }
    }
    closedir(dirptr);
}

void CopyLinkFile(const char *LinkPath, char *NewPath) {
    struct stat statbuf{};
    struct timeval tv[2];
    char path_buf[Path_LEN];
    memset(path_buf, 0, sizeof(path_buf));
    lstat(LinkPath, &statbuf);
    readlink(LinkPath, path_buf, Path_LEN);
    if (symlink(path_buf, NewPath) == -1) {
        printf("create link error!\n");
        _exit(-1);
    }
    printf("软连接文件复制成功\n");
    chmod(NewPath, statbuf.st_mode);
    tv[0].tv_sec = statbuf.st_atime;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = statbuf.st_mtime;
    tv[1].tv_usec = 0;
    lutimes(NewPath, tv);
}

void CopyFile(char *OldPath, char *NewPath) {
    struct stat statbuf{};
    struct utimbuf uTime;
    int NewFd;
    int OldFd;
    int size = 0;
    char buffer[READ_SIZE];
    memset(buffer, 0, sizeof(buffer));

    stat(OldPath, &statbuf);
    NewFd = creat(NewPath, statbuf.st_mode);

    if ((OldFd = open(OldPath, O_RDONLY)) < 0) {
        printf("open file:%s error!\n", OldPath);
        exit(-1);
    }

    while ((size = read(OldFd, buffer, READ_SIZE)) > 0) {
        write(NewFd, &buffer, size);
    }

    uTime.actime = statbuf.st_atime;
    uTime.modtime = statbuf.st_mtime;
    utime(NewPath, &uTime);

    close(OldFd);
    close(NewFd);
}



