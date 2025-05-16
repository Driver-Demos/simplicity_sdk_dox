
#include <stddef.h>
#include "sl_core.h"
#include "sl_common.h"
#include "sl_interrupt_manager.h"
#include "sl_clock_manager.h"
#include "sl_hal_gpio.h"
#include "sl_gpio.h"


#define SL_GPIO_PORT_INTERRUPT  (0xFF)

#define SL_GPIO_DIRECTION_IS_VALID(direction)  (direction <= SL_GPIO_PIN_DIRECTION_OUT)


/**
 * @brief The `sl_gpio_callback_desc_t` structure is designed to manage GPIO interrupt callbacks. It contains an interrupt number (`int_no`) that identifies the specific GPIO pin interrupt, a pointer to a callback function (`callback`) that is executed when the interrupt occurs, and a pointer to a context (`context`) that provides additional data or state information to the callback function. This structure is essential for handling GPIO interrupts in a flexible and efficient manner, allowing for user-defined callback functions to be associated with specific GPIO pin interrupts.
 *
 * @struct sl_gpio_callback_desc_t
 * @var sl_gpio_callback_desc_t::int_no Pin interrupt number in range 0 to 15.
 * @var sl_gpio_callback_desc_t::callback Pointer to callback function.
 * @var sl_gpio_callback_desc_t::context Pointer to callback context.
 */
typedef struct {
  uint32_t int_no;
  void *callback;
  void *context;
} sl_gpio_callback_desc_t;

/**
 * @brief The `sl_gpio_callbacks_t` structure is designed to manage user-defined callback functions for handling GPIO interrupts. It contains two arrays of `sl_gpio_callback_desc_t` structures: `callback_ext` for external interrupts and `callback_em4` for EM4 interrupts. Each array can hold a maximum number of callbacks defined by `SL_HAL_GPIO_INTERRUPT_MAX`, allowing the system to handle a range of interrupt sources efficiently. This structure is crucial for organizing and dispatching interrupt handling routines in a GPIO driver context.
 *
 * @struct sl_gpio_callbacks_t
 * @var sl_gpio_callbacks_t::callback_ext An array of user callbacks for external interrupts configured from 0 to 15 bits.
 * @var sl_gpio_callbacks_t::callback_em4 An array of user callbacks for EM4 interrupts configured from 16 to 31 bits.
 */
typedef struct {
  sl_gpio_callback_desc_t callback_ext[SL_HAL_GPIO_INTERRUPT_MAX];
  sl_gpio_callback_desc_t callback_em4[SL_HAL_GPIO_INTERRUPT_MAX];
} sl_gpio_callbacks_t;


/**
 * @var sl_gpio_callbacks_t
 * @brief The `gpio_interrupts` variable is a static instance of the `sl_gpio_callbacks_t` structure, which is used to manage and organize callback functions for GPIO external and EM4 interrupts. It contains two arrays of `sl_gpio_callback_desc_t` structures: `callback_ext` for external interrupts and `callback_em4` for EM4 interrupts, each capable of handling up to `SL_HAL_GPIO_INTERRUPT_MAX` callbacks.
 *
 * @details This variable is used to store and manage user-defined callback functions for handling GPIO interrupts, allowing the system to execute specific functions when certain GPIO events occur.
 */
static sl_gpio_callbacks_t gpio_interrupts = { 0 };

static void sl_gpio_dispatch_interrupt(uint32_t iflags);

/**
 * @brief The `sl_gpio_init` function initializes the GPIO by enabling the GPIO bus clock and ensuring that the GPIO ODD and EVEN interrupts are enabled if they are currently disabled.
 *
 * @return The function returns `SL_STATUS_OK` to indicate successful initialization of the GPIO.
 */
