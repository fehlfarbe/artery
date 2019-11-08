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

#include "CustomSenderService.h"
#include "artery/traci/VehicleController.h"
#include <omnetpp/cpacket.h>
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <veins/modules/mobility/traci/TraCIScenarioManager.h>


using namespace omnetpp;
using namespace vanetza;

namespace artery
{

    static const simsignal_t scSignalCamReceived = cComponent::registerSignal("CamReceived");

    Define_Module(CustomSenderService)

    CustomSenderService::CustomSenderService()
    {
    }

    CustomSenderService::~CustomSenderService()
    {
        cancelAndDelete(m_self_msg);
    }

    void CustomSenderService::indicate(const btp::DataIndication& ind, cPacket* packet)
    {
        if (packet->getByteLength() == 42) {
            EV_INFO << "packet indication\n";
        }

//        std::cout << mVehicleDataProvider->station_id() << " (sender) got packet at "
//                  << mVehicleDataProvider->latitude().value() << ", " << mVehicleDataProvider->longitude().value()
//                  << std::endl;

        delete(packet);
    }

    void CustomSenderService::initialize()
    {
        ItsG5Service::initialize();
        m_self_msg = new cMessage("DUMMY Service");
//        subscribe(scSignalCamReceived);
//        std::cout << "HOST: " << findHost()->getId() << " MY ID: " << this->getId() << "\n";
//        Veins::TraCIScenarioManager* traci = static_cast<Veins::TraCIScenarioManager*>(this->getModuleByPath("World.traci"));
        mVehicleDataProvider = &getFacilities().get_const<VehicleDataProvider>();
//        std::cout << "Station ID: " << mVehicleDataProvider->station_id() << std::endl;
//            auto router = &getFacilities().get_const<Router>();
//            std::cout << "forward algorithm" << (int)mib->itsGnAreaForwardingAlgorithm << std::endl;

//        scheduleAt(simTime() + 20.0, m_self_msg);

        mStart = simTime();
    }

    void CustomSenderService::finish()
    {
        // you could record some scalars at this point
        ItsG5Service::finish();
        std::cout << "sent " << mPacketCount << " packets" << std::endl;
    }

    void CustomSenderService::handleMessage(cMessage* msg)
    {
        Enter_Method("handleMessage");
        if (msg == m_self_msg) {
            EV_INFO << "self message\n";
        }
    }

    void CustomSenderService::trigger()
    {
        Enter_Method("trigger");
        if(!mSent && simTime() - mStart > mTimestep) {
            btp::DataRequestB req;
            req.destination_port = host_cast<CustomSenderService::port_type>(getPortNumber());
            req.gn.transport_type = geonet::TransportType::GBC;
            req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
            req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;
            req.gn.maximum_hop_limit = 20;
            req.gn.maximum_lifetime = geonet::Lifetime{geonet::Lifetime::Base::One_Second, 60};

            using vanetza::units::si::meters;
            using vanetza::units::degree;

            vanetza::geonet::Area area;
            area.shape = vanetza::geonet::Circle();
            boost::get<vanetza::geonet::Circle>(area.shape).r = 100.0 * vanetza::units::si::meter;
            area.position.latitude = vanetza::units::GeoAngle{49.572717 * degree};
            area.position.longitude = vanetza::units::GeoAngle{11.030770 * degree};
            req.gn.destination = area;


            std::cout << "Station ID: " << mVehicleDataProvider->station_id() << " sends GBC from "
                      << mVehicleDataProvider->latitude().value() << ", " << mVehicleDataProvider->longitude().value()
                      << " to " << boost::get<vanetza::geonet::Area>(req.gn.destination).position.latitude.value()
                      << ", " << boost::get<vanetza::geonet::Area>(req.gn.destination).position.longitude.value()
                      << ", port: " << req.destination_port
                      << ", radius: " << boost::get<vanetza::geonet::Circle>(area.shape).r.value()
                      << std::endl;


            cPacket *packet = new cPacket(std::to_string(mPacketCount++).c_str());
            packet->setByteLength(42);
            request(req, packet);

//            mSent = true;
            mStart = simTime();
        }
    }

    void CustomSenderService::receiveSignal(cComponent* source, simsignal_t signal, cObject*, cObject*)
    {
        std::cout << "Station ID: " << mVehicleDataProvider->station_id() << " got signal" << std::endl;
//        if (signal == scSignalCamReceived) {
//            auto& vehicle = getFacilities().get_const<traci::VehicleController>();
//            EV_INFO << "Vehicle " << vehicle.getVehicleId() << " received a CAM in sibling serivce\n";
//        }
    }

} // namespace artery