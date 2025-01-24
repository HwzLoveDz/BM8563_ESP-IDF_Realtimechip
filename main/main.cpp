/*
MIT License

Copyright (c) 2023 mondraker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

main.cpp - I2C Sensor Multi Test example
Created by mondraker on July 10, 2023.
github:https://github.com/HwzLoveDz
*/

// IIC部分官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/i2c.html

#include "i2c_port.h"

extern "C"
{
    #include "bm8563.hpp"
}

static const char *TAG = "BM8563";

RTC_DateTypeDef dateStruct; // RTC Date Struct
RTC_TimeTypeDef timeStruct; // RTC Time Struct

// #define SET_TIME

void rtc_bm8563_init();
void create_RTC_update_func(void);
void rtcUpdateTask(void *pvParameters);

void rtc_set_time(){
    // Set RTC Date
    dateStruct.WeekDay = 4;
    dateStruct.Month   = 1;
    dateStruct.Date    = 16;
    dateStruct.Year    = 2025;
    rtc.SetDate(&dateStruct);
    // Set RTC Time
    timeStruct.Hours   = 15;
    timeStruct.Minutes = 55;
    timeStruct.Seconds = 10;
    rtc.SetTime(&timeStruct);
}

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());

    rtc_bm8563_init();

    xTaskCreatePinnedToCore(rtcUpdateTask, "rtc_Update_Task", 1024 * 3, NULL, 5, NULL, 0);
}

void rtc_bm8563_init()
{
    if (rtc.begin(twi_read, twi_write, BM8563_ADDR)) //
    {
        ESP_LOGE(TAG, "Error init bm8563 !!!");
        while (1);
    }
    ESP_LOGI(TAG, "Success init bm8563 !!!");

#ifdef SET_TIME
    // Set RTC Time
    rtc_set_time();
#endif
}

void create_RTC_update_func(void)
{
    // Get RTC
    rtc.GetDate(&dateStruct);
    rtc.GetTime(&timeStruct);
    // Print RTC
    ESP_LOGI(TAG, "%04d/%02d/%02d  %02d  %02d:%02d:%02d",
             dateStruct.Year,
             dateStruct.Month,
             dateStruct.Date,
             dateStruct.WeekDay,
             timeStruct.Hours,
             timeStruct.Minutes,
             timeStruct.Seconds);
}

void rtcUpdateTask(void *pvParameters)
{
    for (;;vTaskDelay(pdMS_TO_TICKS(1000)))
    {
        // TODO: 处理一些任务
        create_RTC_update_func();
    }
}

