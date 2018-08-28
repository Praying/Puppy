//
// Created by quran on 2018/8/25.
//


#include "FileManager.hpp"
#include "File.hpp"
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "CriticalSection.hpp"

namespace Flow::MsfsServer {
    int FileManager::initDir() {
        bool isExist = File::isExist(m_disk);
        if (!isExist) {
            u64 ret = File::mkdirNoRecursion(m_disk);
            if (ret) {
                LOG(ERROR)<<"The dir["<<m_disk<<"] set error for code["<<ret<<"],its parent dir may no exists";
                return -1;
            }
        }

        //255 X 255
        char first[10] = {0};
        char second[10] = {0};
        for (int i = 0; i <= FIRST_DIR_MAX; i++) {
            snprintf(first, 5, "%03d", i);
            std::string tmp = std::string(m_disk) + "/" + std::string(first);
            int code = File::mkdirNoRecursion(tmp.c_str());
            if (code && (errno != EEXIST)) {
                LOG(ERROR)<<"Create dir["<<tmp<<"] error["<<errno<<"]";
                return -1;
            }
            for (int j = 0; j <= SECOND_DIR_MAX; j++) {
                snprintf(second, 5, "%03d", j);
                std::string tmp2 = tmp + "/" + std::string(second);
                code = File::mkdirNoRecursion(tmp2.c_str());
                if (code && (errno != EEXIST)) {
                    LOG(ERROR)<<"Create dir["<<tmp2<<"] error["<<errno<<"]";
                    return -1;
                }
                memset(second, 0x0, 10);
            }
            memset(first, 0x0, 10);
        }

        return 0;
    }

    std::string FileManager::createFileRelatePath() {
        char first[10] = {0};
        char second[10] = {0};
        m_cs.Enter();
        snprintf(first, 5, "%03d", (int)getFirstDir());
        snprintf(second, 5, "%03d", (int)getSecondDir());
        m_cs.Leave();

        struct timeval tv;
        gettimeofday(&tv,NULL);
        u64 usec = tv.tv_sec*1000000 + tv.tv_usec;
        u64 tid = (u64)pthread_self();
        char unique[40];
        snprintf(unique, 30, "%llu_%llu", usec, tid);
        std::string path = "/" + std::string(first) + "/" + std::string(second)
                      + "/" + std::string(unique);
        return std::string(path);
    }

    int FileManager::uploadFile(const char *type, const void* content, u32 size,
                                char *url, char *ext) {
        //check file size
        if (size > MAX_FILE_SIZE_PER_FILE) {
            LOG(ERROR)<<"File size["<<size<<"] should less than ["<<MAX_FILE_SIZE_PER_FILE<<"]";
            return -1;
        }

        //append the type suffix
        std::string path = createFileRelatePath();
        if (ext)
            path += "_" + std::string(ext);
        else
            path += "." + std::string(type);

        //construct url with group num
        std::string groups("g0");
        std::string fullUrl = groups + path;
        strncpy(url, fullUrl.c_str(), strlen(fullUrl.c_str()));

        //insert std::map file cache
        m_cs.Enter();
        insertEntry(fullUrl, (u64)size, content);
        updateMapCache();
        m_cs.Leave();

        //open and write file then close
        std::string absPath = std::string(m_disk) + path;
        File * tmpFile = new File(absPath.c_str());
        tmpFile->create();
        tmpFile->write(0, size, content);
        delete tmpFile;
        tmpFile = NULL;

        //increase total file sum
        m_filesCs.Enter();
        m_totFiles++;
        m_filesCs.Leave();

        return 0;
    }

    int FileManager::getRelatePathByUrl(const std::string &url, std::string &path) {
        std::string::size_type pos = url.find("/");
        if (std::string::npos == pos) {
            LOG(INFO)<<"Url ["<<url<<"] format illegal.";
            return -1;
        }
        path = url.substr(pos);
        return 0;
    }

    int FileManager::getAbsPathByUrl(const std::string &url, std::string &path) {
        std::string relate;
        if (getRelatePathByUrl(url, relate)) {
            LOG(ERROR)<<"Get path from url["<<url<<"] error";
            return -1;
        }
        path = std::string(m_disk) + relate;
        return 0;
    }


