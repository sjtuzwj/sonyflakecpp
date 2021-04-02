#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H
#include <chrono>
//#include "butil/synchronization/lock.h"

/**
 * An object that generates IDs.
 * This is broken into a separate class in case
 * we ever want to support multiple worker threads
 * per process
 * 39 bits for time in units of 10 msec
 * 8 bits for a sequence number
 * 16 bits for a machine id (private ip 2 byte)
+--------------------------------------------------------------------------+
| 1 Bit Unused | 39 Bit Timestamp |  16 Bit NodeID  |   8 Bit Sequence ID |
+--------------------------------------------------------------------------+
 */

class SnowFlake
{
private:
    //butil::Lock lock;
    unsigned long long machineId = 0;
    unsigned long long sequence = 0;
    //bias to shade true time, /10 to transfer to sonyflake
    const unsigned  long long twepoch = 128883497465ull;
    const int machineIdBits = 16;
    const int sequenceBits = 8;
    const int machineIdShift = sequenceBits;
    const int timestampLeftShift = sequenceBits + machineIdBits;
    const unsigned long long sequenceMask = (-1ll) ^ ((-1ll) << sequenceBits);
    unsigned long long lastTimestamp = timeGen();
    SnowFlake();
    //wait until nex 10 ms
    unsigned long long tilNextMillis();
    //get current timestamp
    unsigned long long timeGen();
public:
    static SnowFlake & getInstance();
    unsigned long long nextId();
};

#endif 