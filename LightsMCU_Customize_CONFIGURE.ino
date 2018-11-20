/*****************  NEEDED TO MAKE NODEMCU WORK ***************************/
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

/******************  LIBRARY SECTION *************************************/
#include <FastLED.h>
#include <SimpleTimer.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/*****************  LED LAYOUT AND SETUP *********************************/
#define FIRSTFLOOR_LEDS 480
//first floor is actually 474
//first floor layout 473-329-234/115\0
#define SECONDFLOOR_LEDS 480
//second floor is actually 382
//second floor layout 382 / 286 \ 190 189 | 183 182 - 95 94 | 88 87 / 44 \ 0
#define CANDYCANE_LEDS 276
//Candy canes are 11 LEDS per cane, 176 LEDS
#define WREATH_LEDS 280 
//actual count is 246
//0-80    81-148    149-202   203-246
#define NUM_LEDS 100

/***********************  WIFI AND MQTT SETUP *****************************/

const char* ssid = "YOUR_WIRELESS_SSID";
const char* password = "YOUR_WIRELESS_PW";
const char* mqtt_server = "YOUR_MQTT_SERVER_ADDRESS";
const int mqtt_port = 1883;
const char *mqtt_user = "YOUR_MQTT_USERNAME";
const char *mqtt_pass = "YOUR_MQTT_PASSWORD";
const char *mqtt_client_name = "LightMCU"; // Client connections can't have the same connection name


/*****************  DECLARATIONS  ****************************************/
WiFiClient espClient;
PubSubClient client(espClient);
SimpleTimer timer;

CRGB firstFloor[FIRSTFLOOR_LEDS];
CRGB secondFloor[SECONDFLOOR_LEDS];
CRGB candyCanes[CANDYCANE_LEDS];
CRGB wreath[WREATH_LEDS];

CRGBPalette16 gPal;

/*****************  GLOBAL VARIABLES  ************************************/

const int downstairsLights = 4; //marked as D2 on the board
const int upstairsLights = 5; //marked as D1 on the board
const int candyCaneLights = 14; //marked as D5 on the board
const int wreathLights = 12; //marked as D6 on the board

int fireSize = 40;
int SPARKING = 85;
int COOLING =  120;
bool gReverseDirection = false;
uint8_t mark = 0;
uint8_t gHue = 0; 
uint8_t startPosition = 0;
uint8_t glitterChance = 250;
int chaseDelay = 1000;
int lastPosition = 1;
int lightning = 1;
int ledLocator = 0;
int centerFirst = 0;
int centerSecond = 0;
int stepFirst = -1;
int stepSecond = -1;
int raceSpeed = 12;
int previousLED1 = 0;
int previousLED2 = 0;
int previousLED3 = 0;
int BeatsPerMinute = 62;
uint8_t numberOfRainbows = 7;
int twinkleChance = 250;
int eyeChance = 248;


bool boot = true;

String effect = "None";
bool showGlitter = false;
bool showLightning = false;
bool audioEffects = false;
bool showLights = false;

byte red1 = 255;
byte green1 = 0;
byte blue1 = 0;
byte red2 = 0;
byte green2 = 255;
byte blue2 = 0;
byte red3 = 0;
byte green3 = 0;
byte blue3 = 255;
byte brightness = 255;




/*****************  SYSTEM FUNCTIONS  *************************************/
/*****************  SYSTEM FUNCTIONS  *************************************/
/*****************  SYSTEM FUNCTIONS  *************************************/
/*****************  SYSTEM FUNCTIONS  *************************************/
/*****************  SYSTEM FUNCTIONS  *************************************/

void setup_wifi() 
{
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  // Loop until we're reconnected
  int retries = 0;
  while (!client.connected()) {
    if(retries < 150)
    {
        Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass)) 
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        if(boot == true)
        {
          client.publish("checkIn/LightMCU","Rebooted");
          boot = false;
        }
        if(boot == false)
        {
          client.publish("checkIn/LightMCU","Reconnected"); 
        }
        // ... and resubscribe
        client.subscribe("holidayLights/configure");
        client.subscribe("holidayLights/effect");
        client.subscribe("holidayLights/state");
        client.subscribe("hoildayLights/red1");
        client.subscribe("hoildayLights/green1");
        client.subscribe("hoildayLights/blue1");
        client.subscribe("hoildayLights/red2");
        client.subscribe("hoildayLights/green2");
        client.subscribe("hoildayLights/blue2");
        client.subscribe("hoildayLights/red3");
        client.subscribe("hoildayLights/green3");
        client.subscribe("hoildayLights/blue3");
        client.subscribe("holidayLights/power");
        client.subscribe("hoildayLights/brightness");
        client.subscribe("holidayLights/addEffects");
      } 
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        retries++;
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    if(retries > 1500)
    {
    ESP.restart();
    }
  }
}


/************************** MQTT CALLBACK ***********************/
/************************** MQTT CALLBACK ***********************/
/************************** MQTT CALLBACK ***********************/
/************************** MQTT CALLBACK ***********************/
/************************** MQTT CALLBACK ***********************/


