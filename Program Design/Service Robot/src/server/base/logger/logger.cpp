#include "logger.h"

#include <direct.h>
#include <io.h>
#include <windows.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

//一天毫秒数
const int kMilisecADay = 86400000;

void Logger::output(const string& type, const string& message)
{
	time_t t = time(nullptr);
	struct tm* now = localtime(&t);  //时间戳
	ostringstream mg;
	mg << setw(2) << setfill('0') << now->tm_year - 100 << "-"  //年份仅显示后两位
		<< setw(2) << setfill('0') << now->tm_mon + 1 << "-"
		<< setw(2) << setfill('0') << now->tm_mday << "-"
		<< setw(2) << setfill('0') << now->tm_hour << ":"
		<< setw(2) << setfill('0') << now->tm_min << ":"
		<< setw(2) << setfill('0') << now->tm_sec << " "
		<< "[" << type << "]" << message;

	mutx.lock();
	cout << mg.str() << endl;
	logFile << mg.str() << endl;
	mutx.unlock();
}

void getFiles(string path, vector<string>& files)
{
	//文件句柄
	intptr_t  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果不是目录,加入列表
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Logger::checkLogNum()
{
	vector <string> logFiles;
	getFiles(logDicPath, logFiles);
	if (logFiles.size() > logMaxNum)
		remove(logFiles[0].c_str());
}

void Logger::setup(const string& logDicPath, const int& logGenInterval, const int& logMaxNum)
{
	this->logDicPath = logDicPath;
	this->logGenInterval = logGenInterval;
	this->logMaxNum = logMaxNum;
	_mkdir(logDicPath.c_str());  //若日志目录不存在，新建日志目录

	//创建或打开日志文件，当前日期为文件名
	time_t t = time(nullptr);
	struct tm* now = localtime(&t);
	ostringstream date;
	date << setw(2) << setfill('0') << now->tm_year - 100  //年份仅显示后两位
		<< setw(2) << setfill('0') << now->tm_mon + 1
		<< setw(2) << setfill('0') << now->tm_mday;
	logFile.open(logDicPath + "\\" + date.str() + ".txt", ios::out | ios::app);
	logFile << endl;

	checkLogNum();  //检查日志文件数量是否达到上限
	thread th(&generateLog, this);  //多线程处理新建日志文件
	th.detach();
}

void generateLog(Logger* logger)
{
	while (true)
	{
		Sleep(logger->logGenInterval * kMilisecADay);
		logger->mutx.lock();
		logger->logFile.close();
		//新建日志文件，文件名为当前日期
		time_t t = time(nullptr);
		struct tm* now = localtime(&t);
		ostringstream date;
		date << setw(2) << setfill('0') << now->tm_year - 100  //年份仅显示后两位
			<< setw(2) << setfill('0') << now->tm_mon + 1
			<< setw(2) << setfill('0') << now->tm_mday;
		logger->logFile.open(logger->logDicPath + "\\" + date.str() + ".txt", ios::out | ios::app);
		logger->checkLogNum();  //检查日志文件数量是否达到上限
		logger->mutx.unlock();
	}
}