#include "fragment.h"

int computeNumberOfFragments(const size_t _size)
{
    float num_of_frag = _size / IP_PAYLOAD_MAX_SIZE;
    return (int)floorf(num_of_frag) + 1;
}

struct FragmentList* createFragments(const IpPacket* _pckt)
{
    struct FragmentList *fraglist = (struct FragmentList*)malloc(sizeof(struct FragmentList));
    int num_of_frags = computeNumberOfFragments(_pckt->_iphdr->_tlength - IP_HEADER_SIZE);
    IpPacket** frags = (IpPacket**)malloc(num_of_frags * sizeof(IpPacket));
    
    char payload[IP_PAYLOAD_MAX_SIZE];
    memset(payload, 0, IP_PAYLOAD_MAX_SIZE);

    for (int fi = 0; fi < num_of_frags; fi++)
    {
        IpHeader* hdr = IpHeader_new();
        memcpy(hdr, _pckt->_iphdr, IP_HEADER_SIZE);

        int offset = fi * IP_PAYLOAD_MAX_SIZE;
        int _m = fi < num_of_frags - 1 ? M_FLAG_SET : M_FLAG_NOT_SET;
        int _d = fi < num_of_frags - 1 ? D_FLAG_NOT_SET : D_FLAG_SET;
        u_int16_t flagoff = computeFlagOff(X_FLAG_NOT_SET, _d, _m, offset);
        IpHeader_setFlagOffField(hdr, flagoff);

        IpPacket* pckt = IpPacket_new();
        IpPacket_setHeader(pckt, hdr);

        memcpy(payload, _pckt->_payload + offset, IP_PAYLOAD_MAX_SIZE);
        IpPacket_fillPayload(pckt, payload, IP_PAYLOAD_MAX_SIZE);

        *(frags + fi) = pckt;
    }

    fraglist->_frags = frags;
    fraglist->_size  = num_of_frags;

    return fraglist;
}

void FragmentList_delete(struct FragmentList* _fraglist)
{
    for (int fi = 0; fi < _fraglist->_size; fi++)
    {
        free(*(_fraglist->_frags + fi));
    }

    free(_fraglist->_frags);
    free(_fraglist);
}