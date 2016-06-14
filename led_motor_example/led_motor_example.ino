#include <SPI.h>
#include <Ethernet.h>
#include <Stepper.h>

const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int ledPin1 =  4;
const int ledPin2 =  13;
const int ledId1 = 1;
const int ledId2 = 33;
const int motorId = 65;
const int stepsPerRevolution = 512;
const int motorSpeed = 40;

int buttonState1 = 0;
int buttonState2 = 0;
boolean buttonPressed1 = false;
boolean buttonPressed2 = false;
boolean ledLit1 = false;
boolean ledLit2 = false;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(169, 254, 187, 112);
IPAddress dino_server(169, 254, 187, 111);
EthernetServer server(80);
EthernetClient client;
String server_msg = "";
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());

  myStepper.setSpeed(motorSpeed);
}

void loop() {
  // Apenas para imprimir as respostas do servidor
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState1 == HIGH) {
    buttonPressed1 = true;
  }
  else if (buttonPressed1) {
    if (ledLit1) {
      turnDeviceOff(ledId1);
      sendMsgToServer(ledId1, "OFF");
    }
    else {
      turnDeviceOn(ledId1);
      sendMsgToServer(ledId1, "ON");
    }
    buttonPressed1 = false;
  }

  if (buttonState2 == HIGH) {
    buttonPressed2 = true;
  }
  else if (buttonPressed2) {
    if (ledLit2) {
      turnDeviceOff(ledId2);
      sendMsgToServer(ledId2, "OFF");
    }
    else {
      turnDeviceOn(ledId2);
      sendMsgToServer(ledId2, "ON");
    }
    buttonPressed2 = false;
  }

  // Deve existir um nome melhor do que server_client para ser usado aqui...
  EthernetClient server_client = server.available();

  if (server_client) {
    while (server_client.connected()) {
      if (server_client.available()) {
        char c = server_client.read();
        if (c != '\n')
          server_msg += c;
        else {
          Serial.println(server_msg);
          int id = server_msg.substring(0, server_msg.indexOf(':')).toInt();
          int value = server_msg.substring(server_msg.indexOf(':') + 1).toInt();
          Serial.println(id);
          Serial.println(value);
          Serial.println();
          toggleDevice(id, value);
          server_msg = "";
          server_client.flush();
          server_client.stop();
        }
      }
    }
  }
}

void turnDeviceOff(int id) {
  if (id == ledId1) {
    digitalWrite(ledPin1, LOW);
    ledLit1 = false;
  }
  else if (id == ledId2) {
    digitalWrite(ledPin2, LOW);
    ledLit2 = false;
  }
  else if (id == motorId) {
    myStepper.step(-stepsPerRevolution);
  }
}

void turnDeviceOn(int id) {
  if (id == ledId1) {
    digitalWrite(ledPin1, HIGH);
    ledLit1 = true;
  }
  else if (id == ledId2) {
    digitalWrite(ledPin2, HIGH);
    ledLit2 = true;
  }
  else if (id == motorId) {
    myStepper.step(stepsPerRevolution);
  }
}

void toggleDevice(int id, int value) {
  if (value) {
    turnDeviceOn(id);
  }
  else {
    turnDeviceOff(id);
  }
}

void sendMsgToServer(int id, char state[]) {
  client.stop();
  if (client.connect(dino_server, 8080)) {
    client.print("GET /api/device/setStatus/");
    client.print(id);
    client.print("/");
    client.print(state);
    client.println(" HTTP/1.1");
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("Could not connect!");
  }
}

