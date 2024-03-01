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
	printf("\033[31mϵͳ�洢����Ϣ:\033[m\n\n");
	printf("\033[33m�ڴ�ռ��\033[m\t\t:\033[35m%ld%%\033[m\n",statex.dwMemoryLoad);
	printf("\033[33m�ڴ�������\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalPhys/DIV_GB);
	printf("\033[33m�����ڴ�\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailPhys/DIV_GB);
	printf("\033[33m��ҳ�ļ�\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalPageFile/DIV_GB);
	printf("\033[33m����ҳ�ļ�\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailPageFile/DIV_GB);
	printf("\033[33m�������ڴ�\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullTotalVirtual/DIV_GB);
	printf("\033[33m���������ڴ�\033[m\t\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailVirtual/DIV_GB);
	printf("\033[33m������չ�����ڴ�\033[m\t:\033[35m%.2fGB\033[m\n",(float)statex.ullAvailExtendedVirtual/DIV_GB);
	printf("\n\n");
}

void systemInfo() {
	SYSTEM_INFO si;
	ZeroMemory(&si,sizeof(si));
	GetSystemInfo(&si);
	printf("\033[31mϵͳ��Ϣ:\033[m\n\n");
	printf("\033[34mҳ���С��ҳ�汣�����ύ������\033[m\t\t\t\t\t:\033[35m%dKB\033[m\n",(int)si.dwPageSize/DIV_KB);
	printf("\033[34mָ��Ӧ�ó���Ͷ�̬���ӿ�(dll)�ɷ��ʵ�����ڴ��ַ��ָ��\033[m\t\t:\033[35m0x%.8x\033[m\n",si.lpMinimumApplicationAddress);
	printf("\033[34mָ��Ӧ�ó����dll�ɷ��ʵ�����ڴ��ַ��ָ��\033[m\t\t\t:\033[35m0x%x\033[m\n",si.lpMaximumApplicationAddress);
	printf("\033[34m��ǰ���е��߼�����������\033[m\t\t\t\t\t:\033[35m%d\033[m\n",si.dwNumberOfProcessors);
	printf("\033[34m�ɷ��������ڴ����ʼ��ַ������\033[m\t\t\t\t\t:\033[35m%dKB\033[m\n",si.dwAllocationGranularity/DIV_KB);
	printf("\n\n");
}

void performanceInfo() {
	PERFORMANCE_INFORMATION pi;
	pi.cb=sizeof(pi);
	GetPerformanceInfo(&pi, sizeof(pi));
	printf("\033[31m������Ϣ:\033[m\n\n");
	printf("\033[36mϵͳ��ǰ�ύ��ҳ����\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.CommitTotal);
	printf("\033[36m�ڲ���չ��ҳ�ļ�������£�ϵͳ�����ύ�ĵ�ǰ���ҳ����\033[m\t:\033[35m%d\033[m\n",pi.CommitLimit);
	printf("\033[36m���ϴ�ϵͳ��������������ͬʱ�����ύ״̬��ҳ����������\033[m:\033[35m%d\033[m\n",pi.CommitPeak);
	printf("\033[36mҳ����ʵ�������ڴ�\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.PhysicalTotal);
	printf("\033[36m��ǰ���õ������ڴ�\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.PhysicalAvailable);
	printf("\033[36mϵͳ�����ڴ�\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.SystemCache);
	printf("\033[36m��ҳ�ں˳غͷǷ�ҳ�ں˳��е�ǰ�ڴ���ܺ�\033[m\t\t:\033[35m%d\033[m\n",pi.KernelTotal);
	printf("\033[36m��ǰ�ڷǷ�ҳ�ں˳��е��ڴ�\033[m\t\t\t\t:\033[35m%d\033[m\n",pi.KernelNonpaged);
	printf("\033[36mҳ�Ĵ�С\033[m\t\t\t\t\t\t:\033[35m%dKB\033[m\n",pi.PageSize/DIV_KB);
	printf("\033[36m��ǰ�򿪵ľ����\033[m\t\t\t\t\t:\033[35m%d\033[m\n",pi.HandleCount);
	printf("\033[36m��ǰ���̵�����\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.ProcessCount);
	printf("\033[36m��ǰ�߳���\033[m\t\t\t\t\t\t:\033[35m%d\033[m\n",pi.ThreadCount);
	printf("\n\n");
}

void processInfo() {
	PROCESSENTRY32 pe;
	pe.dwSize=sizeof(pe);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bMore = ::Process32First(hProcessSnap, &pe);
	printf("\033[31m���еĽ�����Ϣ:\033[m\n\n");
	printf("\033[44mPID\033[m\t | \033[44mִ���ļ�\033[m\t\t\t\t\t | \033[44m��������С(KB)\033[m\n");
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
	printf("\033[31m������Ҫ������PID\033[m\n");
	SYSTEM_INFO si;
	ZeroMemory(&si,sizeof(si));
	GetSystemInfo(&si);
	HANDLE hProcess;
	while(TRUE) {
		printf("PID:\t");
		scanf("%d",&pid);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess==NULL) {
			printf("�������PID��Ч�������¼�鲢����\n");
		} else {
			break;
		}
	}
	MEMORY_BASIC_INFORMATION mbi;
	ZeroMemory(&mbi,sizeof(mbi));
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
	printf("\033[44m��ַ\033[m\t\t\t| \033[44m��С\033[m\t\t| \033[44m״̬\033[m\t\t| \033[44m����\033[m\t\t\t\t| \033[44m����\033[m\t\t\t| \033[44mģ��\033[m\n\n");
	while(pBlock < si.lpMaximumApplicationAddress) {
		if (VirtualQueryEx(hProcess,pBlock,&mbi,sizeof(mbi))==sizeof(mbi)) {
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
			printf("%.8x-%.8x\t",pBlock,pEnd);
			printf("| %s\t",szSize);
			switch (mbi.State) {
				case MEM_COMMIT:
					printf("| �ύ\t\t");
					break;
				case MEM_FREE:
					printf("| �ͷ�\t\t");
					break;
				case MEM_RESERVE:
					printf("| ����\t\t");
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
					printf("\t\t| ӳ��\t");
					break;
				case MEM_MAPPED:
					printf("\t\t| ӳ��\t");
					break;
				case MEM_PRIVATE:
					printf("\t\t| ˽��\t");
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
		printf("\033[31m����Ҫ��ѯʲô��\033[m\n");
		printf("\033[32m�ڴ���Ϣ\033[m\t\t �� \033[45m1\033[m\n\033[32mϵͳ��Ϣ\033[m\t\t �� \033[45m2\033[m\n\033[32m������Ϣ\033[m\t\t �� \033[45m3\033[m\n\033[32m������Ϣ\033[m\t\t �� \033[45m4\033[m\n\033[32m���������ڴ���Ϣ\033[m\t �� \033[45m5\033[m\n\033[32m�˳�\033[m\t\t\t �� \033[45m0\033[m\n");
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


