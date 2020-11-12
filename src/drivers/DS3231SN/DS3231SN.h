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
 * \file ErriezDS3231.h
 * \brief DS3231SN high precision RTC library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezDS3231
 *      Documentation:  https://erriez.github.io/ErriezDS3231
 */

#ifndef DS3231SN_H_
#define DS3231SN_H_

#include <stdint.h>

////////////////////////

#define LEAP_YEAR(Y)	( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
#define SECS_PER_DAY	(86400UL)

//convenience macros to convert to and from tm years 
#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year 
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)

// The following defines all copied from the original Time lib to keep existing code working

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define dayOfWeek(_time_)  ((( _time_ / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1) // 1 = Sunday
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  // this is number of days since Jan 1 1970
#define elapsedSecsToday(_time_)  (_time_ % SECS_PER_DAY)   // the number of seconds since last midnight 

// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before settting alarms
#define previousMidnight(_time_) (( _time_ / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define nextMidnight(_time_) ( previousMidnight(_time_)  + SECS_PER_DAY )   // time at the end of the given day 
#define elapsedSecsThisWeek(_time_)  (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY) )   // note that week starts on day 1
#define previousSunday(_time_)  (_time_ - elapsedSecsThisWeek(_time_))      // time at the start of the week for the given time
#define nextSunday(_time_) ( previousSunday(_time_)+SECS_PER_WEEK)          // time at the end of the week for the given time

/* Useful Macros for converting elapsed time to a time_t */
#define minutesToTime_t ((M)) ( (M) * SECS_PER_MIN)  
#define hoursToTime_t   ((H)) ( (H) * SECS_PER_HOUR)  
#define daysToTime_t    ((D)) ( (D) * SECS_PER_DAY) // fixed on Jul 22 2011
#define weeksToTime_t   ((W)) ( (W) * SECS_PER_WEEK) 

//! DS3231 registers
#define DS3231_REG_SECONDS      0x00    //!< Seconds register
#define DS3231_REG_MINUTES      0x01    //!< Minutes register
#define DS3231_REG_HOURS        0x02    //!< Hours register
#define DS3231_REG_DAY_WEEK     0x03    //!< Day of the week register
#define DS3231_REG_DAY_MONTH    0x04    //!< Day of the month register
#define DS3231_REG_MONTH        0x05    //!< Month register
#define DS3231_REG_YEAR         0x06    //!< Year register

#define DS3231_REG_ALARM1_SEC   0x07    //!< Alarm 1 seconds register
#define DS3231_REG_ALARM1_MIN   0x08    //!< Alarm 1 minutes register
#define DS3231_REG_ALARM1_HOUR  0x09    //!< Alarm 1 hour register
#define DS3231_REG_ALARM1_DD    0x0A    //!< Alarm 1 day/date register
#define DS3231_REG_ALARM2_MIN   0x0B    //!< Alarm 2 seconds register
#define DS3231_REG_ALARM2_HOUR  0x0C    //!< Alarm 2 hour register
#define DS3231_REG_ALARM2_DD    0x0D    //!< Alarm 2 day/date register

#define DS3231_REG_CONTROL      0x0E    //!< Control register
#define DS3231_REG_STATUS       0x0F    //!< Status register
#define DS3231_REG_AGING_OFFSET 0x10    //!< Aging offset register
#define DS3231_REG_TEMP_MSB     0x11    //!< Temperature MSB register
#define DS3231_REG_TEMP_LSB     0x12    //!< Temperature LSB register

//! DS3231 number of registers
#define DS3231_NUM_REGS         19      //!< 19 RTC register: 0x00..0x12

//! DS3231 register bit defines
#define DS3231_HOUR_12H_24H     6       //!< 12 or 24 hour mode
#define DS3231_HOUR_AM_PM       5       //!< AM/PM

#define DS3231_MONTH_CENTURY    7       //!< Century

#define DS3231_CTRL_EOSC        7       //!< Enable oscillator
#define DS3231_CTRL_BBSQW       6       //!< Battery-Backed Square-Wave Enable
#define DS3231_CTRL_CONV        5       //!< Start temperature conversion
#define DS3231_CTRL_RS2         4       //!< Square wave rate-select 2
#define DS3231_CTRL_RS1         3       //!< Square wave rate-select 1
#define DS3231_CTRL_INTCN       2       //!< Interrupt control
#define DS3231_CTRL_A2IE        1       //!< Alarm 2 interrupt enable
#define DS3231_CTRL_A1IE        0       //!< Alarm 1 interrupt enable

#define DS3231_STAT_OSF         7       //!< Oscillator Stop Flag
#define DS3231_STAT_EN32KHZ     3       //!< Enable 32kHz clock output
#define DS3231_STAT_BSY         2       //!< Temperature conversion busy flag
#define DS3231_STAT_A2F         1       //!< Alarm 2 status flag
#define DS3231_STAT_A1F         0       //!< Alarm 1 status flag

#define DS3231_A1M1             7       //!< Alarm 1 bit 7 seconds register
#define DS3231_A1M2             7       //!< Alarm 1 bit 7 minutes register
#define DS3231_A1M3             7       //!< Alarm 1 bit 7 hours register
#define DS3231_A1M4             7       //!< Alarm 1 bit 7 day/date register
#define DS3231_A2M2             7       //!< Alarm 2 bit 7 minutes register
#define DS3231_A2M3             7       //!< Alarm 2 bit 7 hours register
#define DS3231_A2M4             7       //!< Alarm 2 bit 7 day/date register
#define DS3231_DYDT             6       //!< Alarm 2 bit 6

//! DS3231 I2C 7-bit address
#define DS3231_ADDR             (0xD0 >> 1)

//! Number of seconds between year 1970 and 2000
#define SECONDS_FROM_1970_TO_2000 946684800

typedef struct  { 
	uint8_t Second; 
	uint8_t Minute; 
	uint8_t Hour; 
	uint8_t Wday;   // day of week, sunday is day 1
	uint8_t Day;
	uint8_t Month; 
	uint8_t Year;   // offset from 1970; 
} tmElements_t;

typedef enum { 
	timeNotSet,
	timeNeedsSync,
	timeSet
} timeStatus_t;

typedef struct {
	time_t time;
	void (*function)();
} ezEvent_t;


typedef struct DS3231_DateTime_s {
    uint8_t Second;             //!< Second 0..59
    uint8_t Minute;             //!< Minute 0..59
    uint8_t Hour;               //!< Hour 0..23
    uint8_t Wday;               //!< Day of the week (1 = Monday)
    uint8_t Day;                //!< Day of the month 1..31
    uint8_t Month;              //!< Month 1..12
    uint8_t Year;               //!< Offset from 1970 for compatibility with ezTime 
} DS3231_DateTime; 


typedef enum {
    Alarm1 = 1,                 //!< Alarm ID 1
    Alarm2 = 2                  //!< Alarm ID 2
} AlarmId;


typedef enum
{
    Alarm1EverySecond = 0x0F,   //!< Alarm once per second
    Alarm1MatchSeconds = 0x0E,  //!< Alarm when seconds match
    Alarm1MatchMinutes = 0x0C,  //!< Alarm when minutes and seconds match
    Alarm1MatchHours = 0x08,    //!< Alarm when hours, minutes, and seconds match
    Alarm1MatchDay = 0x10,      //!< Alarm when date, hours, minutes, and seconds match
    Alarm1MatchDate = 0x00,     //!< Alarm when day, hours, minutes, and seconds match
} Alarm1Type;


typedef enum
{
    Alarm2EveryMinute = 0x0E,   //!< Alarm once per minute (00 seconds of every minute)
    Alarm2MatchMinutes = 0x0C,  //!< Alarm when minutes match
    Alarm2MatchHours = 0x08,    //!< Alarm when hours and minutes match
    Alarm2MatchDay = 0x10,      //!< Alarm when date, hours, and minutes match
    Alarm2MatchDate = 0x00,     //!< Alarm when day, hours, and minutes match
} Alarm2Type;


typedef enum {
    SquareWaveDisable = (1 << DS3231_CTRL_INTCN),                           //!< SQW disable
    SquareWave1Hz = ((0 << DS3231_CTRL_RS2) | (0 << DS3231_CTRL_RS1)),      //!< SQW 1Hz
    SquareWave1024Hz = ((0 << DS3231_CTRL_RS2) | (1 << DS3231_CTRL_RS1)),   //!< SQW 1024Hz
    SquareWave4096Hz = ((1 << DS3231_CTRL_RS2) | (0 << DS3231_CTRL_RS1)),   //!< SQW 4096Hz
    SquareWave8192Hz = ((1 << DS3231_CTRL_RS2) | (1 << DS3231_CTRL_RS1)),   //!< SQW 8192Hz
} SquareWave;

const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0

class DS3231SN{
public:
    DS3231SN();
    // Initialize
    bool begin(void);

	// Oscillator functions
	bool isOscillatorStopped(bool clearOSF = false);
	void enableOscillator(bool enable);    

	// Time.h date/time structures' functions
	bool writeRtc(tmElements_t &tm);
	bool readRtc(tmElements_t &tm);
	void breakTime(const time_t time, tmElements_t &tm);
	time_t makeTime(tmElements_t &tm);
	bool setRtcEpoch(time_t t);
	time_t getRtcEpoch();

	// Alarm functions
	void setAlarm1(Alarm1Type alarmType,
						uint8_t dayDate, uint8_t hours, uint8_t minutes, uint8_t seconds);
	void setAlarm2(Alarm2Type alarmType, uint8_t dayDate, uint8_t hours, uint8_t minutes);
	void alarmInterrupt(AlarmId alarmId, bool enable);
	bool getAlarmFlag(AlarmId alarmId);
	void clearAlarmFlag(AlarmId alarmId);

	// Output signal control
	void setSquareWave(SquareWave squareWave);
	void outputClockPinEnable(bool enable);

	// Aging offset compensation
	void setAgingOffset(int8_t val);
	int8_t getAgingOffset();

	// Temperature functions
	void startTemperatureConversion();
	void getTemperature(int8_t *temperature, uint8_t *fraction);

	// BCD conversions
	uint8_t bcdToDec(uint8_t bcd);
	uint8_t decToBcd(uint8_t dec);

	// Read/write status and control registers
	uint8_t readControlRegister();
	void writeControlRegister(uint8_t value);
	uint8_t readStatusRegister();
	void writeStatusRegister(uint8_t value);

	// Read/write register
	uint8_t readRegister(uint8_t reg);
	void writeRegister(uint8_t reg, uint8_t value);

	// Read/write buffer
	void readBuffer(uint8_t reg, void *buffer, uint8_t len);
	void writeBuffer(uint8_t reg, void *buffer, uint8_t len);
};

#endif // DS3231SN_H_
