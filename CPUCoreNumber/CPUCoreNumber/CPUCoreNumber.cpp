// CPUCoreNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
using namespace std;

/****************************************************函数声明部分*******************************************************/
void GetCPUCoreNum();
int GetPhyCpuNum(int &num);


/****************************************************主函数，即入口函数**************************************************/
int main()
{
	//1. 获取CPU的逻辑核数
	//GetCPUCoreNum();

	//2. 获取CPU物理个数
	int num = 0;
	GetPhyCpuNum(num);
	cout << num << endl;
}


/***********************************************************************************************************************/
/*                                                   普通函数                                                           */
/***********************************************************************************************************************/

//获取CPU的总核心数
void GetCPUCoreNum() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("now system cpu num is %d\n", sysInfo.dwNumberOfProcessors);
}

//获取CPU物理个数
int GetPhyCpuNum(int &num)
{
	// -------------------- 得到当前路径 ---------------------
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

	// ------------------- 发送wmic命令查询 结果放到文件中-------------------
	//printf("%s\r\n",path);
	char cmd[512] = { 0 };
	sprintf_s(cmd, 512, "wmic cpu get > %s", path);//printf("%s\r\n",cmd);
	system(cmd);
	Sleep(3000);


	// ------------------- 读文件内容到数组中 -------------------
	FILE *fp = NULL;
	int fileLen;


	fopen_s(&fp, path, "r");
	if (fp == NULL)
	{
		return -2;
	}
	fseek(fp, 0, SEEK_END); // 定位到文件末
	fileLen = ftell(fp); // 得到文件大小 
	if (fileLen == 0)
	{
		fclose(fp);
		return -3;
	}
	fseek(fp, 0L, SEEK_SET); // 定位到文件开头
	char *buffer = new char[fileLen + 1];
	fread(buffer, fileLen, 1, fp); // 一次性读取全部文件内容
	buffer[fileLen] = '\0'; // 字符串结束标志
	fclose(fp);


	// ------------------- 查询行数 -----------------------------
	i = 0;
	int n = 0;
	while (i < fileLen)
	{
		if (buffer[i] == '\r')
			n++;
		i++;
	}


	num = n - 1; // 第一行是列名，要减去
	delete buffer;
	return 0;
}

