#ifndef _FRAGMENT_H
#define _FRAGMENT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <math.h>
#include "ip.h"
#include "utils/version.h"

__BEGIN_DECLS

/**
 * This struct holds a number of fragments created from another buffer
 * using the `createFragments` function. Each fragment is a portion of the
 * original buffer. It represents an ordered lists of fragments, each of
 * them with the same size, except for the last one. 
 */
struct FragmentList
{
    char** _frags;
    size_t _size;
};

/**
 * Compute the number of fragments to create with an IP Packet payload of input size
 */
extern int computeNumberOfFragments(const size_t _sizem, const size_t _max);

/**
 * Create all the fragments and returns the structure containing
 * the list with all pointer to fragmented Ip packets and its size 
 */
extern struct FragmentList* createFragments(
        const char* _buffer, const size_t _size, const size_t _max
    ) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Free the memory allocated for the fragment list
 */
extern void FragmentList_delete(struct FragmentList* _fraglist) __attribute__((nonnull));

__END_DECLS

#endif