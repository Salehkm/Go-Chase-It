#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{

    ROS_INFO_STREAM("This is drive robot");


// Define a service object to pass the velocity valuse 
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    int position ,right,left,center,stop,index;
     right=2;
     center=1;
     left=0;
     stop=3;
    int white_pixel = 255;


    for (int i = 0; i < img.height * img.step; i++) {
        if (img.data[i]== white_pixel) {
            index =i%img.step;
            if (index<img.step/3){
            position=left; //Left
    ROS_INFO_STREAM("left");
            break;
}else if (index<2*img.step/3 ){
            position=center; //Center
    ROS_INFO_STREAM("center");
            break;
}else if( index<img.step){
            position=right; //Right
    ROS_INFO_STREAM("right");
            break;
}else{
            position=stop; //Right
    ROS_INFO_STREAM("stop");
            break;
}
        }
    }

      if(position==left){
          drive_robot(0.0,0.5);
          }else if(position == right){
          drive_robot(0.0,-0.5);
            }else if(position == center){
          drive_robot(0.5,0.0);
}else{
          drive_robot(0.0,0.0);
}
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}

