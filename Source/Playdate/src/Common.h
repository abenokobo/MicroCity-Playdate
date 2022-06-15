#ifndef __COMMON_H
#define __COMMON_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#pragma GCC diagnostic push
#include "pd_api.h"
#pragma GCC diagnostic pop

extern PlaydateAPI* gpd;

#ifdef __cplusplus
}
#endif


#include <assert.h>
#include <memory>


static const int PLAYDATE_ZOOM_SCALE = 3;



#endif // __COMMON_H







