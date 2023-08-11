//
// Created by Victor on 12/5/2017.
//

#ifndef PRIORITYQUEUE_MSG_H
#define PRIORITYQUEUE_MSG_H

#ifdef _WIN32
	#include <Windows.h>
	#undef max
	#undef min
#endif

#include <memory>

#ifndef _WIN32
	using DWORD = unsigned int;
	using BYTE = unsigned char;
#endif

typedef struct tagTDATA {
    BYTE cPriority; //приоритет запроса 0 – 255 (0 – наивысший приоритет)
    DWORD dwTicks; //время формирования запроса в системных тиках
    DWORD dwClientId; //уникальный идентификатор клиента
    char Data[255]; //абстрактные данные
} TDATA, *PTDATA;


class Msg
{
 public:
   Msg(TDATA* pContent) {
       content_.reset(pContent);
    }

    Msg(Msg&& msg) {
        content_ = msg.content_;
        msg.content_.reset();
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

    const TDATA& getContent() const {
        return *content_;
    }

   protected:
    void copyContent(const Msg& srcMsg)
    {
        if (nullptr != srcMsg.content_ )
        {
            // content_ = std::make_shared<TDATA>(new TDATA());
            content_.reset(new TDATA());

            const auto &src = *srcMsg.content_;
            auto& dest = *content_;

            dest.dwClientId = src.dwClientId;
            dest.dwTicks = src.dwTicks;
            dest.cPriority = src.cPriority;
            std::copy(std::begin(src.Data), std::end(src.Data), std::begin(dest.Data));
        }
    }

   private:
    std::shared_ptr<TDATA> content_;
};

#endif //PRIORITYQUEUE_MSG_H
