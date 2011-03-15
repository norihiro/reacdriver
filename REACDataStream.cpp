/*
 *  REACDataSteam.cpp
 *  REAC
 *
 *  Created by Per Eckerdal on 15/03/2011.
 *  Copyright 2011 Per Eckerdal. All rights reserved.
 *
 */

#include "REACDataStream.h"

#include <IOKit/IOLib.h>

#define super OSObject

OSDefineMetaClassAndStructors(REACDataStream, OSObject)

void REACDataStream::processDataStream(const REACPacketHeader* packet) {
    UInt16 fill;
    
    switch (packet->type) {
        case REAC_STREAM_FILLER:
            fill = packet->data[0];
            for (int i=1; i<16; i++) {
                if (packet->data[i] != fill) {
                    IOLog("REACConnection[%p]::processDataStream(): Unexpected type 0 packet!\n", this);
                    break;
                }
            }
            
            break;
            
        case REAC_STREAM_CONTROL:
        case REAC_STREAM_CONTROL2:
            // IOLog("REACConnection[%p]::processDataStream(): Got control [%d]\n", this, packet->counter);
            break;
            
    }
}

bool REACDataStream::checkChecksum(const REACPacketHeader* packet) const {
    u_char expected_checksum = 0;
    for (int i=0; i<31; i++) {
        if (i%2)
            expected_checksum += (u_char) (packet->data[i/2] >> 8);
        else
            expected_checksum += (u_char) packet->data[i/2] & 255;
    }
    expected_checksum = (u_char) (256 - (int)expected_checksum);
    
    return expected_checksum == packet->data[15] >> 8;
}