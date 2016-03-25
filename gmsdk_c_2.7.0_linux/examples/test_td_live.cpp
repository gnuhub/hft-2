#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <iostream>

#include "tdapi.h"

using namespace std;


/**
* 处理委托回报事件
*/
void on_execution(ExecRpt *res)
{
	cout << "execrpt: " << res->strategy_id << "  " << res->volume <<endl;
}

/**
* 委托回报函数类型(回调接口: 订单接受)
*/
void on_order_new(Order *res)
{
    cout << "order_new: " << res->strategy_id << "  " << res->price <<endl;
}

/**
* 委托回报函数类型(回调接口：订单完全成交) 
*/
void on_order_filled(Order *res)
{
    cout << "order_filled: " << res->strategy_id << "  " << res->price <<endl;
}

/**
* 委托回报函数类型(回调接口：订单部分成交)
*/
void on_order_partiall_filled(Order *res)
{
    cout << "order_partiall_filled: " << res->strategy_id << "  " << res->price <<endl;
}

/**
* 委托回报函数类型(回调接口：订单停止)
*/
void on_order_stop_executed(Order *res)
{
    cout << "order_stop_executed: " << res->strategy_id << "  " << res->price <<endl;
}


/**
* 委托回报函数类型(回调接口：订单撤销成功)
*/
void on_order_canceled(Order *res)
{
    cout << "order_canceled: " << res->strategy_id << "  " << res->price <<endl;
}

/**
* 委托回报函数类型(回调接口：撤单失败)
*/
void on_order_cancel_rejected(ExecRpt *res)
{
    cout << "order_cancel_rejected: " << res->strategy_id << "  " << res->price <<endl;
}

/**
* 登录交易服务器成功, 测试下单等
*/
void on_td_login()
{
    Order o;

    //开多，市价单（price==0)
    int ret = gm_td_open_long("CFFEX", "IF1509", 0, 1, &o);

    //平多，市价单（price==0)
    ret=  gm_td_close_long("CFFEX", "IF1509", 0, 1, &o);
    //开空，市价单（price==0)
    ret = gm_td_open_short("CFFEX", "IF1509", 0, 1, &o);
    //平空，市价单（price==0)
    ret = gm_td_close_short("CFFEX", "IF1509", 0, 1, &o);
    
    //撤单
    gm_td_cancel_order(o.cl_ord_id);

    //查单
    Order* order_result = NULL;
    gm_td_get_order(o.cl_ord_id, &order_result);
    if (order_result)
	{
        printf("query order result: %s\n", order_result->cl_ord_id);
	}
	else
	{
        printf("query order: no record.\n");
	}


    //查资金
    Cash* cash = NULL;
    ret = gm_td_get_cash(&cash);
    if (cash)
	{
        printf("query cash result: %f\n", cash->nav);
	}
	else
	{
        printf("query cash: no record.\n");
	}
    

    //查持仓
    Position* po = NULL;
    gm_td_get_position("CFFEX","IF1412",OrderSide_Bid, &po);
    if (po)
	{
        printf("query position result: %f\n", po->amount);
	}
	else
	{
        printf("query position: no record.\n");
	}

    //查全部持仓
    Position *all_po;
	int count;
    gm_td_get_positions(&all_po, &count);
    printf("positions: %d\n", count);

	//查绩效指标
	Indicator *indicator;
	gm_td_get_indicator(&indicator);

	if (indicator)
	{
		printf("query indicator result: %f\n", indicator->nav);
	}
	else
	{
		printf("query indicator: no record.\n");
	}
}

void on_error(int code, const char* msg)
{
	printf("error c=%d,msg=%s\n", code, msg);
}

int main(int argc, char *argv[])
{
	int ret;

    // 设置事件回调函数
    gm_td_set_login_callback(on_td_login);
	gm_td_set_execrpt_callback(on_execution);
	gm_td_set_order_cancelled_callback(on_order_canceled);
	gm_td_set_order_cancel_rejected_callback(on_order_cancel_rejected);
	gm_td_set_order_filled_callback(on_order_filled);
	gm_td_set_order_new_callback(on_order_new);
	gm_td_set_order_partially_filled_callback(on_order_partiall_filled);
	gm_td_set_order_stop_executed_callback(on_order_stop_executed);
	gm_td_set_error_callback(on_error);
		
	ret = gm_login("demo@myquant.cn", "123456");
	if (ret != 0)
	{
		printf("login fail");
		return ret;
	}

	//连接到挖金子网站
	ret = gm_td_init("strategy_1");
	
    //连接到掘金终端
	//ret = gm_td_init("strategy_2", "localhost:8001");


    //初始化失败，退出。
	if (ret)
    {
        printf("gm_td_init return: %d(%s)\n", ret, gm_strerror(ret)); 
		system("pause");
        return ret;
	}
	
	// waiting...
    gm_run();
	return 0;
}
