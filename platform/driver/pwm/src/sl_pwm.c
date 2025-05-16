
#include "sl_pwm.h"

#include "sl_gpio.h"

#if defined(_SILICON_LABS_32B_SERIES_3)
#include "sl_hal_timer.h"
#else
#include "em_timer.h"
#endif

#include "sl_clock_manager.h"
#include "sl_device_peripheral.h"


static void get_timer_clock(TIMER_TypeDef *timer, sl_bus_clock_t *timer_clock, sl_peripheral_t *peripheral)
{
  switch ((uint32_t)timer) {
#if defined(TIMER0_BASE)
    case TIMER0_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER0;
      *peripheral = SL_PERIPHERAL_TIMER0;
      break;
#endif
#if defined(TIMER1_BASE)
    case TIMER1_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER1;
      *peripheral = SL_PERIPHERAL_TIMER1;
      break;
#endif
#if defined(TIMER2_BASE)
    case TIMER2_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER2;
      *peripheral = SL_PERIPHERAL_TIMER2;
      break;
#endif
#if defined(TIMER3_BASE)
    case TIMER3_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER3;
/**
 * @brief The `get_timer_clock` function maps a given timer to its corresponding bus clock and peripheral identifiers.
 *
 * @param timer A pointer to a TIMER_TypeDef structure representing the timer whose clock and peripheral identifiers are to be determined.
 * @param timer_clock A pointer to an sl_bus_clock_t variable where the function will store the bus clock identifier corresponding to the given timer.
 * @param peripheral A pointer to an sl_peripheral_t variable where the function will store the peripheral identifier corresponding to the given timer.
 * @return The function does not return a value but modifies the values pointed to by `timer_clock` and `peripheral` to reflect the bus clock and peripheral identifiers for the specified timer.
 */
      *peripheral = SL_PERIPHERAL_TIMER3;
      break;
#endif
#if defined(TIMER4_BASE)
    case TIMER4_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER4;
      *peripheral = SL_PERIPHERAL_TIMER4;
      break;
#endif
#if defined(TIMER5_BASE)
    case TIMER5_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER5;
      *peripheral = SL_PERIPHERAL_TIMER5;
      break;
#endif
#if defined(TIMER6_BASE)
    case TIMER6_BASE:
      *timer_clock = SL_BUS_CLOCK_TIMER6;
      *peripheral = SL_PERIPHERAL_TIMER6;
      break;
#endif
    default:
      EFM_ASSERT(0);
      break;
  }
}