sl_status_t sl_gpio_init()
{
  sl_clock_manager_enable_bus_clock(SL_BUS_CLOCK_GPIO);

  if (sl_interrupt_manager_is_irq_disabled(GPIO_ODD_IRQn)) {
    sl_interrupt_manager_clear_irq_pending(GPIO_ODD_IRQn);
    sl_interrupt_manager_enable_irq(GPIO_ODD_IRQn);
  }
  if (sl_interrupt_manager_is_irq_disabled(GPIO_EVEN_IRQn)) {
    sl_interrupt_manager_clear_irq_pending(GPIO_EVEN_IRQn);
    sl_interrupt_manager_enable_irq(GPIO_EVEN_IRQn);
  }

  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_set_pin_direction` function sets the direction of a specified GPIO pin to either input or output.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be configured.
 * @param pin_direction An `sl_gpio_pin_direction_t` value indicating the desired direction for the GPIO pin, either input or output.
 * @return Returns an `sl_status_t` value indicating the success or failure of the operation, such as `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, `SL_STATUS_INVALID_PARAMETER`, or `SL_STATUS_INVALID_STATE`.
 */
sl_status_t sl_gpio_set_pin_direction(const sl_gpio_t *gpio,
                                      sl_gpio_pin_direction_t pin_direction)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin) || !SL_GPIO_DIRECTION_IS_VALID(pin_direction)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (sl_hal_gpio_get_lock_status() != 0) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_STATE;
  }

  CORE_ENTER_ATOMIC();

  if (pin_direction == SL_GPIO_PIN_DIRECTION_OUT) {
    sl_hal_gpio_set_pin_mode(gpio, SL_GPIO_MODE_PUSH_PULL, 1);
  } else if (pin_direction == SL_GPIO_PIN_DIRECTION_IN) {
    sl_hal_gpio_set_pin_mode(gpio, SL_GPIO_MODE_INPUT, 0);
  }

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_set_pin_mode` function configures the mode and output value of a specified GPIO pin, ensuring the operation is valid and safe by checking input parameters and locking status.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be configured.
 * @param mode An `sl_gpio_mode_t` value specifying the desired mode for the GPIO pin.
 * @param output_value A boolean indicating the desired output value for the pin if it is configured as an output.
 * @return The function returns an `sl_status_t` value indicating the success or failure of the operation, with possible values being `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, `SL_STATUS_INVALID_PARAMETER`, or `SL_STATUS_INVALID_STATE`.
 */
sl_status_t sl_gpio_set_pin_mode(const sl_gpio_t *gpio,
                                 sl_gpio_mode_t mode,
                                 bool output_value)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_MODE_IS_VALID(mode) || !SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (sl_hal_gpio_get_lock_status() != 0) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_STATE;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_set_pin_mode(gpio, mode, output_value);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_pin_config` function retrieves the current configuration of a specified GPIO pin, including its mode and direction.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be configured.
 * @param pin_config A pointer to an `sl_gpio_pin_config_t` structure where the pin configuration will be stored.
 * @return The function returns an `sl_status_t` status code indicating the success or failure of the operation, such as `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, `SL_STATUS_INVALID_PARAMETER`, or `SL_STATUS_INVALID_MODE`.
 */