void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  String newTopic = topic;
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String newPayload = String((char *)payload);
  int intPayload = newPayload.toInt();
  Serial.println(newPayload);
  Serial.println();
  if (newTopic == "holidayLights/addEffects")
  {
    if(newPayload == "Audio On")
    {
      audioEffects = true;
      client.publish("holidayLights/audio/state","Audio On");
    }
    if(newPayload == "Audio Off")
    {
      audioEffects = false;
      client.publish("holidayLights/audio/state","Audio Off");
    }
    if(newPayload == "Glitter On")
    {
      showGlitter = true;
      client.publish("holidayLights/glitter/state","Glitter On");
    }
    if(newPayload == "Glitter Off")
    {
      showGlitter = false;
      client.publish("holidayLights/glitter/state","Glitter Off");
    }
    if(newPayload == "Lightning On")
    {
      showLightning = true;
      client.publish("holidayLights/lightning/state","Lightning On");
    }
    if(newPayload == "Lightning Off")
    {
      showLightning = false;
      client.publish("holidayLights/lightning/state","Lightning Off");
    }
  }
  if (newTopic == "holidayLights/effect") 
  {
    effect = newPayload;  
    fill_solid(wreath, WREATH_LEDS, CRGB::Black); 
    fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB::Black); 
    fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB::Black); 
  }
  if (newTopic == "hoildayLights/red1") 
  { 
    red1 = intPayload;
  }
  if (newTopic == "hoildayLights/green1") 
  {
    green1 = intPayload;
  }
  if (newTopic == "hoildayLights/blue1") 
  {
    blue1 = intPayload;
  }
  if (newTopic == "hoildayLights/red2") 
  {
    red2 = intPayload;
  }
  if (newTopic == "hoildayLights/green2") 
  {
    green2 = intPayload;
  }
  if (newTopic == "hoildayLights/blue2") 
  {
    blue2 = intPayload;
  }
  if (newTopic == "hoildayLights/red3") 
  {
    red3 = intPayload;
  }
  if (newTopic == "hoildayLights/green3") 
  {
    green3 = intPayload;
  }
  if (newTopic == "hoildayLights/blue3") 
  {
    blue3 = intPayload;
  }
  if (newTopic == "hoildayLights/brightness") 
  {
    brightness = intPayload;
  }
  if (newTopic == "holidayLights/power")
  {
    if(newPayload == "ON")
    {
      showLights = true;
    }
    if(newPayload == "OFF")
    {
      showLights = false;
    }
  }
  if (newTopic == "holidayLights/configure")
  {
    ledLocator = intPayload;
    if(effect == "Fire")
    {
      fireSize = (intPayload/5);
    }
    if(effect == "Double_Crash" || effect == "Single_Race")
    {
      raceSpeed = (intPayload/20);
    }
    if(effect == "BPM")
    {
      BeatsPerMinute = (intPayload/4);
    }
    if(effect == "Color_Chase" || effect == "Blocked_Colors")
    {
      chaseDelay = (intPayload*5);
      if(chaseDelay < 100)
      {
        chaseDelay = 100;
      }
    }
    if(effect == "Color_Glitter")
    {
      glitterChance = (intPayload/2);
    }
    if(effect == "Rainbow")
    {
      numberOfRainbows = (intPayload/30);
    }
    if(effect == "Twinkle")
    {
      twinkleChance = map(intPayload, 0, 500, 0, 255);
    }
    if(effect == "Spooky_Eyes")
    {
      eyeChance = map(intPayload, 0, 500, 225, 255);
    }
  }
}



/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/
/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/
/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/
/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/
/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/


void chase()
{
  if(startPosition == 5)
  {
    startPosition = 0;
  }
  else
  {
    startPosition++;
  }
  timer.setTimeout(chaseDelay, chase);
}

void checkIn()
{
  client.publish("checkIn/LightMCU","OK");
  timer.setTimeout(120000, checkIn);
}

void choosePattern()
{
  if(showLights == true)
  {
   if(effect == "Color_Chase")
   {
      firstRGB();
      secondRGB();
      wreathRGB();
      canesRGB();
   }
   if(effect == "Color_Glitter")
   {
      firstColorGlitter(glitterChance); 
      secondColorGlitter(glitterChance); 
      wreathColorGlitter(glitterChance); 
      canesColorGlitter(glitterChance); 
   }
   if(effect == "Single_Race")
   {
      firstSingleRace();
      secondSingleRace();
      canesSingleRace();
      wreathSingleRace();
   }
   if(effect == "Double_Crash")
   {
      firstCrash();
      secondCrash();
      canesCrash();
      wreathCrash();
   }
   if(effect == "Rainbow")
   {
      firstRainbow();
      secondRainbow();
      canesRainbow();
      wreathRainbow();
   }
   if(effect == "Blocked_Colors")
   {
      firstBlocked();
      secondBlocked();
      wreathBlocked();
      canesBlocked();
   }
   if(effect == "BPM")
   {
      firstBPM();
      secondBPM();
      wreathBPM();
      canesBPM();
   }
   if(effect == "Twinkle")
   {
      firstTwinkle();
      secondTwinkle();
      wreathTwinkle();
      canesTwinkle();
   }
   if(effect == "Fill_Solid")
   {
      firstFloorSolid();
      secondFloorSolid();
      wreathSolid();
   }
   if(effect == "Fire")
   {
      fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 20);
      fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 20);
      fadeToBlackBy( candyCanes, CANDYCANE_LEDS, 20);
      fadeToBlackBy( wreath, WREATH_LEDS, 20);
      fireFirstFloor1();
      fireFirstFloor2();
      fireFirstFloor3();
      fireFirstFloor4();
      fireFirstFloor5();
      fireFirstFloor6();
      fireSecondFloor1();
      fireSecondFloor2();
      fireSecondFloor3();
      fireSecondFloor4();
      fireSecondFloor5();
      fireSecondFloor6();
      fireWreath1();
      fireWreath2();
      fireWreath3();
      fireWreath4();
      fireWreath5();
      fireWreath6();
      fireWreath7();
      fireWreath8();
   }
   if(effect == "Spooky_Eyes")
   {
      firstEyes();
      secondEyes();
      wreathPumpkin();
   }
   if(effect == "Halloween")
   {
      firstEyes();
      secondEyes();
      wreathPumpkin();
      fireFirstFloor1();
      fireFirstFloor6();
      fireSecondFloor1();
      fireSecondFloor6(); 
   }
   if(effect == "LED_Locator")
   {
      firstFloorLocator();
      secondFloorLocator();
      wreathLocator();
      candyCanesLocator();
   }
   if(effect == "Ripple")
   {
      firstFloorRipple();
      secondFloorRipple();
   }
  }
  if(showLights == false)
  {
    fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 20);
    fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 20);
    fadeToBlackBy( candyCanes, CANDYCANE_LEDS, 20);
    fadeToBlackBy( wreath, WREATH_LEDS, 20);
  }
  addGlitter(100);
  addLightning(); 
}

/*********************** PATTERN MODIFIERS ***************************/
/*********************** PATTERN MODIFIERS ***************************/
/*********************** PATTERN MODIFIERS ***************************/
/*********************** PATTERN MODIFIERS ***************************/
/*********************** PATTERN MODIFIERS ***************************/

