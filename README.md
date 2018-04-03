# ConsoleVoiP
Simple C++ VoiP console application

At this point, the repo only contains a proof-of-concept. TCPServer opens a socket listening on port 2112 and TCPClient requires an IP address to write to. 

To run, 3rd party dependencies are required:

* TCP exchanges handled with [boost::asio v1.58.1](https://www.boost.org/users/download/)
* Audio handled with [SFML v2.4.2](https://www.sfml-dev.org/download/sfml/2.4.2/) 
