/*
  Matériel supposé:
  capteur température/humidité: HTS221
  shield LoRa : I-NUCLEO-LRWAN1
*/

#include "LoRaWANNode.h"
#include <HTS221.h>
#define FRAME_DELAY 10000  // in ms. Every 10 seconds
//input code definitions
#define TEMP_CODE 1
#define TIME_CODE 2
#define START_CODE 3
#define STOP_CODE 4
#define RESET_CODE 5

// Serial port use to communicate with the USI shield.
// By default, use D0 (Rx) and D1(Tx).
// For Nucleo64, see "Known limitations" chapter in the README.md
HardwareSerial SerialLora(D0, D1);

// AppKey and AppEUI.
const char appKey[] = "0123456789abcdef0123456789abcdef";
const char appEUI[] = "0101010101010101";

// Data send
char textToSend[] = "";

//cooking management
boolean cooking = true;
boolean warningSent = false;
int desiredTemp = 100; //in celsius degrees. 100 by default.
int timeToCook = 360000; //in ms. 6 minutes by default.
int elapsedTime = 0;
double CookScore = 0;


void setup()
{
  //initialize the temp sensor
  smeHumidity.begin();

  //initialize serial for output
  Serial.begin(9600);

  // Enable the USI module and set the radio band.
  while (!loraNode.begin(&SerialLora, LORA_BAND_EU_868)) {
    Serial.println("Lora module not ready");
    delay(1000);
  }

  // Send a join request and wait the join accept
  while (!loraNode.joinOTAA(appKey, appEUI)) {
    Serial.println("joinOTAA failed!!");
    delay(1000);
  }

  Serial.println("Lora module ready, join accepted.\n");

  String str = "Device EUI: ";
  loraNode.getDevEUI(&str);
  Serial.println(str);
  str = "Application key: ";
  loraNode.getAppKey(&str);
  Serial.println(str);
  str = "Application EUI: ";
  loraNode.getAppEUI(&str);
  Serial.println(str);
}

//main loop
void loop()
{
  double temp = readTemp();

  //cooking in progress block
  if (cooking) {
    int score = calculateScore(temp);
    updateCookScore(score);
    checkWarning();
    checkCooked();
  }

  //request block
  receiveMessage();

  delay(FRAME_DELAY); //wait 10 seconds
  elapsedTime = elapsedTime + FRAME_DELAY; //updated elasped time

}

//general call to get the temperature in °C
double readTemp(void) {
  return readTempHTS221();
}

//general call to send a message
void sendMessage(String message) {
  message.toCharArray(textToSend, message.length());
  transmit(textToSend);
}

//general call to receive a message
void receiveMessage(void) {
  String code = receive();
  if (code != "") {
    String txt = "";
    switch (code.toInt()) {
      case TEMP_CODE:
        break;
      case TIME_CODE :
        if (cooking) {
          txt = elapsedTime / 60000;
        }
        else {
          txt = "0";
        }
        sendMessage(txt);
        break;
      case START_CODE :
        cooking = true;
        break;
      case STOP_CODE :
        cooking = false;
        break;
      case RESET_CODE :
        elapsedTime = 0;
        warningSent = false;
        break;
      default:
        break;
    }
  }
}

//reading the temp with the HTS221 sensor
double readTempHTS221(void) {
  return smeHumidity.readTemperature();
}

//calculating the cooking score with the temperature and the time
int calculateScore(double temp) {
  int score = 0;
  if (temp > 0) {
    score = FRAME_DELAY * temp / desiredTemp;
  }
  return score;
}

//upadting the cooking score
void updateCookScore(int score) {
  CookScore = CookScore + score;
}

//verify if the food is 75% cooked. If so, send a warning message once.
void checkWarning(void) {
  if ((CookScore >= timeToCook - (timeToCook / 4)) and warningSent == false) {
    //sending a warning when 75% cooked
    String txt = "Votre nourriture est cuite à 75% en ";
    txt +=  elapsedTime / 60000 ;
    txt += " minutes.";
    sendMessage(txt);
    warningSent = true;
  }
}

//verify is the food is totaly cooked. If so send a message.
void checkCooked(void) {
  if (CookScore >= timeToCook) {
    //The food is supposedly cooked!
    String txt = "Votre nourriture à été cuite en ";
    txt +=  elapsedTime / 60000 ;
    txt += " minutes.";
    sendMessage(txt);
    cooking = false;
  }
}

String receive(void) {
  uint8_t frameRx[64];
  uint8_t len;
  uint8_t port;

  // Check if data received from a gateway
  if (loraNode.receiveFrame(frameRx, &len, &port)) {
    uint8_t n = 0;
    Serial.print("frame received: 0x");
    while (len > 0) {
      Serial.print(frameRx[n], HEX);
      Serial.print(',');
      len--;
      n++;
    }
    Serial.print(" on port "); Serial.println(port);
  } else {
    return "";
  }
  String stringValue = (char*)frameRx;
  return stringValue;
}

void transmit(char frameTx[]) {
  // Send unconfirmed data to a gateway (port 1 by default)
  int status = loraNode.sendFrame(frameTx, sizeof(frameTx), UNCONFIRMED);
  if (status == LORA_SEND_ERROR) {
    Serial.println("Send frame failed!!!");
  } else if (status == LORA_SEND_DELAYED) {
    Serial.println("Module busy or duty cycle");
  } else {
    Serial.println("Frame sent");
  }
}
