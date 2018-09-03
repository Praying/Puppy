//
// Created by quran on 2018/9/3.
//

#include <base/proto/ProtosGen/IM.Buddy.pb.h>
#include <db_proxy_server/CProxyConn.hpp>
#include <glog/logging.h>
#include "DepartAction.hpp"
#include "CDepartModel.hpp"

namespace Flow::DB_PROXY{
    void getChgedDepart(Network::CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Buddy::IMDepartmentReq msg;
        IM::Buddy::IMDepartmentRsp msgResp;
        if (msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength())) {

            Network::CImPdu* pPduRes = new Network::CImPdu;

            uint32_t nUserId = msg.user_id();
            uint32_t nLastUpdate = msg.latest_update_time();
            std::list<uint32_t> lsChangedIds;
            CDepartModel::getInstance()->getChgedDeptId(nLastUpdate, lsChangedIds);
            std::list<IM::BaseDefine::DepartInfo> lsDeparts;
            CDepartModel::getInstance()->getDepts(lsChangedIds, lsDeparts);

            msgResp.set_user_id(nUserId);
            msgResp.set_latest_update_time(nLastUpdate);
            for(auto it=lsDeparts.begin(); it!=lsDeparts.end(); ++it)
            {
                IM::BaseDefine::DepartInfo* pDeptInfo = msgResp.add_dept_list();
                pDeptInfo->set_dept_id(it->dept_id());
                pDeptInfo->set_priority(it->priority());
                pDeptInfo->set_dept_name(it->dept_name());
                pDeptInfo->set_parent_dept_id(it->parent_dept_id());
                pDeptInfo->set_dept_status(it->dept_status());
            }
            LOG(INFO)<<"userId="<< nUserId<<", last_update="<<nLastUpdate<<", cnt="<< lsDeparts.size();
            msgResp.set_attach_data(msg.attach_data());
            pPduRes->SetPBMsg(&msgResp);
            pPduRes->SetSeqNum(pPdu->GetSeqNum());
            pPduRes->SetServiceId(IM::BaseDefine::SID_BUDDY_LIST);
            pPduRes->SetCommandId(IM::BaseDefine::CID_BUDDY_LIST_DEPARTMENT_RESPONSE);
            CProxyConn::AddResponsePdu(conn_uuid, pPduRes);

        }
        else
        {
            LOG(ERROR)<<("parse pb failed");
        }
    }
}