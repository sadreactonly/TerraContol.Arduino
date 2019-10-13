
#include <SoftwareSerial.h>
#include <dht11.h>
#define DHT11PIN 7

#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN  4

const byte numChars = 32;

SoftwareSerial BTSerial (11, 12);
dht11 DHT11;

byte receivedChars[numChars];
boolean newData = false;
byte startMarker = 0x3C;
byte endMarker = 0x3E;
byte rgb[3];
void setup() {
    BTSerial.begin(9600);
    pinMode (8, OUTPUT);

}

void loop() {
    int chk = DHT11.read(DHT11PIN);
    recvWithStartEndMarkers();
    showNewData();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;

    char rc;
 
    while (BTSerial.available() > 0 && newData == false) {
        rc = BTSerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                //receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        if(receivedChars[0] == 0x13)
        {
          RGB_color(receivedChars[1], receivedChars[2], receivedChars[3]);
        }
        else if(receivedChars[0] == 0x14)
        {
            RGB_color(0, 0, 0); 
        }
        else if(receivedChars[0] == 0x11)
        {
            int temp = (int)DHT11.temperature;
            BTSerial.write('T');
            BTSerial.write(temp);
            BTSerial.write(0xFF);
        }
        else if(receivedChars[0] == 0x12)
        {
            int hum = (int)DHT11.humidity;

            BTSerial.write('H');
            BTSerial.write(hum);
            BTSerial.write(0xFF);
        }
          else
        {
           RGB_color(receivedChars[0], receivedChars[1], receivedChars[2]);
        }
        newData = false;
    }
}
 void playLight() {

}
 void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(RED_PIN, red_light_value);
  analogWrite(GREEN_PIN, green_light_value);
  analogWrite(BLUE_PIN, blue_light_value);
}