void addLightning()
{
  if(showLightning == true)
  {
    fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 1);
    fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 1);
    unsigned int chance = random8();
    if( chance == 255) 
    {
      wreathPumpkinLightning();
      fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB::White); 
      fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB::White); 
      lightning = 20;
      if(audioEffects == true)
      {
        int thunder = random8();
        if( thunder > 128)
        {
          client.publish("Audio","2");
        }
        if( thunder < 127)
        {
          client.publish("Audio","1");
        }
      }
    }
    if(lightning != 1)
    {
      if(lightning > 15)
      {
        fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 150);
        fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 150);
        fadeToBlackBy( wreath, WREATH_LEDS, 150);
        lightning--;
      }
      if(lightning == 15)
      {
        wreathPumpkinLightning();
        fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB::White);
        fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB::White);
        lightning--; 
      }
      if(lightning > 5 && lightning < 15)
      {
        fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 150);
        fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 150);
        fadeToBlackBy( wreath, WREATH_LEDS, 150);
        lightning--;
      }
      if(lightning == 5)
      {
        wreathPumpkinLightning();
        fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB::White);
        fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB::White);
        lightning--; 
      }
      if(lightning > 0 && lightning < 5)
      {
        fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 150);
        fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 150);
        fadeToBlackBy( wreath, WREATH_LEDS, 150);
        lightning--;
      }
    }
  }
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if(showGlitter == true)
  {
    if( random8() < chanceOfGlitter) 
    {
      firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::White;
    }
    if( random8() < chanceOfGlitter) 
    {
      secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::White;
    }
    if( random8() < chanceOfGlitter) 
    {
      wreath[ random16(WREATH_LEDS) ] += CRGB::White;
    }
    if( random8() < chanceOfGlitter) 
    {
      candyCanes[ random16(CANDYCANE_LEDS) ] += CRGB::White;
    }
  }
}





/*****************  SETUP FUNCTION  ****************************************/
/*****************  SETUP FUNCTION  ****************************************/
/*****************  SETUP FUNCTION  ****************************************/
/*****************  SETUP FUNCTION  ****************************************/
/*****************  SETUP FUNCTION  ****************************************/


void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, upstairsLights, GRB>(firstFloor, FIRSTFLOOR_LEDS);
  FastLED.addLeds<WS2812B, downstairsLights, GRB>(secondFloor, SECONDFLOOR_LEDS);
  FastLED.addLeds<WS2812B, candyCaneLights, GRB>(candyCanes, CANDYCANE_LEDS);
  FastLED.addLeds<WS2812B, wreathLights, GRB>(wreath, WREATH_LEDS);
  FastLED.setDither( 0 );
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  ArduinoOTA.setHostname("LightMCU");
  ArduinoOTA.begin(); 

  gPal = HeatColors_p;
  
  timer.setTimeout(10000, chase);
  timer.setTimeout(120000, checkIn);

}


/*****************  MAIN LOOP  ****************************************/
/*****************  MAIN LOOP  ****************************************/
/*****************  MAIN LOOP  ****************************************/
/*****************  MAIN LOOP  ****************************************/
/*****************  MAIN LOOP  ****************************************/


void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  if(showLights == false)
  {
    ArduinoOTA.handle();  
  }
  timer.run();
  choosePattern();
  FastLED[0].showLeds(brightness);
  FastLED[1].showLeds(brightness);
  FastLED[2].showLeds(brightness);
  FastLED[3].showLeds(brightness);
  //FastLED.show();
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
}



/****************** BEGIN ANIMATION SECTION ************************/


/****************** RIPPLES ****************************************/
/****************** RIPPLES ****************************************/
/****************** RIPPLES ****************************************/
/****************** RIPPLES ****************************************/
/****************** RIPPLES ****************************************/

void firstFloorRipple()
{
  for (int i = 0; i < FIRSTFLOOR_LEDS; i++)
  {
    firstFloor[i] = CRGB((red2/75), (green2/75), (blue2/75));
  }   
  switch (stepFirst) 
  {
    case -1:                                                          // Initialize ripple variables.
      centerFirst = random16(FIRSTFLOOR_LEDS);
      stepFirst = 0;
      break;
    case 0:
      firstFloor[centerFirst] = CRGB(red1, green1, blue1);                          // Display the first pixel of the ripple.
      stepFirst ++;
      break;
    case 12:                                                    // At the end of the ripples.
      stepFirst = -1;
      break;
    default:                                                             // Middle of the ripples.
      firstFloor[(centerFirst + stepFirst + FIRSTFLOOR_LEDS) % FIRSTFLOOR_LEDS] += CRGB((red1/(stepFirst * 5)), (green1/(stepFirst * 5)), (blue1/(stepFirst * 5)));
      firstFloor[(centerFirst - stepFirst + FIRSTFLOOR_LEDS) % FIRSTFLOOR_LEDS] += CRGB((red1/(stepFirst * 5)), (green1/(stepFirst * 5)), (blue1/(stepFirst * 5)));
      stepFirst ++;                                                         // Next step.
      break;
  }
}

void secondFloorRipple()
{
  for (int i = 0; i < SECONDFLOOR_LEDS; i++)
  {
    secondFloor[i] = CRGB((red2/75), (green2/75), (blue2/75));
  }   
  switch (stepSecond) 
  {
    case -1:                                                          // Initialize ripple variables.
      centerSecond = random16(SECONDFLOOR_LEDS);
      stepSecond = 0;
      break;
    case 0:
      secondFloor[centerSecond] = CRGB(red1, green1, blue1);                          // Display the second pixel of the ripple.
      stepSecond ++;
      break;
    case 13:                                                    // At the end of the ripples.
      stepSecond = -1;
      break;
    default:                                                             // Middle of the ripples.
      secondFloor[(centerSecond + stepSecond + SECONDFLOOR_LEDS) % SECONDFLOOR_LEDS] += CRGB((red1/(stepSecond * 5)), (green1/(stepSecond * 5)), (blue1/(stepSecond * 5)));
      secondFloor[(centerSecond - stepSecond + SECONDFLOOR_LEDS) % SECONDFLOOR_LEDS] += CRGB((red1/(stepSecond * 5)), (green1/(stepSecond * 5)), (blue1/(stepSecond * 5)));
      stepSecond ++;                                                         // Next step.
      break;
  }
}






/****************** 3 COLOR ALTERNATING ****************************/
/****************** 3 COLOR ALTERNATING ****************************/
/****************** 3 COLOR ALTERNATING ****************************/
/****************** 3 COLOR ALTERNATING ****************************/
/****************** 3 COLOR ALTERNATING ****************************/

