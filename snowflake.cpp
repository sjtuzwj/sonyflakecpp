#include <snowflake.h>
#include <ip.h>
#include <iostream>

/**
 * An object that generates IDs.
 * This is broken into a separate class in case
 * we ever want to support multiple worker threads
 * per process
 * 40 bits for time in units of 10 msec
 * 8 bits for a sequence number
 * 16 bits for a machine id (private ip 2 byte)
 */

    

    SnowFlake & SnowFlake::getInstance()
    {
        static SnowFlake instance;
        return instance;
    }

    SnowFlake::SnowFlake()
    {
        std::string ip = storage::AddrInfo::getInstance().getPublicIP();
        //192.168.*.*
        auto firstDot = ip.find('.', 0);
        auto secondDot = ip.find('.', firstDot+1);
        auto thirdDot = ip.find('.',secondDot+1);
        unsigned long long fb = std::stoull(ip.substr(secondDot +1 ,thirdDot));
        unsigned long long sb = std::stoull(ip.substr(thirdDot+1));
        machineId = fb << 8 | sb;
    }
    //need concurrency control
    unsigned long long SnowFlake::nextId() 
    {
        //butil::AutoLock al(lock);
        //TODO: REPLACE WITH MUTEX
        unsigned long long timestamp = timeGen();
        //clock backward, wait until recover
        if (timestamp < lastTimestamp) 
        {
            timestamp = tilNextMillis();
        }
        //if to many request in 10ms, waite until next 10ms
        if (lastTimestamp == timestamp) 
        {
            sequence = (sequence + 1) & sequenceMask;
            if (sequence == 0) {
                timestamp = tilNextMillis();
            }
        } 
        else {
        sequence = 0;
        }

        lastTimestamp = timestamp;
        return
        ((timestamp - twepoch) << timestampLeftShift) |
        (machineId << machineIdShift) |
        sequence;
    }

    unsigned long long SnowFlake::tilNextMillis()
    {
        unsigned long long timestamp = timeGen();
        while (timestamp <= lastTimestamp) 
        {
            timestamp = timeGen();
        }
        return timestamp;
    }

    unsigned long long SnowFlake::timeGen()
    {
      std::chrono::system_clock::duration tp = std::chrono::system_clock::now().time_since_epoch();
      auto tenms =  std::chrono::duration_cast<std::chrono::milliseconds>(tp).count()/10;
      return tenms;
    }
