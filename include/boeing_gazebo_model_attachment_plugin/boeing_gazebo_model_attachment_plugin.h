#ifndef BOEING_GAZEBO_MODEL_ATTACHMENT_PLUGIN_H
#define BOEING_GAZEBO_MODEL_ATTACHMENT_PLUGIN_H

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boeing_gazebo_model_attachment_plugin/Attach.h>
#include <boeing_gazebo_model_attachment_plugin/Detach.h>
#include <ros/ros.h>

#include <gazebo/common/Events.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/TransportTypes.hh>
#include <gazebo/transport/transport.hh>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace gazebo
{

class ModelAttachmentPlugin : public WorldPlugin
{
  public:
    ModelAttachmentPlugin();

    virtual ~ModelAttachmentPlugin();

  protected:
    void Load(physics::WorldPtr world, sdf::ElementPtr sdf);

  private:
    physics::WorldPtr world_;

    bool attachCallback(boeing_gazebo_model_attachment_plugin::Attach::Request& req,
                        boeing_gazebo_model_attachment_plugin::Attach::Response& res);
    bool detachCallback(boeing_gazebo_model_attachment_plugin::Detach::Request& req,
                        boeing_gazebo_model_attachment_plugin::Detach::Response& res);

    void attach(const std::string& joint_name, physics::ModelPtr m1, physics::ModelPtr m2, physics::LinkPtr l1,
                physics::LinkPtr l2);
    void detach(const std::string& joint_name, physics::ModelPtr m1, physics::ModelPtr m2);

    ros::NodeHandle nh_;
    ros::ServiceServer attach_srv_;
    ros::ServiceServer detach_srv_;
};
}  // namespace gazebo

#endif
