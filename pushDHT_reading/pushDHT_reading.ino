#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "elapsedMillis.h"
#include "DHT.h"

#define PIN 4
#define NUMPIXELS 10

const char *area = "esp8266/bedroom";      //living-room, bathroom, bedroom
const char *ChipID = "ESP8266TestStation";
// Uncomment one of the lines bellow for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Change the credentials below, so your ESP8266 connects to your router
const char *ssid = "TestNetwork";
const char *password = "95$WQQ4vI$r^yt2CV%g6Lt!k";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char *mqtt_server = "192.168.178.22";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// DHT Sensor
const int DHTPin = 2;
const int ledGPIO4 = 4;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

int color[] = {0x80000, 0x110000, 0x1a0000, 0x230000, 0x2b0000, 0x340000, 0x3d0000, 0x460000, 0x4f0000, 0x570000, 0x600000, 0x690000, 0x720000, 0x7b0000, 0x830000, 0x8c0000, 0x950000, 0x9e0000, 0xa70000, 0xaf0000, 0xb80000, 0xc10000, 0xca0000, 0xd30000, 0xdb0000, 0xe40000, 0xed0000, 0xf60000, 0xff0000, 0xff0000, 0xff0200, 0xff0400, 0xff0600, 0xff0800, 0xff0a00, 0xff0d00, 0xff0f00, 0xff1100, 0xff1300, 0xff1500, 0xff1700, 0xff1a00, 0xff1c00, 0xff1e00, 0xff2000, 0xff2200, 0xff2400, 0xff2700, 0xff2900, 0xff2b00, 0xff2d00, 0xff2f00, 0xff3100, 0xff3400, 0xff3600, 0xff3800, 0xff3a00, 0xff3c00, 0xff3e00, 0xff4100, 0xff4300, 0xff4500, 0xff4700, 0xff4900, 0xff4b00, 0xff4e00, 0xff5000, 0xff5200, 0xff5400, 0xff5600, 0xff5800, 0xff5b00, 0xff5d00, 0xff5f00, 0xff6100, 0xff6300, 0xff6500, 0xff6800, 0xff6a00, 0xff6c00, 0xff6e00, 0xff7000, 0xff7200, 0xff7500, 0xff7700, 0xff7900, 0xff7b00, 0xff7d00, 0xff8000, 0xff8000, 0xff8200, 0xff8400, 0xff8700, 0xff8900, 0xff8c00, 0xff8e00, 0xff9100, 0xff9300, 0xff9600, 0xfe9601, 0xfd9602, 0xfd9603, 0xfc9605, 0xfc9606, 0xfb9607, 0xfa9609, 0xfa960a, 0xf9960b, 0xf9960c, 0xf8960e, 0xf7960f, 0xf79610, 0xf69612, 0xf69613, 0xf59614, 0xf59616, 0xf49617, 0xf39618, 0xf39619, 0xf2961b, 0xf2961c, 0xf1961d, 0xf0961f, 0xf09620, 0xef9621, 0xef9622, 0xee9624, 0xed9625, 0xed9626, 0xec9628, 0xec9629, 0xeb962a, 0xeb962c, 0xea962d, 0xe9962e, 0xe9962f, 0xe89631, 0xe89632, 0xe79633, 0xe69635, 0xe69636, 0xe59637, 0xe59639, 0xe4963a, 0xe4963b, 0xe3963c, 0xe2963e, 0xe2963f, 0xe19640, 0xe19642, 0xe09643, 0xdf9644, 0xdf9645, 0xde9647, 0xde9648, 0xdd9649, 0xdc964b, 0xdc964c, 0xdb964d, 0xdb964f, 0xda9650, 0xda9651, 0xd99652, 0xd89654, 0xd89655, 0xd79656, 0xd79658, 0xd69659, 0xd5965a, 0xd5965c, 0xd4965d, 0xd4965e, 0xd3965f, 0xd39661, 0xd29662, 0xd19663, 0xd19665, 0xd09666, 0xd09667, 0xcf9668, 0xce966a, 0xce966b, 0xcd966c, 0xcd966e, 0xcc966f, 0xcb9670, 0xcb9672, 0xca9673, 0xca9674, 0xc99675, 0xc99677, 0xc89678, 0xc79679, 0xc7967b, 0xc6967c, 0xc6967d, 0xc5967f, 0xc49680, 0xc49681, 0xc39682, 0xc39684, 0xc29685, 0xc19686, 0xc19688, 0xc09689, 0xc0968a, 0xbf968b, 0xbf968d, 0xbe968e, 0xbd968f, 0xbd9691, 0xbc9692, 0xbc9693, 0xbb9695, 0xba9696, 0xba9697, 0xb99698, 0xb9969a, 0xb8969b, 0xb8969c, 0xb7969e, 0xb6969f, 0xb696a0, 0xb596a2, 0xb596a3, 0xb496a4, 0xb396a5, 0xb396a7, 0xb296a8, 0xb296a9, 0xb196ab, 0xb096ac, 0xb096ad, 0xaf96ae, 0xaf96b0, 0xae96b1, 0xae96b2, 0xad96b4, 0xac96b5, 0xac96b6, 0xab96b8, 0xab96b9, 0xaa96ba, 0xa996bb, 0xa996bd, 0xa896be, 0xa896bf, 0xa796c1, 0xa796c2, 0xa696c3, 0xa596c5, 0xa596c6, 0xa496c7, 0xa496c8, 0xa396ca, 0xa296cb, 0xa296cc, 0xa196ce, 0xa196cf, 0xa096d0, 0x9f96d1, 0x9f96d3, 0x9e96d4, 0x9e96d5, 0x9d96d7, 0x9d96d8, 0x9c96d9, 0x9b96db, 0x9b96dc, 0x9a96dd, 0x9a96de, 0x9996e0, 0x9896e1, 0x9896e2, 0x9796e4, 0x9796e5, 0x9696e6, 0x9696e8, 0x9595e9, 0x9494ea, 0x9393eb, 0x9292ec, 0x9191ed, 0x9090ee, 0x8e8ef0, 0x8d8df1, 0x8c8cf2, 0x8b8bf3, 0x8a8af4, 0x8989f5, 0x8888f6, 0x8787f7,
               0x8686f8, 0x8585f9, 0x8484fa, 0x8383fb, 0x8282fc, 0x8181fd, 0x8080ff};