void firstRGB()
{
  if(startPosition < 6)
  {
    fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 80);
    for(int pixel = startPosition+1; pixel < FIRSTFLOOR_LEDS; pixel+=6)  
    {
      if(pixel <= FIRSTFLOOR_LEDS)
      {
        firstFloor[pixel] = CRGB(red1,green1,blue1);
      }
    }
    for(int pixel = startPosition+3; pixel < FIRSTFLOOR_LEDS; pixel+=6)
    {
      if(pixel <= FIRSTFLOOR_LEDS)
      {
        firstFloor[pixel] = CRGB(red2,green2,blue2);
      }
    }
    for(int pixel = startPosition+5; pixel < FIRSTFLOOR_LEDS; pixel+=6)
    {
      if(pixel <= FIRSTFLOOR_LEDS)
      {
        firstFloor[pixel] = CRGB(red3,green3,blue3);
      } 
    }
  }
  if(startPosition == 1)
  {
    firstFloor[0]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 2)
  {
    firstFloor[1]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 3)
  {
    firstFloor[2]=CRGB(red3,green3,blue3);
    firstFloor[0]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 4)
  {
    firstFloor[3]=CRGB(red3,green3,blue3);
    firstFloor[1]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 5)
  {
    firstFloor[4]=CRGB(red3,green3,blue3);
    firstFloor[2]=CRGB(red2,green2,blue2);
    firstFloor[0]=CRGB(red1,green1,blue1);
  }
}

void secondRGB()
{
  if(startPosition < 6)
  {
    fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 20);
    for(int pixel = startPosition+1; pixel < SECONDFLOOR_LEDS; pixel+=6) 
    {
      if(pixel <= SECONDFLOOR_LEDS)
      {
        secondFloor[pixel] = CRGB(red1,green1,blue1);
      }
    }
    for(int pixel = startPosition+3; pixel < SECONDFLOOR_LEDS; pixel+=6)  
    {
      if(pixel <= SECONDFLOOR_LEDS)
      {
        secondFloor[pixel] = CRGB(red2,green2,blue2);
      }
    }
    for(int pixel = startPosition+5; pixel < SECONDFLOOR_LEDS; pixel+=6) 
    {
      if(pixel <= SECONDFLOOR_LEDS)
      {
        secondFloor[pixel] = CRGB(red3,green3,blue3);
      }
    }
  }
  if(startPosition == 1)
  {
    secondFloor[0]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 2)
  {
    secondFloor[1]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 3)
  {
    secondFloor[2]=CRGB(red3,green3,blue3);
    secondFloor[0]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 4)
  {
    secondFloor[3]=CRGB(red3,green3,blue3);
    secondFloor[1]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 5)
  {
    secondFloor[4]=CRGB(red3,green3,blue3);
    secondFloor[2]=CRGB(red2,green2,blue2);
    secondFloor[0]=CRGB(red1,green1,blue1);
  }
}

void canesRGB()
{
  fadeToBlackBy( candyCanes, 176, 20);
  if(startPosition == 0 || startPosition == 2 || startPosition == 4 )
  {
    for(int pixel = 0; pixel < 176; pixel+=2)
    {
      if(pixel <= CANDYCANE_LEDS)
      {
        candyCanes[pixel] = CRGB(255,0,0);;
      }
    }
  }
  if(startPosition == 1 || startPosition == 3 || startPosition == 5 )
  {
    for(int pixel = 1; pixel < 176; pixel+=2)
    {
      if(pixel <= CANDYCANE_LEDS)
      {
        candyCanes[pixel] = CRGB(255,0,0);
      } 
    }
  }
}

void wreathRGB()
{
  if(startPosition < 6)
  {
    fadeToBlackBy( wreath, WREATH_LEDS, 20);
    for(int pixel = startPosition+1; pixel < WREATH_LEDS; pixel+=6) 
    {
      if(pixel <= WREATH_LEDS)
      {
        wreath[pixel] = CRGB(red1,green1,blue1);
      } 
      
    }
    for(int pixel = startPosition+3; pixel < WREATH_LEDS; pixel+=6)
    {
      if(pixel <= WREATH_LEDS)
      {
        wreath[pixel] = CRGB(red2,green2,blue2);
      } 
      
    }
    for(int pixel = startPosition+5; pixel < WREATH_LEDS; pixel+=6)
    {
      if(pixel <= WREATH_LEDS)
      {
        wreath[pixel] = CRGB(red3,green3,blue3);
      } 
    }
  }
  if(startPosition == 1)
  {
    wreath[0]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 2)
  {
    wreath[1]=CRGB(red3,green3,blue3);
  }
  if(startPosition == 3)
  {
    wreath[2]=CRGB(red3,green3,blue3);
    wreath[0]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 4)
  {
    wreath[3]=CRGB(red3,green3,blue3);
    wreath[1]=CRGB(red2,green2,blue2);
  }
  if(startPosition == 5)
  {
    wreath[4]=CRGB(red3,green3,blue3);
    wreath[2]=CRGB(red2,green2,blue2);
    wreath[0]=CRGB(red1,green1,blue1); 
  }
}

/****************** COLOR GLITTER *********************************/
/****************** COLOR GLITTER *********************************/
/****************** COLOR GLITTER *********************************/
/****************** COLOR GLITTER *********************************/
/****************** COLOR GLITTER *********************************/

void firstColorGlitter( fract8 chanceOfGlitter) 
{
  fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 2);
  if(startPosition == 0)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Red;
    }
  }
  if(startPosition == 1)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Orange;
    }
  }
  if(startPosition == 2)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Yellow;
    }  
  }
  if(startPosition == 3)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Green;
    }
  }
  if(startPosition == 4)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Blue;
    }
  }
  if(startPosition == 5)
  {
    if( random8() < chanceOfGlitter) 
    {
    firstFloor[ random16(FIRSTFLOOR_LEDS) ] += CRGB::Violet;
    }
  } 
}

void secondColorGlitter( fract8 chanceOfGlitter) 
{
  fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 2);
  if(startPosition == 0)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Red;
    }
  }
  if(startPosition == 1)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Orange;
    }
  }
  if(startPosition == 2)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Yellow;
    }  
  }
  if(startPosition == 3)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Green;
    }
  }
  if(startPosition == 4)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Blue;
    }
  }
  if(startPosition == 5)
  {
    if( random8() < chanceOfGlitter) 
    {
    secondFloor[ random16(SECONDFLOOR_LEDS) ] += CRGB::Violet;
    }
  } 
}

void canesColorGlitter( fract8 chanceOfGlitter) 
{
  fadeToBlackBy( candyCanes, 176, 2);
  if( random8() < chanceOfGlitter) 
  {
    candyCanes[ random16(176) ] += CRGB::Red;
  }
}


