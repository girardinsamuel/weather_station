#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <json-c/json.h>
using namespace std;

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);

const uint8_t pipes[][7] = {"master","slave","idle"};

struct payload_data_t
{
  uint8_t number;
  uint8_t source;
  char message[15];
};

struct payload_ack_t
{
  uint8_t number;
  char message[15];
};

uint16_t timeout_ttl = 1000;

payload_data_t incoming;
payload_ack_t answer;

int main(int argc, char** argv)
{
  radio.begin();
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(1,3);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
  radio.printDetails();   


  // Infinite loop
  while(true) {

    // if we have a message, read it
    if(radio.available()) {
        radio.read(&incoming, sizeof(payload_data_t));
        /* Affichage de la commande demandée dans la console */
        printf("Data reçue \n");
        printf("%d from source [%d]", incoming.number, incoming.source);

        // then ACK it
        radio.stopListening();
        strncpy(answer.message, "OK from RPI");
        radio.write(&answer, sizeof(payload_ack_t));
        delayMicroseconds(100);
        radio.startListening();
    }

}