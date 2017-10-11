//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2017, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------
//
//  Audio attributes for MPEG-1 / MPEG-2
//
//----------------------------------------------------------------------------

#include "tsAudioAttributes.h"
#include "tsDecimal.h"
#include "tsFormat.h"
TSDUCK_SOURCE;


//----------------------------------------------------------------------------
// Default constructor.
//----------------------------------------------------------------------------

ts::AudioAttributes::AudioAttributes() :
    _header(0),
    _layer(0),
    _bitrate(0),
    _sampling_freq(0),
    _mode(0),
    _mode_extension(0)
{
}


//----------------------------------------------------------------------------
// Provides an audio frame.
//----------------------------------------------------------------------------

bool ts::AudioAttributes::moreBinaryData (const void* data, size_t size)
{
    // Audio header is 4 bytes, starting with FFF
    uint32_t header;
    if (size < 4 || ((header = GetUInt32 (data)) & 0xFFF00000) != 0xFFF00000) {
        return false;
    }

    // Mask of fields we use in the header
    const uint32_t HEADER_MASK = 0xFFFEFCF0;

    // If content has not changed, nothing to do
    if (_is_valid && (_header & HEADER_MASK) == (header & HEADER_MASK)) {
        return false;
    }

    // Extract fields (ISO 11172-3, section 2.4.1.3)
    uint8_t id = (header >> 19) & 0x01;
    uint8_t layer = (header >> 17) & 0x03;
    uint8_t bitrate_index = (header >> 12) & 0x0F;
    uint8_t sampling_frequency = (header >> 10) & 0x03;
    _mode = (header >> 6) & 0x03;
    _mode_extension = (header >> 4) & 0x03;
    _header = header;
    _is_valid = true;

    // Audio layer
    switch (layer) {
        case 3:  _layer = 1; break;
        case 2:  _layer = 2; break;
        case 1:  _layer = 3; break;
        default: _layer = 0; // reserved
    }

    // Bitrate in kb/s
    _bitrate = 0;
    if (id == 0) {
        // ISO 13818-3 "lower sampling frequencies" extension
        switch (_layer) {
            case 1: // Layer I, lower sampling frequencies extension
                switch (bitrate_index) {
                    case  1: _bitrate = 32; break;
                    case  2: _bitrate = 48; break;
                    case  3: _bitrate = 56; break;
                    case  4: _bitrate = 64; break;
                    case  5: _bitrate = 80; break;
                    case  6: _bitrate = 96; break;
                    case  7: _bitrate = 112; break;
                    case  8: _bitrate = 128; break;
                    case  9: _bitrate = 144; break;
                    case 10: _bitrate = 160; break;
                    case 11: _bitrate = 176; break;
                    case 12: _bitrate = 192; break;
                    case 13: _bitrate = 224; break;
                    case 14: _bitrate = 256; break;
                    default: _bitrate = 0; break; // reserved
                }
                break;
            case 2: // Layer II, lower sampling frequencies extension
            case 3: // Layer III, lower sampling frequencies extension
                switch (bitrate_index) {
                    case  1: _bitrate = 8; break;
                    case  2: _bitrate = 16; break;
                    case  3: _bitrate = 24; break;
                    case  4: _bitrate = 32; break;
                    case  5: _bitrate = 40; break;
                    case  6: _bitrate = 48; break;
                    case  7: _bitrate = 56; break;
                    case  8: _bitrate = 64; break;
                    case  9: _bitrate = 80; break;
                    case 10: _bitrate = 96; break;
                    case 11: _bitrate = 112; break;
                    case 12: _bitrate = 128; break;
                    case 13: _bitrate = 144; break;
                    case 14: _bitrate = 160; break;
                    default: _bitrate = 0; break; // reserved
                }
                break;
            default: // reserved
                _bitrate = 0;
                break;
        }
    }
    else {
        // No sampling extension
        switch (_layer) {
            case 1: // Layer I
                switch (bitrate_index) {
                    case  1: _bitrate = 32; break;
                    case  2: _bitrate = 64; break;
                    case  3: _bitrate = 96; break;
                    case  4: _bitrate = 128; break;
                    case  5: _bitrate = 160; break;
                    case  6: _bitrate = 192; break;
                    case  7: _bitrate = 224; break;
                    case  8: _bitrate = 256; break;
                    case  9: _bitrate = 288; break;
                    case 10: _bitrate = 320; break;
                    case 11: _bitrate = 352; break;
                    case 12: _bitrate = 384; break;
                    case 13: _bitrate = 416; break;
                    case 14: _bitrate = 448; break;
                    default: _bitrate = 0; break; // reserved
                }
                break;
            case 2: // Layer II
                switch (bitrate_index) {
                    case  1: _bitrate = 32; break;
                    case  2: _bitrate = 48; break;
                    case  3: _bitrate = 56; break;
                    case  4: _bitrate = 64; break;
                    case  5: _bitrate = 80; break;
                    case  6: _bitrate = 96; break;
                    case  7: _bitrate = 112; break;
                    case  8: _bitrate = 128; break;
                    case  9: _bitrate = 160; break;
                    case 10: _bitrate = 192; break;
                    case 11: _bitrate = 224; break;
                    case 12: _bitrate = 256; break;
                    case 13: _bitrate = 320; break;
                    case 14: _bitrate = 384; break;
                    default: _bitrate = 0; break; // reserved
                }
                break;
            case 3: // Layer III
                switch (bitrate_index) {
                    case  1: _bitrate = 32; break;
                    case  2: _bitrate = 40; break;
                    case  3: _bitrate = 48; break;
                    case  4: _bitrate = 56; break;
                    case  5: _bitrate = 64; break;
                    case  6: _bitrate = 80; break;
                    case  7: _bitrate = 96; break;
                    case  8: _bitrate = 112; break;
                    case  9: _bitrate = 128; break;
                    case 10: _bitrate = 160; break;
                    case 11: _bitrate = 192; break;
                    case 12: _bitrate = 224; break;
                    case 13: _bitrate = 256; break;
                    case 14: _bitrate = 320; break;
                    default: _bitrate = 0; break; // reserved
                }
                break;
            default: // reserved
                _bitrate = 0;
                break;
        }
    }

    // Sampling frequency
    _sampling_freq = 0;
    if (id == 0) {
        // ISO 13818-3 "lower sampling frequencies" extension
        switch (sampling_frequency) {
            case 0: _sampling_freq = 22050; break;
            case 1: _sampling_freq = 24000; break;
            case 2: _sampling_freq = 16000; break;
            default: _sampling_freq = 0; break; // reserved
        }
    }
    else {
        switch (sampling_frequency) {
            case 0: _sampling_freq = 44100; break;
            case 1: _sampling_freq = 48000; break;
            case 2: _sampling_freq = 32000; break;
            default: _sampling_freq = 0; break; // reserved
        }
    }

    return true;
}