void wreathColorGlitter( fract8 chanceOfGlitter) 
{
  fadeToBlackBy( wreath, WREATH_LEDS, 2);
  if(startPosition == 0)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Red;
    }
  }
  if(startPosition == 1)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Orange;
    }
  }
  if(startPosition == 2)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Yellow;
    }  
  }
  if(startPosition == 3)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Green;
    }
  }
  if(startPosition == 4)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Blue;
    }
  }
  if(startPosition == 5)
  {
    if( random8() < chanceOfGlitter) 
    {
    wreath[ random16(WREATH_LEDS) ] += CRGB::Violet;
    }
  } 
}

/***************************** SINGLE RACE **************************/
/***************************** SINGLE RACE **************************/
/***************************** SINGLE RACE **************************/
/***************************** SINGLE RACE **************************/
/***************************** SINGLE RACE **************************/

void firstSingleRace()
{
  fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 1);
  int pos = beatsin16( raceSpeed, 0, 474);
  if(pos > previousLED1)
  {
    firstFloor[pos] = CRGB(red1,green1,blue1);
    previousLED1 = pos;
  }
  if(pos < previousLED1)
  {
    firstFloor[pos] = CRGB(red2,green2,blue2);
    previousLED1 = pos;
  }                 
}

void secondSingleRace()
{
  fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 1);
  int pos = beatsin16( raceSpeed, 0, 382);
  if(pos > previousLED2)
  {
    secondFloor[pos] = CRGB(red1,green1,blue1);
    previousLED2 = pos;
  }
  if(pos < previousLED2)
  {
    secondFloor[pos] = CRGB(red2,green2,blue2);
    previousLED2 = pos;
  }                    
}

void canesSingleRace()
{
  fadeToBlackBy( candyCanes, 174, 10);
  int pos = beatsin16( raceSpeed, 0, 15 );
  for(int i = 0; i < 10; i++)
  {
    candyCanes[i+(pos*11)]=CRGB(255,0,0);
  }
}


void wreathSingleRace()
{
  fadeToBlackBy( wreath, WREATH_LEDS, 1);
  int pos = beatsin16( raceSpeed, 0, 246);
  if(pos > previousLED3)
  {
    wreath[pos] = CRGB(red1,green1,blue1);
    previousLED3 = pos;
  }
  if(pos < previousLED3)
  {
    wreath[pos] = CRGB(red2,green2,blue2);
    previousLED3 = pos;
  }                    
}

/****************** DOUBLE CRASH **********************************/
/****************** DOUBLE CRASH **********************************/
/****************** DOUBLE CRASH **********************************/
/****************** DOUBLE CRASH **********************************/
/****************** DOUBLE CRASH **********************************/

void firstCrash()
{
  fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 15);
  int pos1d = beatsin8( raceSpeed, 0, 144 );
  int pos1a = map(pos1d, 0, 144, 0, 115);
  int pos1b = map(pos1d, 0, 144, 0, 119);
  int pos1c = map(pos1d, 0, 144, 0, 95);
  firstFloor[0+pos1a] = CRGB(red1,green1,blue1);
  firstFloor[115-pos1a] = CRGB(red2,green2,blue2);
  firstFloor[115+pos1b] = CRGB(red2,green2,blue2);
  firstFloor[234-pos1b] = CRGB(red1,green1,blue1);
  firstFloor[234+pos1c] = CRGB(red1,green1,blue1);
  firstFloor[329-pos1c] = CRGB(red2,green2,blue2);
  firstFloor[329+pos1d] = CRGB(red2,green2,blue2);
  firstFloor[473-pos1d] = CRGB(red1,green1,blue1);
}

void secondCrash() 
{
  fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 15);
  int pos2a = beatsin8( raceSpeed, 0, 96);
  int pos2b = map(pos2a, 0, 96, 0, 87);
  int pos2c = map(pos2a, 0, 96, 0, 44);
  secondFloor[0+pos2c] = CRGB(red1,green1,blue1);
  secondFloor[44-pos2c] = CRGB(red2,green2,blue2);
  secondFloor[44+pos2c] = CRGB(red2,green2,blue2);
  secondFloor[87-pos2c] = CRGB(red1,green1,blue1);
  secondFloor[95+pos2b] = CRGB(red1,green1,blue1);
  secondFloor[182-pos2b] = CRGB(red2,green2,blue2);
  secondFloor[190+pos2a] = CRGB(red2,green2,blue2);
  secondFloor[286-pos2a] = CRGB(red1,green1,blue1);
  secondFloor[286+pos2a] = CRGB(red1,green1,blue1);
  secondFloor[382-pos2a] = CRGB(red2,green2,blue2);
}

void wreathCrash()
{
  fadeToBlackBy( wreath, WREATH_LEDS, 15);
  int pos = beatsin8( raceSpeed, 0, 246 );
  wreath[0+pos] = CRGB(red1,green1,blue1);
  wreath[246-pos] = CRGB(red2,green2,blue2);
} 

void canesCrash()
{
  fadeToBlackBy( candyCanes, 174, 40);
  int pos = beatsin8( raceSpeed, 0, 15 );
  for(int i = 0; i < 10; i++)
  {
    candyCanes[i+(pos*11)]=CRGB(255,0,0);
    candyCanes[176-(i+(pos*11))]=CRGB(255,0,0);
  }
}


/****************** RAINBOW ***************************************/
/****************** RAINBOW ***************************************/
/****************** RAINBOW ***************************************/
/****************** RAINBOW ***************************************/
/****************** RAINBOW ***************************************/


void firstRainbow()
{
  fill_rainbow( firstFloor, FIRSTFLOOR_LEDS, gHue, numberOfRainbows);
}

void secondRainbow()
{
  fill_rainbow( secondFloor, SECONDFLOOR_LEDS, gHue, numberOfRainbows);
}

void canesRainbow()
{
  fadeToBlackBy( candyCanes, 176, 100);
  for(int i = 0; i < 176; i++)
  {
    candyCanes[i]=CRGB(255,0,0);
  }
}

void wreathRainbow()
{
  fill_rainbow( wreath, WREATH_LEDS, gHue, numberOfRainbows);
}

/****************** BLOCKED COLORS ALTERNATING ********************/
/****************** BLOCKED COLORS ALTERNATING ********************/
/****************** BLOCKED COLORS ALTERNATING ********************/
/****************** BLOCKED COLORS ALTERNATING ********************/
/****************** BLOCKED COLORS ALTERNATING ********************/

