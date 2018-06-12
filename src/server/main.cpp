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

class APP
{
public:
	config_t conf;
	void start(int argc, char *argv[]);
};

void APP::start(int argc, char *argv[])
{
	int stat;
	stat = conf.parse(argc, argv);
	stat = conf.load();
	printf("stat: %d\n", stat);
	return ;
}


int main(int argc, char *argv[])
{
	APP app;
	app.start(argc, argv);
    return 0;
}