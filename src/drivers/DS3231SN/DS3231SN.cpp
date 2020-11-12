/*
 * MIT License
 *
 * Copyright (c) 2018 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \file ErriezDS3231.cpp
 * \brief DS3231 high precision RTC library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezDS3231
 *      Documentation:  https://erriez.github.io/ErriezDS3231
 */


#include <pgmspace.h>
#include <Wire.h>

#include "DS3231SN.h"


DS3231SN::DS3231SN(){
}

// -------------------------------------------------------------------------------------------------
/*!
 * \brief Initialize and detect DS3231 RTC.
 * \details
 *      Call this function from setup().
 * \retval Success
 *      RTC detected.
 * \retval false
 *      RTC not detected.
 * \retval true
 *      Invalid status register or RTC not detected.
 */
bool DS3231SN::begin()
{
    // Check zero bits in status register
    if (readStatusRegister() & 0x70) {
        return true;
    }

    return false;
}

// -------------------------------------------------------------------------------------------------
/*!
 * \brief Enable or disable oscillator when running on V-BAT.
 * \param enable
 *      true:  Enable RTC clock when running on V-BAT.\n
 *      false: Stop RTC clock when running on V-BAT. Oscillator Stop Flag (OSF) bit will be set in
 *      status register which can be read on next power-on.
 */
void DS3231SN::enableOscillator(bool enable)
{
    uint8_t controlReg;

    // Read status register
    controlReg = readControlRegister();

    // Set or clear EOSC bit in control register
    if (enable) {
        // Clear to enable
        controlReg &= ~(1 << DS3231_CTRL_EOSC);
    } else {
        // Set to disable
        controlReg |= (1 << DS3231_CTRL_EOSC);
    }

    // Write control register
    writeControlRegister(controlReg);
}

/*!
 * \brief Read RTC OSF (Oscillator Stop Flag) from status register.
 * \details
 *      The application is responsible for checking the Oscillator Stop Flag (OSF) before reading
 *      date/time date. This function may be used to judge the validity of the date/time registers.
 * \param enable Optionally clears the OSF bit
 * \retval true
 *      RTC oscillator was stopped: The date/time data is invalid. The application should
 *      synchronize and program a new date/time.
 * \retval false
 *      RTC oscillator is running.
 */
bool DS3231SN::isOscillatorStopped(bool clearOSF)
{
    uint8_t statusReg = readStatusRegister();       // read the status register
    bool ret = statusReg & (1 << DS3231_STAT_OSF);  // isolate the osc stop flag to return to caller
    if (ret && clearOSF)                // clear OSF if it's set and the caller wants to clear it
    {
        writeStatusRegister(statusReg & ~(1 << DS3231_STAT_OSF));
    }
    return ret;
}

// Set the RTC time from a tmElements_t structure and clear the
// oscillator stop flag (OSF) in the Control/Status register.
bool DS3231SN::writeRtc(tmElements_t &tm)
{
    uint8_t buffer[7];

    // Encode date time from decimal to BCD
    buffer[0] = (decToBcd(tm.Second));
    buffer[1] = (decToBcd(tm.Minute));
    buffer[2] = (decToBcd(tm.Hour));
    buffer[3] = (tm.Wday);
    buffer[4] = (decToBcd(tm.Day));
    buffer[5] = (decToBcd(tm.Month));
    buffer[6] = (decToBcd(tmYearToY2k(tm.Year)));

    // Write BCD encoded buffer to RTC registers
    writeBuffer(0x00, buffer, sizeof(buffer));

    // Enable oscillator
    enableOscillator(true);

    // Clear oscillator halt flag
    isOscillatorStopped(true);

    // added for compatibility with DS3232RTC lib
    return false;
}

