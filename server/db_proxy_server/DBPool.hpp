//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_DBPOOL_HPP
#define SERVER_DBPOOL_HPP
#include <mysql/mysql.h>
#include <string>
#include <map>
#include <list>
#include <base/common/Thread.hpp>

#define MAX_ESCAPE_STRING_LEN	10240

namespace Flow{
    class CResultSet {
    public:
        CResultSet(MYSQL_RES* res);
        virtual ~CResultSet();

        bool Next();
        int GetInt(const char* key);
        char* GetString(const char* key);
    private:
        int _GetIndex(const char* key);

        MYSQL_RES* 			m_res;
        MYSQL_ROW			m_row;
        std::map<std::string, int>	m_key_map;
    };

    class CPrepareStatement {
    public:
        CPrepareStatement();
        virtual ~CPrepareStatement();

        bool Init(MYSQL* mysql, std::string& sql);

        void SetParam(uint32_t index, int& value);
        void SetParam(uint32_t index, uint32_t& value);
        void SetParam(uint32_t index, std::string& value);
        void SetParam(uint32_t index, const std::string& value);

        bool ExecuteUpdate();
        uint32_t GetInsertId();
    private:
        MYSQL_STMT*	m_stmt;
        MYSQL_BIND*	m_param_bind;
        uint32_t	m_param_cnt;
    };

    class CDBPool;

    class CDBConn {
    public:
        CDBConn(CDBPool* pDBPool);
        virtual ~CDBConn();
        int Init();

        CResultSet* ExecuteQuery(const char* sql_query);
        bool ExecuteUpdate(const char* sql_query);
        char* EscapeString(const char* content, uint32_t content_len);

        uint32_t GetInsertId();

        const char* GetPoolName();
        MYSQL* GetMysql() { return m_mysql; }
    private:
        CDBPool* 	m_pDBPool;	// to get MySQL server information
        MYSQL* 		m_mysql;
        //MYSQL_RES*	m_res;
        char		m_escape_string[MAX_ESCAPE_STRING_LEN + 1];
    };

    class CDBPool {
    public:
        CDBPool(const char* pool_name, const char* db_server_ip, uint16_t db_server_port,
                const char* username, const char* password, const char* db_name, int max_conn_cnt);
        virtual ~CDBPool();

        int Init();
        CDBConn* GetDBConn();
        void RelDBConn(CDBConn* pConn);

        const char* GetPoolName() { return m_pool_name.c_str(); }
        const char* GetDBServerIP() { return m_db_server_ip.c_str(); }
        uint16_t GetDBServerPort() { return m_db_server_port; }
        const char* GetUsername() { return m_username.c_str(); }
        const char* GetPasswrod() { return m_password.c_str(); }
        const char* GetDBName() { return m_db_name.c_str(); }
    private:
        std::string 		m_pool_name;
        std::string 		m_db_server_ip;
        uint16_t	m_db_server_port;
        std::string 		m_username;
        std::string 		m_password;
        std::string 		m_db_name;
        int			m_db_cur_conn_cnt;
        int 		m_db_max_conn_cnt;
        std::list<CDBConn*>	m_free_list;
        CThreadNotify	m_free_notify;
    };

// manage db pool (master for write and slave for read)
    class CDBManager {
    public:
        virtual ~CDBManager();

        static CDBManager* getInstance();

        int Init();

        CDBConn* GetDBConn(const char* dbpool_name);
        void RelDBConn(CDBConn* pConn);
    private:
        CDBManager();

    private:
        static CDBManager*		s_db_manager;
        std::map<std::string, CDBPool*>	m_dbpool_map;
    };


    class DBPool {

    };

}



#endif //SERVER_DBPOOL_HPP
