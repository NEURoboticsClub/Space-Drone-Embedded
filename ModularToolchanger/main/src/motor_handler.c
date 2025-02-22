#include "freertos/FreeRTOS.h"
#include <motor_handler.h>

esp_err_t motor_init(gpio_num_t fwd_pin, gpio_num_t back_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, mt_motor_t *motor_handle)
{
    esp_err_t err;

    err = gpio_set_direction(fwd_pin, GPIO_MODE_OUTPUT);
    if (err)
        return err;
    err = gpio_set_direction(back_pin, GPIO_MODE_OUTPUT);
    if (err)
        return err;

    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = adc_unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    err = adc_oneshot_new_unit(&init_config, &adc_handle);
    if (err)
        return err;

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_0,
    };
    err = adc_oneshot_config_channel(adc_handle, adc_channel, &config);
    if (err)
        return err;

    adc_cali_handle_t cali_handle;
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = adc_unit,
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    err = adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);
    if (err)
        return err;

    *motor_handle = (mt_motor_t){
        .fwd_pin = fwd_pin,
        .back_pin = back_pin,
        .adc_handle = adc_handle,
        .adc_cali = cali_handle,
        .adc_channel = adc_channel,
    };

    return motor_stop(motor_handle);
}

esp_err_t motor_forward(mt_motor_t *motor_handle)
{
    esp_err_t err;
    err = gpio_set_level(motor_handle->back_pin, 0);
    if (err)
        return err;

    return gpio_set_level(motor_handle->fwd_pin, 1);
}

esp_err_t motor_backward(mt_motor_t *motor_handle)
{
    esp_err_t err;
    err = gpio_set_level(motor_handle->fwd_pin, 0);
    if (err)
        return err;

    return gpio_set_level(motor_handle->back_pin, 1);
}

esp_err_t motor_stop(mt_motor_t *motor_handle)
{
    esp_err_t err;
    err = gpio_set_level(motor_handle->fwd_pin, 0);
    if (err)
        return err;

    return gpio_set_level(motor_handle->back_pin, 0);
}

esp_err_t measure_current(mt_motor_t *motor_handle, float *current_out)
{
    esp_err_t err;
    int out_cali;
    err = adc_oneshot_get_calibrated_result(motor_handle->adc_handle, motor_handle->adc_cali, motor_handle->adc_channel, &out_cali);
    if (err)
        return err;

    printf("ADC mV %d\n", out_cali);
    const float magic_number = 1.22;
    *current_out = magic_number * ((out_cali / 1000.0) / (1575 * 0.000001 * 3480));
    return ESP_OK;
}