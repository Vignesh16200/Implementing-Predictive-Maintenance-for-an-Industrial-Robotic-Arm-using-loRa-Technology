#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *WIFI_SSID = "Niryo Hotspot f7-13a-392";
const char *WIFI_PASSWORD = "niryorobot";

const char *MQTT_HOST = "10.10.10.10";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
const char *TOPIC = "/final";

WiFiClient client;
PubSubClient mqttClient(client);

void callback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0';

    // Convert payload to a String
    String payloadString((char *)payload);

    // Extract values using sscanf
    int no_of_Position;
    int speed;
    double seconds;
    int temperature;

    int parsedValues = sscanf(payloadString.c_str(), "No_of_Position:%d,Speed:%d,Seconds:%lf,Temperature:%d", &no_of_Position, &speed, &seconds, &temperature);

    if (parsedValues == 4)
    {
        // Print the extracted values in the desired format
        Serial.print("No_of_Position:");
        Serial.print(no_of_Position);
        Serial.print(",Speed:");
        Serial.print(speed);
        Serial.print(",Seconds:");
        Serial.print(seconds, 8); // Display up to 8 decimal places
        Serial.print(",Temperature:");
        Serial.println(temperature);
    }
    else
    {
        Serial.println("Failed to parse values");
    }
}


void setup()
{
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(callback);

    while (!client.connected())
    {
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("Connected to MQTT broker");
        }
        else
        {
            delay(500);
            Serial.print(".");
        }
    }

    mqttClient.subscribe(TOPIC);
}

void loop()
{
    mqttClient.loop();
}