void firstBlocked()
{
  fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 2);
  for( int mark = 0; mark < FIRSTFLOOR_LEDS; mark+=30)
  {
    for( int i = 0; i < 9; i++) 
    {
      if(startPosition == 0 || startPosition == 1)
      {
        if(i+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+mark] = CRGB(red1,green1,blue1);
        }
        if(i+10+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+10+mark] = CRGB(red2,green2,blue2);
        }
        if(i+20+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+20+mark] = CRGB(red3,green3,blue3);
        }
      }
      if(startPosition == 2 || startPosition == 3)
      {
        if(i+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+mark] = CRGB(red2,green2,blue2);
        }
        if(i+10+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+10+mark] = CRGB(red3,green3,blue3);
        }
        if(i+20+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+20+mark] = CRGB(red1,green1,blue1);
        }
      }
      if(startPosition == 4 || startPosition == 5)
      {
        if(i+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+mark] = CRGB(red3,green3,blue3);
        }
        if(i+10+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+10+mark] = CRGB(red1,green1,blue1);
        }
        if(i+20+mark <= FIRSTFLOOR_LEDS)
        {
          firstFloor[i+20+mark] = CRGB(red2,green2,blue2);
        }
      }
    }
  }
}

void secondBlocked()
{
  FastLED.setBrightness(128);
  fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 2);
  for( int mark = 0; mark < SECONDFLOOR_LEDS; mark+=30)
  {
    for( int i = 0; i < 9; i++) 
    {
      if(startPosition == 0 || startPosition == 1)
      {
        if(i+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+mark] = CRGB(red1,green1,blue1);
        }
        if(i+10+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+10+mark] = CRGB(red2,green2,blue2);
        }
        if(i+20+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+20+mark] = CRGB(red3,green3,blue3);
        }
      }
      if(startPosition == 2 || startPosition == 3)
      {
        if(i+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+mark] = CRGB(red2,green2,blue2);
        }
        if(i+10+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+10+mark] = CRGB(red3,green3,blue3);
        }
        if(i+20+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+20+mark] = CRGB(red1,green1,blue1);
        }
      }
      if(startPosition == 4 || startPosition == 5)
      {
        if(i+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+mark] = CRGB(red3,green3,blue3);
        }
        if(i+10+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+10+mark] = CRGB(red1,green1,blue1);
        }
        if(i+20+mark <= SECONDFLOOR_LEDS)
        {
          secondFloor[i+20+mark] = CRGB(red2,green2,blue2);
        }
      }
    }
  }
}

void canesBlocked()
{
  fadeToBlackBy( candyCanes, 176, 100);
  if(startPosition == 0 || startPosition == 1)
  {
    for(int cane = 0; cane < 176; cane+=33)
    {
      for(int pixel = 0; pixel < 10; pixel++)
      {
        candyCanes[cane+pixel] = CRGB(255,0,0);
      }  
    }
  }
  if(startPosition == 2 || startPosition == 3)
  {
    for(int cane = 11; cane < 176; cane+=33)
    {
      for(int pixel = 0; pixel < 10; pixel++)
      {
        candyCanes[cane+pixel] = CRGB(255,0,0);
      }  
    }
  }
  if(startPosition == 4 || startPosition == 5)
  {
    for(int cane = 22; cane < 176; cane+=33)
    {
      for(int pixel = 0; pixel < 10; pixel++)
      {
        candyCanes[cane+pixel] = CRGB(255,0,0);
      }  
    }
  }  
}


void wreathBlocked()
{
  fadeToBlackBy( wreath, WREATH_LEDS, 2);
  if(startPosition == 0 || startPosition == 1)
  {
    for(int i=0; i<80; i++)
    {
      wreath[i] = CRGB(red1,green1,blue1);
    }
    for(int i=81; i<148; i++)
    {
      wreath[i] = CRGB(red2,green2,blue2);
    }
    for(int i=149; i<202; i++)
    {
      wreath[i] = CRGB(red3,green3,blue3);
    }
    for(int i=203; i<246; i++)
    {
      wreath[i] = CRGB(red1,green1,blue1);
    }
  }
  if(startPosition == 2 || startPosition == 3)
  {
    for(int i=0; i<80; i++)
    {
      wreath[i] = CRGB(red2,green2,blue2);
    }
    for(int i=81; i<148; i++)
    {
      wreath[i] = CRGB(red3,green3,blue3);
    }
    for(int i=149; i<202; i++)
    {
      wreath[i] = CRGB(red1,green1,blue1);
    }
    for(int i=203; i<246; i++)
    {
      wreath[i] = CRGB(red2,green2,blue2);
    }
  }
  if(startPosition == 4 || startPosition == 5)
  {
    for(int i=0; i<80; i++)
    {
      wreath[i] = CRGB(red3,green3,blue3);
    }
    for(int i=81; i<148; i++)
    {
      wreath[i] = CRGB(red1,green1,blue1);
    }
    for(int i=149; i<202; i++)
    {
      wreath[i] = CRGB(red2,green2,blue2);
    }
    for(int i=203; i<246; i++)
    {
      wreath[i] = CRGB(red3,green3,blue3);
    }
  }
}

/****************** BPM *******************************************/
/****************** BPM *******************************************/
/****************** BPM *******************************************/
/****************** BPM *******************************************/
/****************** BPM *******************************************/

void firstBPM()
{
  if((red1*2) > (green1 + blue1))
  {
    CRGBPalette16 palette1 = LavaColors_p; 
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < FIRSTFLOOR_LEDS; i++) 
    {
      firstFloor[i] = ColorFromPalette(palette1, gHue+(i*2), beat-gHue+(i*10));
    } 
  }
  if((green1*2) > (red1 + blue1))
  {
    CRGBPalette16 palette1 = ForestColors_p; 
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < FIRSTFLOOR_LEDS; i++) 
    {
      firstFloor[i] = ColorFromPalette(palette1, gHue+(i*2), beat-gHue+(i*10));
    } 
  }
  if((blue1*2) > (green1 + red1))
  {
    CRGBPalette16 palette1 = CloudColors_p; 
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < FIRSTFLOOR_LEDS; i++) 
    {
      firstFloor[i] = ColorFromPalette(palette1, gHue+(i*2), beat-gHue+(i*10));
    } 
  }
  if((blue1*2) == (green1 + red1))
  {
    CRGBPalette16 palette1 = PartyColors_p; 
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < FIRSTFLOOR_LEDS; i++) 
    {
      firstFloor[i] = ColorFromPalette(palette1, gHue+(i*2), beat-gHue+(i*10));
    } 
  }
}


