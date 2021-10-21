const int TREES_NUM = 8;
int TREES_NAMES[8] = {"Tree_1", "Tree_2", "Tree_3", "Tree_4", "Tree_5", "Tree_6", "Tree_7", "Tree_8"};
int TREES_PINS[TREES_NUM] = {9, };
int TREES_STATUS[TREES_NUM];  // 0: off, 1: fire, -1: burned
int TREES_TIME[TREES_NUM];  // time on fire (after 30 it burns up)
int TREES_PROB[TREES_NUM];  // probability for each to get fire (default P_FIRE, each neighbor is on fire adds P_NEIGHBOR)
int P_FIRE = 2;
int P_NEIGHBOR = 5;
int BURNOUT_TIME = 20;
// Set WIFI_SSID WIFI_PASSWORD FIREBASE_HOST FIREBASE_AUTH

// functions

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

void burn_tree(int i)
{
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

void save_tree(i)
{
  Serial.println();
  Serial.print("Tree saved!");
  Serial.print(": ");
  Serial.println(i);
  Serial.println();
  TREES_STATUS[i] = 0;
  TREES_PROB[(i - 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  TREES_PROB[(i + 1 + TREES_NUM) % TREES_NUM] -= P_NEIGHBOR;
  digitalWrite(TREES_PINS[i], LOW);
  print_arr("probs:", TREES_PROB);
  print_arr("stats:", TREES_STATUS);
}

void store_tree_on_fire(int tree_index)
{
  Firebase.setInt(TREES_NAMES[tree_index], 1);
}

void store_tree_is_burnt(int tree_index)
{
  Firebase.setInt(TREES_NAMES[tree_index], -1);
}

//void update_saved_trees()
//{
//  for (int i = 0; i < TREES_NUM; i++)
//  {
//    status = Firebase.getInt(TREES_NAMES[tree_index]);
//    if (status == 0) {
//      save_tree(i);
//    }
//  }
//}

//////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
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
      status = Firebase.getInt(TREES_NAMES[tree_index]);
      if (status == 0) {
        save_tree(i);
      }
      continue;
    }
    // Tree is off and alive
    if (TREES_STATUS[i] == 0){
      if (random(100) < TREES_PROB[i]) {
        burn_tree(i);
        store_tree_on_fire(i);
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

      store_tree_is_burnt(i);
    }
  }

  
  delay(1000);
}