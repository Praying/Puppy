//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CRELATIONMODEL_HPP
#define SERVER_CRELATIONMODEL_HPP


#include <cstdint>

namespace Flow{
    class CRelationModel {
    public:
        virtual ~CRelationModel();

        static CRelationModel* getInstance();
        uint32_t getRelationId(uint32_t nUserAId, uint32_t nUserBId, bool bAdd);
        bool updateRelation(uint32_t nRelationId, uint32_t nUpdateTime);
        bool removeRelation(uint32_t nRelationId);

    private:
        CRelationModel();
        uint32_t addRelation(uint32_t nSmallId, uint32_t nBigId);
    private:
        static CRelationModel*	m_pInstance;
    };
}


#endif //SERVER_CRELATIONMODEL_HPP
