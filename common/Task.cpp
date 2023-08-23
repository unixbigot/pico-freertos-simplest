#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "Task.h"
#include "log.h"

static std::vector<Task*> tasks;

extern "C" {

static void _task_wrapper(void *params) 
{
    Task *t = static_cast<Task *>(params);
    DEBUG("%s begin");
    
#ifdef BUILD_TYPE_DEBUG
    vTaskDelay(1000*portTICK_PERIOD_MS);
#endif
    NOTICE("%s/%s indahaus", t->getClass(), t->getName());
    while (1) {
        t->loop();
    }
}

}

Task::Task(const char *task_class,
           const char *name,
	   QueueHandle_t eventQueue,
           UBaseType_t eventSize,
	   configSTACK_DEPTH_TYPE stackDepth,
	   UBaseType_t priority)
    : task_class(task_class)
    , name(name)
    , eventQueue(eventQueue)
    , eventSize(eventSize)
    , stackDepth(stackDepth)
    , priority(priority)
{
    DEBUG("Task constructor %s/%s eventSize=%d pri=%d", task_class, name, eventSize,priority);
    tasks.push_back(this);
    DEBUG("Task count is now %d", tasks.size());
    
    start();
}

void Task::start() 
{
    DEBUG("start");
    if (xTaskCreate(_task_wrapper,
                    name, 
                    stackDepth,
                    this,
                    priority,
                    &taskHandle) != pdPASS) {
        ERROR("Could not start %s task", name);
    }
    DEBUG("Thread created");
}


void listTasks(const char *msg) 
{
    DEBUG("listTasks");
    NOTICE("%s: %d tasks", msg, Task::taskCount());
    int n=0;
    for (Task *t: tasks) {
        NOTICE("   task %d: %s", n, t->getName());
        ++n;
    }
}

static int _getTaskCount() 
{
    DEBUG("taskCount");
    int count = tasks.size();
    DEBUG("taskCount is %d", count);
    return count;
}

int Task::taskCount() 
{
    return _getTaskCount();
}

void Task::setName(const char *fmt, ...) 
{
    char name_buf[configMAX_TASK_NAME_LEN+1];
    va_list ap;
    va_start(ap, fmt);
    
    vsnprintf(name_buf, sizeof(name_buf), fmt, ap);
    name = strdup(name_buf);
}

void Task::sendEvent(void *event) 
{
    hexDump("sendEvent", event, eventSize);
    if (eventQueue) {
        xQueueSend(eventQueue, event, portMAX_DELAY);
        DEBUG("Sent event via FreeRTOS event queue");
    }
}

bool Task::awaitEvent(void *event_r, TickType_t timeout)
{
    bool result = false;
    if (eventQueue && event_r && (xQueueReceive(eventQueue, event_r, timeout)==pdPASS)) {
        result = true;
    }
    //if (result && event_r) hexDump("Received event", event_r, eventSize);
    return result;
}

void Task::sendCommand(void *command) 
{
    DEBUG("q=%p c=%p", commandQueue, command);
    if (commandQueue) {
        xQueueSend(commandQueue, command, portMAX_DELAY);
    }
}



// local Variables:
// mode: C++
// c-basic-offset: 4
// indent-tabs-mode: nil
// End:
