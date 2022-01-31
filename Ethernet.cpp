#include "Ethernet.h"

void HexDump(unsigned char *buf, int count, int radix)
{
    int i, j;
    printf("--------:");
    for (i=0; i < radix; i++)
    {
        printf(" %02X", i);
    }
    for (i = 0, j = 0; i < count; i++)
    {
        if (j % radix == 0)
        {
            if (radix == 16)
                printf("\n%08X: ", j);
            else
                printf("\n%08d: ", j);
        }
        printf("%02X ", buf[i]);
        j++;
    }
    printf("\n");
    fflush(stdout);
    fflush(stderr);
}

int ethConnect()
{
    int ret = 0;

    // create socket for UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf ("Socket error : %i\n", sock);
        return -1;
    }

    // set up parameters for UDP
    memset((char *)&addr_out, 0, sizeof(addr_out));
    addr_out.sin_family = AF_INET;
    addr_out.sin_port = htons(9522);
    addr_out.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sock, (struct sockaddr*) &addr_out, sizeof(addr_out));
    // here is the destination IP
    addr_out.sin_addr.s_addr = inet_addr(IP_Broadcast);

    // set options to receive broadcasted packets
    // leave this block and you have normal UDP communication (after the inverter scan)
    struct ip_mreq mreq;

    mreq.imr_multiaddr.s_addr = inet_addr(IP_Broadcast);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq));
    unsigned char loop = 0;
    ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (const char *)&loop, sizeof(loop));

    if (ret < 0)
    {
        printf ("setsockopt IP_ADD_MEMBERSHIP failed\n");
        return -1;
    }
    // end of setting broadcast options

    return 0; //OK
}

int ethRead(unsigned char *buf, unsigned int bufsize)
{
    int bytes_read;
    short timeout = 5;
    socklen_t addr_in_len = sizeof(addr_in);

    fd_set readfds;

    do
    {
        struct timeval tv;
        tv.tv_sec = timeout;     //set timeout of reading
        tv.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        int rc = select(sock+1, &readfds, NULL, NULL, &tv);
        printf("select() returned %d\n", rc);
        if (rc == -1)
            printf("errno = %d\n", errno);

        if (FD_ISSET(sock, &readfds))
            bytes_read = recvfrom(sock, (char*)buf, bufsize, 0, (struct sockaddr *)&addr_in, &addr_in_len);
        else
        {
            puts("Timeout reading socket");
            return -1;
        }

        if ( bytes_read > 0)
        {
            if (bytes_read > MAX_CommBuf)
            {
                MAX_CommBuf = bytes_read;
                printf("MAX_CommBuf is now %d bytes\n", MAX_CommBuf);
            }
            printf("%d bytes received from [%s]\n", bytes_read, inet_ntoa(addr_in.sin_addr));
            if (bytes_read == 600 || bytes_read == 608 || bytes_read == 0)
                printf(" ==> packet ignored\n");

        }
        else
            printf("recvfrom() returned an error: %d\n", bytes_read);

    } while (bytes_read == 600 || bytes_read == 608); // keep on reading if data received from Energy Meter (600 bytes) or Sunny Home Manager (608 bytes)

    return bytes_read;
}

int ethSend(unsigned char *buffer, const char *toIP)
{
    HexDump(buffer, packetposition, 10);

    addr_out.sin_addr.s_addr = inet_addr(toIP);
    size_t bytes_sent = sendto(sock, (const char*)buffer, packetposition, 0, (struct sockaddr *)&addr_out, sizeof(addr_out));

    std::cout << bytes_sent << " Bytes sent to [" << inet_ntoa(addr_out.sin_addr) << "]" << std::endl;

    return bytes_sent;
}

int ethClose()
{
    if (sock != 0)
    {
        close(sock);
        sock = 0;
    }
    return 0;
}

