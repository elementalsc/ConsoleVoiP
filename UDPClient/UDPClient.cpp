#include "UdpAudioClient.h"
#include <conio.h>

int main()
{
  std::string input;
  UdpAudioClient client;

  std::cout << "Enter IP address : ";
  std::string wPort = "2112";

  do 
  {
    std::cin >> input;
  } while(!client.init(input, wPort));

//   bool button = false;
// 
//   while(1)
//   {
//     _getch();
//     button = !button;
//     client.broadcast(button);
//   }

  while(1)
  {
    std::cin >> input;
    client.send(input);
  }

}