sl_status_t sl_pwm_init(sl_pwm_instance_t *pwm, sl_pwm_config_t *config)
{
  sl_bus_clock_t timer_clock;
  sl_peripheral_t peripheral;
  get_timer_clock(pwm->timer, &timer_clock, &peripheral);
  sl_clock_manager_enable_bus_clock(timer_clock);

  sl_clock_manager_enable_bus_clock(SL_BUS_CLOCK_GPIO);

  sl_gpio_t gpio;
  gpio.port = pwm->port;
  gpio.pin = pwm->pin;
  sl_gpio_set_pin_mode(&gpio,
                       SL_GPIO_MODE_PUSH_PULL,
                       config->polarity);

#if defined(_SILICON_LABS_32B_SERIES_2)
/**
 * @brief The `sl_pwm_init` function initializes a PWM instance with specified configuration settings, including setting up the timer, GPIO, and channel parameters for PWM operation.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure that represents the PWM instance to be initialized.
 * @param config A pointer to an `sl_pwm_config_t` structure that contains the configuration settings for the PWM, such as frequency and polarity.
 * @return Returns `SL_STATUS_OK` on successful initialization, or `SL_STATUS_FAIL` if an error occurs during the process.
 */
  TIMER_InitCC_TypeDef channel_init = TIMER_INITCC_DEFAULT;
  channel_init.mode = timerCCModePWM;
  channel_init.cmoa = timerOutputActionToggle;
  channel_init.edge = timerEdgeBoth;
  channel_init.outInvert = (config->polarity == PWM_ACTIVE_LOW);
  TIMER_InitCC(pwm->timer, pwm->channel, &channel_init);
#else
  sl_hal_timer_channel_config_t channel_init = SL_HAL_TIMER_CHANNEL_CONFIG_DEFAULT;
  channel_init.channel_mode = SL_HAL_TIMER_CHANNEL_MODE_PWM;
  channel_init.compare_match_output_action = SL_HAL_TIMER_CHANNEL_OUTPUT_ACTION_TOGGLE;
  channel_init.input_capture_edge = SL_HAL_TIMER_CHANNEL_EDGE_BOTH;
  channel_init.output_invert = (config->polarity == PWM_ACTIVE_LOW);
  sl_hal_timer_channel_init(pwm->timer, pwm->channel, &channel_init);
#endif
  volatile uint32_t * route_register;
#if defined(_GPIO_TIMER_ROUTEEN_MASK)
  route_register = &GPIO->TIMERROUTE[TIMER_NUM(pwm->timer)].CC0ROUTE;
  route_register += pwm->channel;
  *route_register = (pwm->port << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
                    | (pwm->pin << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);
#else
  switch (TIMER_NUM(pwm->timer)) {
    case 0:
      route_register = &GPIO->TIMER0ROUTE[0].CC0ROUTE;
      break;
    case 1:
      route_register = &GPIO->TIMER1ROUTE[0].CC0ROUTE;
      break;
    case 2:
      route_register = &GPIO->TIMER2ROUTE[0].CC0ROUTE;
      break;
    case 3:
      route_register = &GPIO->TIMER3ROUTE[0].CC0ROUTE;
      break;
    default:
      EFM_ASSERT(0);
      return SL_STATUS_FAIL;
  }
  route_register += pwm->channel;
  *route_register = (pwm->port << _GPIO_TIMER0_CC0ROUTE_PORT_SHIFT)
                    | (pwm->pin << _GPIO_TIMER0_CC0ROUTE_PIN_SHIFT);
#endif

  uint32_t clock_freq;
  sl_clock_branch_t clock_branch;

  clock_branch = sl_device_peripheral_get_clock_branch(peripheral);
  sl_clock_manager_get_clock_branch_frequency(clock_branch, &clock_freq);
  uint32_t top = (clock_freq / (config->frequency)) - 1U;
#if defined(_SILICON_LABS_32B_SERIES_2)
  TIMER_TopSet(pwm->timer, top);

  TIMER_CompareSet(pwm->timer, pwm->channel, 0U);

  TIMER_Init_TypeDef timer_init = TIMER_INIT_DEFAULT;
  TIMER_Init(pwm->timer, &timer_init);
#else
  sl_hal_timer_config_t timer_init = SL_HAL_TIMER_CONFIG_DEFAULT;
  sl_hal_timer_init(pwm->timer, &timer_init);
  sl_hal_timer_enable(pwm->timer);

  sl_hal_timer_set_top(pwm->timer, top);

  sl_hal_timer_channel_set_compare(pwm->timer, pwm->channel, 0U);

  sl_hal_timer_start(pwm->timer);
#endif

  return SL_STATUS_OK;
}

sl_status_t sl_pwm_deinit(sl_pwm_instance_t *pwm)
{
  sl_pwm_stop(pwm);

  volatile uint32_t * route_register;
#if defined(_GPIO_TIMER_ROUTEEN_MASK)
  route_register = &GPIO->TIMERROUTE[TIMER_NUM(pwm->timer)].CC0ROUTE;
#else
  switch (TIMER_NUM(pwm->timer)) {
    case 0:
      route_register = &GPIO->TIMER0ROUTE[0].CC0ROUTE;
      break;
    case 1:
      route_register = &GPIO->TIMER1ROUTE[0].CC0ROUTE;
      break;
    case 2:
      route_register = &GPIO->TIMER2ROUTE[0].CC0ROUTE;
      break;
    case 3:
      route_register = &GPIO->TIMER3ROUTE[0].CC0ROUTE;
/**
 * @brief The `sl_pwm_deinit` function deinitializes a PWM instance by stopping the PWM, resetting the timer and GPIO configurations, and disabling the associated clock.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM instance to be deinitialized.
 * @return Returns `SL_STATUS_OK` on successful deinitialization or `SL_STATUS_FAIL` if an invalid timer number is encountered.
 */
      break;
    default:
      EFM_ASSERT(0);
      return SL_STATUS_FAIL;
  }
#endif
  route_register += pwm->channel;
  *route_register = 0;

#if defined(_SILICON_LABS_32B_SERIES_2)
  TIMER_Reset(pwm->timer);
#else
  sl_hal_timer_reset(pwm->timer);
#endif

  sl_gpio_t reset_gpio;
  reset_gpio.port = pwm->port;
  reset_gpio.pin = pwm->pin;
  sl_gpio_set_pin_mode(&reset_gpio,
                       SL_GPIO_MODE_DISABLED,
                       0);

  sl_bus_clock_t timer_clock;
  sl_peripheral_t peripheral;
  get_timer_clock(pwm->timer, &timer_clock, &peripheral);
  (void) peripheral;
  sl_clock_manager_disable_bus_clock(timer_clock);

  return SL_STATUS_OK;
}

void sl_pwm_start(sl_pwm_instance_t *pwm)
{
#if defined(_GPIO_TIMER_ROUTEEN_MASK)
  GPIO->TIMERROUTE_SET[TIMER_NUM(pwm->timer)].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER_ROUTEEN_CC0PEN_SHIFT);
#else
  switch (TIMER_NUM(pwm->timer)) {
    case 0:
      GPIO->TIMER0ROUTE_SET[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER0_ROUTEEN_CC0PEN_SHIFT);
      break;
    case 1:
      GPIO->TIMER1ROUTE_SET[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER1_ROUTEEN_CC0PEN_SHIFT);
      break;
    case 2:
      GPIO->TIMER2ROUTE_SET[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER2_ROUTEEN_CC0PEN_SHIFT);
      break;
    case 3:
      GPIO->TIMER3ROUTE_SET[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER3_ROUTEEN_CC0PEN_SHIFT);
/**
 * @brief The `sl_pwm_start` function enables the PWM output for a specified PWM instance by configuring the appropriate GPIO routing based on the timer and channel information.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure that contains the PWM instance configuration, including the timer and channel to be used.
 * @return This function does not return any value; it performs hardware register modifications to enable PWM output.
 */
      break;
    default:
      EFM_ASSERT(0);
      break;
  }
#endif
}

