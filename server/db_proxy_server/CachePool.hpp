//
// Created by quran on 2018/8/16.
//

#ifndef SERVER_CACHEPOOL_HPP
#define SERVER_CACHEPOOL_HPP
#include <string>
#include <vector>
#include <map>
#include <list>
#include <hiredis/hiredis.h>
#include <base/common/Thread.hpp>

namespace Flow{

    class CachePool;

    class CacheConn {
    public:
        CacheConn(CachePool* pCachePool);
        virtual ~CacheConn();

        int Init();
        const char* GetPoolName();

        std::string get(std::string key);
        std::string setex(std::string key, int timeout, std::string value);
        std::string set(std::string key, std::string& value);

        //批量获取
        bool mget(const std::vector<std::string>& keys, std::map<std::string, std::string>& ret_value);
        // 判断一个key是否存在
        bool isExists(std::string &key);

        // Redis hash structure
        long hdel(std::string key, std::string field);
        std::string hget(std::string key, std::string field);
        bool hgetAll(std::string key, std::map<std::string, std::string>& ret_value);
        long hset(std::string key, std::string field, std::string value);

        long hincrBy(std::string key, std::string field, long value);
        long incrBy(std::string key, long value);
        std::string hmset(std::string key, std::map<std::string, std::string>& hash);
        bool hmget(std::string key, std::list<std::string>& fields, std::list<std::string>& ret_value);

        //原子加减1
        long incr(std::string key);
        long decr(std::string key);

        // Redis std::list structure
        long lpush(std::string key, std::string value);
        long rpush(std::string key, std::string value);
        long llen(std::string key);
        bool lrange(std::string key, long start, long end, std::list<std::string>& ret_value);

    private:
        CachePool* 		m_pCachePool=nullptr;
        redisContext* 	m_pContext= nullptr;
        uint64_t		m_last_connect_time=0;
    };

    class CachePool {
    public:
        CachePool(const char* pool_name, const char* server_ip, int server_port, int db_num, int max_conn_cnt);
        virtual ~CachePool();

        int Init();

        CacheConn* GetCacheConn();
        void RelCacheConn(CacheConn* pCacheConn);

        const char* GetPoolName() { return m_pool_name.c_str(); }
        const char* GetServerIP() { return m_server_ip.c_str(); }
        int GetServerPort() { return m_server_port; }
        int GetDBNum() { return m_db_num; }
    private:
        std::string 		m_pool_name;
        std::string		m_server_ip;
        int			m_server_port;
        int			m_db_num;

        int			m_cur_conn_cnt;
        int 		m_max_conn_cnt;
        std::list<CacheConn*>	m_free_list;
        CThreadNotify		m_free_notify;
    };

    class CacheManager {
    public:
        virtual ~CacheManager();

        static CacheManager* getInstance();

        int Init();
        CacheConn* GetCacheConn(const char* pool_name);
        void RelCacheConn(CacheConn* pCacheConn);
    private:
        CacheManager();

    private:
        static CacheManager* 	s_cache_manager;
        std::map<std::string, CachePool*>	m_cache_pool_map;
    };
}



#endif //SERVER_CACHEPOOL_HPP
