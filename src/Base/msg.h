//
//
#ifndef PRIORITYQUEUE_MSG_H
#define PRIORITYQUEUE_MSG_H

#ifdef _WIN32
	#include <Windows.h>
	#undef max
	#undef min
#endif

#include <memory>
#include <bitset>


typedef struct tagTDATA {
    uint8_t  cPriority;  // packet priority 0 – 255 (0 – higer)
    int64_t  dwTicks;    // packet generation time
    uint64_t dwClientId; // client identifier
    char     Data[255];  // packet's data
} TDATA, *PTDATA;


class Msg
{
 public:
   Msg(TDATA* pContent) {
       content_.reset(pContent);
    }

    Msg(Msg&& msg) {
        content_ = std::move(msg.content_);
    }

    Msg(const Msg& src){
       copyContent(src);
    }

    ~Msg() {
        content_.reset();
    }

    Msg& operator=(const Msg& src)
    {
        if ( this != &src )
            copyContent(src);
        return *this;
    }

    Msg& operator=(Msg&& src)
    {
        if ( this != &src ) {
            content_ = std::move(src.content_);
        }
           
        return *this;
    }

    const TDATA& getContent() const {
        return *content_;
    }

   protected:
    void copyContent(const Msg& srcMsg)
    {
        if (nullptr != srcMsg.content_ )
        {
            content_.reset(new TDATA());

            const auto& src = *srcMsg.content_;
            auto& dest = *content_;
            dest.dwClientId = src.dwClientId;
            dest.dwTicks = src.dwTicks;
            dest.cPriority = src.cPriority;
            std::copy(std::begin(src.Data), std::end(src.Data), std::begin(dest.Data));
        }
    }

   private:
    std::unique_ptr<TDATA> content_;
};

#endif //PRIORITYQUEUE_MSG_H
