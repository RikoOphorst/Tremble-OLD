//#include "connectivity_packet_handler.h"
//#include "../packet_identifiers.h"
//#include "../network_controller.h"
//#include <iostream>
//
//using namespace networking;
//
//ConnectivityPacketHandler::ConnectivityPacketHandler(NetworkController & net_controller) : IPacketHandler(net_controller)
//{
//}
//
//ConnectivityPacketHandler::~ConnectivityPacketHandler()
//{
//}
//
//void ConnectivityPacketHandler::Handle(int packet_id, RakNet::Packet* packet)
//{
//	switch (packet_id)
//	{
//	case ID_CONNECTION_REQUEST_ACCEPTED:
//		HandleAcceptedRequest(packet);
//		break;
//	case ID_CONNECTION_ATTEMPT_FAILED:
//		HandleFailedRequest(packet);
//		break;
//	case ID_ALREADY_CONNECTED:
//		HandleAlreadyConnected(packet);
//		break;
//	case ID_DISCONNECTION_NOTIFICATION:
//		HandleDisconnectNotification(packet);
//		break;
//	case ID_CONNECTION_LOST:
//		HandleConnectionLost(packet);
//		break;
//	case ID_NEW_INCOMING_CONNECTION:
//		HandleNewIncomingConnection(packet);
//		break;
//	case ID_NO_FREE_INCOMING_CONNECTIONS:
//		HandleNoFreeIncomingConnections(packet);
//	}
//}
//
//void ConnectivityPacketHandler::HandleAcceptedRequest(RakNet::Packet * packet)
//{
//	std::cout << "accepted request: " << packet->guid.ToString() << std::endl;
//	net_controller_.SetConnected(true);
//}
//
//void ConnectivityPacketHandler::HandleFailedRequest(RakNet::Packet * packet)
//{
//	std::cout << "failed request: " << packet->guid.ToString() << std::endl;
//}
//
//void ConnectivityPacketHandler::HandleAlreadyConnected(RakNet::Packet * packet)
//{
//	std::cout << "already connected: " << packet->guid.ToString() << std::endl;
//}
//
//void ConnectivityPacketHandler::HandleDisconnectNotification(RakNet::Packet * packet)
//{
//	std::cout << "disconnect notification: " << packet->guid.ToString() << std::endl;
//	net_controller_.SetConnected(false);
//}
//
//void ConnectivityPacketHandler::HandleConnectionLost(RakNet::Packet * packet)
//{
//	std::cout << "connection lost: " << packet->guid.ToString() << std::endl;
//	net_controller_.SetConnected(false);
//}
//
//void ConnectivityPacketHandler::HandleNewIncomingConnection(RakNet::Packet * packet)
//{
//	std::cout << "new incoming connection: " << packet->guid.ToString() << std::endl;
//}
//
//void ConnectivityPacketHandler::HandleNoFreeIncomingConnections(RakNet::Packet * packet)
//{
//	std::cout << "no free incoming connection: " << packet->guid.ToString() << std::endl;
//}
