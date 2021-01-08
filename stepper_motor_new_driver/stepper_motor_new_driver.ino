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

/* Private defines ---------------------------------------------------- */
#define STEPPER_DIR_PIN      (18)
#define STEPPER_STEP_PIN     (19)
#define STEPPER_ENA_PIN      (20)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
AccelStepper stepper(AccelStepper::DRIVER, STEPPER_STEP_PIN, STEPPER_DIR_PIN);

/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
void setup()
{
  Serial.begin(9600);
  stepper.setEnablePin(STEPPER_ENA_PIN);
  stepper.setMaxSpeed(100000);
  stepper.setAcceleration(10000);
  stepper.setSpeed(5000);
}

void move_forward()
{
  stepper.moveTo(10000);
  while (stepper.currentPosition() != 10000)
  {
    stepper.setSpeed(5000);
    stepper.run();
  }
  stepper.stop();
  Serial.println(stepper.currentPosition());
}

void move_backward()
{
  stepper.moveTo(0);
  while (stepper.currentPosition() != 0)
  {
    stepper.setSpeed(-3000);
    stepper.runSpeed();
  }
  stepper.stop();
  Serial.println(stepper.currentPosition());
}

void loop()
{
  while (Serial.available())
  {
    char data = Serial.read();
    if (char(data) == 'A')
    {
      move_forward();

      delay(1000);

      move_backward();

      delay(1000);
    }
  }
}

/* End of file -------------------------------------------------------- */