#ifndef __SYS_LOGGER_H__
#define __SYS_LOGGER_H__

#include <string>
#include "sys.h"

#define MAX_LOGGER_SZ	64

class TalkLogger {
public:
	TalkLogger() { m_sz = 0; };

	void load(void);
	void save(void);
	void insert(int type);
	void remove(int n);

	enum {
		IN = 1, //����
		OUT = 2, //����
		NA = 3, //δ��
	};

	struct {
		std::string id;
		int type; // 1:���� 2:���� 3:δ��
		int done; // 0:δ�� 1:�Ѷ�
		int format; //DNAKE��ʽ
		int timeout;
		time_t ts;
	} data[MAX_LOGGER_SZ];

public:
	int m_sz;
	int m_NA; //��δ������¼
};

class JpegLogger {
public:
	JpegLogger() { m_sz = 0; };

	void load(void);
	void save(void);
	void insert(time_t ts, const char *url);
	void remove(time_t ts);
	const char *url(time_t ts);

	struct {
		std::string url;
		time_t ts;
	} data[MAX_LOGGER_SZ];

public:
	int m_sz;
};

class MsgLogger {
public:
	MsgLogger() { m_sz = 0; m_have = 0; };

	void load(void);
	void save(void);
	void insert(const char *text, time_t ts);
	void remove(int n);
	void done(int n);

	struct {
		std::string text;
		int done; //0:δ�� 1:�Ѷ�
		time_t ts;
	} data[MAX_LOGGER_SZ];

public:
	int m_sz;
	int m_have;
};

class SysLogger {
public:
	SysLogger();
	~SysLogger();

	const char *dir(void);
	const char *jpeg_dir(void);
	void load(void);

public:
	TalkLogger talk;
	JpegLogger jpeg;
	MsgLogger msg;
};

extern SysLogger sLogger;

#endif
