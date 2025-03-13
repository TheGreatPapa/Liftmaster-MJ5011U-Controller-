/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_POWER       true

extern esp_rmaker_device_t *garage_door;

void app_driver_init(void);

void click_the_relay(uint64_t relay_gpio);
int app_driver_set_state(bool state);

