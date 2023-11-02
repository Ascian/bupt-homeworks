#pragma once

#pragma comment(lib, "pthreadVC2.lib")
#include <pthread.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//日志对象，同时输出到标准输出和文件
class Log
{
public:
	explicit Log(const string& log_path)
	{
		log_file_.open(log_path, ios::out);
		pthread_mutex_init(&output_mutex_, nullptr);
	}

	~Log()
	{
		log_file_.close();
		pthread_mutex_destroy(&output_mutex_);
	}

	inline Log& operator<<(const string& output)
	{
		pthread_mutex_lock(&output_mutex_);
		cout << output;
		log_file_ << output;
		pthread_mutex_unlock(&output_mutex_);
		return *this;
	}

	inline Log& operator<<(const int& output)
	{
		pthread_mutex_lock(&output_mutex_);
		cout << output;
		log_file_ << output;
		pthread_mutex_unlock(&output_mutex_);
		return *this;
	}

private:
	ofstream log_file_;
	pthread_mutex_t  output_mutex_;
};
