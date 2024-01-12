// Copyright 2018 Boeing
#include <boeing_gazebo_model_attachment_plugin/boeing_gazebo_model_attachment_plugin.h>

#include <gazebo/physics/World.hh>
#include <sdf/sdf.hh>
#include <string>
#include <vector>

namespace gazebo
{

ModelAttachmentPlugin::ModelAttachmentPlugin()
{
}

ModelAttachmentPlugin::~ModelAttachmentPlugin()
{
}

// cppcheck-suppress unusedFunction
void ModelAttachmentPlugin::Load(physics::WorldPtr world, sdf::ElementPtr)
{
    world_ = world;

    nh_ = ros::NodeHandle("~");

    attach_srv_ = nh_.advertiseService("attach", &ModelAttachmentPlugin::attachCallback, this);
    detach_srv_ = nh_.advertiseService("detach", &ModelAttachmentPlugin::detachCallback, this);
}

bool ModelAttachmentPlugin::attachCallback(boeing_gazebo_model_attachment_plugin::Attach::Request& req,
                                           boeing_gazebo_model_attachment_plugin::Attach::Response& res)
{
    ROS_INFO_STREAM("Received request to attach model: '" << req.model_name_1 << "' to '" << req.model_name_2);

    // block any other physics pose updates
    boost::recursive_mutex::scoped_lock plock(*(world_->Physics()->GetPhysicsUpdateMutex()));

    const std::string& model_1_name = req.model_name_1;
    const std::string& model_2_name = req.model_name_2;
    const gazebo::physics::Model_V models = world_->Models();

    auto m1 = std::find_if(models.begin(), models.end(), [&model_1_name](const gazebo::physics::ModelPtr& ptr) {
        return ptr->GetName() == model_1_name;
    });
    if (m1 == models.end())
    {
        const std::string error_msg = "Could not find model " + req.model_name_1;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    auto m2 = std::find_if(models.begin(), models.end(), [&model_2_name](const gazebo::physics::ModelPtr& ptr) {
        return ptr->GetName() == model_2_name;
    });
    if (m2 == models.end())
    {
        const std::string error_msg = "Could not find model " + req.model_name_2;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    physics::LinkPtr l1 = (*m1)->GetLink(req.link_name_1);
    if (l1 == nullptr)
    {
        const std::string error_msg = "Could not find link " + req.link_name_1 + " on model " + req.model_name_1;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    physics::LinkPtr l2 = (*m2)->GetLink(req.link_name_2);
    if (l2 == nullptr)
    {
        const std::string error_msg = "Could not find link " + req.link_name_2 + " on model " + req.model_name_2;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    try
    {
        attach(req.joint_name, *m1, *m2, l1, l2);
    }
    catch (const std::exception& e)
    {
        const std::string error_msg = "Failed to detach: " + std::string(e.what());
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    res.success = true;
    return true;
}

// cppcheck-suppress constParameterCallback
bool ModelAttachmentPlugin::detachCallback(boeing_gazebo_model_attachment_plugin::Detach::Request& req,
                                           boeing_gazebo_model_attachment_plugin::Detach::Response& res)
{
    ROS_INFO_STREAM("Received request to detach model: '" << req.model_name_1 << "' from '" << req.model_name_2);

    const std::string& model_1_name = req.model_name_1;
    const std::string& model_2_name = req.model_name_2;
    const gazebo::physics::Model_V models = world_->Models();

    auto m1 = std::find_if(models.begin(), models.end(), [&model_1_name](const gazebo::physics::ModelPtr& ptr) {
        return ptr->GetName() == model_1_name;
    });
    if (m1 == models.end())
    {
        const std::string error_msg = "Could not find model " + req.model_name_1;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    auto m2 = std::find_if(models.begin(), models.end(), [&model_2_name](const gazebo::physics::ModelPtr& ptr) {
        return ptr->GetName() == model_2_name;
    });
    if (m2 == models.end())
    {
        const std::string error_msg = "Could not find model " + req.model_name_2;
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    try
    {
        detach(req.joint_name, *m1, *m2);
    }
    catch (const std::exception& e)
    {
        const std::string error_msg = "Failed to detach: " + std::string(e.what());
        ROS_ERROR_STREAM(error_msg);
        res.message = error_msg;
        res.success = false;
        return true;
    }

    res.success = true;
    return true;
}

void ModelAttachmentPlugin::attach(const std::string& joint_name, physics::ModelPtr m1, physics::ModelPtr m2,
                                   physics::LinkPtr l1, physics::LinkPtr l2)
{
    if (m1 == nullptr)
        throw std::runtime_error("Model 1 is null");

    if (m2 == nullptr)
        throw std::runtime_error("Model 2 is null");

    if (l1 == nullptr)
        throw std::runtime_error("Link 1 is null");

    if (l2 == nullptr)
        throw std::runtime_error("Link 2 is null");

    ignition::math::Pose3d m1wp = m1->WorldPose();
    ignition::math::Pose3d l1rl = l1->RelativePose();
    ignition::math::Pose3d l2rl = l2->RelativePose();
    ignition::math::Pose3d p = (m1wp * l1rl * l2rl.Inverse());

    m2->SetWorldPose(p);

    physics::JointPtr joint = m1->CreateJoint(joint_name, "fixed", l1, l2);

    if (joint == nullptr)
        throw std::runtime_error("CreateJoint returned nullptr");

    m1->AddChild(m2);
}

void ModelAttachmentPlugin::detach(const std::string& joint_name, physics::ModelPtr m1, physics::ModelPtr m2)
{
    if (m1 == nullptr)
        throw std::runtime_error("Model 1 is null");

    if (m2 == nullptr)
        throw std::runtime_error("Model 2 is null");

    physics::JointPtr joint = m1->GetJoint(joint_name);
    if (joint == nullptr)
        throw std::runtime_error("No joint on model " + m1->GetName() + " by name " + joint_name);

    bool success = m1->RemoveJoint(joint_name);

    if (!success)
        throw std::runtime_error("Unable to remove joint from model");

    m2->SetParent(m1->GetWorld()->ModelByName("default"));

    // We need to flush the children vector of the parent
    // Calling m1->RemoveChild(boost::dynamic_pointer_cast<physics::Entity>(m2)); will also destroy the child
    physics::Base_V temp_child_objects;
    unsigned int children_count = m1->GetChildCount();
    for (unsigned int i = 0; i < children_count; i++)
    {
        if (m1->GetChild(i) != m2)
            temp_child_objects.push_back(m1->GetChild(i));
    }

    m1->RemoveChildren();

    for (const auto& obj : temp_child_objects)
    {
        m1->AddChild(obj);
    }

    return;
}

GZ_REGISTER_WORLD_PLUGIN(ModelAttachmentPlugin)
}  // namespace gazebo
