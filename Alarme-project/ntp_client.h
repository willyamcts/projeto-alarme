# include <NTPClient.h>
# include <WiFiUdp.h>


const long utcOffsetInSeconds = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void initNTP() {
  timeClient.begin();
}
