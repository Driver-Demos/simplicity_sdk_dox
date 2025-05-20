#ifndef SL_PWM_H
#define SL_PWM_H

#include "em_device.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "sl_status.h"
#include "sl_enum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct sl_pwm_instance {
  TIMER_TypeDef *timer; /**< TIMER instance */
  uint8_t channel;      /**< TIMER channel */
  uint8_t port;         /**< GPIO port */
  uint8_t pin;          /**< GPIO pin */
  uint8_t location;     /**< GPIO location */
} sl_pwm_instance_t;

typedef struct sl_pwm_config {
  int frequency;              /**< PWM frequency */
  sl_pwm_polarity_t polarity; /**< PWM polarity */
} sl_pwm_config_t;

sl_status_t sl_pwm_init(sl_pwm_instance_t *pwm, sl_pwm_config_t *config);

sl_status_t sl_pwm_deinit(sl_pwm_instance_t *pwm);

void sl_pwm_start(sl_pwm_instance_t *pwm);

void sl_pwm_stop(sl_pwm_instance_t *pwm);

void sl_pwm_set_duty_cycle(sl_pwm_instance_t *pwm, uint8_t percent);

uint8_t sl_pwm_get_duty_cycle(sl_pwm_instance_t *pwm);