// Read the current time from the RTC and return it in a tmElements_t
// structure. 
// Returns true if an invalid date/time format was read from the RTC.
bool DS3231SN::readRtc(tmElements_t &tm)
{
    uint8_t buf[7];

    // Read clock date and time registers
    readBuffer(0x00, &buf, sizeof(buf));

    // Convert BCD buffer to Decimal
    tm.Second = bcdToDec(buf[0]);
    tm.Minute = bcdToDec(buf[1]);
    tm.Hour = bcdToDec(buf[2] & 0x3f);    // assumes 24hr clock
    tm.Wday = bcdToDec(buf[3]);
    tm.Day = bcdToDec(buf[4]);
    tm.Month = bcdToDec(buf[5] & 0x1f);  // don't use the Century bit
    tm.Year = y2kYearToTm(bcdToDec(buf[6])); // +30

    // Check buffer for valid data
    if ((tm.Second > 59) ||
        (tm.Minute > 59) ||
        (tm.Hour > 23)   ||
        (tm.Wday < 1)    || (tm.Wday > 7)   ||        
        (tm.Day < 1)     || (tm.Day > 31)   ||
        (tm.Month < 1)   || (tm.Month > 12) ||
        (tm.Year < 30)   || (tm.Year > 129))
    {
        // Invalid date/time read from RTC: Clear date time
        memset(&tm, 0x00, sizeof(tmElements_t));
        return true;
    }

    return false;
}

void DS3231SN::breakTime(const time_t timeInput, tmElements_t &tm){
    // break the given time_t into time components
    // this is a more compact version of the C library localtime function
    // note that year is offset from 1970 !!!

    uint8_t year;
    uint8_t month, monthLength;
    uint32_t time;
    unsigned long days;

    time = (uint32_t)timeInput;
    tm.Second = time % 60;
    time /= 60; // now it is minutes
    tm.Minute = time % 60;
    time /= 60; // now it is hours
    tm.Hour = time % 24;
    time /= 24; // now it is days
    tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 

    year = 0;  
    days = 0;
    while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
        year++;
    }
    tm.Year = year; // year is offset from 1970 

    days -= LEAP_YEAR(year) ? 366 : 365;
    time  -= days; // now it is days in this year, starting at 0

    days=0;
    month=0;
    monthLength=0;
    for (month=0; month<12; month++) {
        if (month==1) { // february
            if (LEAP_YEAR(year)) {
                monthLength=29;
            } else {
                monthLength=28;
            }
        } else {
            monthLength = monthDays[month];
        }

        if (time >= monthLength) {
            time -= monthLength;
        } else {
            break;
        }
    }
    tm.Month = month + 1;  // jan is month 1  
    tm.Day = time + 1;     // day of month
}

// Set the RTC to the given time_t value and clear the
// oscillator stop flag (OSF) in the Control/Status register.
// Returns zero if successful - always.
bool DS3231SN::setRtcEpoch(time_t t)
{
    tmElements_t tm;

    breakTime(t, tm);
    return ( writeRtc(tm) );
}

time_t DS3231SN::makeTime(tmElements_t &tm){
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9

    int i;
    uint32_t seconds;

    // seconds from 1970 till 1 jan 00:00:00 of the given year
    seconds= tm.Year * SECS_PER_DAY * 365UL;

    for (i = 0; i < tm.Year; i++) {
        if (LEAP_YEAR(i)) {
            seconds +=  SECS_PER_DAY;   // add extra days for leap years
        }
    }

    // add days for this year, months start from 1
    for (i = 1; i < tm.Month; i++) {
        if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
            seconds += SECS_PER_DAY * 29UL;
        } else {
            seconds += SECS_PER_DAY * (uint32_t)monthDays[i-1];  //monthDay array starts from 0
        }
    }

    seconds+= (tm.Day-1) * SECS_PER_DAY;
    seconds+= tm.Hour * 3600UL;
    seconds+= tm.Minute * 60UL;
    seconds+= tm.Second;
    
    return (time_t)seconds; 
}

// Read the current time from the RTC and return it as a time_t
// value. 
// Returns true if an invalid date/time format was read from the RTC.
time_t DS3231SN::getRtcEpoch()
{
    tmElements_t tm;

    if ( readRtc(tm) ) return 0;
    return( makeTime(tm) );
}


