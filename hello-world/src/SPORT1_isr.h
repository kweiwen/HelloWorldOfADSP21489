
#include <stdint.h>    /* Get definition of uint32_t. */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/**** some declaration or so *****/
void ProcessingTooLong(void);
void TalkThroughISR(uint32_t iid, void* handlerArg);

#ifdef __cplusplus
}
#endif
