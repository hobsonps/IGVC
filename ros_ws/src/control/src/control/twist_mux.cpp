/**
 * @file twist_mux.cpp
 * @class TwistMux
 * @brief The implementation file for the TwistMux class
 */

#include <twist_mux.h>

TwistMux::TwistMux()
{
    //Ensure that the robot starts in a safe standby mode
    current_mode = Control::standby;
    stopRobot();

    //Setup publishers and subscribers
    joy_sub = nh.subscribe<sensor_msgs::Joy>(JOY_TOPIC, 1, &TwistMux::joyCallback, this);

    twist_pub = nh.advertise<geometry_msgs::Twist>(CONTROL_TOPIC, 1);
}

void TwistMux::update()
{
    twist_pub.publish(cmd_vel);
}

void TwistMux::joyCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
    if(msg->buttons[B_BUTTON] == 1)
    {
        current_mode = Control::standby;
        stopRobot();
        twist_sub = nh.subscribe<geometry_msgs::Twist>(AUTO_TOPIC, 1, &TwistMux::twistCallback, this);
    }
    else if(msg->buttons[A_BUTTON] == 1)
    {
        current_mode = Control::teleop;
        stopRobot();
        twist_sub = nh.subscribe<geometry_msgs::Twist>(TELEOP_TOPIC, 1, &TwistMux::twistCallback, this);
    }
    else if(msg->buttons[Y_BUTTON] == 1)
    {
        current_mode = Control::autonomous;
        stopRobot();
        twist_sub.shutdown();
    }
}

void TwistMux::twistCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    cmd_vel = *msg;
}

void TwistMux::stopRobot()
{
    cmd_vel = geometry_msgs::Twist();
}

