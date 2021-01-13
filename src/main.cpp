#include <Adafruit_BME680.h>
#include <Arduino.h>
#include <ccs811.h>
#include <ThingsBoard.h>
#include <U8x8lib.h>
#include <WiFi.h>

#include "config_private.h"

// The type name is a bit of a mouthful - let's rename it to something
// more easily comprehensible.
typedef U8X8_SSD1306_128X64_NONAME_SW_I2C I2CLcd;

// BME680 chip select pin.
constexpr unsigned int BME_CS = 5;
// Using hardware SPI.
Adafruit_BME680 bme(BME_CS);

// Port on which MQTT server of ThingsBoard is running.
constexpr uint16_t MQTT_PORT = 1883;

CCS811 ccs811;

I2CLcd lcd(
    /* clock=*/ 15,
    /* data=*/ 4,
    /* reset=*/ 16);

WiFiClient client;
ThingsBoard things(client);

String localIP = "";

/**
 * Draws a line on LCD (forward declaration).
 * 
 * @tparam T type of the data to be used
 * @param data data to display
 * @param format format string to use
 * @param line which line to draw on
 */
template<class T>
void draw_data(T data, const char *format, int line);

/**
 * Connects to WiFi network predefined in config_private.h
 * (forward declaration).
 */
void connect_wifi();

/**
 * Configures Serial, connects WiFi, sets up LCD and starts up BME680
 * and CCS811 sensors.
 */
void setup() {
  Serial.begin(115200);

  connect_wifi();

  Wire.begin();

  lcd.setFont(u8x8_font_chroma48medium8_r);
  lcd.begin();

  while (!bme.begin()) {
    Serial.println(F("*** Unable to find BME680. Waiting 3 seconds."));
    delay(3000);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Attempt to initialize CCS811
  while (!ccs811.begin()) {
    // Sometimes when this happens you need to disconnect the board from power
    // and reconnect it again. I wish to know why, too.
    Serial.println("*** Unable to find CCS811. Waiting 3 seconds.");
    delay(3000);
  }

  // Start measuring with CCS811
  if (!ccs811.start(CCS811_MODE_1SEC)) {
    Serial.println("*** Failed to start CCS811 measurement. This program will now stop.");
    lcd.drawString(0, 0, "CCS811 failure");
    while (true) {}
  }
}

/**
 * Where all the actual work happens.
 */
void loop() {
  // Doing CCS811 work while BME is still reading the data.
  bme.beginReading();

  uint16_t eco2, etvoc, errstat, raw;
  ccs811.read(&eco2, &etvoc, &errstat, &raw);

  // Prints out status string explaining current state of CCS811
  Serial.println(ccs811.errstat_str(errstat));

  // TODO(sgzmd): react on error status instead of hardcoded next line
  if (eco2 > 35000) {
    // Not a sane value, can produce at startup.
    return;
  }

  if (!bme.endReading()) {
    draw_data("BME reading failed", "%s", 0);
    return;
  }

  draw_data(eco2, "eCO2: %u", 4);
  draw_data(etvoc, "eTVOC: %u", 5);
  draw_data(bme.temperature, "Temp:\t%.1f", 0);
  draw_data(bme.humidity, "Humid:\t%.1f", 1);
  draw_data(bme.pressure / 100, "Press:\t%d hPa", 2);
  draw_data(bme.gas_resistance / 1000, "Gas:\t%d", 3);

  if (!localIP.isEmpty()) {
    draw_data(localIP.c_str(), "%s", 6);
  }

  // Data reading complete, let's post it to ThingsBoard.
  if (!things.connected()) {
    if (!things.connect(THINGSBOARD_HOST, THINGSBOARD_TOKEN, MQTT_PORT)) {
      Serial.printf("Thingboard down, cannot connect to %s with token %s\n",
                    THINGSBOARD_HOST,
                    THINGSBOARD_TOKEN);
    }
  }

  if (things.connected()) {
    things.sendTelemetryFloat("temperature", bme.temperature);
    things.sendTelemetryInt("eco2", eco2);
    things.sendTelemetryFloat("humidity", bme.humidity);
    things.sendTelemetryInt("etvoc", etvoc);
    things.sendTelemetryFloat("pressure", bme.pressure / 100.0);
    things.sendTelemetryFloat("gas_resistance", bme.gas_resistance / 1000.0);

    things.loop();
  }

  delay(5000);
}

template<class T>
void draw_data(T data, const char *format, int line) {
  lcd.clearLine(line);

  char str[64];
  memset(str, 0, sizeof(str));
  sprintf(str, format, data);
  lcd.drawString(0, line, str);

  Serial.println(str);
}

void connect_wifi() {
  WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  localIP = WiFi.localIP().toString();
}
