/****************************************************************************************************************************
  TCPClientSingle.ino
  For ESP8266/ESP32-AT-command running shields

  ESP_AT_Lib is a wrapper library for the ESP8266/ESP32 AT-command shields

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_Lib
  Licensed under MIT license

  @example TCPClientSingle.ino
  @brief The TCPClientSingle demo of library WeeESP8266.
  @author Wu Pengfei<pengfei.wu@itead.cc>
  @date 2015.02

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

#define HOST_NAME ""
#define HOST_PORT (80)

ESP8266 wifi(&EspSerial);

void setup(void)
{
  ESP_AT_LIB_DEBUG_OUTPUT.begin(115200);
  while (!ESP_AT_LIB_DEBUG_OUTPUT)
  {
  }

#if defined(BOARD_NAME)
  ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart TCPClientSingle on " + String(BOARD_NAME));
#else
  ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart TCPClientSingle");
#endif

  ESP_AT_LIB_DEBUG_OUTPUT.println(ESP_AT_LIB_VERSION);

  // initialize serial for ESP module
  EspSerial.begin(ESP_AT_BAUD);

  ESP_AT_LIB_DEBUG_OUTPUT.print("FW Version:");
  ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getVersion().c_str());

  ESP_AT_LIB_DEBUG_OUTPUT.print("Set AP/STA Mode ");

  if (wifi.setOprToStationSoftAP())
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
  }

  bool connected = false;

  while (!connected)
  {
    connected = wifi.joinAP(SSID, PASSWORD);

    if (connected)
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi OK");
      ESP_AT_LIB_DEBUG_OUTPUT.print("IP: ");
      ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getLocalIP().c_str());
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi failed");
    }

    ESP_AT_LIB_DEBUG_OUTPUT.print("disableMUX ");

    if (wifi.disableMUX())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
    }
  }

  ESP_AT_LIB_DEBUG_OUTPUT.println("Done");
}

void loop(void)
{
  uint8_t buffer[128] = {0};

  ESP_AT_LIB_DEBUG_OUTPUT.print("Create TCP ");

  if (wifi.createTCP(HOST_NAME, HOST_PORT))
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
  }

  char hello[] = "Hello, this is client!";

  if (wifi.send((const uint8_t *)hello, strlen(hello)))
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Send OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Send failed");
  }

  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

  if (len > 0)
  {
    ESP_AT_LIB_DEBUG_OUTPUT.print("Received:[");

    for (uint32_t i = 0; i < len; i++)
    {
      ESP_AT_LIB_DEBUG_OUTPUT.print((char)buffer[i]);
    }

    ESP_AT_LIB_DEBUG_OUTPUT.println("]");
  }

  ESP_AT_LIB_DEBUG_OUTPUT.print("Release TCP ");

  if (wifi.releaseTCP())
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("failed");
  }

  delay(5000);
}
