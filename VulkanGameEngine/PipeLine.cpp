//
// Created by ShaneMonck on 20/03/2024.
//

#include <fstream>
#include <iostream>
#include "PipeLine.h"
#include <cstdio>

// #region Private Methods
std::vector<char> PipeLine::ReadFile(const std::string &filePath) {
    std::ifstream file = std::ifstream(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filePath);
    }

    std::streamsize fileSize = static_cast<std::streamsize>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

// #endRegion

PipeLine::PipeLine(const std::string &vertexFilePath, const std::string &fragmentFilePath) {
    CreateGraphicsPipeline(vertexFilePath, fragmentFilePath);
}

void PipeLine::CreateGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath) {
    auto vertexCode = ReadFile(vertexFilePath);
    auto fragmentCode = ReadFile(fragmentFilePath);

    printf("Vertex Shader Code Size %zu\n", vertexCode.size());
    printf("Fragment Shader Code Size %zu\n", fragmentCode.size());
}


