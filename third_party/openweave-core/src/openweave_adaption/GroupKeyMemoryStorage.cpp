#include "openweave_adaption/GroupKeyMemoryStorage.hpp"

using namespace nl::Weave::Profiles::Security::AppKeys;

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

GroupKeyMemoryStore::GroupKeyMemoryStore()
{
    for (int i = 0; i < kHashSize; i++)
    {
        mKeys[i].KeyId    = WeaveKeyId::kNone;
        mOnceOccupieed[i] = false;
    }
}

WEAVE_ERROR GroupKeyMemoryStore::RetrieveGroupKey(uint32_t keyId, WeaveGroupKey &key)
{
    uint32_t    i    = keyId % kHashSize;
    int         step = 1;
    WEAVE_ERROR err  = WEAVE_NO_ERROR;

    VerifyOrExit(keyId != WeaveKeyId::kNone, err = WEAVE_ERROR_INVALID_ARGUMENT);

    while (mKeys[i].KeyId != keyId && mOnceOccupieed[i])
    {
        i += step * step;
        i %= kHashSize;
        step++;
    }

    if (mKeys[i].KeyId == keyId)
    {
        key = mKeys[i];
    }
    else
    {
        err = WEAVE_ERROR_KEY_NOT_FOUND;
    }
exit:
    return err;
}

WEAVE_ERROR GroupKeyMemoryStore::StoreGroupKey(const WeaveGroupKey &key)
{
    uint32_t    i    = key.KeyId % kHashSize;
    int         step = 1;
    WEAVE_ERROR err  = WEAVE_NO_ERROR;

    VerifyOrExit(key.KeyId != WeaveKeyId::kNone, err = WEAVE_ERROR_INVALID_ARGUMENT);

    while (mKeys[i].KeyId != WeaveKeyId::kNone)
    {
        i += step * step;
        i %= kHashSize;
        step++;
    }

    mOnceOccupieed[i] = true;
    mKeys[i]          = key;
exit:
    return err;
}

WEAVE_ERROR GroupKeyMemoryStore::DeleteGroupKey(uint32_t keyId)
{
    uint32_t    i    = keyId % kHashSize;
    int         step = 1;
    WEAVE_ERROR err  = WEAVE_NO_ERROR;

    VerifyOrExit(keyId != WeaveKeyId::kNone, err = WEAVE_ERROR_INVALID_ARGUMENT);

    while (mKeys[i].KeyId != keyId && mOnceOccupieed[i])
    {
        i += step * step;
        i %= kHashSize;
        step++;
    }

    if (mKeys[i].KeyId == keyId)
    {
        mKeys[i].KeyId = WeaveKeyId::kNone;
    }
    else
    {
        err = WEAVE_ERROR_KEY_NOT_FOUND;
    }
exit:
    return err;
}

WEAVE_ERROR GroupKeyMemoryStore::DeleteGroupKeysOfAType(uint32_t keyType)
{
    for (int i = 0; i < kHashSize; i++)
    {
        if (mKeys[i].KeyId != WeaveKeyId::kNone && WeaveKeyId::GetType(mKeys[i].KeyId) == keyType)
        {
            mKeys[i].KeyId = WeaveKeyId::kNone;
        }
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR GroupKeyMemoryStore::EnumerateGroupKeys(uint32_t  keyType,
                                                    uint32_t *keyIds,
                                                    uint8_t   keyIdsArraySize,
                                                    uint8_t & keyCount)
{
    keyCount = 0;

    for (int i = 0; i < kHashSize && keyCount < keyIdsArraySize; i++)
    {
        if (mKeys[i].KeyId != WeaveKeyId::kNone && WeaveKeyId::GetType(mKeys[i].KeyId) == keyType)
        {
            keyIds[keyCount++] = mKeys[i].KeyId;
        }
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR GroupKeyMemoryStore::Clear(void)
{
    for (int i = 0; i < kHashSize; i++)
    {
        mKeys[i].KeyId    = WeaveKeyId::kNone;
        mOnceOccupieed[i] = false;
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR GroupKeyMemoryStore::GetCurrentUTCTime(uint32_t &utcTime)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

WEAVE_ERROR GroupKeyMemoryStore::RetrieveLastUsedEpochKeyId(void)
{
    LastUsedEpochKeyId = mLastUsedEpochKeyIdStorage;
}

WEAVE_ERROR GroupKeyMemoryStore::StoreLastUsedEpochKeyId(void)
{
    mLastUsedEpochKeyIdStorage = LastUsedEpochKeyId;
}

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl
