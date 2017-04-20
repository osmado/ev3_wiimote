#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "ev3_wiimote/State.h"
#include <iostream>
#include "ev3_wiimote/ev3dev.h"
#include <cwiid.h>

using namespace ev3dev;

int status_motor;
large_motor *ptr_m_l;
large_motor *ptr_m_r;

void wii_buttons_callback_int(const std_msgs::Int32::ConstPtr& msg)
{
  ROS_INFO("Received [%d]",msg->data);
  if ((msg->data & 0x0002) == 0x0002)  // forward
  {
    ptr_m_l->set_duty_cycle_sp(75);
    ptr_m_r->set_duty_cycle_sp(75);
 
    if ((msg->data & 0x0800) == 0x0800) // left
    {
      ptr_m_l->set_polarity(large_motor::polarity_inversed);
      ptr_m_r->set_polarity(large_motor::polarity_normal);
    }
    else  if ((msg->data & 0x0400) == 0x0400) // right
    {
      ptr_m_l->set_polarity(large_motor::polarity_normal);
      ptr_m_r->set_polarity(large_motor::polarity_inversed);
    }
    else
    {
      ptr_m_l->set_polarity(large_motor::polarity_normal);
      ptr_m_r->set_polarity(large_motor::polarity_normal);
    }
    ptr_m_l->run_forever();
    ptr_m_r->run_forever();
  }
  else
  {
    ptr_m_l->stop();
    ptr_m_r->stop();
  }
}
 
void wii_buttons_callback(const ev3_wiimote::State& msg)
{
  // wiimote::State::MSG_BTN_1]
  if (msg.nunchuk_joystick_raw[CWIID_X] > 0) 
  {
      ptr_m_l->set_duty_cycle_sp(msg.nunchuk_joystick_raw[CWIID_Y]);
      ptr_m_r->set_duty_cycle_sp(msg.nunchuk_joystick_raw[CWIID_Y]-25);
  }
  else if (msg.nunchuk_joystick_raw[CWIID_X] < 0) 
  {   
      ptr_m_l->set_duty_cycle_sp(msg.nunchuk_joystick_raw[CWIID_Y]-25);
      ptr_m_r->set_duty_cycle_sp(msg.nunchuk_joystick_raw[CWIID_Y]);
  }  

  if (msg.nunchuk_joystick_raw[CWIID_Y] > 0) 
  {
      ptr_m_l->run_forever();
      ptr_m_r->run_forever();
  }
  else
  {
      ptr_m_l->stop();
      ptr_m_r->stop();
  } 
}

int main(int argc, char **argv)
{
  ros::init(argc, argv,"ev3_wii_listener");

  // Motor initiallization
  ROS_INFO("ev3_wii motors initialization");
  status_motor = 0;
  large_motor _motor_left(OUTPUT_A);
  large_motor _motor_right(OUTPUT_B);
  ptr_m_l = &_motor_left;
  ptr_m_r = &_motor_right;
  _motor_left.set_duty_cycle_sp(75);
  _motor_right.set_duty_cycle_sp(75);
  
  ros::NodeHandle node_obj;
  ROS_INFO("ev3_wii initializaion");
  ros::Subscriber number_subscriber = node_obj.subscribe("/wiimote/state",10,wii_buttons_callback);
  ros::Subscriber number_subscriber_int = node_obj.subscribe("/numbers",10,wii_buttons_callback_int);
  ros::spin();
  
  ROS_INFO("ev3_wii motros stoped");
  _motor_left.stop();
  _motor_right.stop();
  
  return 0;
}
