/** @Date    : 2018-06-12 17:50:50
  * @FileName: config.h
  * @Platform: Linux
  * @Author  : Lweleth   (SoungEarlf@gmail.com)
  * @Link    : github.com/lweleth
  * @Version : $Id$
  */

#ifndef __CONFIG_H__

#define __CONFIG_H__

#include <bits/stdc++.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../MyCJson/myCJson.h"

class config{
private:
	uint16_t 	port;
	int			timeout;
	uint32_t	worker;
	char 	   *root_dir;
	char       *host;
public:
	char       *text;
	char 	   *file_path;
	int		   *file_fd;
	int        *rootdir_fd;
	char 	   *start_cmd;
	config();
	~config(){}
	void usage(const char *exec);
	void init();
	int parse(int argc, char *argv[]);
	int load();
};

typedef config config_t;

config::config()
{
	init();
}

void config::init()
{
	port = 65526;
	timeout = 3000;
	worker = 4;
	text = NULL;
	file_path = "./config.json";
	file_fd = 0;
	start_cmd = "start";
}


void config::usage(const char *exec)
{
	printf("Usage: %s \n"
		"[-c] /path/to/db.conf\n"
		"[-s] server command of option [start|stop|restart]\n"
		"-h show help command\n", exec);
}

int config::parse(int argc, char *argv[])
{
	char ch;
	std::string commd;
	while( ~(ch = getopt(argc, argv, "c:s:h")) )
	{
		switch(ch)
		{
			case 'c':
				this->file_path = optarg;
				break;
			case 's':
				commd = optarg;
				if(commd != "start" && commd != "restart" && commd != "stop")
				{
					fprintf(stderr, "Error Argument:[%s]\n", optarg);
					this->usage(argv[0]);
					exit(1);
				}
				else start_cmd = optarg;
				break;
			case 'h':
				this->usage(argv[0]);
				exit(1);
				break;
			default:
				fprintf(stderr, "Error Argument:[%s]\n", optarg);
				this->usage(argv[0]);
				exit(1);
				break;
		}
	}
	//debug
	std::cout << "conf path:" << file_path << std::endl;
	std::cout << "start cmd:" << start_cmd << std::endl;
	return 0;
}

#define CONF_ERROR(msg, p) fprintf(stderr, "ERROR: " msg " %s\n", (p));

int config::load()
{
	if(text != NULL)
		delete(text);
	//load file
	struct stat st;
	if( stat(file_path, &st) == -1)
	{
		CONF_ERROR("can't find file", file_path);
        return -1;
	}
	int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        CONF_ERROR("can't open file", file_path);
        return -1;
    }
    char *tmp = new char[st.st_size + 1](); // this is not going to be freed
    if (st.st_size != read(fd, tmp, st.st_size))
    {
        CONF_ERROR("can't read file", file_path);
        close(fd);
        return -1;
    }
    close(fd);
    tmp[st.st_size] = '\0';
    //const 
    this->text = tmp;
   	myJson *json = const_cast<myJson*>(json_parse(tmp));
   	if(json == NULL)
   	{
        CONF_ERROR("can't parse json", tmp);
        return -1;   		
   	}
   	//root dir
   	const myJson *rootdir = json_get_object(json, "root");
    if(rootdir != NULL && rootdir->type == JSON_STRING)
        this->root_dir = rootdir->value_text;
    printf("ROOT:%s\n", this->root_dir);

    const myJson *host = json_get_object(json, "host");
    if(host != NULL && host->type == JSON_STRING)
        this->host = host->value_text;
    printf("HOST:%s\n", this->host);

    const myJson *port = json_get_object(json, "port");
    if(port != NULL && port->type == JSON_INTEGER)
        this->port = port->value_int;
    printf("PORT: %d\n", this->port);

    const myJson *timeout = json_get_object(json, "timeout");
    if(timeout != NULL && timeout->type == JSON_INTEGER)
        this->timeout = timeout->value_int;
    printf("TIMEOUT: %d\n", this->timeout);

    const myJson *worker = json_get_object(json, "worker");
    if(worker != NULL && worker->type == JSON_INTEGER)
        this->worker = worker->value_int;
    printf("WORKER: %d\n", this->worker);


    json_free(json);
   	return 0;
}


#endif