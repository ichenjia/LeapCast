LeapCast
========

A http server for Leap Motion controller. It offers RESTful web APIs for accessing Leap Motion frame data.


In essence, the server use polling to acquire data from Leap Motion once a HTTP request is received. Then it display following data in JSON format:

Gestures : /gestures
Devices : /devices
Controller: /controller
Frame: /frame
Fingers :/fingers
Hands: /hands
Pointables: /pointables
Tools: /tools

An example would be

An AJAX call: http://{an ip address}:{port default is 1234}/gestures to retrieve current gestures list in JSON format.

An example of the use case is:

Suppose machine A and machine B are on the same LAN. 

While A is close to Tv while B is close to the couch where Jimmy sits. Only A can connect to TV (with HDMI port). With LeapCast, a gesture detected on B can be sent to A and consequently controls A. 


