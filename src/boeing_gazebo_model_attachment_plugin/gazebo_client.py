#!/usr/bin/env python3

import logging

import rospy

from boeing_gazebo_model_attachment_plugin.srv import Attach, AttachRequest, AttachResponse
from boeing_gazebo_model_attachment_plugin.srv import Detach, DetachRequest, DetachResponse

logger = logging.getLogger(__name__)


class GazeboModelAttachmentClient(object):
    def __init__(self):
        self.__attach_srv = rospy.ServiceProxy(
            name='/gazebo/attach',
            service_class=Attach
        )

        self.__detach_srv = rospy.ServiceProxy(
            name='/gazebo/detach',
            service_class=Detach
        )

        self.__attach_srv.wait_for_service(timeout=20)
        self.__detach_srv.wait_for_service(timeout=20)

    def attach(self, joint_name, model_name_1, link_name_1, model_name_2, link_name_2):
        # type: (str, str, str, str, str) -> None
        response = self.__attach_srv.call(
            AttachRequest(
                joint_name=joint_name,
                model_name_1=model_name_1,
                link_name_1=link_name_1,
                model_name_2=model_name_2,
                link_name_2=link_name_2
            )
        )  # type: AttachResponse
        assert isinstance(response, AttachResponse)

        if response.success:
            logger.info('Successfully attached models by adding joint {}'.format(joint_name))
        else:
            raise Exception('Failed to attach models: {}<--{}-->{} - {}'
                            .format(model_name_1, joint_name, model_name_2, response.message))

    def detach(self, joint_name, model_name_1, model_name_2):
        # type: (str, str, str) -> None
        response = self.__detach_srv.call(
            DetachRequest(
                joint_name=joint_name,
                model_name_1=model_name_1,
                model_name_2=model_name_2
            )
        )  # type: DetachResponse
        assert isinstance(response, DetachResponse)

        if response.success:
            logger.info('Successfully detached models by removing joint {}'.format(joint_name))
        else:
            raise Exception('Failed to detach models: {}<--{}-->{} - {}'
                            .format(model_name_1, joint_name, model_name_2, response.message))
