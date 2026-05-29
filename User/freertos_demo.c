/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ALIENTEK
 * @version     V1.4
 * @date        2026-05-29
 * @brief       FreeRTOS 任务实践：时间片轮转调度实验
 * @license     Copyright (c) 2020-2032,  ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 淘宝地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
/* #include "list.h"   已被 task.h 间接包含，无需显式添加 */

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务配置
 * 优先级: 1 堆栈大小:128 任务句柄:start_task_handler
 */
#define START_TASK_PRIO			1 
#define START_TASK_STACK_SIZE 	128
TaskHandle_t	start_task_handler;
void start_task(void * pvParameters);

/* TASK1 任务配置
 * 优先级: 2 堆栈大小:128 任务句柄:task1_handler
 */
#define TASK1_PRIO				2 
#define TASK1_STACK_SIZE 		128
TaskHandle_t	task1_handler;
void task1(void * pvParameters);

/* TASK2 任务配置
 * 优先级: 3 堆栈大小:512 任务句柄:task2_handler
 */
#define TASK2_PRIO				2
#define TASK2_STACK_SIZE 		128
TaskHandle_t	task2_handler;
void task2(void * pvParameters);
/******************************************************************************************************/

/**
 * @brief       FreeRTOS 任务创建
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
	xTaskCreate ((TaskFunction_t		) start_task,
				(char *					) "start_task",
				(configSTACK_DEPTH_TYPE	) START_TASK_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) START_TASK_PRIO,
				(TaskHandle_t *			) &start_task_handler );
	vTaskStartScheduler();		
}

void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();		/* 进入临界区 */
	xTaskCreate ((TaskFunction_t		) task1,
				(char *					) "task1",
				(configSTACK_DEPTH_TYPE	) TASK1_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) TASK1_PRIO,
				(TaskHandle_t *			) &task1_handler );
				
	xTaskCreate ((TaskFunction_t		) task2,
				(char *					) "task2",
				(configSTACK_DEPTH_TYPE	) TASK2_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) TASK2_PRIO,
				(TaskHandle_t *			) &task2_handler );
				
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();		/* 退出临界区 */
}

/* 任务1: 时间片轮转调度演示 */
void task1(void * pvParameters)
{
	uint32_t task1_num = 0;
	while(1)
	{
		taskENTER_CRITICAL();		/* 进入临界区 */
		printf("task1运行次数：%d\r\n", ++task1_num);
		taskEXIT_CRITICAL();		/* 退出临界区 */
		delay_ms(10);
	}
}

/* 任务2:时间片轮转调度演示 */
void task2(void * pvParameters)
{	
	uint32_t task2_num = 0;
	while(1)
	{
		taskENTER_CRITICAL();		/* 进入临界区 */
		printf("task2运行次数：%d\r\n", ++task2_num);
		taskEXIT_CRITICAL();		/* 退出临界区 */
		delay_ms(10);
		
	}
}
