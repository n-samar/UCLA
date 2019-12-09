#include "provided.h"
#include "support.h"

// TODO: Delete this
int max(int a, int b)
{
	return a > b?a:b;
}

bool operator< (const GeoCoord &lhs, const GeoCoord &rhs)
{
	if(lhs.latitude != rhs.latitude)
    	return (lhs.latitude < rhs.latitude);
    else
    	return (lhs.longitude < rhs.longitude);
}

bool operator== (const GeoCoord &lhs, const GeoCoord &rhs)
{
    if(lhs.latitude == rhs.latitude
        && lhs.longitude == rhs.longitude)
        return true;
    else
        return false;
}

bool operator!= (const GeoCoord &lhs, const GeoCoord &rhs)
{
    return !(lhs == rhs);
}