    FileManager::Entry*
    FileManager::getOrCreateEntry(const std::string& url, bool create) {
        m_cs.Enter();
        EntryMap::iterator it = m_map.find(url);
        if (it != m_map.end()) {
            LOG(ERROR)<<"the std::map has the file while url:"<< url;
            m_cs.Leave();
            return it->second;
        }
        if (!create) {
            m_cs.Leave();
            return NULL;
        }

        std::string path;
        if (getAbsPathByUrl(url, path)) {
            LOG(ERROR)<<"Get abs path from url["<<url<<"] error";
            m_cs.Leave();
            return NULL;
        }

        struct stat buf;
        if(stat(path.c_str(), &buf) == -1)
        {
            m_cs.Leave();
            return NULL;
        }

        if(!S_ISREG(buf.st_mode))
        {
            m_cs.Leave();
            return NULL;
        }


        Entry *e = new Entry();
        u64 fileSize = File::getFileSize(path.c_str());
        e->m_fileSize = fileSize;
        e->m_fileContent = new byte[fileSize];
        memset(e->m_fileContent, 0x0, fileSize);
        e->m_lastAccess = time(NULL);
        File* tmpFile = new File(path.c_str());
        tmpFile->open();
        int ret = tmpFile->read(0, fileSize, e->m_fileContent);
        if (ret) {
            LOG(ERROR)<<"read file error while url:"<< url;
            delete e;
            e = NULL;
            delete tmpFile;
            tmpFile = NULL;
            m_cs.Leave();
            return NULL;
        }
        delete tmpFile;
        tmpFile = NULL;

        std::pair < std::map <std::string, Entry*>::iterator, bool> result;
        result = m_map.insert(EntryMap::value_type(url, e));
        if (result.second == false) {
            LOG(ERROR)<<"Insert url["<<url<<"] to file std::map error";
            delete e;
            e = NULL;
        }
        updateMapCache();
        m_cs.Leave();
        return e;
    }

    int FileManager::downloadFileByUrl(char *url, void *buf, u32 *size) {
        Entry* en = getOrCreateEntry(url, true);
        if (!en) {
            LOG(ERROR)<<"download file error, while url:"<< url;
            return -1;
        }
        memcpy(buf, en->m_fileContent, en->m_fileSize);
        *size = (u32)en->m_fileSize;
        en->m_lastAccess = time(NULL);//todo:need prodect with mutex
        return 0;
    }

    void FileManager::updateMapCache() {
        size_t currSize = m_map.size();
        if (currSize > MAX_FILE_IN_MAP) {
            EntryMap::iterator it = m_map.begin();
            int times = abs(MAX_FILE_IN_MAP - currSize);
            while (it != m_map.end() && times) {
                delete it->second;
                m_map.erase(it++);
                times--;
            }

            it = m_map.begin();
            while (it != m_map.end() && times) {
                time_t currTime = time(NULL);
                if(currTime - it->second->m_lastAccess > 2*60*60) {
                    delete it->second;
                    m_map.erase(it++);
                }
            }
        }
    }

    int FileManager::insertEntry(const std::string& url, size_t fileSize,
                                 const void* content) {
        if (m_map.size()) {
            EntryMap::iterator it = m_map.find(url);
            if (it != m_map.end())
                return -1;
        }
        Entry *e = new Entry();
        e->m_fileSize = fileSize;
        e->m_fileContent = new byte[fileSize];
        e->m_lastAccess = time(NULL);
        memcpy(e->m_fileContent, content, fileSize);

        m_cs.Enter();
        std::pair< std::map<std::string, Entry*>::iterator, bool> ret;
        ret = m_map.insert(EntryMap::value_type(url, e));
        if (ret.second == false) {
            delete e;
            e = NULL;
        }
        updateMapCache();
        m_cs.Leave();
        return 0;
    }

    void FileManager::releaseFileCache(const std::string& url) {
        m_cs.Enter();
        const Entry* entry = getEntry(url);
        if (!entry) {
            LOG(ERROR)<<"std::map has not the url::"<< url;
            m_cs.Leave();
            return;
        }
        delete [] entry->m_fileContent;
        m_map.erase(url);
        m_cs.Leave();
        return;
    }

}