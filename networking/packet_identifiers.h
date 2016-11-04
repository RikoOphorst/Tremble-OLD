//#pragma once
//
//#include "MessageIdentifiers.h"
//
///*
// * @brief The packet ids used by the networking inside the engine.
// *
// * If you want to implement custom packets you can simply repeat what is done in this class, include this file, 
// * create an enum and fill it with your custom packet id's.
// * 
// * The first value in your custom packets enum should equal the last value of this enum (ID_CUSTOM_PACKETS_START).
// */
//enum EnginePacketIdentifiers
//{
//	ID_UNKNOWN_USER_PACKET = -1,
//	ID_HOST_HANDSHAKE = ID_USER_PACKET_ENUM,
//	ID_CUSTOM_PACKETS_START
//};