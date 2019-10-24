//
// Created by kolbe on 01.07.19.
//

#ifndef ARTERY_CUSTOMRECEIVERSERVICE_H
#define ARTERY_CUSTOMRECEIVERSERVICE_H


#include "artery/application/ItsG5Service.h"
#include "artery/application/VehicleDataProvider.h"

namespace artery
{

    class CustomReceiverService : public ItsG5Service
    {
    public:
        CustomReceiverService();
        ~CustomReceiverService();

        void indicate(const vanetza::btp::DataIndication&, omnetpp::cPacket*) override;
        void trigger() override;
        void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, omnetpp::cObject*, omnetpp::cObject*) override;

    protected:
        void initialize() override;
        void finish() override;
        void handleMessage(omnetpp::cMessage*) override;

    private:
        const VehicleDataProvider* mVehicleDataProvider;
//        ssize_t mPacketCount = 0;
        std::vector<ssize_t> mPackets;
    };

} // namespace artery


#endif //ARTERY_CUSTOMRECEIVERSERVICE_H
