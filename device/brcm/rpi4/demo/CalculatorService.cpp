#include "CalculatorService.h"

#include <android/binder_ibinder.h>
#include <android/binder_status.h>
#include <android-base/logging.h>

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace aidl::navis::calculator {

namespace {

bool parseNumber(const char* text, double* value) {
    char* end = nullptr;
    errno = 0;
    *value = std::strtod(text, &end);
    return errno == 0 && end != text && *end == '\0' && std::isfinite(*value);
}

void printUsage(int fd) {
    dprintf(fd, "Navis Calculator Service\n");
    dprintf(fd, "Usage: dumpsys navis.calculator.ICalculator/default ");
    dprintf(fd, "<add|sub|mul|div> <x> <y>\n");
}

}  // namespace

ndk::ScopedAStatus CalculatorService::init() {
    calculationCount_.store(0);
    LOG(INFO) << "uid=" << AIBinder_getCallingUid()
              << " initialized calculator state";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus CalculatorService::add(double in_x, double in_y,
                                          double* _aidl_return) {
    auto status = validateOperands(in_x, in_y);
    if (!status.isOk()) {
        return status;
    }

    *_aidl_return = in_x + in_y;
    ++calculationCount_;
    LOG(INFO) << "uid=" << AIBinder_getCallingUid() << " add(" << in_x
              << ", " << in_y << ") = " << *_aidl_return;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus CalculatorService::sub(double in_x, double in_y,
                                          double* _aidl_return) {
    auto status = validateOperands(in_x, in_y);
    if (!status.isOk()) {
        return status;
    }

    *_aidl_return = in_x - in_y;
    ++calculationCount_;
    LOG(INFO) << "uid=" << AIBinder_getCallingUid() << " sub(" << in_x
              << ", " << in_y << ") = " << *_aidl_return;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus CalculatorService::mul(double in_x, double in_y,
                                          double* _aidl_return) {
    auto status = validateOperands(in_x, in_y);
    if (!status.isOk()) {
        return status;
    }

    *_aidl_return = in_x * in_y;
    ++calculationCount_;
    LOG(INFO) << "uid=" << AIBinder_getCallingUid() << " mul(" << in_x
              << ", " << in_y << ") = " << *_aidl_return;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus CalculatorService::div(double in_x, double in_y,
                                          double* _aidl_return) {
    auto status = validateOperands(in_x, in_y);
    if (!status.isOk()) {
        return status;
    }
    if (in_y == 0.0) {
        return ndk::ScopedAStatus::fromExceptionCodeWithMessage(
                EX_ILLEGAL_ARGUMENT, "Division by zero");
    }

    *_aidl_return = in_x / in_y;
    ++calculationCount_;
    LOG(INFO) << "uid=" << AIBinder_getCallingUid() << " div(" << in_x
              << ", " << in_y << ") = " << *_aidl_return;
    return ndk::ScopedAStatus::ok();
}

binder_status_t CalculatorService::dump(int fd, const char** args,
                                        uint32_t numArgs) {
    if (numArgs == 0) {
        dprintf(fd, "Navis Calculator Service: ready\n");
        dprintf(fd, "Successful calculations: %llu\n",
                static_cast<unsigned long long>(calculationCount_.load()));
        printUsage(fd);
        return STATUS_OK;
    }

    if (numArgs == 1 &&
        (std::strcmp(args[0], "-h") == 0 ||
         std::strcmp(args[0], "--help") == 0)) {
        printUsage(fd);
        return STATUS_OK;
    }

    if (numArgs != 3) {
        dprintf(fd, "Expected an operation and two numbers.\n");
        printUsage(fd);
        return STATUS_BAD_VALUE;
    }

    double x = 0.0;
    double y = 0.0;
    if (!parseNumber(args[1], &x) || !parseNumber(args[2], &y)) {
        dprintf(fd, "x and y must be finite numbers.\n");
        return STATUS_BAD_VALUE;
    }

    double result = 0.0;
    ndk::ScopedAStatus status;
    if (std::strcmp(args[0], "add") == 0) {
        status = add(x, y, &result);
    } else if (std::strcmp(args[0], "sub") == 0) {
        status = sub(x, y, &result);
    } else if (std::strcmp(args[0], "mul") == 0) {
        status = mul(x, y, &result);
    } else if (std::strcmp(args[0], "div") == 0) {
        status = div(x, y, &result);
    } else {
        dprintf(fd, "Unknown operation: %s\n", args[0]);
        printUsage(fd);
        return STATUS_BAD_VALUE;
    }

    if (!status.isOk()) {
        dprintf(fd, "Error: %s\n", status.getDescription().c_str());
        return STATUS_BAD_VALUE;
    }

    dprintf(fd, "%.17g\n", result);
    return STATUS_OK;
}

ndk::ScopedAStatus CalculatorService::validateOperands(double x, double y) {
    if (!std::isfinite(x) || !std::isfinite(y)) {
        return ndk::ScopedAStatus::fromExceptionCodeWithMessage(
                EX_ILLEGAL_ARGUMENT, "Operands must be finite numbers");
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::navis::calculator
