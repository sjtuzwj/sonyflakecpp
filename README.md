# sonyflakecpp

## Quick Start
The project is from a brpc project, so please replace brpc's lock with mutex manually.  
[Sonyflake](https://github.com/sony/sonyflake) is a variant of snowflake, it use 10ms as interval to increase lifetime, and set seq to 8 bits. Which means it can generate fewer IDs per second.   
Sonyflake use lower 2 bytes of private IP to generate ID.   
+--------------------------------------------------------------------------+   
| 1 Bit Unused | 39 Bit Timestamp |  16 Bit NodeID  |   8 Bit Sequence ID |   
+--------------------------------------------------------------------------+   

## Auto IP 
IP generator is a singleton, you can get your machine's IP if you know the interface like wlan0/eth0/ens33.  
```
storage::AddrInfo::getInstance().setInterface("wlan0");
std::string ip = storage::AddrInfo::getInstance().getPublicIP();
```

## Auto ID
ID generator is also a singleton, but you can easily modify it.  
```
    storage::AddrInfo::getInstance().setInterface("wlan0");
    while(true)
    {   
        long long tmp = SnowFlake::getInstance().nextId();
        cout << tmp << endl;
    }
```

##  clock drift
When the object is created, it get absolute time as a base (wall clock) by system clock.   
Dynamically, it get monotonic clock interval to get accurate time offset, and use base + offset as actaul timestamp.
```

      std::chrono::steady_clock::duration tp =  std::chrono::steady_clock::now() - startTime;
      auto inter =  std::chrono::duration_cast<std::chrono::milliseconds>(tp).count();
      return (inter + startTimestamp)/10;
```
 
