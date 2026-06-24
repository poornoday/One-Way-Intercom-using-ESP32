#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

#define I2S_WS      15
#define I2S_SD      32
#define I2S_SCK     14
#define LED_PIN     2

// -----------------------
// WiFi Setup
// -----------------------
const char* ssid = "esp-audio";
const char* password = "12345678";
WiFiUDP udp;
IPAddress receiverIP(192,168,4,2);   // Receiver’s IP
const int udpPort = 5000;

// -----------------------
// I2S Configuration
// -----------------------
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // WiFi AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("Transmitter WiFi AP started");
  Serial.println(WiFi.softAPIP());

  // I2S config
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 6,
    .dma_buf_len = 60
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {
  int32_t i2sData[120];
  size_t bytesRead;

  i2s_read(I2S_NUM_0, i2sData, sizeof(i2sData), &bytesRead, portMAX_DELAY);

  // Convert 32-bit to 16-bit PCM & store into buffer
  uint8_t sendBuf[240];
  for (int i = 0; i < 120; i++) {
    int16_t sample = (i2sData[i] >> 14);  
    sendBuf[i*2] = sample & 0xFF;
    sendBuf[i*2+1] = (sample >> 8) & 0xFF;
  }

  udp.beginPacket(receiverIP, udpPort);
  udp.write(sendBuf, sizeof(sendBuf));
  udp.endPacket();

  // LED lights when talking
  int amplitude = abs((int16_t)(i2sData[0] >> 14));
  digitalWrite(LED_PIN, amplitude > 2000);
}
