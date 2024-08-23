/**************************************************************************/
/*  test_packet_peer.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_PACKET_PEER_H
#define TEST_PACKET_PEER_H

#include "core/io/packet_peer.h"
#include "tests/test_macros.h"

namespace TestPacketPeer {

TEST_CASE("[PacketPeer][PacketPeerStream] Encode buffer max size") {
	SUBCASE("Default value") {
		PacketPeerStream pps;
		CHECK_EQ(8 * 1024 * 1024, pps.get_encode_buffer_max_size());
	}

	SUBCASE("Max encode buffer must be at least 1024 bytes") {
		PacketPeerStream pps;

		ERR_PRINT_OFF
		pps.set_encode_buffer_max_size(42);
		ERR_PRINT_ON

		CHECK_EQ(8 * 1024 * 1024, pps.get_encode_buffer_max_size());
	}

	SUBCASE("Max encode buffer cannot exceed 256 MiB") {
		PacketPeerStream pps;

		ERR_PRINT_OFF
		pps.set_encode_buffer_max_size((256 * 1024 * 1024) + 42);
		ERR_PRINT_ON

		CHECK_EQ(8 * 1024 * 1024, pps.get_encode_buffer_max_size());
	}

	SUBCASE("Should be next power of two") {
		PacketPeerStream pps;

		pps.set_encode_buffer_max_size(2000);

		CHECK_EQ(2048, pps.get_encode_buffer_max_size());
	}
}

TEST_CASE("[PacketPeer][PacketPeerStream] Read a variant from peer") {
	String godot_rules = "Godot Rules!!!";

	Ref<StreamPeerBuffer> spb;
	spb.instantiate();
	spb->put_var(godot_rules);
	spb->seek(0);

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	Variant value;
	CHECK_EQ(Error::OK, pps.get_var(value));
	CHECK_EQ(godot_rules, String(value));
}

TEST_CASE("[PacketPeer][PacketPeerStream] Read a variant from peer fails") {
	PacketPeerStream pps;

	Variant value;
	ERR_PRINT_OFF
	CHECK_EQ(Error::ERR_UNCONFIGURED, pps.get_var(value));
	ERR_PRINT_ON
}

TEST_CASE("[PacketPeer][PacketPeerStream] Put a variant to peer") {
	String godot_rules = "Godot Rules!!!";

	Ref<StreamPeerBuffer> spb;
	spb.instantiate();

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	CHECK_EQ(Error::OK, pps.put_var(godot_rules));

	spb->seek(0);
	CHECK_EQ(godot_rules, String(spb->get_var()));
}

TEST_CASE("[PacketPeer][PacketPeerStream] Put a variant to peer out of memory failure") {
	String more_than_1mb = String("*").repeat(1024 + 1);

	Ref<StreamPeerBuffer> spb;
	spb.instantiate();

	PacketPeerStream pps;
	pps.set_stream_peer(spb);
	pps.set_encode_buffer_max_size(1024);

	ERR_PRINT_OFF
	CHECK_EQ(Error::ERR_OUT_OF_MEMORY, pps.put_var(more_than_1mb));
	ERR_PRINT_ON
}

TEST_CASE("[PacketPeer][PacketPeerStream] Get packet buffer") {
	String godot_rules = "Godot Rules!!!";

	Ref<StreamPeerBuffer> spb;
	spb.instantiate();
	// First 4 bytes are the length of the string
	CharString cs = godot_rules.ascii();
	Vector<uint8_t> buffer = { (uint8_t)cs.length(), 0, 0, 0 };
	buffer.resize(4 + cs.length());
	memcpy(buffer.ptrw() + 4, cs.get_data(), cs.length());
	spb->set_data_array(buffer);

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	buffer.clear();
	CHECK_EQ(Error::OK, pps.get_packet_buffer(buffer));

	CHECK_EQ(godot_rules, String(reinterpret_cast<const char *>(buffer.ptr())));
}

TEST_CASE("[PacketPeer][PacketPeerStream] Get packet buffer from an empty peer") {
	Ref<StreamPeerBuffer> spb;
	spb.instantiate();

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	Vector<uint8_t> buffer;
	ERR_PRINT_OFF
	CHECK_EQ(Error::ERR_UNAVAILABLE, pps.get_packet_buffer(buffer));
	ERR_PRINT_ON
	CHECK_EQ(0, buffer.size());
}

TEST_CASE("[PacketPeer][PacketPeerStream] Put packet buffer") {
	String godot_rules = "Godot Rules!!!";

	Ref<StreamPeerBuffer> spb;
	spb.instantiate();

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	CHECK_EQ(Error::OK, pps.put_packet_buffer(godot_rules.to_ascii_buffer()));

	spb->seek(0);
	CHECK_EQ(godot_rules, spb->get_string());
	// First 4 bytes are the length of the string
	CharString cs = godot_rules.ascii();
	Vector<uint8_t> buffer = { (uint8_t)cs.length(), 0, 0, 0 };
	buffer.resize(4 + cs.length());
	memcpy(buffer.ptrw() + 4, cs.get_data(), cs.length());
	CHECK_EQ(buffer, spb->get_data_array());
}

TEST_CASE("[PacketPeer][PacketPeerStream] Put packet buffer when is empty") {
	Ref<StreamPeerBuffer> spb;
	spb.instantiate();

	PacketPeerStream pps;
	pps.set_stream_peer(spb);

	Vector<uint8_t> buffer;
	CHECK_EQ(Error::OK, pps.put_packet_buffer(buffer));

	CHECK_EQ(0, spb->get_size());
}

} // namespace TestPacketPeer

#endif // TEST_PACKET_PEER_H
