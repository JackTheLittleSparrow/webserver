#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>

namespace sylar{

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();
private:
	const char* m_file = nullptr; //文件名
	int32_t m_line = 0;				//行号
	int32_t m_elapse = 0;			//程序启动到现在的毫秒数
	int32_t m_threadId = 0;			//线程id
	int32_t m_fiberId = 0;			//协程id
	uint64_t m_time;				//时间戳
	std::string m_content;
};

//日志级别
class LogLevel{
public:
	enum Level{
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

};

//日志格式器
class LogFormater{
public:
	typedef std::shared_ptr<LogFormater> ptr;

	std::string formate(LogEvent::ptr event);
private:
};

//日志输出地
class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}

	void log{LogLevel::Level level, LogEvent event);
privite:
	LogLevel::Level m_level;
};

//日志器
class Logger{
public:
	typedef std::shared_prt<Logger> ptr;

	Logger(const std::string name = "root");
	void log(LogLevel::Level level, const LogEvent& event);
private:
	std::string m_name;
	LogLevel::Level m_level;
	LogAppender::ptr;
};


//定义输出到控制台的appender
class StdoutLogAppender:public LogAppender{

};

//定义输出到文件的appender
class FileAppender:public LogAppender{

};

}

#endif
