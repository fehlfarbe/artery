//
// Created by kolbe on 01.07.19.
//

#ifndef ARTERY_CUSTOMSENDERSERVICE_H
#define ARTERY_CUSTOMSENDERSERVICE_H


#include "artery/application/ItsG5Service.h"
#include "artery/application/VehicleDataProvider.h"

namespace artery
{

    class CustomSenderService : public ItsG5Service
    {
    public:
        CustomSenderService();
        ~CustomSenderService();

        void indicate(const vanetza::btp::DataIndication&, omnetpp::cPacket*) override;
        void trigger() override;
        void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, omnetpp::cObject*, omnetpp::cObject*) override;

    protected:
        void initialize() override;
        void finish() override;
        void handleMessage(omnetpp::cMessage*) override;

    private:
        omnetpp::cMessage* m_self_msg;

        bool mSent = false;
        double mTimestep = 10.0;
        omnetpp::simtime_t mStart;
        ssize_t mPacketCount = 0;
        const VehicleDataProvider* mVehicleDataProvider;
    };

} // namespace artery


#endif //ARTERY_CUSTOMSENDERSERVICE_H
