#include <boost/asio.hpp>
#include <deque>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "../TCPClient/AudioRecorder.h"

#define MAX_LENGTH 1024
#define PORT 2112
#define BUFFER_SIZE 4096

using namespace std;

int main()
{


  AudioRecorder recorder;

  std::cout << "Press Enter to start!" << std::endl;
  system("pause");
  recorder.start();

  std::cout << "Press Enter to stop!" << std::endl;
  system("pause");
   recorder.stop();











  /*
  fstream img;
  //img.open("E:\\Musique\\best of tycho.mp3", ios::in | ios::binary | ios::ate);
  img.open("E:\\DnD\\Sword-Coast-Map_HighRes.jpg", ios::in | ios::binary | ios::ate);
  size_t fileSize = img.tellg();
  std::cout << "Filesize is : " << static_cast<unsigned int>(fileSize>>10) << "KB"<< std::endl;

  // setup tcp ip variables
  string mHost = "localhost";
  string mPort = "2112";
  string mMessage = "";
  io_service    mService;
  tcp::socket   mSocket(mService);
  tcp::resolver mResolver(mService);
  tcp::resolver::query  mQuery(mHost, mPort);
  boost::system::error_code ec;


  deque<tuple<size_t,char*>> wByteQueue;

  char* filePtrAsChar = reinterpret_cast<char*>(&img);
  size_t currentFilePos = 0;
  img.seekg(0, std::ios::beg);

  unsigned int blockCount = 0;
  size_t lenToRead = 0;
  size_t readBeg = 0;
  size_t readEnd = 0;

  while(readEnd <= (fileSize-1))
  {
    readBeg = img.tellg();
    //std::cout << "readBeg : " <<  readBeg << std::endl;
    lenToRead = (fileSize - readBeg) < BUFFER_SIZE ? fileSize - readBeg : BUFFER_SIZE;
    char* charBuff = new char[lenToRead];
    img.read(charBuff, lenToRead);
    readEnd = img.tellg();
    //std::cout << "readEnd : " << readEnd << std::endl;

    wByteQueue.push_back(make_tuple(lenToRead, charBuff));
    ++blockCount;
  }

  std::cout << "Number of blocks made : " << blockCount << std::endl;

  try
  {
    connect(mSocket, mResolver.resolve(mQuery),ec);
    
    while(blockCount)
    {
      size_t len = std::get<0>(wByteQueue.front());
      char*  payload = std::get<1>(wByteQueue.front());
      write(mSocket, boost::asio::buffer(payload,len));
      --blockCount;
      wByteQueue.pop_front();
    }
    mSocket.close();
  } 
  catch (std::exception& e)
  {
    std::cerr << "Exception :" << std::endl << e.what() << "\n";
  }
  
  */
}
