#include "Sidetone.h"
#include <conio.h>

int main()
{
  Sidetone sidetone;

  bool button = false;

  while(1)
  {
    _getch();
    button = !button;
    sidetone.enable(button);
  }
}
