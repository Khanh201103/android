#pragma once

#include <aidl/navis/calculator/BnCalculator.h>

#include <atomic>
#include <cstdint>

namespace aidl::navis::calculator {

class CalculatorService final : public BnCalculator {
 public:
    ndk::ScopedAStatus init() override;
    ndk::ScopedAStatus add(double in_x, double in_y,
                           double* _aidl_return) override;
    ndk::ScopedAStatus sub(double in_x, double in_y,
                           double* _aidl_return) override;
    ndk::ScopedAStatus mul(double in_x, double in_y,
                           double* _aidl_return) override;
    ndk::ScopedAStatus div(double in_x, double in_y,
                           double* _aidl_return) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;

 private:
    static ndk::ScopedAStatus validateOperands(double x, double y);

    std::atomic<uint64_t> calculationCount_ = 0;
};

}  // namespace aidl::navis::calculator
