/*
 * Sources:
 *  https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/
 *  https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
 */

# include <NTPClient.h>
# include <WiFiUdp.h>


const long utcOffsetInSeconds = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void initNTP() {
  timeClient.begin();
}
