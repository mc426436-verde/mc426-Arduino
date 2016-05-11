//Programa: Automacao Residencial com Arduino e Ethernet Shield
//Autor: FILIPEFLOP
#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Stepper.h>
//#include <dht11.h>

String readString;

#define STEPS_PER_MOTOR_REVOLUTION 32 
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64 
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);

int  Steps2Take;

int pino_rele1 = 1;
int pino_rele2 = 2;
boolean ligado = true;
boolean ligado_2 = true;

int pino_dht = 4;

int pinopir = 7;  //Pino ligado ao sensor PIR
int acionamento;  //Variavel para guardar valor do sensor

//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 169,254,14,93 };
byte gateway[] = { 169,254,14,92 };
byte subnet[] = { 255, 255, 0, 0 };

EthernetServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);

  pinMode(pinopir, INPUT);   //Define pino sensor como entrada

  //Inicializa Ethernet Shield
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  Serial.println("FILIPEFLOP - Automacao Residencial"); // so I can keep track of what is loaded

  //Desliga os dois reles
  digitalWrite(pino_rele1, HIGH);
  digitalWrite(pino_rele2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*acionamento = digitalRead(pinopir);
  if (acionamento == LOW)  //Sem movimento, mantem led azul ligado
  {
    digitalWrite(pino_rele1, LOW);
  }
  else  //Caso seja detectado um movimento, aciona o led vermelho
  {
    digitalWrite(pino_rele1, HIGH);
  }*/

  //Le o sensor de temperatura.
//  int temp = DHT11.read(pino_dht);
  
    EthernetClient client = server.available();
    if (client) {
      client.println("Bem vindo a Sidsville!");
      Serial.println("Client connected");
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == '\n')
        {
          /*//Controle do rele1
          Serial.println(readString);
          //Liga o Rele 1
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(pino_rele1, LOW);
            Serial.println("Rele 1 Ligado");
            ligado = false;
          }
          else
          {
            //Desliga o Rele 1
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(pino_rele1, HIGH);
              Serial.println("Rele 1 Desligado");
              ligado = true;
            }
          }

          //Controle do rele2
          Serial.println(readString);
          //Liga o Rele 2
          if (readString.indexOf("?2_ligar") > 0)
          {
            digitalWrite(pino_rele2, LOW);
            Serial.println("Rele 2 Ligado");
            ligado_2 = false;
          }
          else
          {
            //Desliga o Rele 2
            if (readString.indexOf("?2_desligar") > 0)
            {
              digitalWrite(pino_rele2, HIGH);
              Serial.println("Rele 2 Desligado");
              ligado_2 = true;
            }
          }*/
          if (readString.startsWith("Open")) {
                  Serial.println(readString);
                  client.println("Äbrindo");
                  /*small_stepper.setSpeed(1);   // SLOWLY Show the 4 step sequence 
                  Steps2Take  =  4;  // Rotate CW
                  small_stepper.step(Steps2Take);
                  delay(2000);
                
                  Steps2Take  =  STEPS_PER_OUTPUT_REVOLUTION / 2;  // Rotate CW 1/2 turn
                  small_stepper.setSpeed(100);   
                  small_stepper.step(Steps2Take);
                  delay(1000);
                  */
                  Steps2Take  =  - STEPS_PER_OUTPUT_REVOLUTION * 2;  // Rotate CCW 1/2 turn  
                  small_stepper.setSpeed(700);  // 700 a good max speed??
                  small_stepper.step(Steps2Take);
                  delay(2000);
          } else if (readString.startsWith("Close")) {
                  Serial.println(readString);
                  client.println("Fechando");
                  Steps2Take  =  STEPS_PER_OUTPUT_REVOLUTION * 2;  // Rotate CCW 1/2 turn  
                  small_stepper.setSpeed(700);  // 700 a good max speed??
                  small_stepper.step(Steps2Take);
                  delay(2000);
          }
          readString = "";
          delay(1);
          client.stop();
        }
      }
    }
    }
}
