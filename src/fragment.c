#include "utils/fragment.h"

int computeNumberOfFragments(const size_t _size, const size_t _max)
{
    float num_of_frag = _size / _max;
    return (int)floorf(num_of_frag) + 1;
}

struct FragmentList* createFragments(const char* _buff, const size_t _size, const size_t _max)
{
    struct FragmentList* fraglist = (struct FragmentList*)malloc(sizeof(struct FragmentList));
    int nfrags = computeNumberOfFragments(_size, _max);
    fraglist->_frags = (char **)malloc(nfrags * sizeof(char *));
    char frags[nfrags][_max];
    size_t offset = 0;
    
    for (int i = 0; i < nfrags; i++)
    {
        size_t fragsize = (i < nfrags - 1) ? _max : _size - i * _max;
        strncpy(frags[i], _buff + offset, fragsize);
        
        *(fraglist->_frags + i) = (char*)malloc(fragsize * sizeof(char));
        memcpy(*(fraglist->_frags + i), frags[i], fragsize);
        offset = offset + _max;
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