//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_PIPELINE_H
#define VULKANGAMEENGINETUTORIAL_PIPELINE_H


#include <string>
#include <vector>

class PipeLine {
private:
    static std::vector<char> ReadFile(const std::string &filePath);
    
    void CreateGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath);

public:
    PipeLine(const std::string &vertexFilePath, const std::string &fragmentFilePath);

};


#endif //VULKANGAMEENGINETUTORIAL_PIPELINE_H
