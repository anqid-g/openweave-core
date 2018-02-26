/*
 *
 *    Copyright (c) 2013-2017 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file implements the concrete class
 *      <tt>nl::Inet::IPPrefix</tt>, which comprise two member fields:
 *      a) a IP address and b) a length field. The Nest Inet Layer
 *      uses objects of this class to represent Internet protocol
 *      address prefixes of both IPv4 and IPv6 address families.
 *
 */

#include <InetLayer/IPPrefix.h>

namespace nl {
namespace Inet {

IPPrefix IPPrefix::Zero;

bool IPPrefix::IsZero() const
{
    return  IPAddr.Addr[0] == 0 &&
            IPAddr.Addr[1] == 0 &&
            IPAddr.Addr[2] == 0 &&
            IPAddr.Addr[3] == 0 &&
            Length == 0;
}

bool IPPrefix::operator==(const IPPrefix& other) const
{
    return IPAddr == other.IPAddr && Length == other.Length;
}

bool IPPrefix::operator!=(const IPPrefix& other) const
{
    return IPAddr != other.IPAddr || Length != other.Length;
}

IPPrefix & IPPrefix::operator=(const IPPrefix& other)
{
    if (this != &other)
    {
        IPAddr = other.IPAddr;
        Length = other.Length;
    }

    return *this;
}

bool IPPrefix::MatchAddress(const IPAddress& addr) const
{

#if WEAVE_SYSTEM_CONFIG_USE_LWIP
    // Only 64 bit prefixes supported for now.
    return (Length == 64 && addr.Addr[0] == IPAddr.Addr[0] && addr.Addr[1] == IPAddr.Addr[1]);
#endif // WEAVE_SYSTEM_CONFIG_USE_LWIP

#if WEAVE_SYSTEM_CONFIG_USE_SOCKETS
    uint8_t l = (Length <= 128) ? Length : 128;
    int i = 0;

    for (; i < 4 && l >= 32; i++, l -= 32)
        if (IPAddr.Addr[i] != addr.Addr[i])
            return false;

    if ((i == 4) || (l == 0))
        return true;

    uint32_t mask = htonl(0xFFFFFFFF << (32 - l));

    return (IPAddr.Addr[i] & mask) == (addr.Addr[i] & mask);
#endif // WEAVE_SYSTEM_CONFIG_USE_SOCKETS

}

} // namespace Inet
} // namespace nl