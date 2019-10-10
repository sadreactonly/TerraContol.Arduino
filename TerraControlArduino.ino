
#include <SoftwareSerial.h>
#include <dht11.h>
#define DHT11PIN 7

dht11 DHT11;
int red_light_pin= 7;
int green_light_pin = 6;
int blue_light_pin = 5;

SoftwareSerial BTSerial (11, 12);

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

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
    byte startMarker = 0x3C;
    byte endMarker = 0x3E;
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
                receivedChars[ndx] = '\0'; // terminate the string
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
        if(receivedChars[0] == 'L')
        {
          playLight();
        }
        else if(receivedChars[0] == 'N')
        {
            RGB_color(0, 0, 0); 
        }
        else if(receivedChars[0] == 'T')
        {
            int temp = (int)DHT11.temperature;
            BTSerial.write('T');
            BTSerial.write(temp);
            BTSerial.write(0xFF);
        }
        else if(receivedChars[0] == 'H')
        {
            int hum = (int)DHT11.humidity;

            BTSerial.write('H');
            BTSerial.write(hum);
            BTSerial.write(0xFF);
        }
          else if(receivedChars[0] == 'C')
        {

        }
        newData = false;
    }
}
 void playLight() {
  RGB_color(255, 0, 0); // Red
  delay(1000);
  RGB_color(0, 255, 0); // Green
  delay(1000);
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  RGB_color(255, 255, 255); // White
  delay(1000);
}
 void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
