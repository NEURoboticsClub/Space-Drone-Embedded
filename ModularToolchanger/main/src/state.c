#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "state.h"

QueueHandle_t state_queue;
TaskHandle_t state_task;

static state_types_t current_state;

esp_err_t queue_state_transition(state_types_t state)
{
    return xQueueSendToFront(state_queue, (void *)state, 0);
}

state_types_t get_current_state()
{
    return current_state;
}

void vStateMachine(void *pv_params)
{
    state_types_t state;

    for (;;)
    {
        if (xQueueReceive(state_queue, &state, 0))
        {
            current_state = state;
            printf("Transitioning to ");
            switch (state)
            {
            case READY:
                printf("ready\n");
                break;
            case ATTACHING:
                printf("attaching\n");
                break;
            case ATTACHED:
                printf("attached\n");
                break;
            case DETACHING:
                printf("detached\n");
                break;
            case FAULTED:
                printf("faulted\n");
                break;
            default:
                break;
            }
        }
    }
}

esp_err_t state_init()
{
    // set a default state
    current_state = READY;

    // create queue for transitioning states
    state_queue = xQueueCreate(2, sizeof(state_types_t));

    // create task which processes changes in this queue
    xTaskCreate(vStateMachine, "StateMachine", 32 * 8, NULL, 10, &state_task);
    return ESP_OK;
}
