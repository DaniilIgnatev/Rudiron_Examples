/****************************************************************************************************************************
  HTTPGET.ino
  For ESP8266/ESP32-AT-command running shields

  ESP_AT_Lib is a wrapper library for the ESP8266/ESP32 AT-command shields

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_Lib
  Licensed under MIT license

  @example HTTPGET.ino
  @brief The HTTPGET demo of library WeeESP8266.
  @author Wu Pengfei<pengfei.wu@itead.cc>
  @date 2015.03

  @par Copyright:
  Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version. \n\n
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 *****************************************************************************************************************************/

#include "ESP_AT_Lib.h"
#include "ESP_AT_Debug.h"

#define SSID ""
#define PASSWORD ""

#define HOST_NAME "142.251.33.68"
#define HOST_PORT (80)

ESP8266 wifi(&EspSerial);

void setup(void)
{
  while (true)
  {
    ESP_AT_LIB_DEBUG_OUTPUT.begin(115200);
    while (!ESP_AT_LIB_DEBUG_OUTPUT)
    {
    }

#if defined(BOARD_NAME)
    ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart HTTPGET on " + String(BOARD_NAME));
#else
    ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart HTTPGET");
#endif

    ESP_AT_LIB_DEBUG_OUTPUT.println(ESP_AT_LIB_VERSION);

    // initialize serial for ESP module
    EspSerial.begin(ESP_AT_BAUD);

    ESP_AT_LIB_DEBUG_OUTPUT.print("FW Version:");
    String version = wifi.getVersion();
    ESP_AT_LIB_DEBUG_OUTPUT.println(version);

    ESP_AT_LIB_DEBUG_OUTPUT.print("Set AP/STA Mode ");

    if (wifi.setOprToStationSoftAP())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
      continue;
    }

    if (wifi.joinAP(SSID, PASSWORD))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi OK");
      ESP_AT_LIB_DEBUG_OUTPUT.print("IP: ");
      ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getLocalIP().c_str());
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi failed");
      continue;
    }

    ESP_AT_LIB_DEBUG_OUTPUT.print("disableMUX ");

    if (wifi.disableMUX())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
      break;
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
      continue;
    }
  }

  ESP_AT_LIB_DEBUG_OUTPUT.println("Done");
}

// Буфер передатчика esp8266 = 2920 байт
char requestStr[] = "GET /index.html HTTP/1.1\r\nHost: 142.251.33.68\r\nConnection: close\r\n\r\n";

// Размер буфера приемника esp8266 = 1460 байт
uint8_t buffer[1460] = {0};

void loop(void)
{
  ESP_AT_LIB_DEBUG_OUTPUT.print("Create TCP ");

  if (wifi.createTCP(HOST_NAME, HOST_PORT))
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
    return;
  }

  if (wifi.send((const uint8_t *)requestStr, strlen(requestStr)))
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Sent a request, OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Sent a request, ERROR");
    if (wifi.releaseTCP())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Release TCP OK");
    }
    return;
  }

  uint32_t len = wifi.recv(buffer, sizeof(buffer), 5000);

  if (len > 0)
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("=========================Received============================");

    for (uint32_t i = 0; i < len; i++)
    {
      ESP_AT_LIB_DEBUG_OUTPUT.print((char)buffer[i]);
    }

    ESP_AT_LIB_DEBUG_OUTPUT.println("\n============================================================");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Received an empty response");
  }

  if (wifi.releaseTCP())
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Release TCP OK");
  }

  delay(10000);
}
