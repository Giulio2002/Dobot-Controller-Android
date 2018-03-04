#!/usr/bin/env python

import sys
import rospy
from rosdobot.srv import *

def add_two_ints_client():
    rospy.wait_for_service('setPTP')
    add_two_ints = rospy.ServiceProxy('setPTP', setPTPCMD)
    resp1 = add_two_ints(200,0,0,0)
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    sys.exit(1)

def usage():
    return "%s [x y]"%sys.argv[0]

if __name__ == "__main__":
    add_two_ints_client()
