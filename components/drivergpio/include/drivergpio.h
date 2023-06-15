/*
    Elaborado por:
        Sebastian De la Cruz Delgado
        Julian Emilio Guttierrez Loya
    Materia:
        Arquitectura de programacion de Hardware
    Universidad:
        ITCH
    Docente:
        Ing. Alfredo Chacon Aldama
    Fecha: 06 / 2023

    Capa: Driver

     * Todos los derechos de autor reservados.
     * El mal uso de estos Drivers no es responsabilidad de los desarrolladores.
*/
#pragma once

#include <stdbool.h>
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "soc/soc_caps.h"
#include "hal/gpio_types.h"
#include "esp_rom_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_PIN_COUNT                      (SOC_GPIO_PIN_COUNT)
/// Comprobar si es un n?mero GPIO v?lido
#define GPIO_IS_VALID_GPIO(gpio_num)        ((gpio_num >= 0) && \
                                              (((1ULL << (gpio_num)) & SOC_GPIO_VALID_GPIO_MASK) != 0))
/// Compruebe si puede ser un n?mero GPIO v?lido del modo de salida
#define GPIO_IS_VALID_OUTPUT_GPIO(gpio_num) ((gpio_num >= 0) && \
                                              (((1ULL << (gpio_num)) & SOC_GPIO_VALID_OUTPUT_GPIO_MASK) != 0))
/// Compruebe si puede ser un pad de E/S digital v?lido
#define GPIO_IS_VALID_DIGITAL_IO_PAD(gpio_num) ((gpio_num >= 0) && \
                                                 (((1ULL << (gpio_num)) & SOC_GPIO_VALID_DIGITAL_IO_PAD_MASK) != 0))

typedef intr_handle_t gpio_isr_handle_t;

/**
 * @brief Manejador de interrupciones GPIO
 *
 * @param arg Datos registrados del usuario
 */
typedef void (*gpio_isr_t)(void *arg);

esp_err_t gpio_pin_output(gpio_num_t gpio_num);

esp_err_t gpio_pin_input(gpio_num_t gpio_num);

esp_err_t gpio_lectura(gpio_num_t gpio_num);

esp_err_t gpio_E_O_digital(gpio_num_t gpio_num, uint32_t level);
#ifdef __cplusplus
}
#endif





void func(void);
