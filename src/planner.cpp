#include <iostream>
#include <fstream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>


using namespace ros;
using namespace std;

class Planner{
	private:
		Publisher pub;
		Subscriber sub;
		geometry_msgs::Twist command;
		double stop_dist;
		int angle;
		//callback function here
		void sub_callback(const sensor_msgs::LaserScan::ConstPtr& scan);
	//	my_planner.getParam("file_name", file_name);


	public:
		NodeHandle my_planner;
		Planner();


};

Planner::Planner(){
	pub = my_planner.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 10);
	sub = my_planner.subscribe("/scan", 1000, &Planner::sub_callback, this);
	command.linear.x = 0.1;
	command.linear.y = 0.0;
	command.linear.z = 0.0;
	command.angular.x = 0.0;
	command.angular.y = 0.0;
	command.angular.z = 0.0;
}
void Planner::sub_callback(const sensor_msgs::LaserScan::ConstPtr& scan){
	double min_dist;
	my_planner.getParam("stop_dist", stop_dist);
	//ROS_INFO("THIS IS THE STOP DISTANCE %f", stop_dist);

	min_dist = 10000000000;
	angle = 0;
	for(int i = 0; i < scan->ranges.size(); i++){
		if(min_dist > scan->ranges[i]){
			min_dist = scan->ranges[i];
			angle = i;
		}
	}

	//if(angle < 125){
	//	command.angular.z = 1;
	//}
	//if(angle > 125){
	//	command.angular.z = -1;
	//}

	if(min_dist < stop_dist){
	//	command.angular.z = -10;
		command.linear.x = -.1;

	}
	if(min_dist > stop_dist){
	//	command.angular.z = 0;
		command.linear.x = .1;
	}
	pub.publish(command);
}

void odomCallback(const nav_msgs::Odometry odom){
	ROS_INFO("LOGGING ODOMETRY %f", odom.pose.pose.position.x);
	ofstream myfile;
	myfile.open("myOdom.txt", ios::app);
	myfile << odom.pose.pose.position.x <<"," << odom.pose.pose.position.y <<"\n" ; //<< "," << odom.pose.pose.orientation << "\n";
	myfile.close();

}

int main(int argc, char **argv) {
	// Initialize the subscriber node
	init(argc, argv, "my_planner");
	Planner pub;
	Subscriber odom;
	odom = pub.my_planner.subscribe("/odom", 1000, odomCallback);
	spin();
}
