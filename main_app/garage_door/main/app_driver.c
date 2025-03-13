/* Fan demo implementation using button and RGB LED

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h> 
#include <esp_rmaker_standard_params.h> 

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

//This is for the relay pins
#define RELAY1_GPIO           7
#define RELAY2_GPIO           5

//Door sensor
#define DOOR_SENSOR_GPIO      6
#define REPORTING_PERIOD      60

#define DEFAULT_HUE         180
#define DEFAULT_SATURATION  100
#define DEFAULT_BRIGHTNESS  ( 20 * 2)

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

//static uint16_t g_hue = DEFAULT_HUE;
//static uint16_t g_saturation = DEFAULT_SATURATION;
//static uint16_t g_value = DEFAULT_BRIGHTNESS;


static bool g_power_state = DEFAULT_POWER;
static TimerHandle_t gate_timer;
char * door_state_str = "Cerrado";

static void app_sensor_update(TimerHandle_t handle)
{
    
    uint8_t door_state = gpio_get_level(DOOR_SENSOR_GPIO);
    if (door_state == 1){
        door_state_str = "Abierto";
    } else {
        door_state_str = "Cerrado";
    }
    
    esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_type(garage_door, ESP_RMAKER_PARAM_OTA_STATUS),
                esp_rmaker_str(door_state_str));
}

static void app_indicator_set(bool state)
{
    if (state) {
        //ws2812_led_set_hsv(g_hue, g_saturation, g_value);
        ws2812_led_set_hsv(241, 100, 100); //blue
        click_the_relay(RELAY1_GPIO);
    } else {
        //ws2812_led_clear();
        click_the_relay(RELAY2_GPIO);
        ws2812_led_set_hsv(128, 100, 100); //green
    }
}
    

static void app_indicator_init(void)
{
    ws2812_led_init();
    //app_indicator_set(g_power_state);
}

static void set_power_state(bool target)
{
    if (target){
        click_the_relay(RELAY1_GPIO);
        //ws2812_led_set_hsv(g_hue, g_saturation, g_value);
        ws2812_led_set_hsv(0, 100, 100); //red
    } else {
        click_the_relay(RELAY2_GPIO);
        //ws2812_led_clear();
        ws2812_led_set_hsv(298, 100, 100); //pink
    }
    
}


void click_the_relay(uint64_t relay_gpio)
{
    gpio_set_level(relay_gpio, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(relay_gpio, 0);
}




esp_err_t app_garage_init(void)
{
    esp_err_t err = ws2812_led_init();
    if (err != ESP_OK) {
        return err;
    }
    
    app_driver_set_state(!g_power_state);
    return ESP_OK;
}

esp_err_t app_sensor_init(void)
{
    gate_timer = xTimerCreate("app_sensor_update_tm", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, app_sensor_update);
    if (gate_timer) {
        xTimerStart(gate_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

static void push_btn_cb(void *arg)
{
    
    app_driver_set_state(!g_power_state);
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(garage_door, "Activar"),
            esp_rmaker_bool(!g_power_state));
    
}

void app_driver_init()
{
    app_garage_init();
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, (button_active_t)BUTTON_ACTIVE_LEVEL);
    
    if (btn_handle) {
        /* Register a callback for a button tap (short press) event */
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_TAP, push_btn_cb, NULL);
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    //Configure pins
    //Relay pins
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << RELAY1_GPIO) | ((uint64_t)1 << RELAY2_GPIO);
    /* Configure the GPIO */
    gpio_config(&io_conf);
    
    //Door sensor
    gpio_config_t sensor_io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
    };
    sensor_io_conf.pin_bit_mask = ((uint64_t)1 << DOOR_SENSOR_GPIO);
    
    gpio_config(&sensor_io_conf);
    app_indicator_init();
    app_sensor_init();
}

int IRAM_ATTR app_driver_set_state(bool state)
{
    if (state != g_power_state) {
        g_power_state = state;
        set_power_state(g_power_state);
    }
    return ESP_OK;
}

bool app_driver_get_state(void)
{
    return g_power_state;
}
