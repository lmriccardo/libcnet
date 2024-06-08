#ifndef _FRAGMENT_H
#define _FRAGMENT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <math.h>
#include "neto.h"

__BEGIN_DECLS

struct FragmentList
{
    IpPacket** _frags;
    size_t     _size;
};

/* Compute the number of fragments to create with an IP Packet payload of input size */
extern int computeNumberOfFragments(const size_t _size);

/* Create all the fragments and returns the structure containing
   the list with all pointer to fragmented Ip packets and its size 
*/
extern struct FragmentList* createFragments(const IpPacket* _pckt) 
    __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Free the memory allocated for the fragment list */
extern void FragmentList_delete(struct FragmentList* _fraglist) __attribute__((nonnull));

__END_DECLS

#endif