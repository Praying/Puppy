//
// Created by quran on 2018/8/26.
//

#ifndef SERVER_PORTABLE_HPP
#define SERVER_PORTABLE_HPP

typedef signed char 		s8;
typedef unsigned char 		u8;
typedef signed short 		s16;
typedef unsigned short 		u16;
typedef signed int 		s32;
typedef unsigned int 		u32;
typedef long long 		s64;
typedef unsigned long long 	u64;
typedef unsigned int 		uint;
typedef unsigned char 		byte;

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif

#endif //SERVER_PORTABLE_HPP
