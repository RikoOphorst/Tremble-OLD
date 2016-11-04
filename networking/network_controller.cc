//#include "network_controller.h"
//#include <iostream>
//
//using namespace networking;
//using namespace RakNet;
//
//NetworkController::NetworkController()
//{
//}
//
//NetworkController::~NetworkController()
//{
//	if(is_listening_)
//		Shutdown();
//}
//
//void NetworkController::Startup(bool is_host, int port, std::string host_address, int max_connections)
//{
//	std::cout << "Starting up network controller. Hosting: " << (is_host ? "true" : "false") << "." << std::endl;
//	is_host_ = is_host;
//	port_ = port;
//	host_address_ = host_address;
//
//	// If left at default the max connections will be 2 for a client, and 12 for a host.
//	if (max_connections == -1)
//		max_connections_ = is_host ? 12 : 2;
//	else
//		max_connections_ = max_connections;
//
//	SocketDescriptor sd = is_host ? SocketDescriptor(port, 0) : SocketDescriptor();
//
//	// Initialize rakpeerinterface.
//	client_ = RakNet::RakPeerInterface::GetInstance();
//	client_->Startup(max_connections_, &sd, 1);
//	guid_ = client_->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
//	std::cout << "GUID: " << guid_.ToString() << std::endl;
//
//	// Start listening for incoming data.
//	packet_receiver_ = new PacketReceiver(*this);
//	is_listening_ = true;
//
//	// Connect to the host if is_host is false.
//	if (!is_host)
//		Connect();
//	else
//		client_->SetMaximumIncomingConnections(max_connections_);
//}
//
//void NetworkController::Shutdown()
//{
//	// Max 500ms timeout.
//	client_->Shutdown(500);
//	delete packet_receiver_;
//	RakNet::RakPeerInterface::DestroyInstance(client_);
//	is_listening_ = false;
//}
//
//void NetworkController::Listen()
//{
//	packet_ = client_->Receive();
//	if (packet_ == 0)
//		return;
//
//	packet_receiver_->Handle(packet_);
//	client_->DeallocatePacket(packet_);
//
//	/*for (packet = client_->Receive(); packet; client_->DeallocatePacket(packet), packet = client_->Receive())
//	{
//		if (packet == 0)
//			return;
//
//		packet_receiver_->Handle(packet);
//	}*/
//
//	//delete packet_;
//}
//
//void NetworkController::SendPacket(PacketBuilder * packet)
//{
//	//client_->Send(packet->GetStream(), MEDIUM_PRIORITY, RELIABLE_ORDERED, 1, otherclient, false);
//}
//
//void NetworkController::Connect()
//{
//	client_->Connect(host_address_.c_str(), port_, 0, 0);
//}
//
//PacketReceiver& NetworkController::GetPacketReceiver()
//{
//	return *packet_receiver_;
//}
//
//void NetworkController::SetConnected(bool connected)
//{
//	is_connected_ = connected;
//}
//
//bool NetworkController::IsConnected()
//{
//	return is_connected_;
//}