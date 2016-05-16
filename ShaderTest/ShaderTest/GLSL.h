//
//  GLSL.h
//  ShaderTest
//
//  Created by HasegawaYasuo on 2016/05/16.
//  Copyright © 2016年 HasegawaYasuo. All rights reserved.
//
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class GLSL {
public:
    GLSL ();
    static int readShaderSource(GLuint shader, const char *name);
    static void printShaderInfoLog(GLuint shader);
    static void printProgramInfoLog(GLuint program);
};
