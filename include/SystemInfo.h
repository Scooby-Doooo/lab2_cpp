// SystemInfo.h
// Get system hardware information

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

// Simple class to get CPU info
class SystemInfo {
public:
    // Returns number of hardware threads available on this CPU
    static unsigned int getHardwareThreads();
};

#endif // SYSTEM_INFO_H
