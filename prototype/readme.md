# Context-aware WAX.
## Aims
* The device location on the body also plays an important role in the accuracy of an activity
recognising algorithm. It is shown that one of the most commonly used location on the body is the
waist since it is very close to the centre of mass of human body thus displaying the most
representative part of human movement.
* The project aims to produce an algorithm optimised for devices worn at the lower back
position for the AX9 Inertial Measurement Unit developed under OpenMovement platform that is
capable of detecting simple activity transitions including postural transitions (standing to walking)
and lying transitions (lying to standing) using accelerometer tri-axis data in a computationally
efficient manner that could be applied to the devices.
* Using this algorithm, devices will be able to perform contextual triggering: turning on
gyroscope when a postural transition is detected only. This will help in saving a lot of power when
the user is inactive.

## Objectives
###Objective 1:
To produce an algorithm optimised for the lower back position that is capable of detecting
two postural transitions: Sitting to standing and vice versa, standing to walking and vice versa
given a three dimensional acceleration data as primary input. The algorithm must be:
* Accurate: The algorithm should be able to work accurately at an acceptable level.
* Computationally efficient: The algorithm should be efficient, elegant that does not put
a high pressure on the device's processor.
* Power efficient (Device implementation): The algorithm should be able help devices
operate over a substantial amount of times that is perceptibly longer than devices under context nonaware implementations..

###Objective 2:
To demonstrate the accuracy, efficiency and feasibility of the produced algorithm by:
* To produce an implementation on a simulated environment in which the targeted
device's specifications are taken into consideration.
* To produce an implementation on the targeted device that dynamically turns on the
gyroscope when a postural transition (Sitting to standing or Standing to walking) is detected by the
algorithm and turns off the gyroscope when users are inactive.
* To performs experiments and evaluation on the produced algorithm and
implementations.
