# ConsoleVoiP
Simple C++ VoiP console application

At this point, the repo only contains a proof-of-concept. TCPServer opens a socket listening on port 2112 and TCPClient requires a IP address to write to.

* TCP exchanges handled with [boost::asio](https://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html)
* Audio handled with [SFML](https://www.sfml-dev.org/tutorials/2.4/#audio-module)
