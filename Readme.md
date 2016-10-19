ev3_wiimote
===========
It is my first project about ros and ev3dev. It is only a tutorial project, in fact it is based on the talker/listener ros tutorial.
I used this project to learn about:
- ros programming (listener and publisher nodes).
- ev3dev programming (large motor controller).
- use of external libraries (libcwiid).

Environment setup
----------------
I created an ev3dev image including ros according to moriarty's project "https://github.com/moriarty/ros-ev3" (thanks a lot for this project).  But I made a little modification in order to run ros_core on ev3dev (view issue #2 of moriarty's project).
After that, I used libcwiid in order to connect ev3 and wiimote.  Ros already has a wimote node but I have not to use it because it kept busy all CPU time.

Source code
-----------
The source code of this project has two ros nodes:
- One node catch wiimote commands and publish them to ros.
- The other node receives the wiimote commands and controls the ev3 motors.

