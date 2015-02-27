// Platform-dependent functions

#include "espmissingincludes.h"
#include "user_pwm.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "user_interface.h"
#include "gpio16.h"
#include "driver/pwm.h"

// ****************************************************************************
// GPIO functions
int platform_gpio_mode( unsigned pin, unsigned mode, unsigned pull )
{
  // NODE_DBG("Function platform_gpio_mode() is called. pin_mux:%d, func:%d\n",pin_mux[pin],pin_func[pin]);
  if (pin >= GPIO_PIN_NUM)
    return -1;
  if(pin == 0){
    if(mode==PLATFORM_GPIO_INPUT)
      gpio16_input_conf();
    else
      gpio16_output_conf();
    return 1;
  }

  platform_pwm_close(pin);    // closed from pwm module, if it is used in pwm

  switch(pull){
    case PLATFORM_GPIO_PULLUP:
      PIN_PULLDWN_DIS(pin_mux[pin]);
      PIN_PULLUP_EN(pin_mux[pin]);
      break;
    case PLATFORM_GPIO_PULLDOWN:
      PIN_PULLUP_DIS(pin_mux[pin]);
      PIN_PULLDWN_EN(pin_mux[pin]);
      break;
    case PLATFORM_GPIO_FLOAT:
      PIN_PULLUP_DIS(pin_mux[pin]);
      PIN_PULLDWN_DIS(pin_mux[pin]);
      break;
    default:
      PIN_PULLUP_DIS(pin_mux[pin]);
      PIN_PULLDWN_DIS(pin_mux[pin]);
      break;
  }

  switch(mode){
    case PLATFORM_GPIO_INPUT:
      GPIO_DIS_OUTPUT(pin_num[pin]);
    case PLATFORM_GPIO_OUTPUT:
      ETS_GPIO_INTR_DISABLE();
      PIN_FUNC_SELECT(pin_mux[pin], pin_func[pin]);
      //disable interrupt
      gpio_pin_intr_state_set(GPIO_ID_PIN(pin_num[pin]), GPIO_PIN_INTR_DISABLE);
      //clear interrupt status
      GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin_num[pin]));
      GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(pin_num[pin])), GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(pin_num[pin]))) & (~ GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE))); //disable open drain;
      ETS_GPIO_INTR_ENABLE();
      break;
    default:
      break;
  }
  return 1;
}

int platform_gpio_write( unsigned pin, unsigned level )
{
  // NODE_DBG("Function platform_gpio_write() is called. pin:%d, level:%d\n",GPIO_ID_PIN(pin_num[pin]),level);
  if (pin >= GPIO_PIN_NUM)
    return -1;
  if(pin == 0){
    gpio16_output_conf();
    gpio16_output_set(level);
    return 1;
  }

  GPIO_OUTPUT_SET(GPIO_ID_PIN(pin_num[pin]), level);
}

int platform_gpio_read( unsigned pin )
{
  // NODE_DBG("Function platform_gpio_read() is called. pin:%d\n",GPIO_ID_PIN(pin_num[pin]));
  if (pin >= GPIO_PIN_NUM)
    return -1;

  if(pin == 0){
    // gpio16_input_conf();
    return 0x1 & gpio16_input_get();
  }

  // GPIO_DIS_OUTPUT(pin_num[pin]);
  return 0x1 & GPIO_INPUT_GET(GPIO_ID_PIN(pin_num[pin]));
}

// ****************************************************************************
// PWMs

static uint16_t pwms_duty[GPIO_PIN_NUM] = {0};

void pwms_init()
{
  int i;
  for(i=0;i<GPIO_PIN_NUM;i++){
    pwms_duty[i] = DUTY(0);
  }
  pwm_init(500, 0);
  // NODE_DBG("Function pwms_init() is called.\n");
}

// Return the PWM clock
// NOTE: Can't find a function to query for the period set for the timer,
// therefore using the struct.
// This may require adjustment if driver libraries are updated.
uint32_t platform_pwm_get_clock( unsigned pin )
{
  // NODE_DBG("Function platform_pwm_get_clock() is called.\n");
  if( pin >= GPIO_PIN_NUM)
    return 0;
  if(!pwm_exist(pin))
    return 0;

  return (uint32_t)pwm_get_freq(pin);
}

// Set the PWM clock
uint32_t platform_pwm_set_clock( unsigned pin, uint32_t clock )
{
  // NODE_DBG("Function platform_pwm_set_clock() is called.\n");
  if( pin >= GPIO_PIN_NUM)
    return 0;
  if(!pwm_exist(pin))
    return 0;

  pwm_set_freq((uint16_t)clock, pin);
  pwm_start();
  return (uint32_t)pwm_get_freq( pin );
}

uint32_t platform_pwm_get_duty( unsigned pin )
{
  // NODE_DBG("Function platform_pwm_get_duty() is called.\n");
  if( pin < GPIO_PIN_NUM){
    if(!pwm_exist(pin))
      return 0;
    // return NORMAL_DUTY(pwm_get_duty(pin));
    return pwms_duty[pin];
  }
  return 0;
}

// Set the PWM duty
uint32_t platform_pwm_set_duty( unsigned pin, uint32_t duty )
{
  // NODE_DBG("Function platform_pwm_set_duty() is called.\n");
  if ( pin < GPIO_PIN_NUM)
  {
    if(!pwm_exist(pin))
      return 0;
    pwm_set_duty(DUTY(duty), pin);
  } else {
    return 0;
  }
  pwm_start();
  pwms_duty[pin] = NORMAL_DUTY(pwm_get_duty(pin));
  return pwms_duty[pin];
}

uint32_t platform_pwm_setup( unsigned pin, uint32_t frequency, unsigned duty )
{
  uint32_t clock;
  if ( pin < GPIO_PIN_NUM)
  {
    platform_gpio_mode(pin, PLATFORM_GPIO_OUTPUT, PLATFORM_GPIO_FLOAT);  // disable gpio interrupt first
    if(!pwm_add(pin)) 
      return 0;
    // pwm_set_duty(DUTY(duty), pin);
    pwm_set_duty(0, pin);
    pwms_duty[pin] = duty;
    pwm_set_freq((uint16_t)frequency, pin);
  } else {
    return 0;
  }
  clock = platform_pwm_get_clock( pin );
  pwm_start();
  return clock;
}

void platform_pwm_close( unsigned pin )
{
  // NODE_DBG("Function platform_pwm_stop() is called.\n");
  if ( pin < GPIO_PIN_NUM)
  {
    pwm_delete(pin);
    pwm_start();
  }
}

void platform_pwm_start( unsigned pin )
{
  // NODE_DBG("Function platform_pwm_start() is called.\n");
  if ( pin < GPIO_PIN_NUM)
  {
    if(!pwm_exist(pin))
      return;
    pwm_set_duty(DUTY(pwms_duty[pin]), pin);
    pwm_start();
  }
}

void platform_pwm_stop( unsigned pin )
{
  // NODE_DBG("Function platform_pwm_stop() is called.\n");
  if ( pin < GPIO_PIN_NUM)
  {
    if(!pwm_exist(pin))
      return;
    pwm_set_duty(0, pin);
    pwm_start();
  }
}