int getLocalIP(unsigned char IPaddress[4])
{
    int rc = 0;
    struct ifaddrs *myaddrs;
    struct in_addr *inaddr;

    if(getifaddrs(&myaddrs) == 0)
    {
        for (struct ifaddrs *ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr != NULL)
            {
                // Find the active network adapter

                // Convert interface name to lower case
                char ifname[32]; // Idk the size?
                strcpy(ifname, ifa->ifa_name);
                int j = 0;
                while (ifname[j]) {
                    ifname[j] = tolower(ifname[j]);
                    j++;
                }

                if ((ifa->ifa_addr->sa_family == AF_INET) && (ifa->ifa_flags & IFF_UP) && (strcmp(ifname, "lo") != 0))
                {
                    struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
                    inaddr = &s4->sin_addr;

                    unsigned long ipaddr = inaddr->s_addr;
                    IPaddress[3] = ipaddr & 0xFF;
                    IPaddress[2] = (ipaddr >> 8) & 0xFF;
                    IPaddress[1] = (ipaddr >> 16) & 0xFF;
                    IPaddress[0] = (ipaddr >> 24) & 0xFF;

                    break;
                }
            }
        }

        freeifaddrs(myaddrs);
    }
    else
        rc = -1;

    return rc;
}

void writeLong(BYTE *btbuffer, unsigned long v)
{
    writeByte(btbuffer,(unsigned char)((v >> 0) & 0xFF));
    writeByte(btbuffer,(unsigned char)((v >> 8) & 0xFF));
    writeByte(btbuffer,(unsigned char)((v >> 16) & 0xFF));
    writeByte(btbuffer,(unsigned char)((v >> 24) & 0xFF));
}

void writeShort(BYTE *btbuffer, unsigned short v)
{
    writeByte(btbuffer,(unsigned char)((v >> 0) & 0xFF));
    writeByte(btbuffer,(unsigned char)((v >> 8) & 0xFF));
}

void writeByte(unsigned char *btbuffer, unsigned char v)
{
    btbuffer[packetposition++] = v;
}

void writeArray(unsigned char *btbuffer, const unsigned char bytes[], int loopcount)
{
    for (int i = 0; i < loopcount; i++)
    {
        writeByte(btbuffer, bytes[i]);
    }
}

int64_t get_longlong(BYTE *buf)
{
    register int64_t lnglng = 0;

	lnglng += *(buf+7);
	lnglng <<= 8;
	lnglng += *(buf+6);
	lnglng <<= 8;
	lnglng += *(buf+5);
	lnglng <<= 8;
	lnglng += *(buf+4);
	lnglng <<= 8;
	lnglng += *(buf+3);
	lnglng <<= 8;
	lnglng += *(buf+2);
	lnglng <<= 8;
	lnglng += *(buf+1);
	lnglng <<= 8;
	lnglng += *(buf);

    return lnglng;
}

int32_t get_long(BYTE *buf)
{
    register int32_t lng = 0;

	lng += *(buf+3);
	lng <<= 8;
	lng += *(buf+2);
	lng <<= 8;
	lng += *(buf+1);
	lng <<= 8;
	lng += *(buf);

    return lng;
}

short get_short(BYTE *buf)
{
    register short shrt = 0;

	shrt += *(buf+1);
	shrt <<= 8;
	shrt += *(buf);

    return shrt;
}



void writePacketHeader(unsigned char *buf, const unsigned int control, const unsigned char *destaddress)
{
    packetposition = 0;


    //Ignore control and destaddress
    writeLong(buf, 0x00414D53);  // SMA\0
    writeLong(buf, 0xA0020400);
    writeLong(buf, 0x01000000);
    writeByte(buf, 0);
    writeByte(buf, 0);          // Placeholder for packet length

}

void writePacketLength(unsigned char *buf)
{

    short dataLength = (short)(packetposition - sizeof(ethPacketHeaderL1L2));
    ethPacketHeaderL1L2 *hdr = (ethPacketHeaderL1L2 *)buf;
    hdr->pcktHdrL1.hiPacketLen = (dataLength >> 8) & 0xFF;
    hdr->pcktHdrL1.loPacketLen = dataLength & 0xFF;

}

int validateChecksum()
{
    FCSChecksum = 0xffff;
    //Skip over 0x7e at start and end of packet
    int i;
    for(i = 1; i <= packetposition - 4; i++)
    {
        FCSChecksum = (FCSChecksum >> 8) ^ fcstab[(FCSChecksum ^ pcktBuf[i]) & 0xff];
    }

    FCSChecksum ^= 0xffff;

    if (get_short(pcktBuf + packetposition - 3) == (short)FCSChecksum)
        return true;
    else
    {
        printf("Invalid chk 0x%04X - Found 0x%02X%02X\n", FCSChecksum, pcktBuf[packetposition-2], pcktBuf[packetposition-3]);
        return false;
    }
}
