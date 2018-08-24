//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPDUMSG_HPP
#define SERVER_CPDUMSG_HPP

#include <push_server/socket/CBaseMsg.hpp>
#include <google/protobuf/message.h>

#define HEAD_LENGTH     16

namespace Flow::PushServer{
    typedef struct tag_head_t
    {
        uint32_t 	length;		// the whole pdu length
        uint16_t 	version;	// pdu version number
        uint16_t	flag;		// not used
        uint16_t	service_id;	//
        uint16_t	command_id;	//
        uint16_t	error;		//
        uint16_t	seq_no;	//
    }ST_HEAD, *LPST_HEAD;

    class CPduMsg : public CBaseMsg
    {
    public:
        CPduMsg();
        virtual ~CPduMsg();

        uint32_t GetPduLength() { return m_sthead.length; }
        uint16_t GetVersion() { return m_sthead.version; }
        uint16_t GetServiceID() { return m_sthead.service_id; }
        uint16_t GetCommandID() { return m_sthead.command_id; }
        uint16_t GetError() { return m_sthead.error; }
        uint16_t GetSeqNum() { return m_sthead.seq_no; }

        void SetServiceID(uint16_t service_id) { m_sthead.service_id = service_id; }
        void SetCommandID(uint16_t command_id) { m_sthead.command_id = command_id; }
        void SetVersion(uint16_t version) { m_sthead.version = version; }
        void SetSeqNum(uint16_t seq_no) { m_sthead.seq_no = seq_no; }

        BOOL ReadHead();
        void WriteHead();

        BOOL CheckMsgAvailable();

        BOOL ParseFromArray(const char* buf, uint32_t len);
        BOOL SerializeToArray(google::protobuf::MessageLite* msg);

    protected:
        ST_HEAD m_sthead;
    };

}

#endif //SERVER_CPDUMSG_HPP
