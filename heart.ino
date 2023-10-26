#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include "Adafruit_GFX.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <NTPClient.h>

#include <WiFiManager.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>


#include <Adafruit_MLX90614.h>

#include <Pangodream_18650_CL.h>
Pangodream_18650_CL BL;

#define ADC_PIN 34
#define CONV_FACTOR 1.7
#define READS 20



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

const char* ssid = "solo";
const char* password = "123412345";


int plus;
Adafruit_MPU6050 mpu;

int menuOption = 5;  // Opsi menu saat ini
int menuBerubah = 1;

#define BUTTON_PIN 21    // GIOP21 pin connected to button
#define BUTTON_ATAS 19   // GIOP21 pin connected to button
#define BUTTON_BAWAH 18  // GIOP21 pin connected to button

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin


#define REPORTING_PERIOD_MS 200

// 'wifi by Freepik', 40x40px ICON
const unsigned char wifiIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x7f,
  0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff,
  0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xfc, 0x00, 0x3f, 0xfe, 0xff, 0xe0, 0x00, 0x07,
  0xff, 0xff, 0x87, 0xff, 0xe1, 0xff, 0x7f, 0x1f, 0xff, 0xf8, 0xff, 0x7e, 0x7f, 0xff, 0xfe, 0x7e,
  0x3c, 0xff, 0xff, 0xff, 0x3c, 0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xe7, 0xff, 0xc0, 0x03,
  0xfe, 0x00, 0x3f, 0xe0, 0x03, 0xf8, 0x7e, 0x1f, 0xc0, 0x01, 0xe3, 0xff, 0xc7, 0x80, 0x00, 0xc7,
  0xff, 0xe3, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff,
  0xf8, 0x00, 0x00, 0x0f, 0xc3, 0xf0, 0x00, 0x00, 0x07, 0x81, 0xe0, 0x00, 0x00, 0x03, 0x3c, 0xc0,
  0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
  0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
  0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
///heart ICON
static const unsigned char PROGMEM logo2_bmp[] = {
  0x03,
  0xC0,
  0xF0,
  0x06,
  0x71,
  0x8C,
  0x0C,
  0x1B,
  0x06,
  0x18,
  0x0E,
  0x02,
  0x10,
  0x0C,
  0x03,
  0x10,
  0x04,
  0x01,
  0x10,
  0x04,
  0x01,
  0x10,
  0x40,
  0x01,
  0x10,
  0x40,
  0x01,
  0x10,
  0xC0,
  0x03,
  0x08,
  0x88,
  0x02,
  0x08,
  0xB8,
  0x04,
  0xFF,
  0x37,
  0x08,
  0x01,
  0x30,
  0x18,
  0x01,
  0x90,
  0x30,
  0x00,
  0xC0,
  0x60,
  0x00,
  0x60,
  0xC0,
  0x00,
  0x31,
  0x80,
  0x00,
  0x1B,
  0x00,
  0x00,
  0x0E,
  0x00,
  0x00,
  0x04,
  0x00,
};

static const unsigned char PROGMEM logo3_bmp[] = { 0x01, 0xF0, 0x0F, 0x80, 0x06, 0x1C, 0x38, 0x60, 0x18, 0x06, 0x60, 0x18, 0x10, 0x01, 0x80, 0x08,
                                                   0x20, 0x01, 0x80, 0x04, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0xC0, 0x00, 0x08, 0x03,
                                                   0x80, 0x00, 0x08, 0x01, 0x80, 0x00, 0x18, 0x01, 0x80, 0x00, 0x1C, 0x01, 0x80, 0x00, 0x14, 0x00,
                                                   0x80, 0x00, 0x14, 0x00, 0x80, 0x00, 0x14, 0x00, 0x40, 0x10, 0x12, 0x00, 0x40, 0x10, 0x12, 0x00,
                                                   0x7E, 0x1F, 0x23, 0xFE, 0x03, 0x31, 0xA0, 0x04, 0x01, 0xA0, 0xA0, 0x0C, 0x00, 0xA0, 0xA0, 0x08,
                                                   0x00, 0x60, 0xE0, 0x10, 0x00, 0x20, 0x60, 0x20, 0x06, 0x00, 0x40, 0x60, 0x03, 0x00, 0x40, 0xC0,
                                                   0x01, 0x80, 0x01, 0x80, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x30, 0x0C, 0x00,
                                                   0x00, 0x08, 0x10, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x01, 0x80, 0x00 };



