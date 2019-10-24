//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "CustomReceiverService.h"
#include "artery/traci/VehicleController.h"
#include <omnetpp/cpacket.h>
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <veins/modules/mobility/traci/TraCIScenarioManager.h>

#include <algorithm>


using namespace omnetpp;
using namespace vanetza;

namespace artery
{

    static const simsignal_t scSignalCamReceived = cComponent::registerSignal("CamReceived");

    Define_Module(CustomReceiverService)

    CustomReceiverService::CustomReceiverService()
    {
    }

    CustomReceiverService::~CustomReceiverService()
    {
//        cancelAndDelete(m_self_msg);
    }

    void CustomReceiverService::indicate(const btp::DataIndication& ind, cPacket* packet)
    {
        if (packet->getByteLength() == 42) {
            EV_INFO << "packet indication\n";
        }

        std::cout << "SUCCESS!: ";
        std::cout << mVehicleDataProvider->station_id() << " got packet at "
                  << mVehicleDataProvider->latitude().value() << ", " << mVehicleDataProvider->longitude().value()
                  << "from " << ind.source_position.latitude.value() << ", " << ind.source_position.longitude.value()
                  << std::endl;
        ssize_t packet_number = std::stoi(packet->getName());
        if(std::find(mPackets.begin(), mPackets.end(), packet_number) != mPackets.end()) {
            std::cout << "Packet already known" << std::endl;
        } else {
            mPackets.push_back(packet_number);
        }

        delete(packet);
    }

    void CustomReceiverService::initialize()
    {
        ItsG5Service::initialize();
//        m_self_msg = new cMessage(std::to_string(mPacketCount+++));
        mVehicleDataProvider = &getFacilities().get_const<VehicleDataProvider>();
    }

    void CustomReceiverService::finish()
    {
        // you could record some scalars at this point
        ItsG5Service::finish();
        std::cout << "received " << mPackets.size() << " packets" << std::endl;
        for(const auto & p : mPackets){
            std::cout << p << ", ";
        }
        std::cout << std::endl;
    }

    void CustomReceiverService::handleMessage(cMessage* msg)
    {
        Enter_Method("handleMessage");
//        if (msg == m_self_msg) {
//            EV_INFO << "self message\n";
//        }
    }

    void CustomReceiverService::trigger()
    {
        Enter_Method("trigger");
    }

    void CustomReceiverService::receiveSignal(cComponent* source, simsignal_t signal, cObject*, cObject*)
    {

    }

} // namespace artery