#ifndef OT_FREERTOS_OPENWEAVE_PLATFORM_MAMAGER_IMPL_HPP_
#define OT_FREERTOS_OPENWEAVE_PLATFORM_MAMAGER_IMPL_HPP_

#include <Weave/Core/WeaveError.h>
#include <Weave/DeviceLayer/PlatformManager.h>
#include <Weave/DeviceLayer/FreeRTOS/GenericPlatformManagerImpl_FreeRTOS.h>

namespace nl {
namespace Weave {
namespace DeviceLayer {

extern template class Internal::GenericPlatformManagerImpl<PlatformManagerImpl>;
extern template class Internal::GenericPlatformManagerImpl_FreeRTOS<PlatformManagerImpl>;

class PlatformManagerImpl final
    : public PlatformManager,
      public Internal::GenericPlatformManagerImpl_FreeRTOS<PlatformManagerImpl>
{
    friend PlatformManager;
    friend Internal::GenericPlatformManagerImpl_FreeRTOS<PlatformManagerImpl>;
private: 
    WEAVE_ERROR _InitWeaveStack(void);

    friend PlatformManager & PlatformMgr(void);
    friend PlatformManagerImpl & PlatformMgrImpl(void);

    static PlatformManagerImpl sInstance;
};

// global platform manager instance
//

inline PlatformManager & PlatformMgr(void)
{
    return static_cast<PlatformManager&>(PlatformManagerImpl::sInstance);
}

inline PlatformManagerImpl & PlatformMgrImpl(void)
{
    return PlatformManagerImpl::sInstance;
}

}
}
}


#endif
