/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#include <kdl_parser/kdl_parser.hpp>

#include "old_robot_state_publisher/old_robot_state_publisher.h"
#include "old_robot_state_publisher/joint_state_listener.h"

using namespace std;
using namespace ros;
using namespace KDL;
using namespace old_robot_state_publisher;

int main(int argc, char** argv)
{
  // Initialize ros
  ros::init(argc, argv, "old_robot_state_publisher");
  NodeHandle node;

  ///////////////////////////////////////// begin deprecation warning
  std::string exe_name = argv[0];
  std::size_t slash = exe_name.find_last_of("/");
  if (slash != std::string::npos) {
    exe_name = exe_name.substr(slash + 1);
  }
  if (exe_name == "state_publisher") {
    ROS_WARN("The 'state_publisher' executable is deprecated. Please use 'robot_state_publisher' instead");
  }
  ///////////////////////////////////////// end deprecation warning

  // gets the location of the robot description on the parameter server
  urdf::Model model;
  if (!model.initParam("robot_description"))
    return 1;

  KDL::Tree tree;
  if (!kdl_parser::treeFromUrdfModel(model, tree)) {
    ROS_ERROR("Failed to extract kdl tree from xml robot description");
    return 1;
  }

  MimicMap mimic;

  for(std::map< std::string, urdf::JointSharedPtr >::iterator i = model.joints_.begin(); i != model.joints_.end(); i++) {
    if(i->second->mimic) {
      mimic.insert(make_pair(i->first, i->second->mimic));
    }
  }

  JointStateListener state_publisher(tree, mimic, model);
  ros::spin();

  return 0;
}
