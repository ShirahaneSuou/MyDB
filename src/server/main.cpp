/** @Date    : 2018-06-12 17:26:22
  * @FileName: main.cpp
  * @Platform: Linux
  * @Author  : Lweleth   (SoungEarlf@gmail.com)
  * @Link    : github.com/lweleth
  * @Version : $Id$
  */


#include <bits/stdc++.h>
#include "config.h"
// #define LL long long
// #define PII pair<int ,int>
// #define MP(x, y) make_pair((x),(y))
// #define fi first
// #define se second
// #define PB(x) push_back((x))
// #define MMG(x) memset((x), -1,sizeof(x))
// #define MMF(x) memset((x),0,sizeof(x))
// #define MMI(x) memset((x), INF, sizeof(x))
using namespace std;

const int INF = 0x3f3f3f3f;
const int N = 1e5+20;
const double eps = 1e-8;



#define ERR_EXIT(con, str) { 								\
	if(con) {  												\
		fprintf(stderr, "%s line:[%d]\n", str, __LINE__);	\
		exit(1);											\
	}														\
}															

class APP
{
private:
	void init();
	void run();
	void kill_process();
	int read_pid();
	int write_pid();
	int remove_pid();
public:
	string pid_filename;
	config_t conf;
	void start(int argc, char *argv[]);
};

// pid file r/w
int APP::read_pid()
{
	FILE *fp = fopen(pid_filename.c_str(), "rb");
	if( fp == NULL )
		return -1;
	int stat = 0;
	int pid = 0;
	char buf[4096];
	while( !feof(fp) && !ferror(fp))
	{
		int len = fread(buf, 1, sizeof(buf), fp);
		for(int i = 0; i < len; i++)
			pid = pid * 10 + (buf[i] - '0');
	}
	fclose(fp);
	return pid;
}
int APP::write_pid()
{
	int pid = getpid();
	char buf[21];//10^20
	snprintf(buf, 20, "%d", pid);
	FILE *fp = fopen(pid_filename.c_str(), "wb");
	if( fp == NULL )
		return -1;
	int len = fwrite(buf, 1, strlen(buf), fp);
	fclose(fp);
	return len == strlen(buf) ? 0 : -1;
}

int APP::remove_pid()
{
	return remove(pid_filename.c_str());
}


void APP::kill_process()
{
	int pid = read_pid();
	if(pid == -1)
	{
		fprintf(stderr, "Cannot read pid file [pid:%s(%s)]!\n", pid_filename.c_str(),strerror(errno));
		exit(1);
	}
	if(kill(pid, 0) == -1 && errno == ESRCH)
	{
		fprintf(stderr, "server [pid:%d] not running\n", pid);
		remove_pid();
		exit(1);
	}
	int stat = kill(pid, SIGTERM);
	if(stat == -1)
	{
		fprintf(stderr, "Failed to kill process [pid:%d]%s\n", pid ,strerror(errno));
		exit(1);
	}
	remove_pid();
}

void APP::init()
{
	pid_filename = "db.pid";
	// check start-command
	if( strcmp(conf.start_cmd, "stop") == 0 )
	{
		kill_process();
		exit(0);
	}
	else if(strcmp(conf.start_cmd, "restart") == 0)
	{
		kill_process();
	}
	if(access(pid_filename.c_str(), F_OK) == 0)
	{
		fprintf(stderr, "process already running pid_file:%s \n", pid_filename.c_str());
		exit(1);
	}
	// daemon process
	return ;
}

void APP::run()
{
	// open database main
	// open netserver 
}

// main funciton
void APP::start(int argc, char *argv[])
{
	int stat;
	stat = conf.parse(argc, argv);
	ERR_EXIT(stat == -1, "parse argument faild");
	stat = conf.load();
	ERR_EXIT(stat == -1, "parse config file faild");
	init();
	if(write_pid() == -1)
	{
		fprintf(stderr, "write pid file [%s]failed\n", pid_filename.c_str());
		exit(1);
	}
	run();
	sleep(10);
	printf("%d", remove_pid());
	return ;
}





int main(int argc, char *argv[])
{
	APP app;
	app.start(argc, argv);
    return 0;
}