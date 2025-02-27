#include "ai.h"
#include <iostream>
#include <filesystem>
#include <onnxruntime_cxx_api.h>
#include <dml_provider_factory.h>

AIModel::AIModel() : env(ORT_LOGGING_LEVEL_INFO, "R6AI") {
}

bool AIModel::initialize(const std::string& model_path, InferenceBackend backend) {
    if (!std::filesystem::exists(model_path)) {
        std::cout << "Model file not found: " << model_path << std::endl;
        return false;
    }

    current_backend = backend;
    switch (backend) {
        case InferenceBackend::ONNX_CPU:
            return initializeONNX(model_path, false);
        case InferenceBackend::ONNX_DML:
            return initializeONNX(model_path, true);
        case InferenceBackend::TENSORRT:
            std::cout << "TensorRT support not yet implemented" << std::endl;
            return false;
        default:
            return false;
    }
}

bool AIModel::initializeONNX(const std::string& model_path, bool use_dml) {
    try {
        Ort::SessionOptions session_options;
        session_options.SetIntraOpNumThreads(1);
        session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

        if (use_dml) {
            session_options.AppendExecutionProvider_DML(0);
            std::cout << "Using DirectML execution provider" << std::endl;
        }

        session = std::make_unique<Ort::Session>(env, model_path.c_str(), session_options);
        setupIONames();
        
        std::cout << "Model loaded successfully" << std::endl;
        std::cout << "Number of inputs: " << input_names.size() << std::endl;
        std::cout << "Number of outputs: " << output_names.size() << std::endl;
        
        return true;
    }
    catch (const Ort::Exception& e) {
        std::cout << "ONNX Runtime error: " << e.what() << std::endl;
        return false;
    }
}

void AIModel::setupIONames() {
    Ort::AllocatorWithDefaultOptions allocator;
    
    size_t num_input_nodes = session->GetInputCount();
    size_t num_output_nodes = session->GetOutputCount();
    
    input_names.reserve(num_input_nodes);
    output_names.reserve(num_output_nodes);
    
    for (size_t i = 0; i < num_input_nodes; i++) {
        input_names.push_back(session->GetInputNameAllocated(i, allocator).get());
    }
    
    for (size_t i = 0; i < num_output_nodes; i++) {
        output_names.push_back(session->GetOutputNameAllocated(i, allocator).get());
    }
}

bool AIModel::infer(const std::vector<float>& input_data, std::vector<float>& output_data) {
    try {
        Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        
        std::vector<int64_t> input_shape = session->GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
        std::vector<int64_t> output_shape = session->GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
        
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            memory_info,
            const_cast<float*>(input_data.data()),
            input_data.size(),
            input_shape.data(),
            input_shape.size()
        );
        
        auto output_tensors = session->Run(
            Ort::RunOptions{nullptr},
            input_names.data(),
            &input_tensor,
            1,
            output_names.data(),
            1
        );
        
        if (output_tensors.size() > 0) {
            float* output_data_ptr = output_tensors[0].GetTensorMutableData<float>();
            size_t output_size = output_tensors[0].GetTensorTypeAndShapeInfo().GetElementCount();
            output_data.assign(output_data_ptr, output_data_ptr + output_size);
            return true;
        }
        
        return false;
    }
    catch (const Ort::Exception& e) {
        std::cout << "Inference error: " << e.what() << std::endl;
        return false;
    }
}
