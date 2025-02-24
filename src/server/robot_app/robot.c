// robot.c

#include "robot.h"
#include "mrpiz.h"

static speed_pct_t speed_pct;
static robot_status_t robot_status = {};

int robot_start(void) {
  int result = 0;

  result = mrpiz_init();

  return result;
}

void robot_set_speed(speed_pct_t left, speed_pct_t right) {
  mrpiz_motor_set(MRPIZ_MOTOR_LEFT, left);
  mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, right);
}

int robot_get_wheel_position(wheel_t wheel_id) {
  return mrpiz_motor_encoder_get(wheel_id);
}

void robot_reset_wheel_pos(void) {
  mrpiz_motor_encoder_reset(MRPIZ_MOTOR_BOTH);
}

robot_status_t robot_get_status(void)
{
  robot_status.left_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
  robot_status.right_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);
  robot_status.left_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT);
  robot_status.right_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT);
  robot_status.center_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER);
  robot_status.battery = mrpiz_battery_level();
  return robot_status;
}

void robot_signal_event(notification_t event) {
  switch (event)
  {
    case ROBOT_OK:
      mrpiz_led_rgb_set(MRPIZ_LED_GREEN);
      break;
    case ROBOT_IDLE:
      mrpiz_led_rgb_set(MRPIZ_LED_GREEN);
      break;
    case ROBOT_PROBLEM:
      mrpiz_led_rgb_set(MRPIZ_LED_RED);
      break;
    case ROBOT_OBSTACLE:
      mrpiz_led_rgb_set(MRPIZ_LED_BLUE);
      break;
    default:
      mrpiz_led_rgb_set(MRPIZ_LED_OFF);
      break;
  }
}

void robot_close(void) {
  robot_set_speed(0, 0);
  mrpiz_close();
}
