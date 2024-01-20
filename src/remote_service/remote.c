#include "remote.h"
#include "PCF8563.h"


static K_SEM_DEFINE(bt_init_ok, 1, 1);
typedef void (*bt_ready_cb_t)(int err);


// void bt_ready(int err){

//     k_sem_give(&bt_init_ok);
// }

//TODO: make globals

uint16_t test_1[] = { 0, 0, 0, 0};       // testing cap
uint8_t date[] = {0,0,0,0,0,0};          // => day, month, year ; sec, minute, hour
uint8_t system_status[] = {0,0,0,0,0,0}; // => main loop counter, init returns, cdn

static struct bt_conn *default_conn;


// TODO: write function with pointer to pass the buffer array
static ssize_t read_test_char(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, test_1, (sizeof(test_1)*sizeof(uint8_t)));
}
static ssize_t read_date(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    update_date();
    update_time();
    return bt_gatt_attr_read(conn, attr, buf, len, offset, date, (sizeof(date)*sizeof(uint8_t)));
}


static ssize_t write_date(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                          const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    if (offset + len > sizeof(date)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(&date[offset], buf, len);

    write_userDate();
    write_userTime();
    return len;
}


static ssize_t read_system_status(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, system_status, (sizeof(system_status)*sizeof(uint8_t)));
}


BT_GATT_SERVICE_DEFINE(test_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(0x1234)),
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x5678),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           read_test_char, NULL, test_1),

    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x2220),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           read_system_status, NULL, system_status),

    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x6660),
                            BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
                            BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           read_date, write_date, date),
);




void update_value_cnt(uint16_t value){
    system_status[0] = value;
}

void update_system_status(uint16_t value, int idx){
    system_status[idx] = value;
}

void update_value(uint16_t value1, uint16_t value2, uint16_t value3){
    
    test_1[1] = value1;
    test_1[2] = value2;
    test_1[3] = value3;
    
}

void update_date(){
    PCF8563_Get_Days(date);

}

void update_time(){
    PCF8563_Get_Time((date + 3));
}

void write_userDate(){
		PCF8563_Set_Days(date[2], date[1], date[0]);
	

}

void write_userTime(){
		PCF8563_Set_Time(date[5], date[4], date[3]);
}



static void call_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        // printk("Connection failed (err %u)\n", err);
    } else {
        // printk("Connected\n");
        if (!default_conn) {
            default_conn = bt_conn_ref(conn);
        }
    }
}

static void call_disconnected(struct bt_conn *conn, uint8_t reason)
{
    // printk("Disconnected (reason %u)\n", reason);
    if (default_conn) {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}


static struct bt_conn_cb conn_callbacks = {
    .connected = call_connected,
    .disconnected = call_disconnected,
};

int bluetooth_init(void){

    int err = 0;

    err += bt_enable(NULL);
    // k_sem_take(&bt_init_ok, K_FOREVER);

    //err = bt_set_name("nrf");
    err = bt_le_adv_start(BT_LE_ADV_CONN, NULL, 0, NULL, 0);

    bt_conn_cb_register(&conn_callbacks);

    return err;
}

