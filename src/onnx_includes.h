#pragma once

#ifndef ORT_API_MANUAL_INIT
#define ORT_API_MANUAL_INIT
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include <onnxruntime_cxx_api.h>
#include <onnxruntime_c_api.h>
#include <dml_provider_factory.h>

using namespace Ort;
using GraphOptimizationLevel = GraphOptimizationLevel; 