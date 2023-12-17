#include "remote.h"


static K_SEM_DEFINE(bt_init_ok, 1, 1);
typedef void (*bt_ready_cb_t)(int err);


// void bt_ready(int err){

//     k_sem_give(&bt_init_ok);
// }

static struct bt_conn *default_conn;

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

