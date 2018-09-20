#include <iostream>
#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
using namespace std;
turtlesim::Pose chaser;
turtlesim::Pose runner;

void changePC(const turtlesim::Pose::ConstPtr& pcinf )
{
	chaser.x = pcinf->x;
	chaser.y = pcinf->y;
	chaser.linear_velocity = pcinf->linear_velocity;
	chaser.angular_velocity = pcinf->angular_velocity;
	chaser.theta = pcinf->theta;

}
void changePR(const turtlesim::Pose::ConstPtr& prinf )
{
	runner.x = prinf->x;
        runner.y = prinf->y;
        runner.linear_velocity = prinf->linear_velocity;
        runner.angular_velocity = prinf->angular_velocity;
        runner.theta = prinf->theta;
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "chaser");
	ros::NodeHandle R;
	ros::NodeHandle C;
	ros::NodeHandle M;
	ros::Subscriber chaser_sub = C.subscribe("/turtle2/pose", 100, changePC);
        ros::Subscriber runner_sub = R.subscribe("/turtle1/pose", 100, changePR);
	ros::Publisher master_pub = M.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 100);
	ros::Rate loop_rate(1);
	geometry_msgs::Twist msgM;
	while (ros::ok())
	{
		cout << "chasing!" << endl; 
		float euclidist = sqrt(pow(runner.x - chaser.x , 2)+pow(runner.y - chaser.y , 2));
		float Sangle = atan2(( chaser.y- runner.y) , (chaser.x -runner.x ));
		if(euclidist>=0.9)
		{
			msgM.linear.x = 0.3 * euclidist;
			msgM.linear.y = 0;
			msgM.linear.z = 0;
			msgM.angular.x = 0;
			msgM.angular.y = 0;
			msgM.angular.z = 0.3 * (Sangle - chaser.theta); 
                        master_pub.publish(msgM);
		}
		else
		{
			msgM.linear.x = 0;
                        msgM.linear.y = 0;
                        msgM.linear.z = 0;
                        msgM.angular.x = 0;
                        msgM.angular.y = 0;
                        msgM.angular.z = 0;
		master_pub.publish(msgM);
                }
		ros::spinOnce();
		loop_rate.sleep();
	}	
        cout << "catch!:D";
	ros::spin();
	return 0;
}
