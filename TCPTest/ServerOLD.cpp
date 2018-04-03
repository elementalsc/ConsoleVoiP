#include <mutex>
#include <deque>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/asio.hpp>
#include <SFML/Audio.hpp>

#define PORT 2112
#define BUFFER_SIZE 4096

////////////////////////////////////////////////////////////////////////////////////////////////////////
//      HEADERS 
////////////////////////////////////////////////////////////////////////////////////////////////////////

class Server {

  public:
    // Ctor
    Server(boost::asio::io_service &iService,const short iPort);
    
  private:
    // Methods
    void doAccept();

    // Variables
    boost::asio::ip::tcp::acceptor mAcceptor;
    boost::asio::ip::tcp::socket mSocket;
    unsigned int mSessionCount;
};

class Session : public std::enable_shared_from_this<Session> {
    
  public:
    // Ctor
    Session(boost::asio::ip::tcp::socket && iSocket, Server * iServerPtr, unsigned int && iSessionId);
    ~Session();

    // Methods
    void start();

  private:
    // Methods
    void doRead();
    void attemptFileLoad();

    // Variables
    boost::asio::ip::tcp::socket mSocket;
    uint8_t mData[BUFFER_SIZE];

    Server *mServerPtr;
    unsigned int mSessionId;
    mutable std::mutex mSessionMutex;
    std::deque<std::tuple<size_t,unsigned char*>> mSessionChunkQueue;
};




////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SERVER
////////////////////////////////////////////////////////////////////////////////////////////////////////

Server::Server(boost::asio::io_service & service, const short port) : 
  mAcceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), 
  mSocket(service),
  mSessionCount(0)
{
    mAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address());
    doAccept();
}

void Server::doAccept() {
    mAcceptor.async_accept(mSocket, [this](boost::system::error_code error) {
        if (!error) std::make_shared<Session>(std::move(mSocket), this, std::move(mSessionCount++))->start();
        doAccept();
    });
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SESSION 
////////////////////////////////////////////////////////////////////////////////////////////////////////

Session::Session(boost::asio::ip::tcp::socket &&iSocket, Server *iServerPtr, unsigned int && iSessionId) : 
  mSocket(std::move(iSocket)), 
  mServerPtr(iServerPtr),
  mSessionId(iSessionId) {
  std::cout << "Session " << mSessionId << " started :  " << mSocket.local_endpoint().address().to_string() << ":" << mSocket.local_endpoint().port() << std::endl;
}

Session::~Session(){
  std::cout << "Session " << mSessionId << " closed  :  " << mSocket.local_endpoint().address().to_string() << ":" << mSocket.local_endpoint().port() << std::endl;
}

void Session::start() { 
  doRead(); 
}

void Session::doRead() {

  std::shared_ptr<Session> self(shared_from_this());

  mSocket.async_read_some(
    boost::asio::buffer(mData, BUFFER_SIZE),
    [this, self](boost::system::error_code error, size_t iBytesReceived) {
      if (error) return;
      
      doRead();

      if(iBytesReceived > 0) {
        std::lock_guard<std::mutex> lock(mSessionMutex);
        mSessionChunkQueue.push_back(std::make_tuple(static_cast<size_t>(iBytesReceived), mData));
      }
    attemptFileLoad();
  });
}


void Session::attemptFileLoad(){
  
  std::fstream outputFile;
  outputFile.open("yolo.jpg", std::ios::out | std::ios::binary | std::ios::app);
  
  while(mSessionChunkQueue.size()) {    
    outputFile.write(reinterpret_cast<const char*>(std::get<1>(mSessionChunkQueue.front())),std::get<0>(mSessionChunkQueue.front()));
    {
      std::lock_guard<std::mutex> lock(mSessionMutex);
      mSessionChunkQueue.pop_front();
    }
  }

  outputFile.close();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//      MAIN 
////////////////////////////////////////////////////////////////////////////////////////////////////////
// int main()
// {
//   boost::asio::io_service ioService;
// 
//   std::cout << "Starting server..." << std::endl;
// 
//   Server mServer(ioService, PORT);
// 
//   std::thread serviceThread([&](){ioService.run();}); 
// 
//   serviceThread.join();
// }
// 
// 
// 


