/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

/// 单例模式类声明宏，需包含头文件<folly/Singleton.h>
#define FLOW_SINGLETON_DECLARE(Type) \
public:\
static Type* instance();\
protected:\
    Type(const Type& ) = delete;\
    Type& operator=(const Type& ) = delete;

/// 单例模式定义宏，用在类实现部分，需写在namespace 之中
#define FLOW_SINGLETON_IMP(Type) \
Type* Type::instance()\
{\
    static Type instance;\
    return &instance;\
}

#define READ_BLOCK_SIZE 4096

#ifdef BOOST_ASIO_HAS_IOCP
#define FLOW_SOCKET_USE_IOCP
#endif

#define WORLD_SLEEP_CONST 50
#define NUM_THREADS 4
#define ROUTER_CONFIG "router_server.ini"