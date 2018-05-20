/*
 * Created by Praying on 2018/2/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <unordered_map>
#include <string>

namespace Flow
{
    namespace Common
    {
        /// @brief 定义服务器组件类型
        enum class ComponentType
        {
            COMPONENT_TYPE_NONE = 0, /// Database Manager 负责处理数据库部分
            COMPONENT_TYPE_DATABASE_MANAGER = 1, /// Login 负责处理登录的请求
            COMPONENT_TYPE_LOGIN = 2, /// Login Manager 负责处理
            COMPONENT_TYPE_LOGIN_MANAGER = 3, /// Base 负责处理与客户端的连接以及场外规则
            COMPONENT_TYPE_BASE = 4, /// Base Manager 负责 Base 的高可用
            COMPONENT_TYPE_BASE_MANAGER = 5, /// Cell 负责处理游戏逻辑
            COMPONENT_TYPE_CELL = 6, /// Cell Manager 负责Cell的高可用
            COMPONENT_TYPE_CELL_MANAGER = 7, /// Machine负责监视每台机器的资源问题
            COMPONENT_TYPE_MACHINE = 8, COMPONENT_TYPE_CONSOLE = 9, COMPONENT_TYPE_END = 10
        };

        typedef std::unordered_map<ComponentType, std::string> ComponentNameMap;
        typedef std::unordered_map<std::string, ComponentType> NameComponentMap;
        namespace
        {
            ComponentNameMap componentName = {{ComponentType::COMPONENT_TYPE_NONE,             "unknown"},
                                              {ComponentType::COMPONENT_TYPE_BASE,             "BaseApp"},
                                              {ComponentType::COMPONENT_TYPE_BASE_MANAGER,     "BaseAppMgr"},
                                              {ComponentType::COMPONENT_TYPE_LOGIN,            "LoginApp"},
                                              {ComponentType::COMPONENT_TYPE_LOGIN_MANAGER,    "LoginAppMgr"},
                                              {ComponentType::COMPONENT_TYPE_DATABASE_MANAGER, "DatabaseMgr"},
                                              {ComponentType::COMPONENT_TYPE_CELL,             "CellApp"},
                                              {ComponentType::COMPONENT_TYPE_CELL_MANAGER,     "CellAppMgr"},
                                              {ComponentType::COMPONENT_TYPE_MACHINE,          "Machine"},
                                              {ComponentType::COMPONENT_TYPE_CONSOLE,          "Console"},};
            NameComponentMap nameToComponent{{"unkown",        ComponentType::COMPONENT_TYPE_NONE},
                                             {"base_app",      ComponentType::COMPONENT_TYPE_BASE},
                                             {"base_app_mgr",  ComponentType::COMPONENT_TYPE_BASE_MANAGER},
                                             {"login_app",     ComponentType::COMPONENT_TYPE_LOGIN},
                                             {"login_app_mgr", ComponentType::COMPONENT_TYPE_LOGIN_MANAGER},
                                             {"db_mgr",        ComponentType::COMPONENT_TYPE_DATABASE_MANAGER},
                                             {"cell_app",      ComponentType::COMPONENT_TYPE_CELL},
                                             {"cell_app_mgr",  ComponentType::COMPONENT_TYPE_CELL_MANAGER},
                                             {"machine",       ComponentType::COMPONENT_TYPE_MACHINE},
                                             {"console",       ComponentType::COMPONENT_TYPE_CONSOLE},};
        }

        inline const std::string getComponentNameByType(const ComponentType type)
        {
            if (componentName.find(type) != componentName.end())
            {
                return componentName[type];
            }
            else
            {
                return "unknown";
            }
        }
        inline ComponentType getTypeByName(const std::string &type_name)
        {
            if (nameToComponent.find(type_name) != nameToComponent.end())
            {
                return nameToComponent[type_name];
            }
            else
            {
                return ComponentType::COMPONENT_TYPE_NONE;
            }
        }

        enum class ShutdownExitCode : uint32_t
        {
            SHUTDOWN_EXIT_CODE = 0, ERROR_EXIT_CODE = 1, RESTART_EXIT_CODE = 2
        };

    }

}