sl_status_t sl_gpio_get_pin_config(const sl_gpio_t *gpio,
                                   sl_gpio_pin_config_t *pin_config)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL || pin_config == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  pin_config->mode = sl_hal_gpio_get_pin_mode(gpio);
  switch (pin_config->mode) {
    case SL_GPIO_MODE_INPUT:
    case SL_GPIO_MODE_INPUT_PULL:
    case SL_GPIO_MODE_INPUT_PULL_FILTER:
      pin_config->direction = SL_GPIO_PIN_DIRECTION_IN;
      break;

    case SL_GPIO_MODE_DISABLED:
    case SL_GPIO_MODE_PUSH_PULL:
    case SL_GPIO_MODE_PUSH_PULL_ALTERNATE:
    case SL_GPIO_MODE_WIRED_OR:
    case SL_GPIO_MODE_WIRED_OR_PULL_DOWN:
    case SL_GPIO_MODE_WIRED_AND:
    case SL_GPIO_MODE_WIRED_AND_FILTER:
    case SL_GPIO_MODE_WIRED_AND_PULLUP:
    case SL_GPIO_MODE_WIRED_AND_PULLUP_FILTER:
    case SL_GPIO_MODE_WIRED_AND_ALTERNATE:
    case SL_GPIO_MODE_WIRED_AND_ALTERNATE_FILTER:
    case SL_GPIO_MODE_WIRED_AND_ALTERNATE_PULLUP:
    case SL_GPIO_MODE_WIRED_AND_ALTERNATE_PULLUP_FILTER:
      pin_config->direction = SL_GPIO_PIN_DIRECTION_OUT;
      break;

    default:
      CORE_EXIT_ATOMIC();
      EFM_ASSERT(false);
      return SL_STATUS_INVALID_MODE;
  }

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_set_pin` function sets the digital output (DOUT) of a specified GPIO pin to a high state, ensuring atomic operation and validating input parameters.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure that specifies the GPIO port and pin to be set.
 * @return The function returns an `sl_status_t` status code, which is `SL_STATUS_OK` on success, `SL_STATUS_NULL_POINTER` if the `gpio` pointer is `NULL`, or `SL_STATUS_INVALID_PARAMETER` if the GPIO port or pin is invalid.
 */
sl_status_t sl_gpio_set_pin(const sl_gpio_t *gpio)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_set_pin(gpio);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_clear_pin` function clears the digital output (DOUT) of a specified GPIO pin, ensuring atomic operation and validating input parameters.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be cleared.
 * @return Returns an `sl_status_t` value indicating the success or failure of the operation, specifically `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, or `SL_STATUS_INVALID_PARAMETER`.
 */
sl_status_t sl_gpio_clear_pin(const sl_gpio_t *gpio)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_clear_pin(gpio);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_toggle_pin` function toggles the digital output state of a specified GPIO pin, ensuring atomic operation and validating input parameters.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be toggled.
 * @return Returns an `sl_status_t` value indicating the success or failure of the operation, specifically `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, or `SL_STATUS_INVALID_PARAMETER`.
 */
sl_status_t sl_gpio_toggle_pin(const sl_gpio_t *gpio)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_toggle_pin(gpio);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_pin_output` function retrieves the output state of a specified GPIO pin and stores it in a provided boolean variable.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin whose output state is to be retrieved.
 * @param pin_value A pointer to a boolean variable where the output state of the GPIO pin will be stored.
 * @return The function returns an `sl_status_t` value indicating the success or failure of the operation, with `SL_STATUS_OK` for success, `SL_STATUS_NULL_POINTER` if any input pointer is `NULL`, and `SL_STATUS_INVALID_PARAMETER` if the GPIO port or pin is invalid.
 */
sl_status_t sl_gpio_get_pin_output(const sl_gpio_t *gpio,
                                   bool *pin_value)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL || pin_value == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  *pin_value = sl_hal_gpio_get_pin_output(gpio);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_pin_input` function retrieves the input state of a specified GPIO pin and stores it in a provided boolean variable.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin whose input state is to be retrieved.
 * @param pin_value A pointer to a boolean variable where the input state of the GPIO pin will be stored.
 * @return The function returns an `sl_status_t` value indicating the success or failure of the operation, with possible values being `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, or `SL_STATUS_INVALID_PARAMETER`.
 */
sl_status_t sl_gpio_get_pin_input(const sl_gpio_t *gpio,
                                  bool *pin_value)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL || pin_value == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  *pin_value = sl_hal_gpio_get_pin_input(gpio);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_set_port` function sets the output state of specified pins on a given GPIO port.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port to be modified.
 * @param pins A `uint32_t` value representing the pins to be set on the specified port.
 * @return The function returns an `sl_status_t` value, which is `SL_STATUS_OK` if the operation is successful, or `SL_STATUS_INVALID_PARAMETER` if the port is invalid.
 */
sl_status_t sl_gpio_set_port(sl_gpio_port_t port,
                             uint32_t pins)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_set_port(port, pins);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_clear_port` function clears the specified pins on a given GPIO port, ensuring atomic operation and validating the port before execution.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port to be cleared.
 * @param pins A `uint32_t` value representing the specific pins on the port to be cleared.
 * @return The function returns an `sl_status_t` value, which is `SL_STATUS_OK` if the operation is successful, or `SL_STATUS_INVALID_PARAMETER` if the port is invalid.
 */
