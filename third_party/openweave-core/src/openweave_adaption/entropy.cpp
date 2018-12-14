#include <openthread/platform/random.h>
#include <Weave/Support/crypto/WeaveRNG.h>

#include "openweave_adaption/entropy.hpp"


namespace nl {
namespace Weave {
namespace OtFreeRTOS {

static int WeaveRngAdapter(uint8_t *buf, size_t bufSize)
{
    return otPlatRandomGetTrue(buf, bufSize);
}

WEAVE_ERROR InitEntropy()
{
    return nl::Weave::Platform::Security::InitSecureRandomDataSource(WeaveRngAdapter, 64, NULL, 0);
}

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl
