#pragma once
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

class Task;

class Task
{
protected:
	const char *task_class=NULL;
	const char *name=NULL;
	QueueHandle_t eventQueue=NULL;
	QueueHandle_t commandQueue=NULL;
	TaskHandle_t taskHandle=NULL;
	configSTACK_DEPTH_TYPE stackDepth;
	UBaseType_t priority;
	UBaseType_t eventSize=0;

	bool awaitEvent(void *event, TickType_t timeout=10);
	void sendEvent(void *event);
public:

	Task(const char *task_class, const char *name, QueueHandle_t eventQueue=NULL, UBaseType_t eventSize=0,configSTACK_DEPTH_TYPE stackDepth=256, UBaseType_t priority=3);
	virtual void loop() = 0;

	const char *getClass() { return task_class?task_class:"undefined";}
	const char *getName() { return name?name:"undefined";}
	virtual void setName(const char *fmt, ...);
	void start();
	void setEventQueue(QueueHandle_t q) {eventQueue=q;}
	void setCommandQueue(QueueHandle_t q) {commandQueue=q;}

	void sendCommand(void *command);

	static int taskCount();

};

extern void listTasks(const char *msg);
