/*
 * Created by Praying on 2018/4/8.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <vector>

namespace Flow
{


    class BString;

    typedef std::vector<BString> BStringVector;

//======================================================================================================================

#define BSTRING_ALLOC_BLOCK_SIZE      64

    enum BStringType
    {
        BSTRType_ANSI, BSTRType_Unicode16, BSTRType_UTF8
    };

//======================================================================================================================

    class BString
    {
    public:
        BString();

        BString(BStringType type, uint16_t length);

        ~BString();

        BString(const char *data);

        BString(const uint16_t *data);

        BString(const wchar_t *data);

        BString(const BString &data);

        uint16_t initRawBSTR(char *data, BStringType type);

        // Operator overloads
        bool operator==(char *string);

        bool operator==(const char *data);

        bool operator==(const BString &data);

        bool operator==(BString data);

        BString &operator=(const char *data);

        BString &operator=(char *data);

        BString &operator=(const uint16_t *data);

        BString &operator=(const wchar_t *data);

        BString &operator=(const BString &data);

        BString &operator<<(const char *data);

        BString &operator<<(const uint16_t *data);

        BString &operator<<(char *data);

        BString &operator<<(uint16_t *data);

        // Accessor Methods
        const char *getAnsi() const;

        const wchar_t *getUnicode16() const;

        void convert(BStringType type);

        void substring(BString &dest, uint16_t start, uint16_t end);

        int split(BStringVector &retVec, char delimiter);

        void toLower();

        void toUpper();

        void toUpperFirst();

        void toLowerFirst();

        bool isNumber();

        void setLength(uint16_t length);

        void setType(BStringType type);

        uint16_t getLength() const;

        uint32_t getDataLength() const;

        uint32_t getCharacterWidth() const;

        BStringType getType() const;

        char *getRawData() const;

        uint32_t getAllocated() const;

        uint32_t getCrc() const;

        static uint32_t CRC(char *data);

    private:
        void _allocate();

        char *mString;          // Pointer to the allocated buffer
        BStringType mType;            // What format is the current string in.
        uint16_t mAllocated;       // Length of the allocated buffer which should be a multiple of
        uint16_t mCharacterWidth;  // Size of a single character in bytes
        uint16_t mLength;          // Length of the string itself.  BStrings are NOT null terminated

        static uint32_t mCrcTable[256];
    };
}


