#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/String.h>


using namespace ros;


class Planner{
	private:
		NodeHandle my_planner;
		Publisher pub;
		Subscriber sub;
		geometry_msgs::Twist command;
		double stop_dist;
		//callback function here
		void sub_callback(const sensor_msgs::LaserScan::ConstPtr& scan);

	public:
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
	ROS_INFO("THIS IS THE STOP DISTANCE %f", stop_dist);

	min_dist = 10000000000;
	for(int i = 0; i < scan->ranges.size(); i++){
		if(min_dist > scan->ranges[i]){
			min_dist = scan->ranges[i];
		}
	}

	if(min_dist< stop_dist){
		command.linear.x = 0;
	}
	else{
		command.linear.x = .1;
	}
	pub.publish(command);
}

int main(int argc, char **argv) {
	// Initialize the subscriber node
	init(argc, argv, "my_planner");
	Planner pub;
	spin();
}
