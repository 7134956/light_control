// Platform-specific functions

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "pin_map.h"
#include "c_types.h"

#include "driver/pwm.h"
// Error / status codes
enum
{
  PLATFORM_ERR,
  PLATFORM_OK,
  PLATFORM_UNDERFLOW = -1
};


void pwms_init();

// GPIO subsection
#define PLATFORM_GPIO_FLOAT 0
#define PLATFORM_GPIO_PULLUP 1
#define PLATFORM_GPIO_PULLDOWN 2

#define PLATFORM_GPIO_INT 2
#define PLATFORM_GPIO_OUTPUT 1
#define PLATFORM_GPIO_INPUT 0

#define PLATFORM_GPIO_HIGH 1
#define PLATFORM_GPIO_LOW 0



int platform_can_exists( unsigned id );
uint32_t platform_can_setup( unsigned id, uint32_t clock );
int platform_can_send( unsigned id, uint32_t canid, uint8_t idtype, uint8_t len, const uint8_t *data );
int platform_can_recv( unsigned id, uint32_t *canid, uint8_t *idtype, uint8_t *len, uint8_t *data );


// *****************************************************************************
// PWM subsection



#define NORMAL_DUTY(d) (((unsigned)(d)*PWM_DEPTH) / PWM_DEPTH)
#define DUTY(d) ((uint16_t)( ((unsigned)(d)*PWM_DEPTH) / PWM_DEPTH) )

// The platform PWM functions
int platform_pwm_exists( unsigned id );
uint32_t platform_pwm_setup( unsigned id, uint32_t frequency, unsigned duty );
void platform_pwm_close( unsigned id );
void platform_pwm_start( unsigned id );
void platform_pwm_stop( unsigned id );
uint32_t platform_pwm_set_clock( unsigned id, uint32_t data );
uint32_t platform_pwm_get_clock( unsigned id );
uint32_t platform_pwm_set_duty( unsigned id, uint32_t data );
uint32_t platform_pwm_get_duty( unsigned id );




#endif
