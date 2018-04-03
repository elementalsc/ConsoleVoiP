
#include "TcpAudioClient.h"
#include <conio.h>

int main()
{

  std::string input;
  TcpAudioClient client;

  std::cout << "Enter IP address : ";
    
  do 
  {
    std::cin >> input;
  } while(!client.init(input, "2112"));
  
  std::cout << "Enabled successfully! Press enter to toggle broadcasting..." << std::endl;
    
  bool button = false;

  while(1)
  {
    _getch();
    button = !button;
    client.broadcast(button);
  }
}
