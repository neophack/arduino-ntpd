/*
 * File: GPSTimeSource.h
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef GPS_TIMESOURCE_H
#define GPS_TIMESOURCE_H

#if defined(ARDUINO)
#include "Arduino.h"
#else
#define micros() (0)
#define interrupts() 
#define noInterrupts()
#endif // defined(ARDUINO)

#include <TinyGPS.h>
#include "config.h"
#include "ITimeSource.h"
#include "IDataSource.h"

/*
 * A time source based on GPS satellite time.
 * WARNING: only one instance of this class should be instantiated if PPS mode is enabled.
 */
class GPSTimeSource : public ITimeSource
{
public:
    GPSTimeSource(IDataSource &dataSource)
        : dataSource_(dataSource),
          secondsSinceEpoch_(0),
          fractionalSecondsSinceEpoch_(0),
          microsecondsPerSecond_(0),
          hasLocked_(false)
    {
        // empty
    }
    
    virtual ~GPSTimeSource()
    {
        // empty
    }
    
    /*
     * Returns the number of seconds since the epoch (defined currently as
     * 1 January 1900 00:00 UTC per the NTP specification.)
     */
    virtual uint32_t getSecondsSinceEpoch(void);
    
    /*
     * Returns the number of fractions of a second. 0xFFFFFFFF/1000
     * would correspond to one millisecond, for example.
     */
    virtual uint32_t getFractionalSecondsSinceEpoch(void);
    
    /* 
     * Grabs latest time from the time source.
     */
    virtual void now(uint32_t *secs, uint32_t *fract);

    /*
     * Enables interrupts.
     */
    void enableInterrupts();    
private:
    static GPSTimeSource *Singleton_;
    
    TinyGPS gps_;
    IDataSource &dataSource_;
    uint32_t secondsSinceEpoch_;
    uint32_t fractionalSecondsSinceEpoch_;
    uint32_t millisecondsOfLastUpdate_;
    uint32_t microsecondsPerSecond_;
    bool hasLocked_;
    
#ifdef PPS_INTERRUPT_LINE
    static void PpsInterrupt_();
#endif // PPS_INTERRUPT_LINE

    void updateFractionalSeconds_();
};

#endif // GPS_TIMESOURCE_H
