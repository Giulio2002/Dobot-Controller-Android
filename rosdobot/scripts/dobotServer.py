#!/usr/bin/env python
from server import *

if __name__ == "__main__":
    #test
    rospy.init_node('dobot_server')
    print "testing..."
    #setStartPose()
    print "done server started"
    #threads initilization
    T_setPTPCMD = Thread(target = setPTP)
    T_setPTPCMD.setDaemon(True);
    T_setPTPCMD.start()
    print " "
    T_setcpCMD = Thread(target = set_cp)
    T_setcpCMD.setDaemon(True);
    T_setcpCMD.start()
    print " "
    T_setParameters = Thread(target = set_parameters)
    T_setParameters.setDaemon(True);
    T_setParameters.start()
    device.get_pose();
    #loop
    while not rospy.is_shutdown():
        pass
