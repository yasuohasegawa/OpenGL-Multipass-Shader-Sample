//
//  GLSL.cpp
//  ShaderTest
//
//  Created by HasegawaYasuo on 2016/05/16.
//  Copyright © 2016年 HasegawaYasuo. All rights reserved.
//  code based from http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20101208
//
#include "GLSL.h"

GLSL::GLSL() {
    
}

int GLSL::readShaderSource(GLuint shader, const char *name)
{
    int ret;
    
    std::ifstream file(name, std::ios::binary);
    if (file.fail()) {
        std::cerr << "Can't open file: " << name << std::endl;
        ret = -1;
    }
    else {
        
        file.seekg(0L, std::ios::end);
        GLsizei length = file.tellg();
        
        const GLchar *source = new GLchar[length];
        
        file.seekg(0L, std::ios::beg);
        file.read((char *)source, length);
        
        if (file.bad()) {
            
            std::cerr << "Could not read file: " << name << std::endl;
            ret = -1;
        }
        else {
            
            glShaderSource(shader, 1, &source, &length);
            ret = 0;
        }
        
        delete[] source;
    }
    
    return ret;
    
}

void GLSL::printShaderInfoLog(GLuint shader)
{
    GLsizei bufSize;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
    
    if (bufSize > 1) {
        GLchar *infoLog;
        
        infoLog = (GLchar *)malloc(bufSize);
        if (infoLog != NULL) {
            GLsizei length;
            
            glGetShaderInfoLog(shader, bufSize, &length, infoLog);
            fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
            free(infoLog);
        }
        else
            fprintf(stderr, "Could not allocate InfoLog buffer.\n");
    }
}

void GLSL::printProgramInfoLog(GLuint program)
{
    GLsizei bufSize;
    
    glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
    
    if (bufSize > 1) {
        GLchar *infoLog;
        
        infoLog = (GLchar *)malloc(bufSize);
        if (infoLog != NULL) {
            GLsizei length;
            
            glGetProgramInfoLog(program, bufSize, &length, infoLog);
            fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
            free(infoLog);
        }
        else
            fprintf(stderr, "Could not allocate InfoLog buffer.\n");
    }
}