#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

//日志类，输出调试信息到标准输出和日志文件
//example:
//    初始化日志对象
//    logger.setup(logDicPath, logGenInterval, logMaxNum);
//    输出错误信息
//    logger.error("error");
//    输出注意信息
//    logger.note("note");
//    关闭日志对象
//    logger.close();
class Logger
{
public:
	//初始化日志对象
	//parameter:
	//    const string& logDicPath - 日志文件目录地址
	//    const int& logGenInterval - 新建日志文件间隔, 单位为天
	//    const int& logMaxNum - 目录下最大日志文件数量
	void setup(const string& logDicPath, const int& logGenInterval, const int& logMaxNum);

	//关闭日志对象
	inline void close() { logFile.close(); }

	//输出错误信息
	//parameter:
	//    const string& message - 信息
	inline void error(const string& message) { output("error", message); }

	//输出注意信息
	//parameter:
	//    const string& message - 信息
	inline void note(const string& message) { output("note", message); }

private:
	//日志文件目录地址
	string logDicPath;
	//日志文件流
	ofstream logFile;
	//日志输出锁
	mutex mutx;
	//新建日志文件间隔, 单位为天
	int logGenInterval;
	//目录下最大日志文件数量
	int logMaxNum;

	//输出信息到标准输出和日志文件流
	//parameter:
	//    const string& type - 信息类型
	//    const string& message - 信息内容
	void output(const string& type, const string& message);

	//检查日志文件目录下文件数量是否超过最大值，若是，删除最早日志文件
	void checkLogNum();

	//新建日志文件
	friend void generateLog(Logger* logger);
};

void generateLog(Logger* logger);

#endif  //LOGGER_H_