sl_status_t sl_gpio_clear_port(sl_gpio_port_t port,
                               uint32_t pins)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_clear_port(port, pins);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_port_output` function retrieves the output state of all pins on a specified GPIO port.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port whose output state is to be retrieved.
 * @param port_value A pointer to a `uint32_t` where the output state of the port will be stored.
 * @return The function returns an `sl_status_t` status code, which is `SL_STATUS_OK` on success, or an error code such as `SL_STATUS_INVALID_PARAMETER` or `SL_STATUS_NULL_POINTER` if the input validation fails.
 */
sl_status_t sl_gpio_get_port_output(sl_gpio_port_t port,
                                    uint32_t *port_value)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (port_value == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }

  CORE_ENTER_ATOMIC();

  *port_value = sl_hal_gpio_get_port_output(port);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_port_input` function retrieves the input state of all pins on a specified GPIO port and stores it in a provided variable.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port from which to read the input state.
 * @param port_value A pointer to a `uint32_t` where the input state of the port will be stored.
 * @return The function returns an `sl_status_t` status code, which is `SL_STATUS_OK` on success, or an error code such as `SL_STATUS_INVALID_PARAMETER` or `SL_STATUS_NULL_POINTER` if the inputs are invalid.
 */
sl_status_t sl_gpio_get_port_input(sl_gpio_port_t port,
                                   uint32_t *port_value)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (port_value == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }

  CORE_ENTER_ATOMIC();

  *port_value = sl_hal_gpio_get_port_input(port);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_configure_external_interrupt` function configures a GPIO pin for external interrupt handling and registers a callback for the interrupt.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be configured.
 * @param int_no A pointer to an integer where the interrupt number will be stored.
 * @param flags An `sl_gpio_interrupt_flag_t` value specifying the interrupt configuration flags.
 * @param gpio_callback A callback function of type `sl_gpio_irq_callback_t` to be called when the interrupt occurs.
 * @param context A pointer to user-defined data to be passed to the callback function.
 * @return Returns an `sl_status_t` indicating the success or failure of the configuration process, such as `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, `SL_STATUS_INVALID_PARAMETER`, or `SL_STATUS_NOT_FOUND`.
 */
