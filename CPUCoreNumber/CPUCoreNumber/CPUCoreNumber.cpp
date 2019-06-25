// CPUCoreNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
using namespace std;

/****************************************************������������*******************************************************/
void GetCPUCoreNum();
int GetPhyCpuNum(int &num);


/****************************************************������������ں���**************************************************/
int main()
{
	//1. ��ȡCPU���߼�����
	//GetCPUCoreNum();

	//2. ��ȡCPU�������
	int num = 0;
	GetPhyCpuNum(num);
	cout << num << endl;
}


/***********************************************************************************************************************/
/*                                                   ��ͨ����                                                           */
/***********************************************************************************************************************/

//��ȡCPU���ܺ�����
void GetCPUCoreNum() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("now system cpu num is %d\n", sysInfo.dwNumberOfProcessors);
}

//��ȡCPU�������
int GetPhyCpuNum(int &num)
{
	// -------------------- �õ���ǰ·�� ---------------------
	char path[256] = { 0 };
	char exePath[256] = { 0 };
	GetModuleFileName(NULL, exePath, sizeof(exePath));
	printf("%s\r\n",exePath);

	int len = 0;
	len = strlen(exePath);


	int i;
	for (i = len - 1; i > 0; i--)
	{
		if (exePath[i] == '\\')
			break;
	}
	if (i == 0)
		return -1;
	else
	{
		strncpy_s(path, exePath, i + 1);
		strcat_s(path, "cpuinfo.txt");
	}

	// ------------------- ����wmic�����ѯ ����ŵ��ļ���-------------------
	//printf("%s\r\n",path);
	char cmd[512] = { 0 };
	sprintf_s(cmd, 512, "wmic cpu get > %s", path);//printf("%s\r\n",cmd);
	system(cmd);
	Sleep(3000);


	// ------------------- ���ļ����ݵ������� -------------------
	FILE *fp = NULL;
	int fileLen;


	fopen_s(&fp, path, "r");
	if (fp == NULL)
	{
		return -2;
	}
	fseek(fp, 0, SEEK_END); // ��λ���ļ�ĩ
	fileLen = ftell(fp); // �õ��ļ���С 
	if (fileLen == 0)
	{
		fclose(fp);
		return -3;
	}
	fseek(fp, 0L, SEEK_SET); // ��λ���ļ���ͷ
	char *buffer = new char[fileLen + 1];
	fread(buffer, fileLen, 1, fp); // һ���Զ�ȡȫ���ļ�����
	buffer[fileLen] = '\0'; // �ַ���������־
	fclose(fp);


	// ------------------- ��ѯ���� -----------------------------
	i = 0;
	int n = 0;
	while (i < fileLen)
	{
		if (buffer[i] == '\r')
			n++;
		i++;
	}


	num = n - 1; // ��һ����������Ҫ��ȥ
	delete buffer;
	return 0;
}

