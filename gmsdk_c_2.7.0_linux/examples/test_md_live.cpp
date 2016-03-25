#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "mdapi.h"
#include <string.h>
#include <iomanip>
#include <unistd.h>

using namespace std;

static char strList[1000] = {0};


//处理逐笔行情事件
void on_tick(Tick *tick)
{	cout << endl;
	cout << tick->sec_id << endl;
    if(tick->ask_p5) {
	    cout << setprecision(4) << tick->ask_p5 << "\t" << tick->ask_v5 / 100 << endl;
    	cout << setprecision(4) << tick->ask_p4 << "\t" << tick->ask_v4 / 100 << endl;
	    cout << setprecision(4) << tick->ask_p3 << "\t" << tick->ask_v3 / 100 << endl;
	    cout << setprecision(4) << tick->ask_p2 << "\t" << tick->ask_v2 / 100 << endl;
	    cout << setprecision(4) << tick->ask_p1 << "\t" << tick->ask_v1 / 100 << endl;
    }

	cout << '\t' << setprecision(4) << tick->last_price << "\t" << (tick->last_price - tick->pre_close) / tick->pre_close * 100.0 << endl;
    
    if(tick->bid_p1) {
	    cout << setprecision(4) << tick->bid_p1 << "\t" << tick->bid_v1 / 100 << endl;
	    cout << setprecision(4) << tick->bid_p2 << "\t" << tick->bid_v2 / 100 << endl;
	    cout << setprecision(4) << tick->bid_p3 << "\t" << tick->bid_v3 / 100 << endl;
	    cout << setprecision(4) << tick->bid_p4 << "\t" << tick->bid_v4 / 100 << endl;
	    cout << setprecision(4) << tick->bid_p5 << "\t" << tick->bid_v5 / 100 << endl;
    }

    if (strList[0]) {
    	int ret = gm_md_subscribe(strList);
    	printf("gm_md_subscribe return %d\n %s", ret, strList);	
    	memset(strList, 0, sizeof(strList));
    }

}

//处理分时行情事件

void on_bar(Bar *bar)
{
	cout << "代码:" << bar->sec_id << " 时间:" << bar->strtime << " bar类型:" << bar->bar_type << endl;
}


void  on_error(int code, const char *msg)
{
	std::cout << "code:" << code << " msg:" << msg << std::endl;
}

void  on_login()
{
	std::cout << "登录成功" << std::endl;
}


void* GetInput(void*)
{
	if(!strList[0]){
		fgets(strList, sizeof(strList), stdin);
		for (int i = sizeof(strList) - 1; i > 0; --i)
		{
			if(strList[i] == '\n') {
				strList[i] = '\0';
				break;
			}
		}
	} else {
		getchar();
	}
	GetInput(NULL);
    return NULL;
}
 
int main(int argc, char *argv[])
{

	int ret;
    ret = gm_login("cdh321@sina.cn", "aA123456");
	if (ret != 0)
	{
		printf("login fail");
		return ret;
	}

    // 设置事件回调函数
	gm_md_set_tick_callback(on_tick);
	gm_md_set_bar_callback(on_bar);
	gm_md_set_error_callback(on_error);
	gm_md_set_login_callback(on_login);

	ret = gm_md_init(MD_MODE_LIVE, "SHSE.000001.tick,SZSE.399006.tick,SZSE.300098.tick,SZSE.300160.tick");//,SZSE.002072.tick");
    //初始化失败，退出。
	if (ret)
    {
        printf("gm_md_login return: %d\n", ret); 
        return ret;
	}

		
    // 创建一个线程， 来添加回调代码
	pthread_t ntid;
    int err = pthread_create(&ntid, NULL, GetInput, NULL);
    if (err != 0)
         printf("can't create thread");

    // waiting...
    gm_run();
	return 0;
}