int colorindex = 0; //index number for the color lists
int pulsebrightness = 1;

elapsedSeconds lastMeasure;
elapsedSeconds LEDBrighter;
elapsedMillis LEDBrighterPulse; //pulse speed

bool trigger = false;
bool alarm = false;
bool pulse = false;

bool pulsating_led = false;

void setup() {
  dht.begin();

  void LEDOff();
  void sunset();
  void relax();
  void evening();
  void pink();
  void pulsating(int); //initilizes functions

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pinMode(ledGPIO4, OUTPUT);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

      if (LEDBrighter > 6 && alarm == true && trigger == true)
    {
        colorindex++; //increases index by one so the function takes the next value from the lists above

        if (colorindex < 299)
        { //stops at 299 because there are only 299 values in a list
            int red = (color[colorindex] >> 16) & 0xFF; //bit shifting
            int green = (color[colorindex] >> 8) & 0xFF;
            int blue = color[colorindex] & 0xFF; //takes the colorindex number and applies the RGB value that is indexed by this number

            for (int i = 0; i < NUMPIXELS; i++)
            {

                pixels.setPixelColor(i, pixels.Color(red, green, blue));
                pixels.show();
                LEDBrighter = 0;
            }
        }
    }



    if (pulse == true and LEDBrighterPulse > 100) //100 milliseconds timer
    {
        pulsating(pulsebrightness);
        if (pulsebrightness <= 115 and pulsating_led == false) //if LED is under 115 RGB value count up
        {
            pulsebrightness += 1;
        }
        else
        {
            pulsating_led = true;   
            pulsebrightness -= 1;  //if RGB value equals 115 count down   
        }
        if (pulsebrightness == 0)
        {
            pulsating_led = false;
        }
        LEDBrighterPulse = 0;
    }
    
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()){
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       client.connect("ESP8266Client");
     If you want more devices connected to the MQTT broker, you can do it like this:
       client.connect("ESPOffice");
     Then, for the other ESP:
       client.connect("ESPGarage");
      That should solve your MQTT multiple connections problem

     THE SECTION IN recionnect() function should match your device name
    */
    client.connect("ESP8266Client");

  }
  // Publishes new temperature and humidity every 10 seconds
  if (lastMeasure > 10) {

    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    
    // Uncomment to compute temperature values in Fahrenheit 
    // float hif = dht.computeHeatIndex(f, h);
    // static char temperatureTemp[7];
    // dtostrf(hic, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    // Publishes Temperature and Humidity values
    client.publish("/esp8266/temperature", temperatureTemp);
    client.publish("/esp8266/humidity", humidityTemp);
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
    // Serial.print(hif);
    // Serial.println(" *F");
    lastMeasure = 0;
  }
}

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
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
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        //Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }

    if (topic == "esp8266/CustomColor")
    {
        String red = messageTemp.substring(0, 3);
        String green = messageTemp.substring(4, 7);
        String blue = messageTemp.substring(8, 11); //CustomColor comes in string xxx;xxx;xxx, 3 lines extract those values

        for (int i = 0; i < NUMPIXELS; i++)
        {
            pixels.setPixelColor(i, pixels.Color(red.toInt() - 100, green.toInt() - 100, blue.toInt() - 100)); //in html form RGB values are + 100, brings it back to original value
            pixels.show();
        }
    }

    if (topic == "esp8266/all") //all is only for LEDoff and alarm, pushes to all ESP's no mather what channel
    {
        if (messageTemp == "0")
        {
            trigger = false;
            pulse = false;

            LEDOff();
            colorindex = 0;
        }


        else if (messageTemp == "alarm1")
        {
            alarm = true;

        }
        
        else if (messageTemp == "alarm0")
        {
            alarm = false;
        }
        else if (messageTemp == "trigger")
        {
            trigger = true;

        }
    }

    if (topic == area)                       //specific area ESP are talked to here
    { //living-room, bathroom, bedroom
        if (messageTemp == "1")
        {
            pulse = false;
            sunset();
        }
        
        else if (messageTemp == "2")
        {
            pulse = false;
            relax();
        }
        
        else if (messageTemp == "3")
        {
            pulse = false;
            evening();
        }
        
        else if (messageTemp == "4")
        {
            pulse = false;
            pink();
        }
        
        else if (messageTemp == "5")
        {
            pulse = true;
        }


        if (messageTemp == "0")
        {
            LEDOff();
            colorindex = 0;
        }
    }
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect


    if (client.connect(ChipID)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
        client.subscribe("esp8266/all");
        client.subscribe("esp8266/CustomColor");
        client.subscribe(area); //living-room, bathroom, bedroom
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
