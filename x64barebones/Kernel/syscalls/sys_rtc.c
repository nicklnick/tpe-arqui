#include <syscalls.h>

/* constants */
#define HOUR 1
#define DATE 2
#define SUCCESS 1

/* fields */
#define F_SEC   0x00
#define F_MIN   0x02
#define F_HOURS 0x04

#define F_DAY   0x07
#define F_MONTH 0x08
#define F_YEAR  0x09

extern int getRTC(uint8_t field);

unsigned int sys_rtc(unsigned int option) 
{
        int hours, min, sec;
        int day, month, year;

        switch(option) {
                // Me piden horario
                // Devuelve HHMMSS
                case HOUR:
                        hours = getRTC(F_HOURS);     // hours
                        hours = ( (hours & 0x0F) + (((hours & 0x70) / 16) * 10) ) | (hours & 0x80);

                        min = getRTC(F_MIN);         // min
                        min = (min & 0x0F) + ((min / 16) * 10);

                        sec = getRTC(F_SEC);         // sec
                        sec = (sec & 0x0F) + ((sec / 16) * 10);

                        return (hours - 3) * 10000 + min * 100 + sec;

                // Me piden fecha
                // Devuelve DDMMYY
                case DATE:
                        day = getRTC(F_DAY);             // day
                        day = (day & 0x0F) + ((day / 16) * 10);

                        month = getRTC(F_MONTH);       // month
                        month = (month & 0x0F) + ((month / 16) * 10);

                        year = getRTC(F_YEAR);        // year
                        year = (year & 0x0F) + ((year / 16) * 10);
                        return day * 10000 + month * 100 + year;
        }
        return 0;       // error?
}