#include <stdio.h>

#include <esp_types.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

#include "drivergpio.h"
//#include "driver/gpio.h"

//#include "driver/rtc_io.h"
#include "soc/soc.h"
#include "soc/periph_defs.h"
#if !CONFIG_FREERTOS_UNICORE
#include "esp_ipc.h"
#endif

#include "soc/soc_caps.h"
#include "soc/gpio_periph.h"
#include "esp_log.h"
#include "esp_check.h"

//#include "D:\Enero_Julio_2023\Arquitectura\Unidad_4\vsc_workspace\sample_project\DRIVERS\gpio_hal_driver.h"
#include "hal/gpio_hal.h"

#include "esp_rom_gpio.h"
//---------------------------------------------------------------------------------------------//
static const char *GPIO_TAG = "gpio";
#define GPIO_CHECK(a, str, ret_val) ESP_RETURN_ON_FALSE(a, ret_val, GPIO_TAG, "%s", str)

#define GPIO_ISR_CORE_ID_UNINIT    (3)

//default value for SOC_GPIO_SUPPORT_RTC_INDEPENDENT is 0
#ifndef SOC_GPIO_SUPPORT_RTC_INDEPENDENT
#define SOC_GPIO_SUPPORT_RTC_INDEPENDENT 0
#endif

typedef struct {
    gpio_isr_t fn;   /*!< isr function */
    void *args;      /*!< isr function args */
} gpio_isr_func_t;

// Used by the IPC call to register the interrupt service routine.
typedef struct {
    int source;               /*!< ISR source */
    int intr_alloc_flags;     /*!< ISR alloc flag */
    void (*fn)(void*);        /*!< ISR function */
    void *arg;                /*!< ISR function args*/
    void *handle;             /*!< ISR handle */
    esp_err_t ret;
} gpio_isr_alloc_t;

typedef struct {
    gpio_hal_context_t *gpio_hal;
    portMUX_TYPE gpio_spinlock;
    uint32_t isr_core_id;
    gpio_isr_func_t *gpio_isr_func;
    gpio_isr_handle_t gpio_isr_handle;
    uint64_t isr_clr_on_entry_mask; // for edge-triggered interrupts, interrupt status bits should be cleared before entering per-pin handlers
} gpio_context_t;

static gpio_hal_context_t _gpio_hal = {
    .dev = GPIO_HAL_GET_HW(GPIO_PORT_0)
};

static gpio_context_t gpio_context = {
    .gpio_hal = &_gpio_hal,
    .gpio_spinlock = portMUX_INITIALIZER_UNLOCKED,
    .isr_core_id = GPIO_ISR_CORE_ID_UNINIT,
    .gpio_isr_func = NULL,
    .isr_clr_on_entry_mask = 0,
};

esp_err_t gpio_pin_output(gpio_num_t gpio_num)
{
    GPIO_CHECK(GPIO_IS_VALID_OUTPUT_GPIO(gpio_num), "GPIO output gpio_num error", ESP_ERR_INVALID_ARG);
    gpio_hal_output_enable(gpio_context.gpio_hal, gpio_num);
    esp_rom_gpio_connect_out_signal(gpio_num, SIG_GPIO_OUT_IDX, false, false);
    return ESP_OK;
}

esp_err_t gpio_pin_input(gpio_num_t gpio_num)
{
    gpio_hal_input_enable(gpio_context.gpio_hal, gpio_num);
    esp_rom_gpio_connect_out_signal(gpio_num, SIG_GPIO_OUT_IDX, false, false);
    return ESP_OK;
}

int gpio_lectura(gpio_num_t gpio_num)
{
    return gpio_hal_get_level(gpio_context.gpio_hal, gpio_num);
}

esp_err_t gpio_E_O_digital(gpio_num_t gpio_num, uint32_t level)
{
    GPIO_CHECK(GPIO_IS_VALID_OUTPUT_GPIO(gpio_num), "GPIO output gpio_num error", ESP_ERR_INVALID_ARG);
    gpio_hal_set_level(gpio_context.gpio_hal, gpio_num, level);
    return ESP_OK;
}

void func(void)
{

}
