const int TREES_NUM = 1;
int TREES_PINS[TREES_NUM] = {9, };
int TREES_STATUS[TREES_NUM];  // 0: off, 1: fire, -1: burned
int TREES_TIME[TREES_NUM];  // time on fire (after 30 it burns up)
int TREES_PROB[TREES_NUM];  // probability for each to get fire (default P_FIRE, each neighbor is on fire adds P_NEIGHBOR)
int P_FIRE = 2;
int P_NEIGHBOR = 5;
int BURNOUT_TIME = 20;


void print_arr(String name, int* arr)
{
  Serial.println(name);
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < TREES_NUM; i++)
  {
    Serial.print("start");
    Serial.print("\t");
    Serial.println(i);
    
    pinMode(TREES_PINS[i], OUTPUT);
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
    // Tree is burning
    if (TREES_STATUS[i] == 1)
    {
      // Check firebase 
    }
    // Tree is off and alive
    if (TREES_STATUS[i] == 0){
      if (random(100) < TREES_PROB[i]) {
        Serial.print("fire");
        Serial.print("\t");
        Serial.println(i);
        digitalWrite(TREES_PINS[i], HIGH);
        TREES_STATUS[i] = 1;
        TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] += P_NEIGHBOR;
        TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] += P_NEIGHBOR;

        print_arr("probs:", TREES_PROB);
        print_arr("stats:", TREES_STATUS);
      }
    }
  }

  for (int i = 0; i < TREES_NUM; i++)
  {
    
    if (TREES_STATUS[i] == -1)
      continue;

    if (TREES_STATUS[i] == 0)
      TREES_TIME[i] = 0;
      continue;
      
    TREES_TIME[i] += 1;
    if (TREES_TIME[i] > BURNOUT_TIME)
    {
      Serial.println();
      Serial.print("Tree burned");
      Serial.print(": ");
      Serial.println(i);
      Serial.println();
      TREES_STATUS[i] = -1;
      TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
      TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
      digitalWrite(TREES_PINS[i], LOW);
      print_arr("probs:", TREES_PROB);
      print_arr("stats:", TREES_STATUS);
    }
  }

  
  delay(1000);
}