//--------------------------------------------------------------------------------------------------
/*!
* \brief Set Alarm 1.
* \details
*      Alarm 1 contains several alarm modes which can be configured with the alarmType parameter.
*      Unused matches can be set to zero. The alarm interrupt must be enabled after setting the
*      alarm, followed by clearing the alarm interrupt flag.
* \param alarmType
*      Alarm 1 types:\n
*          Alarm1EverySecond\n
*          Alarm1MatchSeconds\n
*          Alarm1MatchMinutes\n
*          Alarm1MatchHours\n
*          Alarm1MatchDay\n
*          Alarm1MatchDate\n
* \param dayDate
*      Alarm match day of the week or day of the month. This depends on alarmType.
* \param hours
*      Alarm match hours.
* \param minutes
*      Alarm match minutes.
* \param seconds
*      Alarm match seconds.
*/
void DS3231SN::setAlarm1(Alarm1Type alarmType,
                    uint8_t dayDate, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    uint8_t buffer[4];

    // Store alarm 1 registers in buffer
    buffer[0] = decToBcd(seconds);
    buffer[1] = decToBcd(minutes);
    buffer[2] = decToBcd(hours);
    buffer[3] = decToBcd(dayDate);

    // Set alarm 1 bits
    if (alarmType & 0x01) { buffer[0] |= (1 << DS3231_A1M1); }
    if (alarmType & 0x02) { buffer[1] |= (1 << DS3231_A1M2); }
    if (alarmType & 0x04) { buffer[2] |= (1 << DS3231_A1M3); }
    if (alarmType & 0x08) { buffer[3] |= (1 << DS3231_A1M4); }
    if (alarmType & 0x10) { buffer[3] |= (1 << DS3231_DYDT); }

    // Write alarm 1 registers
    writeBuffer(DS3231_REG_ALARM1_SEC, buffer, sizeof(buffer));

    // Clear alarm 1 flag
    clearAlarmFlag(Alarm1);
}

/*!
* \brief Set Alarm 2.
* \details
*      Alarm 2 contains different alarm modes which can be configured with the alarmType
*      parameter. Unused matches can be set to zero. The alarm interrupt must be enabled after
*      setting the alarm, followed by clearing the alarm interrupt flag.
* \param alarmType
*      Alarm 2 types:\n
*          Alarm2EveryMinute\n
*          Alarm2MatchMinutes\n
*          Alarm2MatchHours\n
*          Alarm2MatchDay\n
*          Alarm2MatchDate\n
* \param dayDate
*      Alarm match day of the week or day of the month. This depends on alarmType.
* \param hours
*      Alarm match hours.
* \param minutes
*      Alarm match minutes.
*/
void DS3231SN::setAlarm2(Alarm2Type alarmType, uint8_t dayDate, uint8_t hours, uint8_t minutes)
{
    uint8_t buffer[3];

    // Store alarm 2 registers in buffer
    buffer[0] = decToBcd(minutes);
    buffer[1] = decToBcd(hours);
    buffer[2] = decToBcd(dayDate);

    // Set alarm 2 bits
    if (alarmType & 0x02) { buffer[0] |= (1 << DS3231_A1M2); }
    if (alarmType & 0x04) { buffer[1] |= (1 << DS3231_A1M3); }
    if (alarmType & 0x08) { buffer[2] |= (1 << DS3231_A1M4); }
    if (alarmType & 0x10) { buffer[2] |= (1 << DS3231_DYDT); }

    // Write alarm 2 registers
    writeBuffer(DS3231_REG_ALARM2_MIN, buffer, sizeof(buffer));

    // Clear alarm 2 flag
    clearAlarmFlag(Alarm2);
}

