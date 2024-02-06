#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
using namespace std;
void calGrid(int x1,int x2,int y1,int y2,nav_msgs::OccupancyGrid& grid){
    for (int row = y1 + (grid.info.height / 2);
       row <= y2 + (grid.info.height / 2); ++row) {
    for (int col = x1 + (grid.info.width / 2);
         col <= x2 + (grid.info.width / 2); ++col) {
      int index = row * grid.info.width + col;
      grid.data[index] = 100;  // 设置为占据状态
    }
  }
}
int main(int argc, char** argv) {
  ros::init(argc, argv, "map_pub");
  ros::NodeHandle nh;
  int obs1_x1, obs1_y1, obs1_x2, obs1_y2;
  int obs2_x1,obs2_y1,obs2_x2,obs2_y2;
  int obs3_x1,obs3_y1,obs3_x2,obs3_y2;
  double new_x, new_y;
  nh.param("/map_pub/obs1_x1", obs1_x1, 10);
  nh.param("/map_pub/obs1_y1", obs1_y1, -10);
  nh.param("/map_pub/obs1_x2", obs1_x2, 20);
  nh.param("/map_pub/obs1_y2", obs1_y2, 10);
    nh.param("/map_pub/obs2_x1", obs2_x1, 30);
  nh.param("/map_pub/obs2_y1", obs2_y1, -10);
  nh.param("/map_pub/obs2_x2", obs2_x2, 40);
  nh.param("/map_pub/obs2_y2", obs2_y2, 10);
    nh.param("/map_pub/obs3_x1", obs3_x1, 150);
  nh.param("/map_pub/obs3_y1", obs3_y1, -10);
  nh.param("/map_pub/obs3_x2", obs3_x2, 60);
  nh.param("/map_pub/obs3_y2", obs3_y2, 10);
  nh.param("/map_pub/new_x", new_x, 5.0);
  nh.param("/map_pub/new_y", new_y, 3.0);
  ros::Publisher map_pub =
      nh.advertise<nav_msgs::OccupancyGrid>("map", 1, true);
  ros::Publisher odom_pub =
      nh.advertise<nav_msgs::Odometry>("Odometry", 1, true);

  // 填充地图参数
  nav_msgs::OccupancyGrid grid;
  grid.header.frame_id = "camera_init";
  grid.info.resolution = 0.1;  // 栅格分辨率（单位：米/栅格）
  grid.info.width = 300;       // 地图宽度（单位：栅格）
  grid.info.height = 300;      // 地图高度（单位：栅格）
  grid.info.origin.position.x = -15;  // 地图原点x坐标（单位：米）
  grid.info.origin.position.y = -15;  // 地图原点y坐标（单位：米）

  // 填充地图数据，这里将地图视为一个全空的栅格地图
  grid.data.resize(grid.info.width * grid.info.height, 0);
  nav_msgs::OccupancyGrid grid_new = grid;
  // 填充地图数据，将矩形区域置为占据状态
  calGrid(obs1_x1,obs1_x2,obs1_y1,obs1_y2,grid);
  calGrid(obs2_x1,obs2_x2,obs2_y1,obs2_y2,grid);
  calGrid(obs3_x1,obs3_x2,obs3_y1,obs3_y2,grid);

  nav_msgs::Odometry odom, odom_new;
  odom.header.frame_id = "camera_init";
  odom.child_frame_id = "body";
  odom.pose.pose.position.x = 0;
  odom.pose.pose.position.y = 0;
  odom_new = odom;
  odom_new.pose.pose.position.x = new_x;
  odom_new.pose.pose.position.y = new_y;
  ros::Rate loop_rate(0.5);  // 发布频率为1Hz

  while (ros::ok()) {
    grid.header.seq = ros::Time::now().toSec();
    odom.header.seq = ros::Time::now().toSec();
    map_pub.publish(grid);
    odom_pub.publish(odom);
    // static int i = 0;
    // 
    // if (i % 2 == 0) {
    //   map_pub.publish(grid);
    //   odom_pub.publish(odom_new);
    //   i++;
    // } else if (i % 3 == 0) {
    //   map_pub.publish(grid_new);
    //   odom_pub.publish(odom_new);
    //   i++;
    // } else {
    //   map_pub.publish(grid);
    //   odom_pub.publish(odom);
    //   i++;
    // }
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
