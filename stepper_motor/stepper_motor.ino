#include <Stepper.h>

/* Private defines ---------------------------------------------------- */
#define STEPPER_HP_1                (8)
#define STEPPER_HP_2                (9)
#define STEPPER_HP_3                (10)
#define STEPPER_HP_4                (11)

#define STEPPER_STEPS_PER_REV       (200)
#define NUM_STEPS_PER_REQ           (10)
#define STEPPER_SPEED               (35)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private constan ---------------------------------------------------- */
static const String SET_SPEED_CMD               = "SP";
static const String CLOCKWISE_CMD               = "TL";
static const String COUNTER_CLOCKWISE_CMD       = "TR";

/* Private variables -------------------------------------------------- */
static String m_uart_data_receive = "";
static String m_uart_cmd = "";
static String m_uart_data = "";
static boolean m_uart_string_complete = false;
static float m_float_data_value = 0;
Stepper stepper_motor_hp(STEPPER_STEPS_PER_REV, STEPPER_HP_1, STEPPER_HP_2, STEPPER_HP_3, STEPPER_HP_4);

/* Private function prototypes ---------------------------------------- */
static void uart_receive_and_execute(void);

void setup()
{
  Serial.begin(9600);

  stepper_motor_hp.setSpeed(STEPPER_SPEED);
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

      if (CLOCKWISE_CMD == m_uart_cmd)
      {
        Serial.println("Set motor run clockwise");
        stepper_motor_hp.step(m_float_data_value);
      }
      else if (COUNTER_CLOCKWISE_CMD == m_uart_cmd)
      {
        Serial.println("Set motor run counter clockwise");
        stepper_motor_hp.step(-m_float_data_value);
      }
      else if (SET_SPEED_CMD == m_uart_cmd)
      {
        Serial.println("Set motor speed");
        stepper_motor_hp.setSpeed(m_float_data_value);
      }

      m_uart_data_receive = "";
    }
  }
}
