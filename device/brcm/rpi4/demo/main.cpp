#include "CalculatorService.h"

#include <aidl/navis/calculator/ICalculator.h>
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <cstdlib>
#include <string>

using aidl::navis::calculator::CalculatorService;
using aidl::navis::calculator::ICalculator;

namespace {

constexpr uint32_t kBinderThreadCount = 4;

}  // namespace

int main(int /* argc */, char** argv) {
    android::base::InitLogging(argv);
    android::base::SetDefaultTag("NavisCalculatorService");

    ABinderProcess_setThreadPoolMaxThreadCount(kBinderThreadCount);
    ABinderProcess_startThreadPool();

    const auto service = ndk::SharedRefBase::make<CalculatorService>();
    const std::string instance =
            std::string(ICalculator::descriptor) + "/default";

    const binder_status_t status =
            AServiceManager_addService(service->asBinder().get(), instance.c_str());
    if (status != STATUS_OK) {
        LOG(ERROR) << "Failed to register " << instance << ", status=" << status;
        return EXIT_FAILURE;
    }

    LOG(INFO) << "Ready: " << instance;
    ABinderProcess_joinThreadPool();

    LOG(ERROR) << "Binder thread pool unexpectedly exited";
    return EXIT_FAILURE;
}
