#include "UdpAudioClient.h"


UdpAudioClient::UdpAudioClient()
{

}

UdpAudioClient::~UdpAudioClient()
{
    if(!mEndpoint) delete mEndpoint;
    if(!mSocket)
    {
      mSocket->close();
      delete mSocket;
    }
}

bool UdpAudioClient::init(std::string & iHost, std::string & iPort)
{
  if(!std::regex_match(iHost,std::regex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}")))
  {
    std::cout << "Host address invalid!" << std::endl;
    return false;
  }

  if(!std::regex_match(iPort,std::regex("\\d{1,5}")))
  { 
    std::cout << "Port number invalid!" << std::endl;
    return false;
  }

  mHost = iHost;
  mPort = stoi(iPort);

  boost::asio::ip::address wIpAddress = boost::asio::ip::address::from_string(mHost);
  mEndpoint = new boost::asio::ip::udp::endpoint(wIpAddress, mPort);
  mSocket = new boost::asio::ip::udp::socket(mService);
  mSocket->open(boost::asio::ip::udp::v4());

  std::cout << "Input some text..." << std::endl;

  return true;
}

void UdpAudioClient::send(std::string & iTextToSend)
{
  //iTextToSend.append('\0');
  mSocket->send_to(boost::asio::buffer(iTextToSend),*mEndpoint);

}