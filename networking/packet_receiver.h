//#pragma once
//
//#include "packet_handler.h"
//#include <map>
//
//namespace networking
//{
//	class NetworkController;
//
//	/**
//	* The packet receiver makes sure all incoming packets are handled by the correct packet handler.
//	*
//	* @author Simon Kok
//	*/
//	class PacketReceiver
//	{
//	public:
//		PacketReceiver(NetworkController& net_controller);
//		~PacketReceiver();
//
//		/**
//		* @brief Map a packet handler to a certain packet identifier.
//		* @param[in] packet_id The packet identifier associated with this handler.
//		*/
//		void AddPacketHandler(int packet_id, IPacketHandler* handler);
//
//		void Handle(RakNet::Packet* packet);
//
//		static unsigned char GetPacketIdentifier(RakNet::Packet* p); //!< Taken from http://www.raknet.net/raknet/manual/receivingpackets.html
//	private:
//		NetworkController& net_controller_; //!< The network controller associated with this packet receiver.
//
//		std::map<int, IPacketHandler*> packet_handlers_; //!< Packet handlers mapped to their designated packet identifier.
//
//		void CreatePacketHandlers();
//	};
//}