//----------------------------------------------------------------------------
// Convert to a string object
//----------------------------------------------------------------------------

std::string ts::AudioAttributes::toString() const
{
    if (!_is_valid) {
        return "";
    }

    std::string desc ("Audio ");
    desc += layerName();

    if (_bitrate != 0) {
        desc += ", ";
        desc += Decimal (_bitrate);
        desc += " kb/s";
    }

    if (_sampling_freq != 0) {
        desc += ", @";
        desc += Decimal (_sampling_freq);
        desc += " Hz";
    }

    std::string stereo (stereoDescription());
    if (!stereo.empty()) {
        desc += ", ";
        desc += stereo;
    }

    return desc;
}


//----------------------------------------------------------------------------
// Layer name
//----------------------------------------------------------------------------

std::string ts::AudioAttributes::layerName() const
{
    if (!_is_valid) {
        return "";
    }

    switch (_layer) {
        case 1:  return "layer I";
        case 2:  return "layer II";
        case 3:  return "layer III";
        default: return Format("layer %d", _layer);
    }
}


//----------------------------------------------------------------------------
// Mono/stereo modes
//----------------------------------------------------------------------------

std::string ts::AudioAttributes::stereoDescription() const
{
    if (!_is_valid) {
        return "";
    }

    switch (_mode) {
        case 0:
            return "stereo";
        case 1: // joint stereo
            if (_layer == 1 || _layer == 2) {
                switch (_mode_extension) {
                    case 0:  return "subbands 4-31 in intensity stereo";
                    case 1:  return "subbands 8-31 in intensity stereo";
                    case 2:  return "subbands 12-31 in intensity stereo";
                    case 3:  return "subbands 16-31 in intensity stereo";
                    default: return "";
                }
            }
            else {
                switch (_mode_extension) {
                    case 0:  return "";
                    case 1:  return "intensity stereo";
                    case 2:  return "ms stereo";
                    case 3:  return "intensity & ms stereo";
                    default: return "";
                }
            }
        case 2:
            return "dual channel";
        case 3:
            return "single channel";
        default:
            return "";
    }
}
