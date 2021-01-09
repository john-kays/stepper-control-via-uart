/**
 * @file       stepper_motor_new_driver.c
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-26
 * @author     Thuan Le
 * @brief      Support control the stepper by stepper driver
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include <AccelStepper.h>
#include "stdint.h"

/* Private defines ---------------------------------------------------- */
#define STEPPER_STEP_PIN     (8)
#define STEPPER_DIR_PIN      (9)
#define STEPPER_ENA_PIN      (10)

/*
-----------------------------------------
800     pulses  => 360 degrees
800/360 pulses  => 1   degree
-----------------------------------------
800             => 1 revolution/second
800/60          => 1 revolution/minute
-----------------------------------------
*/

#define STEP_PER_DEGREE             (2.22)
#define REVOLUTION_PER_MINUITE      (13.33)

#define STEPPER_DEFAULT_SPEED_RPM   (60)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private constant --------------------------------------------------- */

static const String SET_SPEED_CMD               = "SP";
static const String CLOCKWISE_CMD               = "TL";
static const String COUNTER_CLOCKWISE_CMD       = "TR";

static const String READ_MULTI_TURN_ANGLE_CMD   = "MT";
static const String READ_SPEED_CMD              = "RP";
static const String READ_ENCODER_CMD            = "RE";
static const String READ_TEMP_CMD               = "RT";

/* Private variables -------------------------------------------------- */
static String m_uart_data_receive = "";
static String m_uart_cmd = "";
static String m_uart_data = "";
static boolean m_uart_string_complete = false;
static float m_float_data_value = 0;

static int16_t m_stepper_speed_rpm = STEPPER_DEFAULT_SPEED_RPM;
static int16_t m_stepper_speed_ppm = REVOLUTION_PER_MINUITE * m_stepper_speed_rpm;

AccelStepper stepper(AccelStepper::DRIVER, STEPPER_STEP_PIN, STEPPER_DIR_PIN);

/* Private function prototypes ---------------------------------------- */
static void uart_receive_and_execute(void);
void m_stepper_set_position(int16_t degree);

/* Function definitions ----------------------------------------------- */
void setup()
{
  Serial.begin(115200);
  stepper.setEnablePin(STEPPER_ENA_PIN);
  stepper.setMaxSpeed(100000);
  stepper.setSpeed(5000);
}

void loop()
{
  uart_receive_and_execute();
}

/* Private function definitions --------------------------------------- */
/**
 * @brief       Uart receive data and execute
 *
 * @param[in]   None
 *
 * @attention   None
 *
 * @return      None
 */
static void uart_receive_and_execute(void)
{
  while (Serial.available()) // Receive data from computer
  {
    char data = (char)Serial.read();
    m_uart_data_receive += data;

    if (data == '\n')
    {
      m_uart_string_complete = true;
    }

    if (m_uart_string_complete)
    {
      m_uart_string_complete = false;
      Serial.println(m_uart_data_receive);

      m_uart_cmd = m_uart_data_receive.substring(0, 2);
      m_uart_data = m_uart_data_receive.substring(3);
      m_float_data_value = m_uart_data.toFloat();
      Serial.println(m_uart_cmd);
      Serial.println(m_uart_data);
      Serial.println(m_float_data_value);
      m_uart_data_receive = "";

      if (CLOCKWISE_CMD == m_uart_cmd)
      {
        Serial.println("Set motor run clockwise");
        m_stepper_set_position(m_float_data_value);
      }
      else if (COUNTER_CLOCKWISE_CMD == m_uart_cmd)
      {
        Serial.println("Set motor run counter clockwise");
        m_stepper_set_position(-m_float_data_value);
      }
      else if (SET_SPEED_CMD == m_uart_cmd)
      {
        Serial.println("Set motor speed");
        m_stepper_set_speed(m_float_data_value);
      }
      else if (READ_MULTI_TURN_ANGLE_CMD == m_uart_cmd)
      {
        Serial.print("Motor multi turn angle:");
        Serial.println(stepper.currentPosition() / STEP_PER_DEGREE);
      }
      else if (READ_SPEED_CMD == m_uart_cmd)
      {
        Serial.print("Motor speed rpm: ");
        Serial.println(m_stepper_speed_rpm);
      }
      else if (READ_ENCODER_CMD == m_uart_cmd)
      {
        int16_t encoder = 0;
        encoder = (int)(stepper.currentPosition() / STEP_PER_DEGREE) % 360;
        encoder = encoder < 0 ? (-encoder) : encoder;
        Serial.print("Motor encoder: ");
        Serial.println(encoder);
      }
    }
  }
}

void m_stepper_set_position(int16_t degree)
{
  signed long step;
  Serial.print("Step per degree: ");
  Serial.println(STEP_PER_DEGREE);

  step =  degree * STEP_PER_DEGREE;
  Serial.print("Pulses have to run: ");
  Serial.println(step);

  stepper.setCurrentPosition(0);
  stepper.moveTo(step);
  while (stepper.currentPosition() != step)
  {
    if (step > 0)
    {
      stepper.setSpeed(m_stepper_speed_ppm);
    }
    else
    {
      stepper.setSpeed(-m_stepper_speed_ppm);
    }
    stepper.runSpeed();
    uart_receive_and_execute();
  }
  stepper.stop();
  Serial.println(stepper.currentPosition());
}

void m_stepper_set_speed(uint16_t speed_rpm)
{
  m_stepper_speed_rpm = speed_rpm;
  m_stepper_speed_ppm = REVOLUTION_PER_MINUITE * speed_rpm;
}

/* End of file -------------------------------------------------------- */
