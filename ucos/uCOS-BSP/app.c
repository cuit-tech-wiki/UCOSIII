#include "application.h"

#define START_TASK_PRIO		3
#define START_STK_SIZE		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];


#define LED0_TASK_PRIO		4
#define LED0_STK_SIZE		128
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];


#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE		128
OS_TCB Led1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];

void start_task(void *p_arg);
void led0_task(void *p_arg);
void led1_task(void *p_arg);


void MX_UCOS_Init(void)
{
	OS_ERR err;
	
	CPU_SR_ALLOC();
	OSInit(&err);				//初始化 UCOSIII
	OS_CRITICAL_ENTER();		//进入临界段代码

	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,				            //指向任务的任务控制块
				 (CPU_CHAR* )"start task", 				                //指定任务的名字
				 (OS_TASK_PTR)start_task, 			                    //执行任务函数，函数名
				 (void		* )0,										//函数传递参数
				 (OS_PRIO	  )START_TASK_PRIO,                         //任务优先级
				 (CPU_STK * )&START_TASK_STK[0],	                    //指向任务栈基地址
				 (CPU_STK_SIZE)START_STK_SIZE/10,	                    //任务栈的堆栈深度
				 (CPU_STK_SIZE)START_STK_SIZE,		                    //任务堆栈大小
				 (OS_MSG_QTY)0,						                    //最大接受消息数量
				 (OS_TICK	  )0,						                //时间片
				 (void   	* )0,						                //TCB扩展内存
				 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,   //检查任务堆栈，清除任务堆栈
				 (OS_ERR 	* )&err);				                            
	OS_CRITICAL_EXIT();	       //退出临界段代码                                      

	OSStart(&err); //uC/OS-III启动任务，开始调度
	
}


void start_task(void *p_arg)
{
	OS_ERR err;
	(void)p_arg;

	CPU_SR_ALLOC();
	

	BSP_Init();                                                   /* Initialize BSP functions */
  //CPU_Init();
  //Mem_Init();                                                 /* Initialize Memory Management Module */

#if OS_CFG_STAT_TASK_EN > 0u	//如果使能统计任务
   OSStatTaskCPUUsageInit(&err);  		          
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN	//如果使能了中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN	//如果使能了时间片轮转
	 //5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,0,&err);  
#endif		
	
	OS_CRITICAL_ENTER();
	/* LED0 -task */
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	/* LED1 -task*/
	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
				 			 
//	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起任务		 
	OS_CRITICAL_EXIT();
	OSTaskDel((OS_TCB*)&StartTaskTCB, &err);
}


void led0_task(void *p_arg)
{
	OS_ERR err;
	(void)p_arg;
	
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
	}
}


void led1_task(void *p_arg)
{
	OS_ERR err;
	(void)p_arg;

	while(1)
	{
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);	
	}
}


