//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CBYTESTREAM_HPP
#define SERVER_CBYTESTREAM_HPP

#include <string>

namespace Flow::PushServer{

    class CByteStream
    {
    public:
        CByteStream(uchar_t* buf, uint32_t len);
        CByteStream(CSimpleBuffer* pSimpBuf, uint32_t pos);
        ~CByteStream() {}

        unsigned char* GetBuf() { return m_pSimpBuf ? (uchar_t*)m_pSimpBuf->GetBuffer() : m_pBuf; }
        uint32_t GetPos() { return m_pos; }
        uint32_t GetLen() { return m_len; }
        void Skip(uint32_t len) { m_pos += len; }

        static int16_t ReadInt16(uchar_t* buf);
        static uint16_t ReadUint16(uchar_t* buf);
        static int32_t ReadInt32(uchar_t* buf);
        static uint32_t ReadUint32(uchar_t* buf);
        static void WriteInt16(uchar_t* buf, int16_t data);
        static void WriteUint16(uchar_t* buf, uint16_t data);
        static void WriteInt32(uchar_t* buf, int32_t data);
        static void WriteUint32(uchar_t* buf, uint32_t data);

        void operator << (int8_t data);
        void operator << (uint8_t data);
        void operator << (int16_t data);
        void operator << (uint16_t data);
        void operator << (int32_t data);
        void operator << (uint32_t data);

        void operator >> (int8_t& data);
        void operator >> (uint8_t& data);
        void operator >> (int16_t& data);
        void operator >> (uint16_t& data);
        void operator >> (int32_t& data);
        void operator >> (uint32_t& data);

        void WriteString(const char* str);
        void WriteString(const char* str, uint32_t len);
        void WriteString(std::string str);
        char* ReadString(uint32_t& len);
        std::string ReadString();

        void WriteData(uchar_t* data, uint32_t len);
        uchar_t* ReadData(uint32_t& len);
    private:
        void _WriteByte(void* buf, uint32_t len);
        void _ReadByte(void* buf, uint32_t len);
    private:
        CSimpleBuffer*	m_pSimpBuf;
        uchar_t*		m_pBuf;
        uint32_t		m_len;
        uint32_t		m_pos;
    };

}

#endif //SERVER_CBYTESTREAM_HPP
