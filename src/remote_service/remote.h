
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/gatt.h>


int bluetooth_init(void);
void update_value_cnt(uint16_t value);
void update_system_status(uint16_t value, int idx);
void update_value(uint16_t value1, uint16_t value2, uint16_t value3);
void update_date();
void update_time();

