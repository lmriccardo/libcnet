#include "utils/fragment.h"

int computeNumberOfFragments(const size_t _size)
{
    float num_of_frag = _size / IP_PAYLOAD_MAX_SIZE;
    return (int)floorf(num_of_frag) + 1;
}

struct FragmentList* createFragments(const char* _buff, const size_t _size)
{
    struct FragmentList* fraglist = (struct FragmentList*)malloc(sizeof(struct FragmentList));
    int nfrags = computeNumberOfFragments(_size);
    fraglist->_frags = (unsigned char **)malloc(nfrags * sizeof(char *));
    unsigned char frags[nfrags][IP_PAYLOAD_MAX_SIZE];
    size_t offset = 0;
    
    for (int i = 0; i < nfrags; i++)
    {
        size_t fragsize = (i < nfrags - 1) ? IP_PAYLOAD_MAX_SIZE : _size - i * IP_PAYLOAD_MAX_SIZE;
        strncpy(frags[i], _buff + offset, fragsize);
        
        *(fraglist->_frags + i) = (unsigned char*)malloc(fragsize * sizeof(char));
        memcpy(*(fraglist->_frags + i), frags[i], fragsize);
        offset = offset + IP_PAYLOAD_MAX_SIZE;
    }
    
    fraglist->_size = nfrags;
    
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