/*!
* \brief Enable or disable Alarm 1 or 2 interrupt.
* \details
*      Enabling the alarm interrupt will disable the Square Wave output on the INT/SQW pin. The
*      INT pin remains high until an alarm match occurs.
* \param alarmId
*      Alarm1 or Alarm2 enum.
* \param enable
*      true: Enable alarm interrupt.\n
*      false: Disable alarm interrupt.
*/
void DS3231SN::alarmInterrupt(AlarmId alarmId, bool enable)
{
    uint8_t controlReg;

    // Clear alarm flag
    clearAlarmFlag(alarmId);

    // Read control register
    controlReg = readControlRegister();

    // Disable square wave out and enable INT
    controlReg |= (1 << DS3231_CTRL_INTCN);

    // Set or clear alarm interrupt enable bit
    if (enable) {
        controlReg |= (1 << (alarmId - 1));
    } else {
        controlReg &= ~(1 << (alarmId - 1));
    }

    // Write control register
    writeControlRegister(controlReg);
}

/*!
* \brief Get Alarm 1 or 2 flag.
* \details
*      Call this function to retrieve the alarm status flag. This function can be used in
*      polling as well as with interrupts enabled.
*
*      The INT pin changes to low when an Alarm 1 or Alarm 2 match occurs and the interrupt is
*      enabled. The pin remains low until both alarm flags are cleared by the application.
* \param alarmId
*      Alarm1 or Alarm2 enum.
* \retval true
*      Alarm interrupt flag set.
* \retval false
*      Alarm interrupt flag cleared.
*/
bool DS3231SN::getAlarmFlag(AlarmId alarmId)
{
    // Mask alarm flags
    if (readStatusRegister() & (1 << (alarmId - 1))) {
        return true;
    } else {
        return false;
    }
}

/*!
* \brief Clear alarm flag.
* \details
*      This function should be called when the alarm flag has been handled in polling and
*      interrupt mode. The INT pin changes to high when both alarm flags are cleared and alarm
*      interrupts are enabled.
* \param alarmId
*      Alarm1 or Alarm2 enum.
* \retval Success
* \retval Failure
*      Incorrect alarm ID.
*/
void DS3231SN::clearAlarmFlag(AlarmId alarmId)
{
    uint8_t statusReg;

    // Read status register
    statusReg = readStatusRegister();

    // Clear alarm interrupt flag
    statusReg &= ~(1 << (alarmId - 1));

    // Write status register
    writeStatusRegister(statusReg);
}

// -------------------------------------------------------------------------------------------------
/*!
* \brief Configure SQW (Square Wave) output pin.
* \details
*      This will disable or initialize the SQW clock pin. The alarm interrupt INT pin will be
*      disabled.
* \param squareWave
*      SquareWave configuration:\n
*          Disable:    SquareWaveDisable\n
*          1Hz:        SquareWave1Hz\n
*          1024Hz:     SquareWave1024Hz\n
*          4096Hz:     SquareWave4096Hz\n
*          8192Hz:     SquareWave8192Hz
* \retval Success
* \retval Failure
*      Incorrect squareWave.
*/
void DS3231SN::setSquareWave(SquareWave squareWave)
{
    uint8_t controlReg;

    // Read control register
    controlReg = readRegister(DS3231_REG_CONTROL);
    controlReg &= ~((1 << DS3231_CTRL_BBSQW) |
                    (1 << DS3231_CTRL_INTCN) |
                    (1 << DS3231_CTRL_RS2) |
                    (1 << DS3231_CTRL_RS1));
    controlReg |= squareWave;

    // Write control register
    writeBuffer(DS3231_REG_CONTROL, &controlReg, sizeof(controlReg));
}

/*!
* \brief Enable or disable 32kHz output clock pin.
* \param enable
*      true: Enable 32kHz output clock pin.\n
*      false: Disable 32kHz output clock pin.
*/
void DS3231SN::outputClockPinEnable(bool enable)
{
    uint8_t statusReg;

    // Read status register
    statusReg = readStatusRegister();

    // Set or clear EN32kHz flag in status register
    if (enable) {
        statusReg |= (1 << DS3231_STAT_EN32KHZ);
    } else {
        statusReg &= ~(1 << DS3231_STAT_EN32KHZ);
    }

    // Write status register
    writeStatusRegister(statusReg);
}

