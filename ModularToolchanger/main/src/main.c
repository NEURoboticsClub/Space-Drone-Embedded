#include "freertos/FreeRTOS.h"
#include "state.h"

void app_main(void)
{
    printf("Starting Modular Toolchanger!\n");

    // Init all functions
    state_init();

    // transition to ready to begin car
    queue_state_transition(READY);
}
