/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

/// 单例模式类声明宏，需包含头文件<folly/Singleton.h>
#define FLOW_SINGLETON_DECLARE(Type) \
friend class folly::Singleton<Type>; \
public:\
static std::shared_ptr<Type> instance();\
protected:\
    Type(const Type& ) = delete;\
    Type& operator=(const Type& ) = delete;

/// 单例模式定义宏，用在类实现部分，需写在namespace 之中
#define FLOW_SINGLETON_IMP(Type) \
namespace \
{\
folly::Singleton<Type> instance_;\
}\
std::shared_ptr<Type> Type::instance()\
{\
return instance_.try_get();\
}

#define READ_BLOCK_SIZE 4096

#ifdef BOOST_ASIO_HAS_IOCP
#define FLOW_SOCKET_USE_IOCP
#endif

#define WORLD_SLEEP_CONST 50
#define NUM_THREADS 4