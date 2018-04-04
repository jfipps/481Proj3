#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>

using namespace std;

ros::Subscriber pose_subscriber;	// to determine the position for turning the robot in an absolute orientation --> in the setDesiredOrientation fn
turtlesim::Pose turtlesim_pose;
ros::master::V_TopicInfo master_topics;

const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool cloclwise);	//this will rotate the turtle at specified angle from its current angle
double degrees2radians(double angle_in_degrees);		
double setDesiredOrientation(double desired_angle_radians);	//this will rotate the turtle at an absolute angle, whatever its current angle is
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);	//Callback fn everytime the turtle pose msg is published over the /turtle1/pose topic.
void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance);	//this will move robot to goal
double getDistance(double x1, double y1, double x2, double y2);
void getPosition();


int main(int argc, char **argv)
{
	// Initiate new ROS node named "talker"
	ros::init(argc, argv, "turtlesim_cleaner");
	ros::NodeHandle n;
	//velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);	//call poseCallback everytime the turtle pose msg is published over the /turtle1/pose topic.
	ros::Rate loop_rate(0.5);

	//	/turtle1/cmd_vel is the Topic name
	//	/geometry_msgs::Twist is the msg type 
	ROS_INFO("\n\n\n ********START TESTING*********\n");
	ros::master::getTopics(master_topics);

	for (ros::master::V_TopicInfo::iterator it = master_topics.begin() ; it != master_topics.end(); it++) {
    		const ros::master::TopicInfo& info = *it;
    		std::cout << "Topic : " << it - master_topics.begin() << ": " << info.name << " -> " << info.datatype << std::endl;
	}
	while(ros::ok()) {

		loop_rate.sleep();
		ros::spinOnce();
		getPosition();
	}

	return 0;
}


void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
}


double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

void getPosition() {
	ROS_INFO("Turtle X: %f", turtlesim_pose.x);
	ROS_INFO("Turtle Y: %f", turtlesim_pose.y);
}


