/****************************************************************************************************************************
  HTTPGET.ino
  For ESP8266/ESP32-AT-command running shields

  ESP_AT_Lib is a wrapper library for the ESP8266/ESP32 AT-command shields

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_Lib
  Licensed under MIT license

  @example ConnectWiFi.ino
  @brief The ConnectWiFi demo of library WeeESP8266.
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

ESP8266 wifi(&EspSerial);

void setup(void)
{
  ESP_AT_LIB_DEBUG_OUTPUT.begin(115200);
  while (!ESP_AT_LIB_DEBUG_OUTPUT){

  }

#if defined(BOARD_NAME)
  ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart ConnectWiFi on " + String(BOARD_NAME));
#else
  ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart ConnectWiFi");
#endif

  ESP_AT_LIB_DEBUG_OUTPUT.println(ESP_AT_LIB_VERSION);

  // initialize serial for ESP module
  EspSerial.begin(ESP_AT_BAUD);

  ESP_AT_LIB_DEBUG_OUTPUT.print("FW Version: ");
  ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation())
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Set STA Mode OK");
  }
  else
  {
    ESP_AT_LIB_DEBUG_OUTPUT.println("Set STA Mode failed");
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
  }

  ESP_AT_LIB_DEBUG_OUTPUT.println("Done");
}

void loop(void)
{
}