/////SP02 ICON 24
const unsigned char oksigen[] PROGMEM = {

  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff

};

///32
const unsigned char oksigen1[] PROGMEM = {

  0xff, 0xff, 0xff, 0xff, 0xdd, 0xff, 0xcf, 0x3f, 0xbe, 0xff, 0xbf, 0xdf, 0xff, 0xfe, 0xff, 0xf7,
  0xff, 0xff, 0xdf, 0xff, 0xff, 0xfd, 0xbf, 0xfb, 0xbe, 0xfb, 0xff, 0xfd, 0xdd, 0xfb, 0xff, 0xfd,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7c, 0xff, 0xff, 0xfd, 0xff, 0x7f, 0xff,
  0xf7, 0xff, 0xdf, 0xfd, 0xef, 0xff, 0xef, 0xfd, 0xff, 0xff, 0xff, 0xfb, 0xdf, 0x9f, 0xf7, 0xff,
  0xfe, 0xf7, 0xfb, 0xf7, 0xbe, 0xf7, 0xfb, 0xdf, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xbe, 0xf7, 0xfb, 0xff,
  0xfe, 0xf7, 0x7b, 0xc7, 0xdf, 0x9e, 0x77, 0xbf, 0xff, 0xff, 0xff, 0x7d, 0xef, 0xff, 0xef, 0x7f,
  0xf7, 0xff, 0xdf, 0x7f, 0xfd, 0xff, 0x7f, 0xfd, 0xff, 0x7d, 0xff, 0xdb, 0xff, 0xff, 0xff, 0xff
};

const unsigned char oksigen2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00,
  0x00, 0xc6, 0x00, 0x00, 0x00, 0x83, 0x01, 0x00, 0x00, 0x03, 0x03, 0x00, 0x80, 0x01, 0x03, 0x00,
  0xc0, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x06, 0x00, 0x60, 0x00, 0x0c, 0x00, 0x60, 0x00, 0x0c, 0x00,
  0x20, 0x00, 0x08, 0x00, 0x30, 0x00, 0x0e, 0x00, 0x30, 0x80, 0x3f, 0x00, 0x30, 0xc0, 0x30, 0x00,
  0x30, 0xc0, 0x60, 0x00, 0x30, 0x40, 0x60, 0x00, 0x60, 0x60, 0x60, 0x0d, 0x60, 0x60, 0x60, 0x09,
  0xc0, 0x40, 0x60, 0x04, 0x80, 0xc3, 0x30, 0x02, 0x00, 0x9f, 0x1f, 0x01, 0x00, 0x18, 0x0f, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int sensorDataHeart[REPORTING_PERIOD_MS];  // Array untuk menyimpan data
int sensorDataOxy[REPORTING_PERIOD_MS];    // Array untuk menyimpan data
int sensorDataSuhu[10];                    // Array untuk menyimpan data

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

PulseOximeter pox;

uint32_t tsLastReport = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);  // Gunakan server NTP yang tersedia



String data = "";
bool customLoopActive = true;

bool customAll = true;




// void display_battery() {
//   display.clearDisplay();

//   display.display();
//   //delay(200);

//   // String formattedTime = timeClient.getFormattedTime();  // Assuming formattedTime is in format "HH:MM:SS"

//   // int colonPos = formattedTime.indexOf(':');

//   // // Extract hour substring
//   // String hourString = formattedTime.substring(0, colonPos);

//   // // Extract minute substring
//   // String minuteString = formattedTime.substring(colonPos + 1, colonPos + 3);  // Assumes minutes are always two digits

//   // int hour = hourString.toInt();      // Convert string to integer
//   // int minute = minuteString.toInt();  // Convert string to integerg
// }

bool cek = true;

