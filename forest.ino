#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 10

#define FIREBASE_HOST "fire-truck-iot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "xseu33wYRo60cQHOJwuevCzMokATlwMg13rZq5Wo"
#define WIFI_SSID "OnePlus 5"
#define WIFI_PASSWORD "26111996"

const int TREES_NUM = 8;
const String ids[8] = {"0", "1", "2", "3", "4", "5", "6", "7"};
int TREES_PINS[TREES_NUM] = {0, 5, 15, 2, 14, 12, 13, 4};
int TREES_STATUS[TREES_NUM];  // 0: off, 1: fire, -1: burned
int TREES_TIME[TREES_NUM];    // time on fire (after 30 it burns up)
int TREES_PROB[TREES_NUM];    // probability for each to get fire 
                              //(default P_FIRE, each neighbor is on fire adds P_NEIGHBOR)
Adafruit_NeoPixel pixels[TREES_NUM] = {Adafruit_NeoPixel(NUMPIXELS, 0, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 15, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 2, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 14, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 12, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 13, NEO_GRB + NEO_KHZ800),
                                       Adafruit_NeoPixel(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800),};
int P_FIRE = 2;
int P_NEIGHBOR = 5;
int BURNOUT_TIME = 20;
int score = 0;

typedef enum {RED=0, GREEN=1, BLACK=2} color;


void print_arr(String name, int* arr)
{
  Serial.println(name);
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print(i);
    Serial.print(", ");
  }
  Serial.println();
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void color_tree(int i, color c)
{
  switch (c)
  {
    case (GREEN):
    {
      for(int j=0; j<7; j++) {
        pixels[i].setPixelColor(j, pixels[i].Color(0, 255, 0));
        pixels[i].show();
      }
      break;
    }
      
    case (RED):
    {
      for(int j=0; j<7; j++) {
        pixels[i].setPixelColor(j, pixels[i].Color(255, 100, 0));
        pixels[i].show();
      }
      break;
    }    
    case (BLACK):
    {
      for(int j=0; j<7; j++) {
        pixels[i].setPixelColor(j, pixels[i].Color(0, 0, 0));
        pixels[i].show();
      }
      break;
    }
  }
}

void fire_tree(int i)
{
  Serial.println();
  Serial.print("Tree fired: ");
  Serial.println(i);
  
  TREES_STATUS[i] = 1;
  TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] += P_NEIGHBOR;
  TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] += P_NEIGHBOR;
  color_tree(i, RED);
  delay(100);
  Serial.println("set fire");
  Serial.println(millis());
  Firebase.setInt(ids[i], 1);
  Serial.println("sot fire");
  Serial.println(millis());
}

void save_tree(int i)
{
  Serial.println();
  Serial.print("Tree saved: ");
  Serial.println(i);
  Serial.println();
  
  TREES_STATUS[i] = 0;
  TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  color_tree(i, GREEN);
  score += 1;
  
  delay(100);
  Serial.println("set score +");
  Serial.println(millis());
  Firebase.setInt("score", score);
  Serial.println("sot score +");
  Serial.println(millis());
}

void burn_tree(int i)
{
  Serial.println();
  Serial.print("Tree burned: ");
  Serial.println(i);
  Serial.println();

  TREES_STATUS[i] = -1;
  TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  color_tree(i, BLACK);
  score -= 1;

  delay(400);
  Serial.println("set fired");
  Serial.println(millis());
  Firebase.setInt(ids[i], -1);
  delay(400);
  Serial.println("set score -");
  Serial.println(millis());
  Firebase.setInt("score", score);
  Serial.println("sot score -");
  Serial.println(millis());
}

    

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.print("connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print("initializing ");
    Serial.println(i);
    delay(100);
    Firebase.setInt(ids[i], 0);
  }
  delay(100);
  Firebase.setInt("score", score);
  
  
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print("start");
    Serial.print("\t");
    Serial.println(i);
    pixels[i].begin();
    color_tree(i, GREEN);
    TREES_STATUS[i] = 0;
    TREES_TIME[i] = 0;
    TREES_PROB[i] = P_FIRE;
  }
  Serial.println();
}

void loop()
{
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print(i);
    delay(100);

    // Tree is burning
    if (TREES_STATUS[i] == 1)
    {
//      delay(2000);
      Serial.println("get status");
      Serial.println(millis());
      int status = Firebase.getInt(ids[i]);
      Serial.println("got status");
      Serial.println(millis());
      if (status == 0) {
        save_tree(i);
      }
    }
    // Tree is off and alive
    if (TREES_STATUS[i] == 0){
      if (random(100) < TREES_PROB[i]) {
        fire_tree(i);
      }
    }
  }

  Serial.println("update times");
  for (int i = 0; i < TREES_NUM; i++)
  {
    if (TREES_STATUS[i] == -1)
      continue;

    if (TREES_STATUS[i] == 0)
    {
      TREES_TIME[i] = 0;
      continue;
    }
    TREES_TIME[i] += 1;
    if (TREES_TIME[i] > BURNOUT_TIME)
      burn_tree(i);
  }

  Serial.println();
  print_arr("status:", TREES_STATUS);
  print_arr("times:", TREES_TIME);

  Serial.println();

  delay(1000);
}
