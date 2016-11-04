//#include "packet_handler.h"
//#include "network_controller.h"
//
//using namespace networking;
//
//IPacketHandler::IPacketHandler(NetworkController& net_controller) : net_controller_(net_controller)
//{
//	
//}
//
//RakNet::BitStream & IPacketHandler::GetPacketData(RakNet::Packet * packet)
//{
//	RakNet::BitStream& bs = RakNet::BitStream(packet->data, packet->length, false);
//	bs.IgnoreBytes(sizeof(RakNet::MessageID));
//	return bs;
//}
