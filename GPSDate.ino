/* 
*/
 
 
#include <TinyGPS++.h>        // include TinyGPS++ library
#include <TimeLib.h>          // include Arduino time library
#include <SoftwareSerial.h>   // include software serial library
#include "RTClib.h"  // RTC-Bibliothek einbinden

//RTC_DS3231 RTC;      // RTC Module set to UTC
RTC_DS1307 RTC;
//RTC_PFC8563 RTC;
 
TinyGPSPlus gps;
 
#define S_RX    3   // define software serial RX pin
#define S_TX    4   // define software serial TX pin
 
SoftwareSerial SoftSerial(S_RX, S_TX);   // configure SoftSerial library
 
 
//#define time_offset   0  // define a clock offset of 3600 seconds (1 hour) ==> UTC + 1
 
// variable definitions
char Time[]  = "TIME: 00:00:00";
char Date[]  = "DATE: 00 00 2000";
byte last_second, Second, Minute, Hour, Day, Month;
int Year;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
 
void setup(void)
{
  RTC.begin();
  SoftSerial.begin(9600);  // initialize software serial at 9600 baud
 
  Serial.begin(9600);

}
 
void loop()
{
  while (SoftSerial.available() > 0)
  {
    if (gps.encode(SoftSerial.read()))
    {
      // get time from GPS module
      if (gps.time.isValid())
      {
        Minute = gps.time.minute();
        Second = gps.time.second();
        Hour   = gps.time.hour();
      }
 
      // get date drom GPS module
      if (gps.date.isValid())
      {
        Day   = gps.date.day();
        Month = gps.date.month();
        Year  = gps.date.year();
      }
 
      if(last_second != gps.time.second())  // if time has changed
      {
        last_second = gps.time.second();
 
        // set current UTC time
        setTime(Hour, Minute, Second, Day, Month, Year);
        RTC.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
        // add the offset to get local time
 //       adjustTime(time_offset);
 
        // update time array
        Time[12] = second() / 10 + '0';
        Time[13] = second() % 10 + '0';
        Time[9]  = minute() / 10 + '0';
        Time[10] = minute() % 10 + '0';
        Time[6]  = hour()   / 10 + '0';
        Time[7]  = hour()   % 10 + '0';
 
        // update date array
        Date[14] = (year()  / 10) % 10 + '0';
        Date[15] =  year()  % 10 + '0';
        Date[9]  =  month() / 10 + '0';
        Date[10] =  month() % 10 + '0';
        Date[6]  =  day()   / 10 + '0';
        Date[7]  =  day()   % 10 + '0';
 
        // print time & date
  //      print_wday(weekday());   // print day of the week
        Serial.print(Time);         // print time (HH:MM:SS)
        Serial.println(Date);         // print date (DD-MM-YYYY)
 
      }
 
    }
 
  }
 
}
 
// function for displaying day of the week
void print_wday(byte wday)
{
  switch(wday)
  {
    case 1:  Serial.println(" SUNDAY  ");   break;
    case 2:  Serial.println(" MONDAY  ");   break;
    case 3:  Serial.println(" TUESDAY ");   break;
    case 4:  Serial.println("WEDNESDAY");   break;
    case 5:  Serial.println("THURSDAY ");   break;
    case 6:  Serial.println(" FRIDAY  ");   break;
    default: Serial.println("SATURDAY ");
  }
 
}
// end of code
