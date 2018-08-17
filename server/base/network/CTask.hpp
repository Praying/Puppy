//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CTASK_HPP
#define SERVER_CTASK_HPP

namespace Flow::Network{
    class CTask {
    public:
        CTask(){}
        virtual ~CTask(){}

        virtual void run() = 0;
    private:
    };
}

#endif //SERVER_CTASK_HPP
