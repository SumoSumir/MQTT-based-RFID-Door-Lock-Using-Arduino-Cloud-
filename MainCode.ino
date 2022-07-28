#include "arduino_secrets.h"
/*
  Arduino IoT Cloud Variables
  String entryID;
  String whiteList;
*/

#include "thingProperties.h"
#include <SPI.h>
#include <MFRC522.h>
#include <cstring>
#include <string>

/*
  PINOUT

  MOSI: GPIO 13 - D7
  MISO: GPIO 12 - D6
  SCK/ CLK: GPIO 14 - D5 (Slave Clock or Clock)
  SS/ CS/ SDA : GPIO 15 - D8 (Chip select or Slave Select)
  RST : GPIO 2 - D4
*/

/*
  TAG UID's:
  keychain uid C0 62 E3 72
  keycard uid 65 E4 59 D9
*/

#define SS_PIN 15
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//space given for 50 entrys can be more
unsigned long rfid_data[4], Localwhitelist[50][4];
char names[50][30];
int count = 0, LWL_count = 0;
int i = 0, c = 2, flag = 0, j = 0,err_count=0,timehour=millis();


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("Initializing...");
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_38dB); //38dB MAX  RANGE is RxGain_max, but prefer this antenna gain better power usage/penetration
  mfrc522.PCD_AntennaOn();
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  i = 0;

  //Serial.flush();
  // Your code here
  /*
    Serial.println("Begin");
    Serial.println(MOSI);
    Serial.println(MISO);
    Serial.println(SCK);
    Serial.println(SS); */
}

void loop() {
  ArduinoCloud.update();

  if (!whiteList) // && flag!=1) //if whitelist blank
    return;
  else if (flag == 0)
  {
    onWhiteListChange();
  }
  /*  //If uid was read whole
    if (c==mfrc522.uid.size)
      {
        Serial.print("\n\nWelcome:\n");
        Serial.println(names[i]);
        c = 2;
      }
  */
  //if(millis()%1000==500)
  //{
  // Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent()) // if none found
  {
    return;
  }
  else
  { // less than or lte
    if (!mfrc522.PICC_ReadCardSerial() && c <= mfrc522.uid.size) //if unable to read card data/ didn't read uid full
    {
      Serial.println("Couldn't Read");
      return;
    }
    else
    {
      for (int f = 0; f < mfrc522.uid.size; f++)
      {
        rfid_data[f] = mfrc522.uid.uidByte[f];
      }
      for (i = 0; i < count; i++)
      {
        c = 0;
        for (int f = 0; f < mfrc522.uid.size; f++)
        {
          if (rfid_data[f] == Localwhitelist[i][f])
            c++;
          else
          {
            c = 0;
            break;
          }
        }
        if (c >= mfrc522.uid.size)
        {
          entryID = names[i];
          Serial.print("\n\nWelcome:\n");
          Serial.println(names[i]);
          break;
        }
        else if(c==0)
        {
          if((millis()-timehour)<3600000)
          err_count++;
          else
          err_count=0;
          if(err_count==0)
          timehour=millis();
          Serial.println(err_count); //only for ddemonstration
        }
        if(err_count>=300) // keep 3 for demonstration 
        {
          Serial.println("Too many invalid attempts"); //only kept for demonstration
          entryID="Too many invalid attempts";
          err_count=0;
          timehour=millis();
        }
      }
    }
  }
  //}
}

/*
  Since EntryID is READ_WRITE variable, onEntryIDChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onEntryIDChange()  {
  /*
    // Add your code here to act upon EntryID change
    Serial.println("OnEntryIDFunc says: Welcome ");
    Serial.print(entryID);
  */
}

/*
  Since WhiteList is READ_WRITE variable, onWhiteListChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onWhiteListChange()  {
  /*

    Example whiteList -> "00_00_00_00-Jai Vaswani;00_01_02_03-Sumir Broota"

    1) First, split based on ';'
    2) For each part of string after splitting, split based on '-'
    3) First part of this gets added to decimal_array
    4) Second part of this gets added to names_array

  */
  // Add your code here to act upon WhiteList change
  //**********************************************
  if (!whiteList)
    return;
  else if (flag == 0)
  {
    flag = 1;
  }
  char arr[whiteList.length() + 1];

  strcpy(arr, whiteList.c_str());
  Serial.println(whiteList);

  char test[50], *ptr2, *ptr;
  char temporary_storage[30][50];

  //** Step 1) -
  ptr = strtok(arr, " ; "); // use strtok() function to separate string using ; delimiter.
  //strtok only takes char array
  while (ptr != NULL)
  {
    //Serial.println(ptr);
    strcpy(temporary_storage[count], ptr);
    count++;
    ptr = strtok (NULL, " ; ");
  }
  //***

  //** Step 2) to step 4)
  for (int i = 0; i < count; i++)
  {
    ptr = strtok(temporary_storage[i], "-"); // use strtok() function to separate string using - delimiter.
    strcpy(test, ptr);
    ptr = strtok(NULL, "-");
    ptr2 = strtok(test, " _ "); // use strtok() function to separate string using - delimiter.
    LWL_count = 0;
    while (ptr2 != NULL)
    {
      Localwhitelist[i][LWL_count++] = atoi(ptr2);
      ptr2 = strtok (NULL, " _ ");
    }
    //strcpy(test,ptr);
    ptr = replace(ptr, '&', ' ');
    strcpy(names[i], ptr);
    Serial.print("Name added:");
    Serial.println(names[i]);
  }
  //************************************************
}

char* replace(char *s, const char c1, const char c2)
{
  //int l = s.length();
  int i = 0;
  // loop to traverse in the string
  //for (int i = 0; i < l; i++) {
  while (*(s + i) != '\0')
  {
    // check for c1 and replace
    if (s[i] == c1)
      s[i] = c2;
    i++;
  }
  return s;
}