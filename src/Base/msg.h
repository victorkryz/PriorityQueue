//
//
#pragma once

#include <utility>


#ifdef _WIN32
	#include <Windows.h>
	#undef max
	#undef min
#endif


typedef struct tagTDATA {
    uint8_t  cPriority;  // packet priority 0 – 255 (0 – higher)
    int64_t  dwTicks;    // packet generation time
    uint64_t dwClientId; // client identifier
    char     Data[255];  // packet's data
} TDATA, *PTDATA;


class Msg 
{
  public:
    Msg() = default;

    explicit Msg(const TDATA& data) : content_(data) {}
    explicit Msg(TDATA&& data) : content_(std::move(data)) {}

    const TDATA& getContent() const noexcept { return content_; }
    TDATA& getContent() noexcept { return content_; }

private:
    TDATA content_{};  
};



