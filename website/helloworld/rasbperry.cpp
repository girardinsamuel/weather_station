//Include Libraries
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
using namespace std;

//create an RF24 object
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);

//address through which two modules communicate.
const byte address[6] = "00001";

int main(int argc, char** argv)
{

  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();

  while(1) {
    //Read the data if available in buffer
    if (radio.available())
    {
      char text[32] = {0};
      radio.read(&text, sizeof(text));
      printf(text);
    }
  }
}
