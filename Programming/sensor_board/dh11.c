#include "project-defs.h"


static GpioConfig dht11_pin = GPIO_PIN_CONFIG(DHT11_PORT, DHT11_PIN, GPIO_BIDIRECTIONAL_MODE);
DHT11_STATUS dht11_status = DHT11_IDLE_SUCCESSFUL;
uint16_t dht11_temp_last_value;
uint16_t dht11_humidity_last_value;

void dht11_init(void) {

  gpioConfigure(&dht11_pin);

}

bool dht11_start_signal(void) {
    gpioWrite(&dht11_pin, 0);
    delay1ms(18);
    gpioWrite(&dht11_pin, 1);
    delay1us(20);

    return true;
}

bool dht11_check_response(void) {
    delay1us(40);
    if (!gpioRead(&dht11_pin)) {
        delay1us(80);
        if (gpioRead(&dht11_pin)) {
            delay1us(80);
            return true;
        }
    }
    return false;
}

uint8_t dht11_read_byte(void) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        while (!gpioRead(&dht11_pin));  // Wait for the pin to go high
        delay1us(30);
        if (gpioRead(&dht11_pin)) {
            result |= (1 << (7 - i));
        }
        while (gpioRead(&dht11_pin));  // Wait for the pin to go low
    }
    return result;
}

DHT11_STATUS dht11_read(void) {
    uint8_t data[5] = {0};

    if (!dht11_start_signal()) {
        return false; // ?!?!? the start signal never returns False
    }

    if (!dht11_check_response()) {
      return DHT11_NO_RESPONSE;
    }

    for (int i = 0; i < 5; i++) {
        data[i] = dht11_read_byte();
    }

    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) {
      return DHT11_CHECKSUM_ERROR;
    }

    dht11_humidity_last_value = data[0] * DHT11_TO_PERCENT;
    dht11_temp_last_value = data[2] * DHT11_TO_CELCIUS;

    return DHT11_IDLE_SUCCESSFUL;
}

uint16_t get_dht11_temp_last_value(void) {

  return dht11_temp_last_value;

}

uint16_t get_dht11_humidity_last_value(void) {

  return dht11_humidity_last_value;

}