/*!
* \brief Set aging offset register
* \details
*      The aging offset register capacitance value is added or subtracted from the capacitance
*      value that the device calculates for each temperature compensation.
* \param val
*      Aging offset value -127..127, 0.1ppm per LSB (Factory default value: 0).\n
*      Negative values increases the RTC oscillator frequency.
*/
void DS3231SN::setAgingOffset(int8_t val)
{
    uint8_t regVal;

    // Convert 8-bit signed value to register value
    if (val < 0) {
        // Calculate two's complement for negative value
        regVal = ~(-val) + 1;
    } else {
        // Positive register value
        regVal = (uint8_t)val;
    }

    // Write aging offset register
    writeRegister(DS3231_REG_AGING_OFFSET, regVal);

    // A temperature conversion is required to apply the aging offset change
    startTemperatureConversion();
}

/*!
* \brief Get aging offset register
* \details
*      The aging offset register capacitance value is added or subtracted from the capacitance
*      value that the device calculates for each temperature compensation.
* \return val
*      Aging offset value.
*/
int8_t DS3231SN::getAgingOffset()
{
    uint8_t regVal;

    // Read aging register
    regVal = readRegister(DS3231_REG_AGING_OFFSET);

    // Convert to 8-bit signed value
    if (regVal & 0x80) {
        // Calculate two's complement for negative aging register value
        return regVal | ~((1 << 8) - 1);
    } else {
        // Positive aging register value
        return regVal;
    }
}

/*!
* \brief Start temperature conversion
* \details
*      Starting a conversion is only needed when the application requires temperature reads within
*      64 seconds, or changing the aging offset register.
*/
void DS3231SN::startTemperatureConversion()
{
    uint8_t controlReg;

    // Check if temperature busy flag is set
    if (readStatusRegister() & (1 << DS3231_STAT_BSY)) {
        return;
    }

    // Start temperature conversion
    controlReg = readControlRegister() | (1 << DS3231_CTRL_CONV);

    // Write control register
    writeControlRegister(controlReg);
}

/*!
* \brief Read temperature
* \param temperature
*      8-bit signed temperature in degree Celsius.
* \param fraction
*      Temperature fraction in steps of 0.25 degree Celsius. The returned value is a decimal value
*      to prevent floating point usage. The application should divided the fraction by 100.
*/
void DS3231SN::getTemperature(int8_t *temperature, uint8_t *fraction)
{
    uint8_t temp[2];

    // Read temperature MSB and LSB registers
    readBuffer(DS3231_REG_TEMP_MSB, &temp, sizeof(temp));

    // Set temperature argument
    *temperature = temp[0];

    // Calculate two's complement when negative
    if (*temperature & 0x80) {
        *temperature |= ~((1 << 8) - 1);
    }

    // Shift fraction bits 6 and 7 with 0.25 degree Celsius resolution
    *fraction = (temp[1] >> 6) * 25;
}

//https://github.com/FabioCuomo/FabioCuomo-DS3231
// float RTC_DS3231::getTemp() {
// int8_t temp_msb, temp_lsb;

// Wire.beginTransmission(DS3231_ADDRESS);
// Wire.write(DS3231_TEMP);
// Wire.endTransmission();

// Wire.requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)2);
// temp_msb = Wire._I2C_READ();
// temp_lsb = (Wire._I2C_READ() >> 6) & 0x03;
// Wire.endTransmission();

// if(temp_msb & 0b10000000) {     //check if negative number
// 	temp_msb  ^= 0b11111111;
// 	temp_msb  += 0x1;
// 	return (-1.0 * ((float)temp_msb) + ((float)temp_lsb * 0.25));
// }
// else {
// 	return ((float)temp_msb + ((float)temp_lsb * 0.25));
// }
// }

/*************************************************************************************************************//*!
* @brief     returns the internal temperature in tenths of degrees Celsius
* @details   Using 1/10 of degrees allows use of integer rather than floating point
* @return    temperature in 1/10 degrees Celsius
****************************************************************************************************************/
// int32_t DS3231M_Class::temperature(){
// 	int32_t temp = readByte(DS3231M_TEMPERATURE)<<8; // read MSB
// 	temp |= readByte(DS3231M_TEMPERATURE+1);         // get LSB
// 	temp = temp>>6;                                  // Shift over 6 bits
// 	if (temp & 0x200){
// 		temp |= 0xFC00; // keep negative by setting bits
// 	} // if-then a negative number
// 	temp = temp*100/4;                               // value is in .25C increments
// 	return temp;                                     // Return computed temperature
// } // of method temperature()

