#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"


typedef struct
{
    gpio_num_t fwd_pin;
    gpio_num_t back_pin;
    adc_oneshot_unit_handle_t adc_handle;
    adc_cali_handle_t adc_cali;
    adc_channel_t adc_channel;
} mt_motor_t;

/**
 * @brief Init a motor.
 *
 * @param fwd_pin The GPIO pin A
 * @param back_pin The GPIO pin B
 */
esp_err_t motor_init(gpio_num_t fwd_pin, gpio_num_t back_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, mt_motor_t *motor_handle);

esp_err_t motor_forward(mt_motor_t *motor_handle);
esp_err_t motor_backward(mt_motor_t *motor_handle);
esp_err_t motor_stop(mt_motor_t *motor_handle);
esp_err_t measure_current(mt_motor_t *motor_handle, float *current_out);
