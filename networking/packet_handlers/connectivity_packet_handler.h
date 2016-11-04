//#pragma once
//
//#include "../packet_handler.h"
//
//namespace networking
//{
//	class NetworkController;
//
//	/*
//	 * @brief Handles incoming packets related to connectivity.
//	 * 
//	 * @author Simon Kok
//	 */
//	class ConnectivityPacketHandler : public IPacketHandler
//	{
//	public:
//		ConnectivityPacketHandler(NetworkController& net_controller);
//		~ConnectivityPacketHandler();
//
//		void Handle(int packet_id, RakNet::Packet* packet);
//
//	private:
//		void HandleAcceptedRequest(RakNet::Packet* packet);
//		void HandleFailedRequest(RakNet::Packet* packet);
//		void HandleAlreadyConnected(RakNet::Packet* packet);
//		void HandleDisconnectNotification(RakNet::Packet* packet);
//		void HandleConnectionLost(RakNet::Packet* packet);
//		void HandleNewIncomingConnection(RakNet::Packet* packet);
//		void HandleNoFreeIncomingConnections(RakNet::Packet* packet);
//	};
//}