void ConnectWIFI() {

  WiFiManager wifiManager;

  // Coba untuk terhubung ke jaringan WiFi yang sudah disimpan
  if (!wifiManager.autoConnect("ALKES")) {
    display.clearDisplay();
    display.drawBitmap(44, 5, wifiIcon, 40, 40, WHITE);

    display.println("Mencari akses poin lain...");
    display.display();



    // Loop untuk mencoba terhubung ke jaringan WiFi baru
    while (!wifiManager.autoConnect()) {
      display.clearDisplay();

      display.setCursor(25, 50);
      display.print("Connecting ...");
      display.display();

      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.drawBitmap(44, 5, wifiIcon, 40, 40, WHITE);
  display.setCursor(33, 50);
  display.print("Connected!");
  display.display();
}
////////////////

void setup() {
  Serial.begin(115200);
  // WiFi.begin(ssid, password);
  // Serial.println("Connecting");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
  // Serial.println(WiFi.localIP());

  // Start WiFiManager for configuration
  // wifiManager.resetSettings();
  // WiFiManager wifiManager;

  // wifiManager.autoConnect("ALKES");  // "AutoConnectAP" is the name of the access point

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.clearDisplay();
  display.drawBitmap(44, 5, wifiIcon, 40, 40, WHITE);

  display.setCursor(0, 45);
  display.println("Sambungkan WiFi anda ke ALKES");
  display.display();
  display.display();

  Serial.println("MPU6050 display demo");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU6050 sensor");


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  ConnectWIFI();
  delay(3000);
  timeClient.begin();
  timeClient.update();

  display.display();

  Serial.print("Initializing pulse oximeter..");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ATAS, INPUT_PULLUP);
  pinMode(BUTTON_BAWAH, INPUT_PULLUP);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    // while (1)
    //   ;
  };
}



