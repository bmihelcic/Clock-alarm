/*
 * DS3231 time calibrating
 *  1. set hours and minutes to upcoming numbers
 *  2. set seconds to 8-10  <-- this is compensation for time it takes to upload
 *  3. compile (ctrl+r)
 *  4. when minute we gave as 2nd parameter of function setTime() starts, upload (ctrl+u)
 * 
 *  e.g. 
 *  It's 15:36 and the clock shows 15:42
 *  function parameters would be "rtc.setTime(15,38,10)"
 *  we compile the code and wait for clock to change from 15:37 to 15:38
 *  when 38th minute starts we upload the calibration code
 *  
 * SDA ===> A4
 * SCL ===> A5
 * 
*/

#include <DS3231.h>
DS3231  rtc(SDA,SCL);

void setup(){
  rtc.begin();
  rtc.setTime(22,03,10);
}

void loop(){
  
}
