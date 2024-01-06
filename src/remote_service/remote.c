#include "remote.h"


static K_SEM_DEFINE(bt_init_ok, 1, 1);
typedef void (*bt_ready_cb_t)(int err);


// void bt_ready(int err){

//     k_sem_give(&bt_init_ok);
// }
static uint16_t test_1[] = { 666, 123};
static uint8_t test_2[] = "test2";

static struct bt_conn *default_conn;

static ssize_t read_test_char(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, test_1, (sizeof(test_1)*sizeof(uint8_t)));
}
static ssize_t read_test_char2(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                              void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, test_2, sizeof(test_2));
}

BT_GATT_SERVICE_DEFINE(test_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(0x1234)),
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x5678),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           read_test_char, NULL, test_1),
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x6660),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           read_test_char2, NULL, test_2),
);



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

