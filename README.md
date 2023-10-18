| Distro | CI Status |
| ------ | --------- |
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

- None

### Installation from Packages

*PENDING APPROVAL ON ROS_DISTRO*

To install all packages from this repository as Debian packages use

    sudo apt-get install ros-noetic-boeing-gazebo-model-attachment-plugin
    
Or better, use `rosdep`:

	sudo rosdep install --from-paths src

#### Building

To build from source, clone the latest version from this repository into your ros workspace and compile the package using catkin

	cd ros_ws/src
	git clone https://github.com/boeing/gazebo_model_attachment_plugin.git
	cd ../
	rosdep install --from-paths . --ignore-src
	catkin make

## Usage

Add your library path to the `GAZEBO_PLUGIN_PATH`

    export GAZEBO_PLUGIN_PATH=$HOME/ros_ws/build:$GAZEBO_PLUGIN_PATH

Add the plugin to your world file
    
    <sdf version='1.6'>
    <world name='default'>

     <plugin name="model_attachment" filename="libgazebo_model_attachment_plugin_lib.so"></plugin>
        
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

		ros service call /gazebo_model_attachment_plugin/attach

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

		ros2 service call /gazebo_model_attachment_plugin/detach

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

Copyright 2023 The Boeing Company

Licensed under the Apache License, Version 2.0 (the "License") with the following modification;
you may not use this file except in compliance with the Apache License and the following modification to it:

(Appended as Section 10)

By accepting this software, recipient agrees that the representations, warranties, obligations, and liabilities of The Boeing Company set forth in this software, if any, are exclusive and in substitution for all other all other representations, warranties, obligations, and liabilities of The Boeing Company.
Recipient hereby waives, releases, and renounces all other rights, remedies, and claims (including tortious claims for loss of or damage to property) of recipient against The Boeing Company with respect to this software.
The Boeing Company hereby disclaims all implied warranties, including but not limited to, all implied warranties of merchantability, fitness, course of dealing, and usage of trade.
The Boeing Company shall have no liability, whether arising in contract (including warranty), tort (whether or not arising from the negligence of The Boeing Company), or otherwise, for any loss of use, revenue, or profit, or for any other unspecified direct, indirect, incidental, or consequential damages for anything delivered or otherwise provided by The Boeing Company under this software.

You may obtain a copy of the original, unmodified License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
echo 

# Contributing

Any contribution that you make to this repository will
be under the Modified Apache 2 License, as dictated by that
[license](http://www.apache.org/licenses/LICENSE-2.0):

```
5. Submission of Contributions. Unless You explicitly state otherwise,
   any Contribution intentionally submitted for inclusion in the Work
   by You to the Licensor shall be under the terms and conditions of
   this License, without any additional terms or conditions.
   Notwithstanding the above, nothing herein shall supersede or modify
   the terms of any separate license agreement you may have executed
   with Licensor regarding such Contributions.
```

To contribute, issue a PR and @brta-jc (jason.cochrane@boeing.com)
