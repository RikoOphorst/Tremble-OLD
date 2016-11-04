//#pragma once
//
//#include "packet_identifiers.h"
//#include "BitStream.h"
//
//namespace networking
//{
//	/*
//	 * @brief Saves you some time when creating a packet.
//	 * 
//	 * @author Simon Kok
//	 */
//	class PacketBuilder
//	{
//	public:
//		/*
//		 * Writes the packet_id to the packet stream.
//		 *
//		 * @param[in] packet_id The identifier for the packet you're building.
//		 */
//		PacketBuilder(EnginePacketIdentifiers packet_id);
//
//		RakNet::BitStream& GetStream(); //!< @return The packet data that this PacketBuilder represents as RakNet::BitStream.
//
//	private:
//		RakNet::BitStream stream_; //!< The stream containing the packet data for this PacketBuilder.
//		EnginePacketIdentifiers packet_id_; //!< The identifier associated with this packet.
//	};
//}