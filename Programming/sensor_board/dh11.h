#ifndef DH11_H
#define DH11_H

typedef enum {
  DHT11_IDLE_SUCCESSFUL,
  DHT11_READ_IN_PROGRESS,
  DHT11_NO_RESPONSE,
  DHT11_CHECKSUM_ERROR
} DHT11_STATUS;


void dht11_init(void);
bool dht11_start_signal(void);
bool dht11_check_response(void);
uint8_t dht11_read_byte(void);
DHT11_STATUS dht11_read(void);
uint16_t get_dht11_temp_last_value(void);
uint16_t get_dht11_humidity_last_value(void);

#endif
