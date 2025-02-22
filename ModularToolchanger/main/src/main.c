#include "freertos/FreeRTOS.h"
#include "state.h"
#include "driver/gpio.h"
#include <esp_adc/adc_oneshot.h>
#include "motor_handler.h"

void app_main(void)
{
    printf("Starting Modular Toolchanger!\n");

    // Init all functions
    // ESP_ERROR_CHECK(state_init());
    // // transition to ready to begin car
    // ESP_ERROR_CHECK(queue_state_transition(READY));

    // IN 1
    mt_motor_t motor_handle_a;
    motor_init(GPIO_NUM_5, GPIO_NUM_6, ADC_UNIT_1, ADC_CHANNEL_3, &motor_handle_a);

    // IN 2
    mt_motor_t motor_handle_b;
    motor_init(GPIO_NUM_15, GPIO_NUM_16, ADC_UNIT_1, ADC_CHANNEL_6, &motor_handle_b);

    // IN 3
    mt_motor_t motor_handle_c;
    motor_init(GPIO_NUM_18, GPIO_NUM_8, ADC_UNIT_2, ADC_CHANNEL_6, &motor_handle_c);

    int i = 0;
    for (;;)
    {
        // printf("CALI ADC: %dmV\n", );
        // printf("AMPS: %f\n", );

        if (i >= 3)
        {
            i = 0;
            printf("ALIVE... FWD\n");
            ESP_ERROR_CHECK(motor_forward(&motor_handle_a));
            vTaskDelay(pdMS_TO_TICKS(2000));
            float current_out;
            ESP_ERROR_CHECK(measure_current(&motor_handle_a, &current_out));
            printf("CALI AMPS %f\n", current_out);
        }
        else if (i >= 2)
        {
            printf("HELLO... BACKWARD\n");
            ESP_ERROR_CHECK(motor_backward(&motor_handle_a));
            vTaskDelay(pdMS_TO_TICKS(2000));
            float current_out;
            ESP_ERROR_CHECK(measure_current(&motor_handle_a, &current_out));
            printf("CALI AMPS %f\n", current_out);
        }
        else
        {
            printf("HELLO... SETTLING\n");
            motor_forward(&motor_handle_c);
            ESP_ERROR_CHECK(motor_stop(&motor_handle_a));
            vTaskDelay(pdMS_TO_TICKS(2000));
            float current_out;
            ESP_ERROR_CHECK(measure_current(&motor_handle_a, &current_out));
            printf("CALI AMPS %f\n", current_out);
            // IN 1
        }
        i++;
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
