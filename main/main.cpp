/////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////
//! 请注意外设最大支持电压，调压可能会损坏外设器件
// IIC部分官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/i2c.html

#include "i2c_port.h"

extern "C"
{
    #include "bm8563.hpp"
}

static const char *TAG = "BM8563";

void bm8563_test();

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());

    if (rtc.begin(twi_read, twi_write, BM8563_ADDR))    //
    {
        ESP_LOGE(TAG, "Error init bm8563 !!!");
        while (1);
    }
    ESP_LOGI(TAG, "Success init bm8563 !!!");

    bm8563_test();
}

void bm8563_test(){

    RTC_DateTypeDef dateStruct; // RTC Date Struct
    RTC_TimeTypeDef timeStruct; // RTC Time Struct


    /* 第一遍下载设置日期时间，后面注释掉 */
    // // Set RTC Date
    // dateStruct.WeekDay = 2;
    // dateStruct.Month   = 6;
    // dateStruct.Date    = 13;
    // dateStruct.Year    = 2023;
    // rtc.SetDate(&dateStruct);
    // // Set RTC Time
    // timeStruct.Hours   = 14;
    // timeStruct.Minutes = 48;
    // timeStruct.Seconds = 10;
    // rtc.SetTime(&timeStruct);
    /**********************************/

    for(;;) {
        // Get RTC
        rtc.GetDate(&dateStruct);
        rtc.GetTime(&timeStruct);
        // Print RTC
        ESP_LOGI(TAG, "%04d/%02d/%02d %02d:%02d:%02d\n",
                        dateStruct.Year,
                        dateStruct.Month,
                        dateStruct.Date,
                        timeStruct.Hours,
                        timeStruct.Minutes,
                        timeStruct.Seconds
                    );
        // Wait
        vTaskDelay(100);
    }
}

