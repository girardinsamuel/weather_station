//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 10);  // CE, CSN

//address through which two modules communicate.
const uint8_t addresses[][7] = {"master", "node"};

void setup()
{
  radio.begin();
  radio.printDetails();
  //set the address
  radio.openWritingPipe(addresses[0]);
  
  //Set module as transmitter
  radio.stopListening();

}

void loop()
{
  //Send message to receiver
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  Serial.print('Sent : ');
  Serial.println(text);
  delay(1000);
}