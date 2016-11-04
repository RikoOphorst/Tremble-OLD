//#include "packet_receiver.h"
//#include "network_controller.h"
//#include <iostream>
//
//#include "packet_handlers.h"
//#include "packet_identifiers.h"
//
//using namespace networking;
//
//PacketReceiver::PacketReceiver(NetworkController& net_controller) : net_controller_(net_controller)
//{
//	CreatePacketHandlers();
//}
//
//PacketReceiver::~PacketReceiver()
//{
//	std::map<int, IPacketHandler*>::iterator it;
//	for (it = packet_handlers_.begin(); it != packet_handlers_.end(); it++)
//	{
//		delete it->second;
//	}
//}
//
//void PacketReceiver::CreatePacketHandlers()
//{
//	std::cout << "Creating packet handlers." << std::endl;
//	packet_handlers_[ID_UNKNOWN_USER_PACKET] = new UnknownPacketHandler(net_controller_);
//
//	ConnectivityPacketHandler* connectivity_packet_handler = new ConnectivityPacketHandler(net_controller_);
//	packet_handlers_[ID_CONNECTION_REQUEST_ACCEPTED] = connectivity_packet_handler;
//	packet_handlers_[ID_CONNECTION_ATTEMPT_FAILED] = connectivity_packet_handler;
//	packet_handlers_[ID_ALREADY_CONNECTED] = connectivity_packet_handler;
//	packet_handlers_[ID_DISCONNECTION_NOTIFICATION] = connectivity_packet_handler;
//	packet_handlers_[ID_CONNECTION_LOST] = connectivity_packet_handler;
//	packet_handlers_[ID_NEW_INCOMING_CONNECTION] = connectivity_packet_handler;
//	packet_handlers_[ID_NO_FREE_INCOMING_CONNECTIONS] = connectivity_packet_handler;
//}
//
//void PacketReceiver::AddPacketHandler(int packet_id, IPacketHandler* handler)
//{
//	if (packet_handlers_.count(packet_id) > 0)
//		packet_handlers_[packet_id] = handler;
//	else
//		std::cout << "A packet with that identifier already exists (" << packet_id << ")." << std::endl;
//}
//
//void PacketReceiver::Handle(RakNet::Packet * packet)
//{
//	unsigned char packet_id = GetPacketIdentifier(packet);
//
//	if (packet_handlers_.count(packet_id) > 0)
//	{
//		packet_handlers_[packet_id]->Handle(packet_id,packet);
//	}
//	else
//	{
//		packet_handlers_[ID_UNKNOWN_USER_PACKET]->Handle(packet_id, packet);
//	}
//}
//
//// Taken from http://www.raknet.net/raknet/manual/receivingpackets.html
//unsigned char PacketReceiver::GetPacketIdentifier(RakNet::Packet * packet)
//{
//	if (packet == 0)
//		return 255;
//
//	if ((unsigned char)packet->data[0] == ID_TIMESTAMP)
//	{
//		RakAssert(packet->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
//		return (unsigned char)packet->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
//	}
//	else
//		return (unsigned char)packet->data[0];
//}
