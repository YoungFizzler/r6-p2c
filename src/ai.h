#pragma once
#include <memory>
#include <string>
#include <vector>
#include <onnxruntime_cxx_api.h>

enum class InferenceBackend {
    ONNX_CPU,
    ONNX_DML,
    TENSORRT,
};

class AIModel {
private:
    std::unique_ptr<Ort::Session> session;
    Ort::Env env;
    std::vector<const char*> input_names;
    std::vector<const char*> output_names;
    InferenceBackend current_backend;

public:
    AIModel();
    ~AIModel() = default;
    
    bool initialize(const std::string& model_path, InferenceBackend backend = InferenceBackend::ONNX_DML);
    bool infer(const std::vector<float>& input_data, std::vector<float>& output_data);
    InferenceBackend getCurrentBackend() const { return current_backend; }
    
private:
    bool initializeONNX(const std::string& model_path, bool use_dml = true);
    void setupIONames();
};
