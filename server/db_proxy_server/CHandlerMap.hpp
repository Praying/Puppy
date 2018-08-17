//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CHANDLERMAP_HPP
#define SERVER_CHANDLERMAP_HPP

#include "CProxyTask.hpp"
#include <unordered_map>
namespace Flow{

    typedef std::unordered_map<uint32_t, pdu_handler_t> HandlerMap_t;

    class CHandlerMap {
    public:
        virtual ~CHandlerMap();

        static CHandlerMap* getInstance();

        void Init();
        pdu_handler_t GetHandler(uint32_t pdu_type);
    private:
        CHandlerMap();

    private:
        static  CHandlerMap* s_handler_instance;

        HandlerMap_t 	m_handler_map;
    };

}


#endif //SERVER_CHANDLERMAP_HPP