void secondBPM()
{
  if((red1*2) > (green1 + blue1))
  {
    CRGBPalette16 palette = LavaColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < SECONDFLOOR_LEDS; i++) 
    {
      secondFloor[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((green1*2) > (red1 + blue1))
  {
    CRGBPalette16 palette = ForestColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < SECONDFLOOR_LEDS; i++) 
    {
      secondFloor[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((blue1*2) > (green1 + red1))
  {
    CRGBPalette16 palette = CloudColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < SECONDFLOOR_LEDS; i++) 
    {
      secondFloor[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((red1*2) == (green1 + blue1))
  {
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < SECONDFLOOR_LEDS; i++) 
    {
      secondFloor[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
}


void canesBPM()
{
  uint8_t beat = beatsin8( BeatsPerMinute, 80, 255);
  for( int i = 0; i < 176; i++) 
  {
    candyCanes[i] = CHSV(0,100,beat);
  }
}

void wreathBPM()
{
  if((red1*2) > (green1 + blue1))
  {
    CRGBPalette16 palette = LavaColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < WREATH_LEDS; i++) 
    {
      wreath[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((green1*2) > (red1 + blue1))
  {
    CRGBPalette16 palette = ForestColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < WREATH_LEDS; i++) 
    {
      wreath[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((blue1*2) > (green1 + red1))
  {
    CRGBPalette16 palette = CloudColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < WREATH_LEDS; i++) 
    {
      wreath[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
  if((red1*2) == (green1 + blue1))
  {
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < WREATH_LEDS; i++) 
    {
      wreath[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
  }
}

/******************** TWINKLE *************************************/
/******************** TWINKLE *************************************/
/******************** TWINKLE *************************************/
/******************** TWINKLE *************************************/
/******************** TWINKLE *************************************/


void firstTwinkle()
{
  fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 80);
  for( int i = 0; i < FIRSTFLOOR_LEDS-5; i+=5)
  {
    if(i <= FIRSTFLOOR_LEDS)
    {
      if (random8() > twinkleChance)
      {
        firstFloor[i] = CRGB( red1, green1, blue1);
      }
      else
      {
        firstFloor[i] = CRGB( 150, 100, 40);
      }  
    }
  }                     
}



void secondTwinkle()
{
  fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 80);
  for( int i = 0; i < SECONDFLOOR_LEDS-5; i+=5)
  {
    if(i <= SECONDFLOOR_LEDS)
    {
      if (random8() > twinkleChance)
      {
        secondFloor[i] = CRGB( red1, green1, blue1);
      }
      else
      {
        secondFloor[i] = CRGB( 150, 100, 40);
      } 
    } 
  }                     
}

void canesTwinkle()
{
  fadeToBlackBy( candyCanes, 175, 80);
  for(int i=0; i<174 ; i++)
  {
    if(random8() >twinkleChance)
    {
      candyCanes[i]= CRGB( red1, green1, blue1);
    }
    else
    {
      candyCanes[i]=CRGB(50,0,0);
    }
  }
}


void wreathTwinkle()
{
  fadeToBlackBy( wreath, 246, 80);
  for(int i=0; i<246 ; i++)
  {
    if(random8() >twinkleChance)
    {
      wreath[i]= CRGB( red1, green1, blue1);
    }
    else
    {
      wreath[i]=CRGB(40, 20, 20);
    }
  }
}

/******************* SPOOKY EYES **********************************/
/******************* SPOOKY EYES **********************************/
/******************* SPOOKY EYES **********************************/
/******************* SPOOKY EYES **********************************/
/******************* SPOOKY EYES **********************************/

void firstEyes()
{
  unsigned int chance = random8();
  if( chance > eyeChance) 
  {
    unsigned int eye = random16(FIRSTFLOOR_LEDS);  
    firstFloor[eye] = CRGB(red1,green1,blue1);
    firstFloor[eye-4] = CRGB(red1,green1,blue1);
  }
  if( chance > 39) 
  {
    fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 10);
  }
}

void secondEyes()
{
  unsigned int chance = random8();
  if( chance > eyeChance) 
  {
    unsigned int eye = random16(SECONDFLOOR_LEDS);  
    secondFloor[eye] = CRGB(red1,green1,blue1);
    secondFloor[eye-4] = CRGB(red1,green1,blue1);
  }
  if( chance > 39) 
  {
    fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 10);
  }
}

/******************* WREATH PUMPKIN ******************************/
/******************* WREATH PUMPKIN ******************************/
/******************* WREATH PUMPKIN ******************************/
/******************* WREATH PUMPKIN ******************************/
/******************* WREATH PUMPKIN ******************************/

void wreathPumpkin()
{
  if(lightning < 2)
  {
    for(int body = 1; body < 79; body++)
    {
      wreath[body]=CRGB( 255, 20, 0); 
    }
    for(int mouth = 164; mouth < 186; mouth++)
    {
      wreath[mouth]=CRGB( 255, 0, 0); 
    }
    for(int mouth2 = 221; mouth2 < 229; mouth2++)
    {
      wreath[mouth2]=CRGB( 255, 0, 0); 
    }
    //stem
    wreath[0]=CRGB( 0, 255, 0); 
    wreath[79]=CRGB( 0, 255, 0);
    //eyes
    wreath[206]=CRGB( 255, 0, 0); 
    wreath[207]=CRGB( 255, 0, 0); 
    wreath[208]=CRGB( 255, 0, 0);
    wreath[242]=CRGB( 255, 0, 0); 
    wreath[241]=CRGB( 255, 0, 0); 
    wreath[240]=CRGB( 255, 0, 0); 
    wreath[154]=CRGB( 255, 0, 0);
    wreath[155]=CRGB( 255, 0, 0);
    wreath[195]=CRGB( 255, 0, 0);
    wreath[194]=CRGB( 255, 0, 0);  
  }
}

void wreathPumpkinLightning()
{
  for(int body = 1; body < 79; body++)
  {
    wreath[body]=CRGB( 0, 255, 0); 
  }
  for(int mouth = 164; mouth < 186; mouth++)
  {
    wreath[mouth]=CRGB(0, 255, 0); 
  }
  for(int mouth2 = 221; mouth2 < 229; mouth2++)
  {
    wreath[mouth2]=CRGB(0, 255, 0); 
  }
  //stem
  wreath[0]=CRGB( 0, 255, 0); 
  wreath[79]=CRGB( 0, 255, 0);
  //eyes
  wreath[206]=CRGB(0, 255, 0);
  wreath[207]=CRGB(0, 255, 0); 
  wreath[208]=CRGB(0, 255, 0);
  wreath[242]=CRGB(0, 255, 0); 
  wreath[241]=CRGB(0, 255, 0); 
  wreath[240]=CRGB(0, 255, 0); 
  wreath[154]=CRGB(0, 255, 0);
  wreath[155]=CRGB(0, 255, 0);
  wreath[195]=CRGB(0, 255, 0);
  wreath[194]=CRGB(0, 255, 0);  
}

/***************************** ALARM ****************************/
/***************************** ALARM ****************************/
/***************************** ALARM ****************************/
/***************************** ALARM ****************************/
/***************************** ALARM ****************************/

void alarm()
{
  chaseDelay = 500;
  if(startPosition == 0 || startPosition == 2 || startPosition == 4)
  {
    fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB::Red);
    fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB::Red);
  }
  if(startPosition == 1 || startPosition == 3 || startPosition == 5)
  {
    fill_solid(secondFloor, SECONDFLOOR_LEDS, 0);
    fill_solid(firstFloor, FIRSTFLOOR_LEDS, 0);
  } 
}

/**************************** FILL SOLID ****************************/
/**************************** FILL SOLID ****************************/
/**************************** FILL SOLID ****************************/
/**************************** FILL SOLID ****************************/
/**************************** FILL SOLID ****************************/

void firstFloorSolid()
{
  fill_solid(firstFloor, FIRSTFLOOR_LEDS, CRGB(red1, green1, blue1));
}

void secondFloorSolid()
{
  fill_solid(secondFloor, SECONDFLOOR_LEDS, CRGB(red1, green1, blue1));
}

void wreathSolid()
{
  fill_solid(wreath, WREATH_LEDS, CRGB(red1, green1, blue1));
}

/**************************** LED LOCATOR ****************************/
/**************************** LED LOCATOR ****************************/
/**************************** LED LOCATOR ****************************/
/**************************** LED LOCATOR ****************************/
/**************************** LED LOCATOR ****************************/

void firstFloorLocator()
{
  if(ledLocator < FIRSTFLOOR_LEDS)
  {
    fadeToBlackBy( firstFloor, FIRSTFLOOR_LEDS, 64);
    firstFloor[ledLocator]=CRGB(red1, green1, blue1);
  }
}

void secondFloorLocator()
{
  if(ledLocator < SECONDFLOOR_LEDS)
  {
    fadeToBlackBy( secondFloor, SECONDFLOOR_LEDS, 64);
    secondFloor[ledLocator]=CRGB(red2, green2, blue2);
  }
}

void wreathLocator()
{
  if(ledLocator < WREATH_LEDS)
  {
    fadeToBlackBy( wreath, WREATH_LEDS, 64);
    wreath[ledLocator]=CRGB(red3, green3, blue3);
  }
}

void candyCanesLocator()
{
  if(ledLocator < CANDYCANE_LEDS)
  {
    fadeToBlackBy( candyCanes, CANDYCANE_LEDS, 64);
    candyCanes[ledLocator]=CRGB(red1, green1, blue1);
  }
}


/******************** EXTRA UNUSED ANIMATIONS ************************/
/******************** EXTRA UNUSED ANIMATIONS ************************/
/******************** EXTRA UNUSED ANIMATIONS ************************/
/******************** EXTRA UNUSED ANIMATIONS ************************/
/******************** EXTRA UNUSED ANIMATIONS ************************/


void candyCaneEachBounce()
{
  fadeToBlackBy( candyCanes, 175, 15);
  int pos = beatsin16( 16, 0, 10 );
  for(int i=0; i<176; i+=11)
  {
    candyCanes[i+pos] = CRGB (200,0,0);
  } 
}

void wreathCrazy()
{
  fadeToBlackBy( wreath, WREATH_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    wreath[beatsin16( i+7, 0, WREATH_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}






/*****************  FIRE EFFECT ********************************/
/*****************  FIRE EFFECT ********************************/
/*****************  FIRE EFFECT ********************************/
/*****************  FIRE EFFECT ********************************/
/*****************  FIRE EFFECT ********************************/


void fireFirstFloor1()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[flat] = color;
    }
}

void fireFirstFloor2()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[234-flat] = color;
    }
}
void fireFirstFloor3()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[234+flat] = color;
    }
}
void fireFirstFloor4()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[329-flat] = color;
    }
}
void fireFirstFloor5()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[329+flat] = color;

    }
}
void fireFirstFloor6()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      firstFloor[473-flat] = color;
    }
}
void fireSecondFloor1()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      secondFloor[flat] = color;
    }
}
void fireSecondFloor2()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      secondFloor[87-flat] = color;
    }
}
void fireSecondFloor3()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      secondFloor[95+flat] = color;
    }
}
void fireSecondFloor4()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);

      secondFloor[182-flat] = color;
    }
}
void fireSecondFloor5()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      secondFloor[190+flat] = color;
    }
}
void fireSecondFloor6()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      secondFloor[382-flat] = color;
    }
}

