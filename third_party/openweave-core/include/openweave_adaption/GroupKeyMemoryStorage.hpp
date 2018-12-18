#ifndef OT_FREERTOS_GROUP_KEY_MEMORY_STORAGE_HPP_
#define OT_FREERTOS_GROUP_KEY_MEMORY_STORAGE_HPP_

#include <Weave/Core/WeaveFabricState.h>
#include <Weave/Profiles/security/WeaveApplicationKeys.h>

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

template<int K> 
class Pow2RoundUp {
public:
    enum {
        kValue = Pow2RoundUp<(K+1)/2>::kValue * 2,
    };
};

template<> 
class Pow2RoundUp<1> {
public:
    enum {
        kValue = 1,
    };
};

class GroupKeyMemoryStore : public Profiles::Security::AppKeys::GroupKeyStoreBase
{
public:
    enum
    {
        kMaxGroupKeys = WEAVE_CONFIG_MAX_APPLICATION_EPOCH_KEYS +       // Maximum number of Epoch keys
                        WEAVE_CONFIG_MAX_APPLICATION_GROUPS +           // Maximum number of Application Group Master keys
                        1 +                                             // Maximum number of Root keys (1 for Service root key)
                        1,                                              // Fabric secret
        kHashSize = Pow2RoundUp<kMaxGroupKeys>::kValue * 2,
    };

    GroupKeyMemoryStore();

    // Manage application group key material storage.
    WEAVE_ERROR RetrieveGroupKey(uint32_t keyId, Profiles::Security::AppKeys::WeaveGroupKey &key) override;
    WEAVE_ERROR StoreGroupKey(const Profiles::Security::AppKeys::WeaveGroupKey &key) override;
    WEAVE_ERROR DeleteGroupKey(uint32_t keyId) override;
    WEAVE_ERROR DeleteGroupKeysOfAType(uint32_t keyType) override;
    WEAVE_ERROR EnumerateGroupKeys(uint32_t  keyType,
                                   uint32_t *keyIds,
                                   uint8_t   keyIdsArraySize,
                                   uint8_t & keyCount) override;
    WEAVE_ERROR Clear(void) override;

    // Get current platform UTC time in seconds.
    WEAVE_ERROR GetCurrentUTCTime(uint32_t &utcTime) override;

protected:
    WEAVE_ERROR RetrieveLastUsedEpochKeyId(void) override;
    WEAVE_ERROR StoreLastUsedEpochKeyId(void) override;
    
private:
    uint32_t mLastUsedEpochKeyIdStorage;

    Profiles::Security::AppKeys::WeaveGroupKey mKeys[kHashSize];
    bool mOnceOccupieed[kHashSize];
};

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

#endif // OT_FREERTOS_GROUP_KEY_MEMORY_STORAGE_HPP_
