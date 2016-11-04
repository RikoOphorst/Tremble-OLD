//#pragma once
//
//#include "RakPeerInterface.h"
//#include "BitStream.h"
//
//namespace networking
//{
//	class NetworkController;
//
//	/*
//	 * @brief The packet handler interface.
//	 *
//	 * @author Simon Kok
//	 */
//	class IPacketHandler
//	{
//	public:
//		IPacketHandler(NetworkController& net_controller);
//		virtual ~IPacketHandler() {};
//
//		/*
//		 * @brief Handles an incoming packet
//		 * @param[in] packet_id The packet id.
//		 * @param[in] packet The RakNet::Packet object.
//		 */
//		virtual void Handle(int packet_id, RakNet::Packet* packet) = 0;
//
//	protected:
//		NetworkController& net_controller_; //!< The network controller associated with this packet handler.
//
//		/*
//		 * @brief Extracts packet data as a RakNet::BitStream.
//		 * @param[in] packet The RakNet::Packet object.
//		 * @return A bitstream containing the packet data.
//		 */
//		RakNet::BitStream& GetPacketData(RakNet::Packet* packet);
//	};
//}