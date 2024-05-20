include <zephyr/kernel.h>
include <zephyr/net/socket.h>
include <zephyr/net/mqtt.h>
include <string.h>
include <zephyr/net/net_ip.h>

/* Buffers pour les données reçues et à envoyer par le client MQTT. */
static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];

static struct mqtt_client client;

static struct sockaddr_storage broker;

void configure_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(1883); // Default MQTT port
    inet_pton(AF_INET, "127.0.0.1", &broker4->sin_addr); // Broker IP address
    printk("Broker configured\n");
}

void subscribe_to_topic(void) {
    struct mqtt_topic subscription_topic = { .topic = { .utf8 = "test/topic", .size = strlen("test/topic") }, .qos = MQTT_QOS_1_AT_LEAST_ONCE };
    const struct mqtt_subscription_list subscription_list = {
        .list = &subscription_topic,
        .list_count = 1,
        .message_id = 1234
    };
    
    int rc = mqtt_subscribe(&client, &subscription_list);
    if (rc != 0) {
        printk("Failed to subscribe %d\n", rc);
    } else {
        printk("Subscription initiated\n");
    }
}

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printk("MQTT connect failed %d\n", evt->result);
            } else {
                printk("MQTT connected\n");
                subscribe_to_topic();
                publish_message();
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected %d\n", evt->result);
            break;
        default:
            printk("Unhandled MQTT event %d\n", evt->type);
            break;
    }
}

void main(void)
{
    int rc;

    configure_broker();
    printk("Attempting to connect to MQTT broker\n");
    printk("Broker IP: %s\n", inet_ntoa(((struct sockaddr_in *)&broker)->sin_addr));
    printk("Broker Port: %d\n", ntohs(((struct sockaddr_in *)&broker)->sin_port));

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_mqtt_client";
    client.client_id.size = sizeof("zephyr_mqtt_client") - 1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.protocol_version = MQTT_VERSION_3_1_1;

    /* Connect to MQTT broker */
    printk("Attempting to connect to MQTT broker\n");

    /* Connect to MQTT broker */
    rc = mqtt_connect(&client);
    printk("Connect request sent, result: %d\n", rc);
    if (rc != 0) {
        printk("Failed to connect to MQTT broker %d\n", rc);
    } else {
        printk("Connection request sent\n");
    }
}
