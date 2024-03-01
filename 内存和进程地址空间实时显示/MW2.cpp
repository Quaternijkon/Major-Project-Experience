#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<cstdio>
#include<psapi.h>
#include"Psapi.h"
#include"tlhelp32.h"
#include"shlwapi.h"

#define DIV_GB (1024*1024*1024)
#define DIV_KB (1024)
using namespace std;

void memoryInfo() {
	MEMORYSTATUSEX statex;
	statex.dwLength=sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	printf("\033[31m系统存储器信息:\033[m\n\n");
	printf("\033[33m内存占用\033[m\t\t:\033[35m%ld%%\033[m\n",statex.dwMemoryLoad);
	printf("\033[33m内存总容量\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalPhys/DIV_GB);
	printf("\033[33m可用内存\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailPhys/DIV_GB);
	printf("\033[33m总页文件\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalPageFile/DIV_GB);
	printf("\033[33m可用页文件\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailPageFile/DIV_GB);
	printf("\033[33m总虚拟内存\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalVirtual/DIV_GB);
	printf("\033[33m可用虚拟内存\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailVirtual/DIV_GB);
	printf("\033[33m可用扩展虚拟内存\033[m\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailExtendedVirtual/DIV_GB);
	printf("\n\n");
}

void systemInfo() {
	SYSTEM_INFO si;
	ZeroMemory(&si,sizeof(si));
	GetSystemInfo(&si);
	printf("\033[31m系统信息:\033[m\n\n");
	printf("\033[34m页面大小和页面保护和提交的粒度\033[m\t\t\t\t\t:\033[35m%dKB\033[m\n",(int)si.dwPageSize/DIV_KB);
	printf("\033[34m指向应用程序和动态链接库(dll)可访问的最低内存地址的指针\033[m\t\t:\033[35m0x%.8x\033[m\n",si.lpMinimumApplicationAddress);
	printf("\033[34m指向应用程序和dll可访问的最高内存地址的指针\033[m\t\t\t:\033[35m0x%x\033[m\n",si.lpMaximumApplicationAddress);
	printf("\033[34m当前组中的逻辑处理器数量\033[m\t\t\t\t\t:\033[35m%d\033[m\n",si.dwNumberOfProcessors);
	printf("\033[34m可分配虚拟内存的起始地址的粒度\033[m\t\t\t\t\t:\033[35m%dKB\033[m\n",si.dwAllocationGranularity/DIV_KB);
	printf("\n\n");
}

void performanceInfo() {
	PERFORMANCE_INFORMATION pi;
	pi.cb=sizeof(pi);
	GetPerformanceInfo(&pi, sizeof(pi));
	printf("\033[31m性能信息:\033[m\n\n");
	printf("\033[36m系统当前提交的页面数\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.CommitTotal);
	printf("\033[36m在不扩展分页文件的情况下，系统可以提交的当前最大页面数\033[m\t:\033[35m%d\033[m\n",pi.CommitLimit);
	printf("\033[36m自上次系统重新启动以来，同时处于提交状态的页面的最大数量\033[m:\033[35m%d\033[m\n",pi.CommitPeak);
	printf("\033[36m页面中实际物理内存\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.PhysicalTotal);
	printf("\033[36m当前可用的物理内存\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.PhysicalAvailable);
	printf("\033[36m系统缓存内存\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.SystemCache);
	printf("\033[36m分页内核池和非分页内核池中当前内存的总和\033[m\t\t:\033[35m%d\033[m\n",pi.KernelTotal);
	printf("\033[36m当前在非分页内核池中的内存\033[m\t\t\t\t:\033[35m%d\033[m\n",pi.KernelNonpaged);
	printf("\033[36m页的大小\033[m\t\t\t\t\t\t:\033[35m%dKB\033[m\n",pi.PageSize/DIV_KB);
	printf("\033[36m当前打开的句柄数\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.HandleCount);
	printf("\033[36m当前进程的数量\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.ProcessCount);
	printf("\033[36m当前线程数\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.ThreadCount);
	printf("\n\n");
}

void processInfo() {
	PROCESSENTRY32 pe;
	pe.dwSize=sizeof(pe);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bMore = ::Process32First(hProcessSnap, &pe);
	printf("\033[31m所有的进程信息:\033[m\n\n");
	printf("\033[44mPID\033[m\t | \033[44m执行文件\033[m\t\t\t\t\t | \033[44m工作集大小(KB)\033[m\n");
	while(bMore) {
		HANDLE hP = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		PROCESS_MEMORY_COUNTERS pmc;
		ZeroMemory(&pmc, sizeof(pmc));
		if(GetProcessMemoryInfo(hP, &pmc, sizeof(pmc))) {
			int len=strlen(pe.szExeFile);
			printf("%d\t | %s",pe.th32ProcessID,pe.szExeFile);
			for(int i=0; i<=45-len; i++) {
				printf(" ");
			}
			printf("| %.2f\n",(float)pmc.WorkingSetSize/DIV_KB);
		}
		bMore = ::Process32Next(hProcessSnap, &pe);
	}
	CloseHandle(hProcessSnap);
}


inline bool TestSet(DWORD dwTarget,DWORD dwMask) {
	return ((dwTarget &dwMask)==dwMask);
}

#define SHOWMASK(dwTarget,type) if(TestSet(dwTarget,PAGE_##type)) {cout << "|" << #type;}

void ShowProtection(DWORD dwTarget) {
	SHOWMASK(dwTarget,READONLY);
	SHOWMASK(dwTarget,GUARD);
	SHOWMASK(dwTarget,NOCACHE);
	SHOWMASK(dwTarget,READWRITE);
	SHOWMASK(dwTarget,WRITECOPY);
	SHOWMASK(dwTarget,EXECUTE_READ);
	SHOWMASK(dwTarget,EXECUTE);
	SHOWMASK(dwTarget,EXECUTE_READWRITE);
	SHOWMASK(dwTarget,EXECUTE_WRITECOPY);
	SHOWMASK(dwTarget,NOACCESS);
}

void processVirtualMemoryInfo() {
	int pid;
	printf("\033[31m请输入要搜索的PID\033[m\n");
	SYSTEM_INFO si;
	ZeroMemory(&si,sizeof(si));
	GetSystemInfo(&si);
	HANDLE hProcess;
	while(TRUE) {
		printf("PID:\t");
		scanf("%d",&pid);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess==NULL) {
			printf("您输入的PID无效，请重新检查并输入\n");
		} else {
			break;
		}
	}
	MEMORY_BASIC_INFORMATION mbi;
	ZeroMemory(&mbi,sizeof(mbi));
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	printf("\033[44m地址\033[m\t\t\t| \033[44m大小\033[m\t\t| \033[44m状态\033[m\t\t| \033[44m保护\033[m\t\t\t\t| \033[44m类型\033[m\t\t\t| \033[44m模块\033[m\n\n");
	while(pBlock < si.lpMaximumApplicationAddress) {
		if (VirtualQueryEx(hProcess,pBlock,&mbi,sizeof(mbi))==sizeof(mbi)) {
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
			printf("%.8x-%.8x\t",pBlock,pEnd);
			printf("| %s\t",szSize);
			switch (mbi.State) {
				case MEM_COMMIT:
					printf("| 提交\t\t");
					break;
				case MEM_FREE:
					printf("| 释放\t\t");
					break;
				case MEM_RESERVE:
					printf("| 保留\t\t");
					break;
				default:
					break;
			}
			if (mbi.Protect == 0 && mbi.State != MEM_FREE) {
				mbi.Protect = PAGE_READONLY;
			}
			ShowProtection(mbi.Protect);
			printf("\t");
			switch (mbi.Type) {
				case MEM_IMAGE:
					printf("\t\t| 映像\t");
					break;
				case MEM_MAPPED:
					printf("\t\t| 映射\t");
					break;
				case MEM_PRIVATE:
					printf("\t\t| 私有\t");
					break;
				default:
					break;
			}
			TCHAR szFilename[MAX_PATH];
			if (GetModuleFileName((HINSTANCE)pBlock,szFilename,MAX_PATH)>0) {
				PathStripPath(szFilename);
				printf("\t| %s",szFilename);
			}
			printf("\n");
			pBlock=pEnd;
		}
	}
}

void clear() {
	system("cls");
}

int main(int argc, char const *argv[]) {
	while(TRUE) {
		printf("\033[31m您需要查询什么？\033[m\n");
		printf("\033[32m内存信息\033[m\t\t 按 \033[45m1\033[m\n\033[32m系统信息\033[m\t\t 按 \033[45m2\033[m\n\033[32m性能信息\033[m\t\t 按 \033[45m3\033[m\n\033[32m进程信息\033[m\t\t 按 \033[45m4\033[m\n\033[32m进程虚拟内存信息\033[m\t 按 \033[45m5\033[m\n\033[32m退出\033[m\t\t\t 按 \033[45m0\033[m\n");
		char c=getchar();
		if(c=='0') {
			break;
		} else if(c=='1') {
			getchar();
			memoryInfo();
		} else if(c=='2') {
			getchar();
			systemInfo();
		} else if(c=='3') {
			getchar();
			performanceInfo();
		} else if(c=='4') {
			getchar();
			processInfo();
		} else if(c=='5') {
			getchar();
			processVirtualMemoryInfo();
			getchar();
		}
	}
	return 0;
}


