//
// Created by quran on 2018/9/3.
//

#pragma once

#include <list>

namespace Flow {

    class CDepartModel {
    public:
        static CDepartModel* getInstance();
        ~CDepartModel() {}
        void getChgedDeptId(uint32_t& nLastTime, std::list<uint32_t>& lsChangedIds);
        void getDepts(std::list<uint32_t>& lsDeptIds, std::list<IM::BaseDefine::DepartInfo>& lsDepts);
        void getDept(uint32_t nDeptId, IM::BaseDefine::DepartInfo& cDept);
    private:
        CDepartModel(){};
    private:
        static CDepartModel* m_pInstance;
    };

}