void displayMenu() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  // display.print(map(BL.getBatteryChargeLevel(), 0, 10, 0, 100));
  // display.println("%");


  switch (menuOption) {
    case 0:

      display.fillRect(0, 0, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.println("DETAK JANTUNG");


      display.setTextColor(SSD1306_WHITE);
      display.println("SPO2");


      display.println("AKSELERASI");

      display.println("SUHU");


      display.println("SEMUA DATA");
      display.println("BERANDA");
      break;
    case 1:


      display.println("DETAK JANTUNG");


      display.fillRect(0, 8, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);

      display.println("SPO2");
      display.setTextColor(SSD1306_WHITE);




      display.println("AKSELERASI");

      display.println("SUHU");





      display.println("SEMUA DATA");
      display.println("BERANDA");

      break;
    case 2:


      display.println("DETAK JANTUNG");


      display.println("SPO2");


      display.fillRect(0, 16, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);

      display.println("AKSELERASI");


      display.setTextColor(SSD1306_WHITE);

      display.println("SUHU");



      display.println("SEMUA DATA");
      display.println("BERANDA");

      break;
    case 3:


      display.println("DETAK JANTUNG");


      display.println("SPO2");


      display.println("AKSELERASI");



      display.fillRect(0, 24, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);

      display.println("SUHU");

      display.setTextColor(SSD1306_WHITE);


      display.println("SEMUA DATA");
      display.println("BERANDA");


      break;
    case 4:

      display.println("DETAK JANTUNG");

      display.println("SPO2");


      display.println("AKSELERASI");
      display.println("SUHU");



      display.fillRect(0, 32, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.println("SEMUA DATA");

      display.setTextColor(SSD1306_WHITE);

      display.println("BERANDA");


      break;


    case 5:

      display.println("DETAK JANTUNG");

      display.println("SPO2");


      display.println("AKSELERASI");
      display.println("SUHU");



      display.println("SEMUA DATA");



      display.fillRect(0, 40, 128, 8, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.println("BERANDA");


      break;
  }



  display.display();
}


void loop() {

  Pangodream_18650_CL BL(ADC_PIN, CONV_FACTOR, READS);


  // Serial.print("Value from pin: ");
  // Serial.println(analogRead(34));
  // Serial.print("Average value from pin: ");
  // Serial.println(BL.pinRead());
  // Serial.print("Volts: ");
  // Serial.println(BL.getBatteryVolts());
  // Serial.print("Charge level: ");
  // Serial.println(BL.getBatteryChargeLevel());


  // Serial.print("Hasil mapping: ");
  // Serial.println(map(BL.getBatteryChargeLevel(), 0, 10, 0, 100));
  Serial.println("");

  if (menuBerubah == 1) {
    display_jam_awal();
    if (digitalRead(BUTTON_ATAS) == LOW || digitalRead(BUTTON_BAWAH) == LOW) {
      menuBerubah = 0;
    }
  }

  if (digitalRead(BUTTON_ATAS) == LOW) {
    menuOption--;
    if (menuOption < 0) {
      menuOption = 5;  // Jumlah opsi menu minus satu
    }
    displayMenu();
    delay(200);  // Hindari bouncing tombol
  }

  if (digitalRead(BUTTON_BAWAH) == LOW) {
    menuOption++;
    if (menuOption > 5) {
      menuOption = 0;
    }
    displayMenu();
    delay(200);  // Hindari bouncing tombol
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    // Logika ketika tombol "Select" ditekan
    // Contoh:
    if (menuOption == 0) {
      // Aksi yang terkait dengan opsi pertama
      while (!pox.begin()) {
        Serial.println("FAILED");
      }
      display.clearDisplay();
      Serial.println("Counting...");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Counting...");
      display.display();
      data = "";
      int heart = 0;
      int bb = 0;

      for (int x = 0; x < REPORTING_PERIOD_MS; x++) {
        pox.update();

        bb = pox.getHeartRate();

        if (bb > 0) {
          heart = heart + bb;
          Serial.print(".");
          sensorDataHeart[x] = bb;
        }
        delay(REPORTING_PERIOD_MS / 2);
      }

      int dataSizeHeart = sizeof(sensorDataHeart) / sizeof(sensorDataHeart[0]);
      int modeValueHeart = calculateMode(sensorDataHeart, dataSizeHeart);

      // Menampilkan hasil modus di Serial Monitor
      for (int i = 0; i < REPORTING_PERIOD_MS; i++) {
        Serial.print("Data ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(sensorDataHeart[i]);
      }

      Serial.print("Modus dari data heart adalah: ");
      Serial.println(modeValueHeart);

      ///serial
      display.clearDisplay();
      Serial.println("");
      Serial.print("hr: ");
      if (bb > 0) {
        Serial.print("hr: ");
        Serial.println(heart);
        Serial.print("modus: ");


        Serial.print(modeValueHeart);
      } else {
        Serial.print("");
      }
      Serial.println("BPM");

      ///display
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10, 3);
      bool logo = true;

      // display.print("DETAK JANTUNG: ");
      if (bb > 0) {
        while (logo) {
          if (digitalRead(BUTTON_ATAS) == LOW || digitalRead(BUTTON_BAWAH) == LOW) {
            break;
          }
          display.clearDisplay();  // Clear the display

          // Draw and display logo2_bmp
          display.drawBitmap(5, 5, logo2_bmp, 24, 21, WHITE);
          display.println("");
          display.setTextSize(2);
          display.setCursor(0, 40);
          display.print(modeValueHeart);
          display.print(" BPM");
          display.display();
          delay(500);
          display.clearDisplay();  // Clear the display

          // Draw and display logo3_bmp
          display.drawBitmap(0, 0, logo3_bmp, 32, 32, WHITE);
          display.println("");
          display.setTextSize(2);
          display.setCursor(0, 40);
          display.print(modeValueHeart);
          display.print(" BPM");
          display.display();
          delay(500);
        }
      } else {
        display.print("");
      }
      display.println("BPM");
      display.display();

    } else if (menuOption == 1) {
      while (!pox.begin()) {
        Serial.println("FAILED");
      }


      display.clearDisplay();
      Serial.println("Counting...");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Counting...");
      display.display();
      data = "";

      int oxy = 0, countOxy = 0;

      // int posX = 0, posY = 0, posZ = 0;
      //

      int aa = 0;
      // baca pox
      for (int x = 0; x < REPORTING_PERIOD_MS; x++) {
        pox.update();

        aa = pox.getSpO2();

        if (aa > 0) {
          countOxy++;
          oxy = oxy + aa;
          Serial.print(".");
          sensorDataOxy[x] = aa;
        }
        delay(REPORTING_PERIOD_MS / 2);
      }


      int dataSizeOxy = sizeof(sensorDataOxy) / sizeof(sensorDataOxy[0]);


      int modeValueOxy = calculateMode(sensorDataOxy, dataSizeOxy);

      // Menampilkan hasil modus di Serial Monitor

      display.clearDisplay();
      Serial.println("");

      Serial.print("oxy: ");
      if (aa > 0) {
        Serial.print(modeValueOxy);
      } else {
        Serial.print("");
      }
      Serial.println("%");

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      // display.print("TINGKAT OKSIGEN: ");
      bool logo = true;
      if (aa > 0) {
        while (logo) {
          if (digitalRead(BUTTON_ATAS) == LOW || digitalRead(BUTTON_BAWAH) == LOW) {
            break;
          }
          display.clearDisplay();  // Clear the display

          // Draw and display logo2_bmp
          display.drawBitmap(5, 5, oksigen, 24, 24, WHITE);
          display.println("");
          display.setTextSize(2);
          display.setCursor(0, 40);
          display.print(modeValueOxy);
          display.print(" %");
          display.display();
          delay(500);
          display.clearDisplay();  // Clear the display

          // Draw and display logo3_bmp
          display.drawBitmap(0, 0, oksigen1, 32, 32, WHITE);
          display.println("");
          display.setTextSize(2);
          display.setCursor(0, 40);
          display.print(modeValueOxy);
          display.print(" %");
          display.display();
          delay(500);
        }
      } else {
        display.print("");
      }
      display.println("%");
      display.display();

    } else if (menuOption == 2) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      display.println("AKSELOMETER:");
      display.print("-X:");
      display.println(a.acceleration.x, 1);
      display.print("-Y:");
      display.println(a.acceleration.y, 1);
      display.print("-Z:");
      display.println(a.acceleration.z, 1);
      delay(100);
      display.display();

      //


    } else if (menuOption == 3) {

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);



      display.println("suhu:");



      int cc = 0;
      for (int x = 0; x < 10; x++) {


        cc = mlx.readObjectTempC();

        Serial.print(".");
        sensorDataSuhu[x] = cc;

        delay(50);
      }

      int dataSizeSuhu = sizeof(sensorDataSuhu) / sizeof(sensorDataSuhu[0]);

      int modeValueSuhu = calculateMode(sensorDataSuhu, dataSizeSuhu);

      display.print(modeValueSuhu);
      display.println("C");

      display.display();







    } else if (menuOption == 4) {




      while (!pox.begin()) {
        Serial.println("FAILED");
      }


      display.clearDisplay();
      Serial.println("Counting...");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Counting...");
      display.display();
      data = "";

      int oxy = 0, countOxy = 0;
      int heart = 0;

      // int posX = 0, posY = 0, posZ = 0;
      //

      int aa = 0;
      int bb = 0;
      // baca pox
      for (int x = 0; x < REPORTING_PERIOD_MS; x++) {
        pox.update();

        aa = pox.getSpO2();
        bb = pox.getHeartRate();

        if (bb > 0) {
          countOxy++;
          oxy = oxy + aa;
          heart = heart + bb;
          Serial.print(".");
          sensorDataHeart[x] = bb;
          sensorDataOxy[x] = aa;
        }
        delay(REPORTING_PERIOD_MS / 2);
      }

      int dataSizeHeart = sizeof(sensorDataHeart) / sizeof(sensorDataHeart[0]);
      int dataSizeOxy = sizeof(sensorDataOxy) / sizeof(sensorDataOxy[0]);

      int modeValueHeart = calculateMode(sensorDataHeart, dataSizeHeart);
      int modeValueOxy = calculateMode(sensorDataOxy, dataSizeOxy);

      // Menampilkan hasil modus di Serial Monitor

      for (int i = 0; i < REPORTING_PERIOD_MS; i++) {
        Serial.print("Data ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(sensorDataHeart[i]);
      }

      Serial.print("Modus dari data heart adalah: ");
      Serial.println(modeValueHeart);



      display.clearDisplay();
      Serial.println("");
      Serial.print("hr: ");
      if (bb > 0) {
        Serial.print("hr: ");
        Serial.println(heart);
        Serial.print("modus: ");

        Serial.print(modeValueHeart);
      } else {
        Serial.print("");
      }
      Serial.println("BPM");

      Serial.print("oxy: ");
      if (aa > 0) {
        Serial.print(modeValueOxy);
      } else {
        Serial.print("");
      }
      Serial.println("%");

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("DETAK JANTUNG: ");
      if (bb > 0) {
        display.print(modeValueHeart);
      } else {
        display.print("");
      }
      display.println("BPM ");


      display.print("TINGKAT OKSIGEN: ");
      if (aa > 0) {
        display.print(modeValueOxy);
      } else {
        display.print("");
      }
      display.println("%");

      //mlx
      display.print("SUHU:");
      display.print(mlx.readObjectTempC());
      display.println("C");


      // mpu
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      display.println("AKSELOMETER :");
      display.print("-X: ");
      display.println(a.acceleration.x, 1);
      display.print("-Y:");
      display.println(a.acceleration.y, 1);
      display.print("-Z:");
      display.println(a.acceleration.z, 1);
      display.display();
      String serverName2 = "http://127.0.0.1:3000/senddatatosps?hr=" + String(modeValueHeart) + "&spo2=" + String(modeValueOxy) + "&akselox=" + String(a.acceleration.x) + "&akseloy=" + String(a.acceleration.y) + "&akseloz=" + String(a.acceleration.z);

      String sensorReadings = httpGETRequest(serverName2);
      Serial.println(sensorReadings);
      Serial.println(serverName2);



    } else if (menuOption == 5) {
      delay(1000);


      while (customAll) {
        display_jam_awal();

        if (digitalRead(BUTTON_ATAS) == LOW || digitalRead(BUTTON_BAWAH) == LOW) {
          break;

          //TRUE
          //CONDITION  OR FALSE
        }
      }
    }

    Serial.println(menuOption);
  }
}


void updateDisplay(String timeStr, String dateStr) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  int screenWidth = 128;  // Lebar layar display
  // Mengatur koordinat x berdasarkan lebar layar
  int xRect = screenWidth - 3.5 - 20;
  xRect -= 5;
  int xText = screenWidth - 3.5 - 20;
  int xText2 = screenWidth - 0.5 - 24;

  display.drawRect(xRect, 5, 25, 10, WHITE);
  display.drawRect(xRect + 25, 7.5, 2.5, 5, WHITE);

  display.setTextColor(WHITE);
  display.setTextSize(1);
  // Menempatkan teks "battery" di sebelah kanan objek
  display.setCursor(xText2, 5 + 2);
  display.print(map(BL.getBatteryChargeLevel(), 0, 10, 0, 100));
  display.print("%");
  display.println("");

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(dateStr);



  // display_battery();

  display.setTextSize(3);
  display.setCursor(0, 25);

  display.println(timeStr);
  display.display();
}