//--------------------------------------------------------------------------------------------------

/*!
* \brief BCD to decimal conversion.
* \param bcd
*      BCD encoded value.
* \return
*      Decimal value.
*/
uint8_t DS3231SN::bcdToDec(uint8_t bcd)
{
    return (uint8_t)(10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

/*!
* \brief Decimal to BCD conversion.
* \param dec
*      Decimal value.
* \return
*      BCD encoded value.
*/
uint8_t DS3231SN::decToBcd(uint8_t dec)
{
    return (uint8_t)(((dec / 10) << 4) | (dec % 10));
}

//--------------------------------------------------------------------------------------------------
/*!
* \brief Read control register
* \return 8-bit unsigned register value
*/
uint8_t DS3231SN::readControlRegister()
{
    return readRegister(DS3231_REG_CONTROL);
}

/*!
* \brief Write control register
* \param value 8-bit unsigned register value
*/
void DS3231SN::writeControlRegister(uint8_t value)
{
    return writeRegister(DS3231_REG_CONTROL, value);
}

/*!
* \brief Read status register
* \return 8-bit unsigned register value
*/
uint8_t DS3231SN::readStatusRegister()
{
    return readRegister(DS3231_REG_STATUS);
}

/*!
* \brief Write status register
* \param value 8-bit unsigned register value
*/
void DS3231SN::writeStatusRegister(uint8_t value)
{
    return writeRegister(DS3231_REG_STATUS, value);
}

/*!
* \brief Read register.
* \details
*      Please refer to the RTC datasheet.
* \param reg
*      RTC register number 0x00..0x12.
* \returns value
*      8-bit unsigned register value.
*/
uint8_t DS3231SN::readRegister(uint8_t reg)
{
    uint8_t value;

    // Read buffer with one 8-bit unsigned value
    readBuffer(reg, &value, 1);

    return value;
}

/*!
* \brief Write to RTC register.
* \details
*      Please refer to the RTC datasheet.
* \param reg
*      RTC register number 0x00..0x12.
* \param value
*      8-bit unsigned register value.
*/
void DS3231SN::writeRegister(uint8_t reg, uint8_t value)
{
    // Write buffer with one 8-bit unsigned value
    writeBuffer(reg, &value, 1);
}

/*!
* \brief Write buffer to RTC.
* \details
*      Please refer to the RTC datasheet.
* \param reg
*      RTC register number 0x00..0x12.
* \param buffer
*      Buffer.
* \param len
*      Buffer length. Writing is only allowed within valid RTC registers.
*/
void DS3231SN::writeBuffer(uint8_t reg, void *buffer, uint8_t len)
{
    // Start I2C transfer by writing the I2C address, register number and optional buffer
    Wire.beginTransmission(DS3231_ADDR);
    Wire.write(reg);
    for (uint8_t i = 0; i < len; i++) {
        Wire.write(((uint8_t *)buffer)[i]);
    }
    Wire.endTransmission(true);
}

/*!
* \brief Read buffer from RTC.
* \param reg
*      RTC register number 0x00..0x12.
* \param buffer
*      Buffer.
* \param len
*      Buffer length. Reading is only allowed within valid RTC registers.
*/
void DS3231SN::readBuffer(uint8_t reg, void *buffer, uint8_t len)
{
    // Start I2C transfer by writing the I2C address and register number
    Wire.beginTransmission(DS3231_ADDR);
    Wire.write(reg);
    // Generate a repeated start, followed by a read buffer
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)DS3231_ADDR, len);
    for (uint8_t i = 0; i < len; i++) {
        ((uint8_t *)buffer)[i] = (uint8_t)Wire.read();
    }
}
