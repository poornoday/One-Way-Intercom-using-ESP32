 #include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "esp-audio";
const char* password = "12345678";

WiFiUDP udp;
const int udpPort = 5000;

#define DAC_PIN 25

void setup() {
  Serial.begin(115200);
  pinMode(DAC_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  udp.begin(udpPort);
}

void loop() {
  uint8_t packet[240];

  int len = udp.parsePacket();
  if (len == 240) {

    udp.read(packet, 240);

    // 120 samples → play at 16 kHz
    for (int i = 0; i < 120; i++) {

      int16_t sample = packet[i*2] | (packet[i*2+1] << 8);

      // convert PCM to DAC range
      uint8_t dacVal = (sample >> 8) + 128;

      dacWrite(DAC_PIN, dacVal);

      delayMicroseconds(63);   // *** THIS FIXES AUDIO ***
    }
  }
}
