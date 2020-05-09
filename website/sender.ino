#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/* Définition des Pins CE, CSN et connexion de la LED sur ARDUINO */
#define RF_CE    9
#define RF_CSN   10
#define LED_PIN  6

/* Définition de l'id de l'Arduino */
byte myID = 0x01;
byte pipes[][7] = {"master","slave","idle"};


struct payload_ack_t
{
  char message[15];
};

struct payload_data_t
{
  uint8_t number;
  uint8_t source;
  //char message[15];
};

uint16_t timeout_ttl = 1000;

payload_ack_t answer;
payload_data_t outgoing;

void sendData()
{
  /* Creation data */
  outgoing.source = myID;
  outgoing.number = 25 // sensor data, then use more complex structure that every proble will use
  /* Message visible si la console est ouverte */
  Serial.print("Envoi data \"");
  Serial.print(outgoing.number);
  Serial.println("\"");
  
  /* Retour de la reponse au demandeur */
  radio.stopListening();
  radio.openWritingPipe(pipes[1]);
  delay(10);
  radio.write(&outgoing,sizeof(payload_data_t)+1);
  delay(10);
  radio.startListening();
}

void setup() 
{
  Serial.begin(9600);
  /* Définition de pin de la LED comme SORTIE */
  pinMode(LED_PIN, OUTPUT);
  /* On met le pin à l'état éteind */
  digitalWrite(LED_PIN, LOW);

  printf_begin();
  radio.begin();   
  radio.setAutoAck(1); 
  radio.setRetries(1,3);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();   
}

void loop(void)
{ 
  delay(10000);
  sendData();
  // wait for ACK packet
  while(!radio.available()) {

  }
  if(radio.available()) {
    radio.read(&answer, sizeof(payload_ack_t));
    /* Affichage de la commande demandée dans la console */
    Serial.print("ACK reçue \"");
    Serial.println(answer.message);
  }
  Serial.println("end of loop;")
}
