#include <MaixAmigo.h>
#include <driver/i2s.h>

#define CONFIG_I2S_BCK_PIN 12
#define CONFIG_I2S_LRCK_PIN 0
#define CONFIG_I2S_DATA_PIN 2
#define CONFIG_I2S_DATA_IN_PIN 34

#define Speak_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1
#define DATA_SIZE 1024

uint8_t microphonedata0[1024 * 100];
int data_offset = 0;

bool InitI2SSpeakOrMic(int mode)
{
    esp_err_t err = ESP_OK;
    i2s_driver_uninstall(Speak_I2S_NUMBER);
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };
    if (mode == MODE_MIC)
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    }
    else
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll = false;
        i2s_config.tx_desc_auto_clear = true;
    }
    err += i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);
    i2s_pin_config_t tx_pin_config;
    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;
    err += i2s_set_pin(Speak_I2S_NUMBER, &tx_pin_config);
    err += i2s_set_clk(Speak_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    return true;
}

void DisplayInit(void)
{
  MA.Lcd.fillScreen(TFT_WHITE);
  MA.Lcd.setTextColor(TFT_BLACK);
  MA.Lcd.setTextSize(2);
}

void setup() {
  MA.begin(true, true, true, true);
  MA.Axp.SetSpkEnable(true);
  DisplayInit();
  MA.Lcd.setTextColor(TFT_RED);
  MA.Lcd.setCursor(10, 10);
  MA.Lcd.printf("Recorder!");
  MA.Lcd.setTextColor(TFT_BLACK);
  MA.Lcd.setCursor(10, 26);
  MA.Lcd.printf("Press Left Button to recording!");
  delay(100);
}

void loop() {
  TouchPoint_t pos= MA.Touch.getPressPoint();
  if(pos.y > 240)
    if(pos.x < 109)
    {
        MA.Axp.SetLDOEnable(3,true);
        delay(100);
        MA.Axp.SetLDOEnable(3,false); 
        data_offset = 0;
        InitI2SSpeakOrMic(MODE_MIC);
        size_t byte_read;
        while (1)
        {
            i2s_read(Speak_I2S_NUMBER, (char *)(microphonedata0 + data_offset), DATA_SIZE, &byte_read, (100 / portTICK_RATE_MS));
            data_offset += 1024;
            if(data_offset == 1024 * 100 || MA.Touch.ispressed() != true)
              break;             
        }
        size_t bytes_written;
        InitI2SSpeakOrMic(MODE_SPK);
        i2s_write(Speak_I2S_NUMBER, microphonedata0, data_offset, &bytes_written, portMAX_DELAY);
    }
}
