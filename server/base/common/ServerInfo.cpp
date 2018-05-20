/*
 * Created by Praying on 2018/3/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include "ServerInfo.hpp"
#include <sstream>
#include <unordered_map>

#if !defined(_WIN32)
#include <unistd.h>
#include <limits.h>
#endif

#include <cstring>
namespace Flow
{
    ServerInfo::ServerInfo()
    {
#if defined(_WIN32)
        name_ = "Not yet implmented";
        cpuInfo_ = "Not yet implemented";
        memUsed_ = 0;
        memTotal_ = 0;
        cpuSpeeds_.push_back(1.0);
#else
        char hostName[512]={};
        gethostname(hostName,512);
        hostName[512-1] = '\0';
        char* firstDot = strchr(hostName ,'.');
        if(firstDot != nullptr)
        {
            *firstDot = '\0';
        }
        name_ = hostName;

        fetchLinuxCpuInfo();
        fetchLinuxMemInfo();
#endif
    }

    void ServerInfo::updateMem()
    {

    }
#if !defined(__WIN32)
    void ServerInfo::fetchLinuxCpuInfo()
    {
        FILE* pProcCpu;
        char procLine[BUFSIZ]={};
        char* pInterest;
        float mhz;
        typedef std::unordered_map<std::string, int> modelNameMap;
        modelNameMap modelNames;

        pProcCpu = fopen( "/proc/cpuinfo", "r" );
        if ( pProcCpu == NULL )
        {
            return;
        }

        while ( fgets( procLine, BUFSIZ, pProcCpu ) != NULL )
        {
            pInterest = strchr( procLine, '\n' );
            if ( pInterest != NULL )
            {
                *pInterest = '\0';
            }
            pInterest = strchr( procLine, '\r' );
            if ( pInterest != NULL )
            {
                *pInterest = '\0';
            }

            if ( procLine[ 0 ] != '\0' )
            {
                pInterest = strchr( procLine, '\t' );
                if ( pInterest == NULL )
                {
                    continue;
                }
                *pInterest = '\0';
                pInterest++;
                pInterest = strchr( pInterest, ':' );
                if ( pInterest == NULL )
                {
                    continue;
                }
                pInterest += 2;

                if (!strcmp( procLine, "cpu MHz" ) )
                {
                    sscanf( pInterest, "%f", &mhz );
                    cpuSpeeds_.push_back( mhz );
                }
                else
                if ( !strcmp( procLine, "model name" ) )
                {
                    modelNames[ pInterest ] += 1;
                }
            }
        }

        fclose( pProcCpu );

        std::stringstream cpuInfoBuf;

        modelNameMap::const_iterator modelNamesIt;
        modelNameMap::const_iterator modelNamesEnd = modelNames.end();

        bool first = true;

        for ( modelNamesIt = modelNames.begin(); modelNamesIt != modelNamesEnd;
              ++modelNamesIt )
        {
            if ( first )
            {
                first = false;
            }
            else
            {
                cpuInfoBuf << ", ";
            }

            if ( modelNamesIt->second == 1 )
            {
                cpuInfoBuf << modelNamesIt->first;
            }
            else
            {
                cpuInfoBuf << modelNamesIt->first << " x" << modelNamesIt->second;
            }
        }

        cpuInfo_ = cpuInfoBuf.str();
    }
/**
 * Parse /proc/meminfo and fill in our memInfo_, memTotal_ and memUsed_
 *
 */
    void ServerInfo::fetchLinuxMemInfo()
    {
        // /proc/meminfo format assumption
        // Each line is either blank, or is 'label: *Value kB'

        // MemTotal:  Total usable mem (i.e. physical mem minus a few reserved
        //            bits and the kernel binary code)
        // MemFree:   Is sum of LowFree+HighFree (overall stat)
        // Buffers:   Memory in buffer cache. mostly useless as metric nowadays
        // Cached:    Memory in the pagecache (diskcache) minus SwapCache
        // Slab:      The total amount of memory, in kilobytes, used by the kernel
        //            to cache data structures for its own use.

        // The unsigned long type is determined in Linux source (fs/proc/meminfo.c)
        unsigned long memTotal, memFree, buffers, cached, slab;
        FILE* pProcMem;
        char procLine[BUFSIZ];

        pProcMem = fopen( "/proc/meminfo", "r" );
        if ( pProcMem == NULL )
        {
            return;
        }

        // This is the number of values we're interested in, lets us early-out
        // of the parse loop
#define INTERESTING_VALUES 5
        int i = 0;
        while ( fgets( procLine, BUFSIZ, pProcMem ) != NULL )
        {
            if (sscanf( procLine, "MemTotal: %lu kB", &memTotal ) == 1 )
            {
                i++;
            }
            else if (sscanf( procLine, "MemFree: %lu kB", &memFree ) == 1 )
            {
                i++;
            }
            else if (sscanf( procLine, "Buffers: %lu kB", &buffers ) == 1 )
            {
                i++;
            }
            else if (sscanf( procLine, "Cached: %lu kB", &cached ) == 1 )
            {
                i++;
            }
            else if (sscanf( procLine, "Slab: %lu kB", &slab ) == 1 )
            {
                i++;
            }

            if ( i >= INTERESTING_VALUES )
            {
                break;
            }
        }

        fclose( pProcMem );

        // We store bytes, kernel info is in kilobytes
        memTotal_ = memTotal << 10;

        // We store bytes, kernel info is in kilobytes
        memUsed_ = ( memTotal - memFree - buffers - cached - slab ) << 10;
    }
#endif
}