sl_status_t sl_gpio_configure_external_interrupt(const sl_gpio_t *gpio,
                                                 int32_t *int_no,
                                                 sl_gpio_interrupt_flag_t flags,
                                                 sl_gpio_irq_callback_t gpio_callback,
                                                 void *context)
{
  uint32_t enabled_interrupts;
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL || int_no == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }
  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin) && (gpio->port != SL_GPIO_PORT_INTERRUPT)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (!SL_GPIO_FLAG_IS_VALID(flags)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  if (gpio->port != SL_GPIO_PORT_INTERRUPT) {
    *int_no = sl_hal_gpio_configure_external_interrupt(gpio, *int_no, flags);
  }

  if (*int_no == SL_GPIO_INTERRUPT_UNAVAILABLE && gpio->port == SL_GPIO_PORT_INTERRUPT) {
    enabled_interrupts = sl_hal_gpio_get_enabled_interrupts();
    *int_no = sl_hal_gpio_get_external_interrupt_number(gpio->pin, enabled_interrupts);
  }

  if (*int_no != SL_GPIO_INTERRUPT_UNAVAILABLE) {
    gpio_interrupts.callback_ext[*int_no].callback = (void *)gpio_callback;
    gpio_interrupts.callback_ext[*int_no].context = context;

    if (gpio->port != SL_GPIO_PORT_INTERRUPT) {
      sl_hal_gpio_enable_interrupts(1 << *int_no);
    }
  } else {
    CORE_EXIT_ATOMIC();
    return SL_STATUS_NOT_FOUND;
  }

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_deconfigure_external_interrupt` function deconfigures a specified GPIO external interrupt and unregisters its callback.
 *
 * @param int_no An integer representing the interrupt number to be deconfigured, which must be within a valid range and not equal to `SL_GPIO_INTERRUPT_UNAVAILABLE`.
 * @return The function returns an `sl_status_t` value, which is `SL_STATUS_OK` if the deconfiguration is successful, or `SL_STATUS_INVALID_PARAMETER` if the input interrupt number is invalid.
 */
sl_status_t sl_gpio_deconfigure_external_interrupt(int32_t int_no)
{
  CORE_DECLARE_IRQ_STATE;

  if (!((int_no != SL_GPIO_INTERRUPT_UNAVAILABLE) && (int_no <= SL_HAL_GPIO_INTERRUPT_MAX) && (int_no >= 0))) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_clear_interrupts(1 << int_no);
  sl_hal_gpio_disable_interrupts(1 << int_no);

  gpio_interrupts.callback_ext[int_no].callback = NULL;
  gpio_interrupts.callback_ext[int_no].context = NULL;

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_enable_interrupts` function enables GPIO interrupts based on the specified flags in an atomic operation.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt flags to be enabled.
 * @return The function returns `SL_STATUS_OK`, indicating that the GPIO interrupts were successfully enabled.
 */
sl_status_t sl_gpio_enable_interrupts(uint32_t flags)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();

  sl_hal_gpio_enable_interrupts(flags);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_disable_interrupts` function disables specified GPIO interrupts in an atomic operation.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt flags to be disabled.
 * @return The function returns `SL_STATUS_OK`, indicating that the operation was successful.
 */
sl_status_t sl_gpio_disable_interrupts(uint32_t flags)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();

  sl_hal_gpio_disable_interrupts(flags);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The function `sl_gpio_configure_wakeup_em4_interrupt` configures a GPIO pin to act as an external interrupt for waking up from EM4 mode and registers a callback for the interrupt.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure representing the GPIO pin to be configured.
 * @param em4_int_no A pointer to an integer where the function will store the interrupt number associated with the EM4 wakeup.
 * @param polarity A boolean indicating the desired polarity for the interrupt (true for high, false for low).
 * @param gpio_callback A callback function of type `sl_gpio_irq_callback_t` to be called when the interrupt occurs.
 * @param context A pointer to user-defined data to be passed to the callback function.
 * @return The function returns an `sl_status_t` indicating the success or failure of the operation, such as `SL_STATUS_OK`, `SL_STATUS_NULL_POINTER`, `SL_STATUS_INVALID_PARAMETER`, or `SL_STATUS_NOT_FOUND`.
 */
sl_status_t sl_gpio_configure_wakeup_em4_interrupt(const sl_gpio_t *gpio,
                                                   int32_t *em4_int_no,
                                                   bool polarity,
                                                   sl_gpio_irq_callback_t gpio_callback,
                                                   void *context)
{
  CORE_DECLARE_IRQ_STATE;

  if (gpio == NULL || em4_int_no == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }

  if (!SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin) && (gpio->port != SL_GPIO_PORT_INTERRUPT)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  if (gpio->port != SL_GPIO_PORT_INTERRUPT) {
    *em4_int_no = sl_hal_gpio_configure_wakeup_em4_external_interrupt(gpio, *em4_int_no, polarity);
  }

  if (*em4_int_no != SL_GPIO_INTERRUPT_UNAVAILABLE) {
    gpio_interrupts.callback_em4[*em4_int_no].callback = (void *)gpio_callback;
    gpio_interrupts.callback_em4[*em4_int_no].context = context;

    if (gpio->port != SL_GPIO_PORT_INTERRUPT) {
      sl_hal_gpio_enable_interrupts(1 << (*em4_int_no + SL_HAL_GPIO_EM4WUEN_SHIFT));
    }
  } else {
    CORE_EXIT_ATOMIC();
    return SL_STATUS_NOT_FOUND;
  }

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The function `sl_gpio_deconfigure_wakeup_em4_interrupt` deconfigures a GPIO pin interrupt for waking up from EM4 mode by clearing interrupts, disabling wakeup, and unregistering callbacks.
 *
 * @param em4_int_no An integer representing the EM4 interrupt number to be deconfigured, which must be within a valid range and not equal to `SL_GPIO_INTERRUPT_UNAVAILABLE`.
 * @return The function returns an `sl_status_t` value, which is `SL_STATUS_OK` if the deconfiguration is successful, or `SL_STATUS_INVALID_PARAMETER` if the input is invalid.
 */
sl_status_t sl_gpio_deconfigure_wakeup_em4_interrupt(int32_t em4_int_no)
{
  CORE_DECLARE_IRQ_STATE;

  if (!((em4_int_no != SL_GPIO_INTERRUPT_UNAVAILABLE) && (em4_int_no <= SL_HAL_GPIO_INTERRUPT_MAX) && (em4_int_no >= 0))) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_clear_interrupts(1 << (em4_int_no + SL_HAL_GPIO_EM4WUEN_SHIFT));
  sl_hal_gpio_disable_pin_em4_wakeup(1 << (em4_int_no + SL_HAL_GPIO_EM4WUEN_SHIFT));
  sl_hal_gpio_disable_interrupts(1 << (em4_int_no + SL_HAL_GPIO_EM4WUEN_SHIFT));

/**
 * @brief The function `sl_gpio_enable_pin_em4_wakeup` configures GPIO pins to wake up the system from EM4 mode by enabling interrupts and setting their polarity.
 *
 * @param em4_int_mask A 32-bit mask specifying which EM4 wake-up interrupts to enable.
 * @param em4_polarity_mask A 32-bit mask specifying the polarity for the EM4 wake-up interrupts.
 * @return The function returns `SL_STATUS_OK` to indicate successful configuration of the EM4 wake-up interrupts.
 */
sl_status_t sl_gpio_enable_pin_em4_wakeup(uint32_t em4_int_mask,
                                          uint32_t em4_polarity_mask)
{
  uint32_t int_mask = 0;
  uint32_t polarity_mask = 0;

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();

  int_mask |= (em4_int_mask << _GPIO_EM4WUEN_EM4WUEN_SHIFT);
  polarity_mask |= (em4_polarity_mask << _GPIO_EM4WUEN_EM4WUEN_SHIFT);
  sl_hal_gpio_enable_pin_em4_wakeup(int_mask, polarity_mask);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The function `sl_gpio_disable_pin_em4_wakeup` disables the EM4 wakeup functionality for specified GPIO pins.
 *
 * @param em4_int_mask A 32-bit integer mask representing the EM4 wakeup pins to be disabled.
 * @return The function returns `SL_STATUS_OK` to indicate that the operation was successful.
 */
sl_status_t sl_gpio_disable_pin_em4_wakeup(uint32_t em4_int_mask)
{
  uint32_t int_mask = 0;

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();

  int_mask |= (em4_int_mask << _GPIO_EM4WUEN_EM4WUEN_SHIFT);
  sl_hal_gpio_disable_pin_em4_wakeup(int_mask);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The function `sl_gpio_set_pin_em4_retention` enables or disables GPIO pin retention in EM4 mode.
 *
 * @param enable A boolean value indicating whether to enable (true) or disable (false) GPIO pin retention in EM4 mode.
 * @return The function returns `SL_STATUS_OK`, indicating that the operation was successful.
 */
sl_status_t sl_gpio_set_pin_em4_retention(bool enable)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();

  sl_hal_gpio_set_pin_em4_retention(enable);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_set_slew_rate` function sets the slew rate for a specified GPIO port.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port for which the slew rate is to be set.
 * @param slewrate A `uint8_t` value representing the desired slew rate to be set for the specified port.
 * @return The function returns an `sl_status_t` value, which is `SL_STATUS_OK` if the operation is successful, or `SL_STATUS_INVALID_PARAMETER` if the port is invalid.
 */
sl_status_t sl_gpio_set_slew_rate(sl_gpio_port_t port,
                                  uint8_t slewrate)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }

  CORE_ENTER_ATOMIC();

  sl_hal_gpio_set_slew_rate(port, slewrate);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_get_slew_rate` function retrieves the slew rate configuration for a specified GPIO port.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port for which the slew rate is to be retrieved.
 * @param slewrate A pointer to a `uint8_t` where the retrieved slew rate value will be stored.
 * @return The function returns an `sl_status_t` status code, which is `SL_STATUS_OK` if the operation is successful, or an error code such as `SL_STATUS_INVALID_PARAMETER` or `SL_STATUS_NULL_POINTER` if there are issues with the input parameters.
 */
sl_status_t sl_gpio_get_slew_rate(sl_gpio_port_t port,
                                  uint8_t *slewrate)
{
  CORE_DECLARE_IRQ_STATE;

  if (!SL_HAL_GPIO_PORT_IS_VALID(port)) {
    EFM_ASSERT(false);
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (slewrate == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }

  CORE_ENTER_ATOMIC();

  *slewrate = sl_hal_gpio_get_slew_rate(port);

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_lock` function locks the GPIO configuration to prevent further changes.
 *
 * @return The function returns `SL_STATUS_OK`, indicating that the GPIO configuration has been successfully locked.
 */