void sl_pwm_stop(sl_pwm_instance_t *pwm)
{
#if defined(_GPIO_TIMER_ROUTEEN_MASK)
  GPIO->TIMERROUTE_CLR[TIMER_NUM(pwm->timer)].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER_ROUTEEN_CC0PEN_SHIFT);
#else
  switch (TIMER_NUM(pwm->timer)) {
    case 0:
      GPIO->TIMER0ROUTE_CLR[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER0_ROUTEEN_CC0PEN_SHIFT);
      break;
    case 1:
      GPIO->TIMER1ROUTE_CLR[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER1_ROUTEEN_CC0PEN_SHIFT);
      break;
/**
 * @brief The `sl_pwm_stop` function disables the PWM output for a specified PWM instance without stopping the timer, allowing other channels to continue operation.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM instance to be stopped.
 * @return The function does not return a value; it performs an action to disable the PWM output for the specified instance.
 */
    case 2:
      GPIO->TIMER2ROUTE_CLR[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER2_ROUTEEN_CC0PEN_SHIFT);
      break;
    case 3:
      GPIO->TIMER3ROUTE_CLR[0].ROUTEEN = 1 << (pwm->channel + _GPIO_TIMER3_ROUTEEN_CC0PEN_SHIFT);
      break;
    default:
      EFM_ASSERT(0);
      break;
  }
#endif
}

void sl_pwm_set_duty_cycle(sl_pwm_instance_t *pwm, uint8_t percent)
{
#if defined(_SILICON_LABS_32B_SERIES_2)
  uint32_t top = TIMER_TopGet(pwm->timer);

  TIMER_CompareBufSet(pwm->timer, pwm->channel, (top * percent) / 100);
#else
/**
 * @brief The `sl_pwm_set_duty_cycle` function sets the duty cycle of a PWM signal to a specified percentage.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM instance to configure.
 * @param percent An 8-bit unsigned integer representing the desired duty cycle percentage (0-100).
 * @return The function does not return a value; it modifies the PWM instance to reflect the new duty cycle.
 */
  uint32_t top = sl_hal_timer_get_top(pwm->timer);

  sl_hal_timer_channel_set_compare_buffer(pwm->timer, pwm->channel, (top * percent) / 100);
#endif
}

uint8_t sl_pwm_get_duty_cycle(sl_pwm_instance_t *pwm)
{
#if defined(_SILICON_LABS_32B_SERIES_2)
/**
 * @brief The `sl_pwm_get_duty_cycle` function calculates and returns the current duty cycle percentage of a PWM signal for a given PWM instance.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM instance for which the duty cycle is to be retrieved.
 * @return The function returns an 8-bit unsigned integer representing the duty cycle percentage of the PWM signal.
 */
  uint32_t top = TIMER_TopGet(pwm->timer);
  uint32_t compare = TIMER_CaptureGet(pwm->timer, pwm->channel);
#else
  uint32_t top = sl_hal_timer_get_top(pwm->timer);
  uint32_t compare = sl_hal_timer_channel_get_compare(pwm->timer, pwm->channel);
#endif

  uint8_t percent = (uint8_t)((compare * 100) / top);

  return percent;
}
