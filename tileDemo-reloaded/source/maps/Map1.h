
#ifndef _Map1Data_h_
#define _Map1Data_h_

// Contains mapping of tiles of the map. Each value of the array indicates which tile must be displayed
// (first column is horizontal position, second column is vertical position, unit is tile)
extern const signed short Map1Data[51][87];

// Contains mapping of collides on the map. Each value of the array indicates which kind of collide has to be handled
// (first column is horizontal position, second column is vertical position, unit is tile)
extern const signed int Map1Info[51][87];

#endif //_Map1Data_h_