sl_status_t sl_gpio_lock(void)
{
  sl_hal_gpio_lock();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_unlock` function unlocks the GPIO configuration registers by calling a hardware abstraction layer function and returns a success status.
 *
 * @return The function returns `SL_STATUS_OK`, a status code indicating successful execution.
 */
sl_status_t sl_gpio_unlock(void)
{
  sl_hal_gpio_unlock();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_is_locked` function checks if the GPIO configuration registers are locked and returns the status through a pointer.
 *
 * @param state A pointer to a boolean variable where the lock status of the GPIO configuration registers will be stored.
 * @return The function returns `SL_STATUS_OK` if successful, or `SL_STATUS_NULL_POINTER` if the input pointer is NULL, and sets the boolean pointed to by `state` to indicate whether the GPIO configuration registers are locked.
 */
sl_status_t sl_gpio_is_locked(bool *state)
{
  uint32_t status;
  CORE_DECLARE_IRQ_STATE;

  if (state == NULL) {
    EFM_ASSERT(false);
    return SL_STATUS_NULL_POINTER;
  }

  CORE_ENTER_ATOMIC();

  status =  sl_hal_gpio_get_lock_status();
  if (status) {
    *state = true;
  } else {
    *state = false;
  }

  CORE_EXIT_ATOMIC();
  return SL_STATUS_OK;
}

/**
 * @brief The `sl_gpio_dispatch_interrupt` function processes GPIO interrupt flags and invokes registered user callbacks for each active interrupt.
 *
 * @param iflags A 32-bit unsigned integer representing the interrupt flags that need to be processed.
 * @return The function does not return a value; it executes registered callbacks for each interrupt flag processed.
 */
static void sl_gpio_dispatch_interrupt(uint32_t iflags)
{
  uint32_t irq_idx;
  sl_gpio_callback_desc_t *callback;
  sl_gpio_irq_callback_t func;

  while (iflags != 0) {
    irq_idx = SL_CTZ(iflags);
    iflags &= ~(1UL << irq_idx);

    if (irq_idx <= SL_HAL_GPIO_INTERRUPT_MAX) {
      callback = &gpio_interrupts.callback_ext[irq_idx];
    } else {
      callback = &gpio_interrupts.callback_em4[irq_idx - SL_HAL_GPIO_EM4WUEN_SHIFT];
      irq_idx = irq_idx - SL_HAL_GPIO_EM4WUEN_SHIFT;
    }
    if (callback->callback) {
      func = (sl_gpio_irq_callback_t)(callback->callback);
      func((uint8_t)irq_idx, callback->context);
    }
  }
}

/**
 * @brief The `GPIO_EVEN_IRQHandler` function handles even GPIO interrupts by clearing the interrupt flags and dispatching the appropriate callbacks.
 *
 * @return The function does not return any value; it performs actions to handle and dispatch even GPIO interrupts.
 */
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t even_flags;

  even_flags = sl_hal_gpio_get_enabled_pending_interrupts() & SL_HAL_GPIO_INT_IF_EVEN_MASK;
  sl_hal_gpio_clear_interrupts(even_flags);

  sl_gpio_dispatch_interrupt(even_flags);
}

/**
 * @brief The `GPIO_ODD_IRQHandler` function handles odd GPIO interrupts by clearing them and dispatching the relevant interrupt flags to a dispatcher function.
 *
 * @return The function does not return any value; it processes and dispatches odd GPIO interrupts.
 */
void GPIO_ODD_IRQHandler(void)
{
  uint32_t odd_flags;

  odd_flags = sl_hal_gpio_get_enabled_pending_interrupts() & SL_HAL_GPIO_INT_IF_ODD_MASK;
  sl_hal_gpio_clear_interrupts(odd_flags);

  sl_gpio_dispatch_interrupt(odd_flags);
}
