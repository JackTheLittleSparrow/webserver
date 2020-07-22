#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <stringstream>
#include <fstream>

namespace sylar{

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();

    const char* getFile() const { return m_file;}
    int32_t getLine() const { return m_line;}
    int32_t getElapse() const { return m_elapse;}
    int32_t getThreadId() const { return m_threadId;}
    int32_t getFiberId() const { return m_fiberId;}
    uint64_t getTime() const { return m_time;}
    const std::string& getContent() const { return m_content;}
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
        UKNOWN = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};
    static const char* ToString(LogLevel::Level level);
};

//日志格式器
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    //%t  %thread_id %m%n
	std::string format(LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;

};

//日志输出地
class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}

	void log(LogLevel::Level level, LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr val) { m_formatter = val;}
    LogFormatter::ptr getFormatter() const { return m_formatter;}
private:
	LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};

//日志器
class Logger{
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name = "root");
	void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const { return m_level;}
    void setLevel(LogLevel::Level val){ m_level = val;}
private:
	std::string m_name;         
	LogLevel::Level m_level;
	std::list<LogAppender::ptr> m_appenders;           //APPENDER集合
};

//定义输出到控制台的appender
class StdoutLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;
};

//定义输出到文件的appender
class FileAppender:public LogAppender{
public:
    typedef std::shared_ptr<FileAppender> ptr;
    FileAppender(const std:string& filename);
    void log(LogLevel::Level level, LogEvent::ptr event) override;

    bool reopen();
private:
    std::string  m_name;
    std::ofstream m_filename;
};

}

#endif
