| Distro | CI Status                                                                                                                                                                                                 |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Noetic | [![CI](https://github.com/Boeing/gazebo_model_attachment_plugin/actions/workflows/main.yml/badge.svg?branch=noetic)](https://github.com/Boeing/gazebo_model_attachment_plugin/actions/workflows/main.yml) |
| Humble | [![CI](https://github.com/Boeing/gazebo_model_attachment_plugin/actions/workflows/main.yml/badge.svg?branch=humble)](https://github.com/Boeing/gazebo_model_attachment_plugin/actions/workflows/main.yml) |

# Gazebo Model Attachment Plugin

## Overview

 Once a model has been spawned within Gazebo, it can often be necessary to add or remove _Links_ at runtime without destroying the model. This can be necessary for simulating actions such as end effector swap, item transport (loading and uploading), or pick and place operations. This package enables such simulations by allowing Models to be attached to each other.

### Assumptions

- The models must have been spawned and contain the specified link names.

### Limitations

- The plugin will not teleport the child model to make the pose of  _model_name_1/link_name_1_ match _model_name_2/link_name_2_. The attachment will instead occur with the pose difference at the time of the service call being maintained.
  - If the user wishes to have zero pose difference a call to the _gazebo/set_link_state_ service can be made.

## Installation

### Dependencies
Due to this [change](https://github.com/ros2/rosidl_python/pull/131) in rosidl_python currently you can not build a package that both generates interfaces and installs a Python package. While we are waiting for the fix to be merged you must build the following branch of rosidl_python locally.

- [rosidl_python - fix-141](https://github.com/knorth55/rosidl_python/tree/fix-141)


#### Building

To build from source, clone the latest version from this repository into your ros workspace and compile the package using colcon

	cd ros_ws/src
	git clone https://github.com/boeing/boeing_gazebo_model_attachment_plugin.git
	cd ../
	rosdep install --from-paths . --ignore-src
	colcon build

## Usage

Add your library path to the `GAZEBO_PLUGIN_PATH`

    export GAZEBO_PLUGIN_PATH=$HOME/ros_ws/build:$GAZEBO_PLUGIN_PATH

Add the plugin to your world file
    
    <sdf version='1.6'>
    <world name='default'>

     <plugin name="model_attachment" filename="libboeing_gazebo_model_attachment_plugin_lib.so"></plugin>
        
        <include>
          <uri>model://ground_plane</uri>
        </include>

        <include>
          <uri>model://sun</uri>
        </include>

        <plugin name="factory" filename="libfactory.so"/>
      </world>
    </sdf>

Run Gazebo

    gazebo ~/gazebo_plugin_tutorial/test.world

#### Provided Services

* **`attach`** (boeing_gazebo_model_attachment_plugin/Attach.srv)

	Creates a joint between two links

		ros2 service call /gazebo/attach

  ##### Parameters

  * **`joint_name`** (string)
  The name of the joint to be created

  * **`model_1`** (string) -
	The name of the parent model.

  * **`link_1`** (string) -
  The name of the link on the parent model.
  
  * **`model_2`** (string) -
	The name of the child model.

  * **`link_2`** (string) -
  The name of the link on the child model.

  ##### Response
  * **`success`** (bool)
  True if the operation is successful
  * **`message`** (string)
  Contains error message if operation fails.
<br>

* **`detach`** (boeing_gazebo_model_attachment_plugin/Detach.srv)
  removes a joint between two links.

		ros2 service call /gazebo/detach

  ##### Parameters
  * **`joint_name`** (string)
  The name of the joint to be created

  * **`model_1`** (string) -
	The name of the parent model.
  
  * **`model_2`** (string) -
	The name of the child model.

  ##### Response
  * **`success`** (bool)
  True if the operation is successful
  * **`message`** (string)
  Contains error message if operation fails.
# Authors
The Boeing Company

     	Beau Colley-Allerton
     	Jason Cochrane

# License

This package is released under the Apache 2.0 License

# Contributing

Any contribution that you make to this repository will
be under the Apache-2.0 License, as dictated by that
[license](http://www.apache.org/licenses/LICENSE-2.0)

To contribute, issue a PR and @brta-jc (jason.cochrane@boeing.com)