void fireWreath1()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (39-flat);
      if(thisFlame >= 0 && thisFlame <=39)
      {
        wreath[thisFlame] = color;
      }
    }
}

void fireWreath2()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (40+flat);
      if(thisFlame >= 40 && thisFlame <=113)
      {
        wreath[thisFlame] = color;
      }
    }
}

void fireWreath3()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (114+flat);
      if(thisFlame >= 114 && thisFlame <=176)
      {
        wreath[thisFlame] = color;
      }
    }
}


void fireWreath4()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (114-flat);
      if(thisFlame >= 40 && thisFlame <=176)
      {
        wreath[thisFlame] = color;
      }
    }
}

void fireWreath5()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (176+flat);
      if(thisFlame >= 176 && thisFlame <=224)
      {
        wreath[thisFlame] = color;
      }
    }
}


void fireWreath6()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (176-flat);
      if(thisFlame >= 114 && thisFlame <=224)
      {
        wreath[thisFlame] = color;
      }
    }
}

void fireWreath7()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (224-flat);
      if(thisFlame >= 176 && thisFlame <=WREATH_LEDS)
      {
        wreath[thisFlame] = color;
      }
    }
}

void fireWreath8()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      int flat = map(pixelnumber, 0, 100, 0, fireSize);
      int thisFlame = (224+flat);
      if(thisFlame >= 224 && thisFlame <=WREATH_LEDS)
      {
        wreath[thisFlame] = color;
      }
    }
}