void display_jam_awal() {

  unsigned long lastMinute = 61;  // Inisialisasi dengan angka yang tidak mungkin dalam menit (1 lebih dari 60)
  time_t epochTime = timeClient.getEpochTime();

  struct tm* ptm = gmtime((time_t*)&epochTime);


  timeClient.update();  // Memperbarui waktu dari server NTP
  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;

  // Periksa apakah menit telah berubah
  if (timeClient.getSeconds() > 0 && timeClient.getMinutes() != lastMinute) {
    lastMinute = timeClient.getMinutes();

    String mins;
    if (timeClient.getMinutes() < 10) {
      mins = "0" + String(timeClient.getMinutes());
    } else {
      mins = String(timeClient.getMinutes());
    }

    String timeStr = String(timeClient.getHours()) + ":" + mins;
    String dateStr = String(day) + "/" + String(month) + "/" + String(year);

    updateDisplay(timeStr, dateStr);

    Serial.println(timeStr);
  }

  delay(1000);  // Tunggu selama 1 detik sebelum memeriksa kembali
}


String httpGETRequest(String serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


int calculateMode(int data[], int dataSize) {
  // Membuat array untuk menghitung frekuensi kemunculan setiap elemen
  int counts[dataSize];

  // Mengisi array counts dengan 0
  for (int i = 0; i < dataSize; i++) {
    counts[i] = 0;
  }

  // Menghitung frekuensi kemunculan setiap elemen
  for (int i = 0; i < dataSize; i++) {
    for (int j = 0; j < dataSize; j++) {
      if (data[j] == data[i]) {
        counts[i]++;
      }
    }
  }

  // Mencari elemen dengan frekuensi kemunculan tertinggi (modus)
  int maxCount = 0;
  int modeValue = -1;

  for (int i = 0; i < dataSize; i++) {
    if (counts[i] > maxCount) {
      maxCount = counts[i];
      modeValue = data[i];
    }
  }

  return modeValue;
}
