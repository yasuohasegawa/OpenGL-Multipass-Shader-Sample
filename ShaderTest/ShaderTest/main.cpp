//
//  main.cpp
//  ShaderTest
//
//  Created by HasegawaYasuo on 2016/05/06.
//  Copyright © 2016年 HasegawaYasuo. All rights reserved.
//  code based from http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20101208
//
#include <cstdio>
#include <cstdlib>

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GLSL.h"


using namespace std;

#define CYCLE 1000.0f
#define FBOWIDTH  1024
#define FBOHEIGHT 768

static GLuint pass1;
static GLuint pass2;
static GLint _time;
static GLint _resolution;
static GLint _time2;
static GLint _resolution2;
static GLint _tex;

static GLuint fb;
static GLuint cb;
static GLuint rb;

static int width, height;


static GLuint loadShader(const char *vert, const char *frag)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    if (GLSL::readShaderSource(vertShader, vert)) exit(1);
    if (GLSL::readShaderSource(fragShader, frag)) exit(1);
    
    GLint compiled, linked;
    
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    GLSL::printShaderInfoLog(vertShader);
    if (compiled == GL_FALSE) {
        fprintf(stderr, "Compile error in vertex shader.\n");
        exit(1);
    }
    
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    GLSL::printShaderInfoLog(fragShader);
    if (compiled == GL_FALSE) {
        fprintf(stderr, "Compile error in fragment shader.\n");
        exit(1);
    }
    
    GLuint gl2Program = glCreateProgram();
    
    glAttachShader(gl2Program, vertShader);
    glAttachShader(gl2Program, fragShader);
    
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    glLinkProgram(gl2Program);
    glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
    GLSL::printProgramInfoLog(gl2Program);
    if (linked == GL_FALSE) {
        fprintf(stderr, "Link error.\n");
        exit(1);
    }
    
    return gl2Program;
}

static void init(void)
{
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    
    pass1 = loadShader("shader.vert","shader.frag");
    pass2 = loadShader("shader2.vert","shader2.frag");
    
    _time = glGetUniformLocation(pass1, "time");
    _resolution = glGetUniformLocation(pass1, "resolution");
    _time2 = glGetUniformLocation(pass2, "time");
    _resolution2 = glGetUniformLocation(pass2, "resolution");
    _tex = glGetUniformLocation(pass2, "tex");
    
    // color buffer
    glGenTextures(1, &cb);
    glBindTexture(GL_TEXTURE_2D, cb);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTH, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // depth buffer
    glGenRenderbuffersEXT(1, &rb);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    
    // FBO
    glGenFramebuffersEXT(1, &fb);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    
    // combine FBO to color buffer
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb, 0);
    
    // combine FBO to depth buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
    
    // reset FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

static void idle(void)
{
    glutPostRedisplay();
}

static const GLenum bufs[] = {
    GL_COLOR_ATTACHMENT0_EXT
};

static void display(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLdouble)width / (GLdouble)height, 1.0, 10.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 2.8, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glViewport(0, 0, width, height);
    
    // combine FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    
    // set up render target
    glDrawBuffers(sizeof bufs / sizeof bufs[0], bufs);
    
    // pass1
    glUseProgram(pass1);
    float elapsedTime = (float)glutGet(GLUT_ELAPSED_TIME) / CYCLE;
    glUniform1f(_time, elapsedTime);
    glUniform2f(_resolution, width, height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(-1.0, -1.0);
    glVertex2d( 1.0, -1.0);
    glVertex2d( 1.0,  1.0);
    glVertex2d(-1.0,  1.0);
    glEnd();
    
    // backup render target
    glDrawBuffer(GL_FRONT);
    
    // reset FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    glViewport(0, 0, width, height);
    
    // activate texturemap
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cb);
    
    // pass2
    glUseProgram(pass2);
    
    glUniform1i(_tex, 0);
    glUniform1f(_time2, elapsedTime);
    glUniform2f(_resolution2, width, height);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(-1.0, -1.0);
    glVertex2d( 1.0, -1.0);
    glVertex2d( 1.0,  1.0);
    glVertex2d(-1.0,  1.0);
    glEnd();
    
    glUseProgram(0);
    glFlush();
}

static void resize(int w, int h)
{
    width = w;
    height = h;
}

int main(int argc, char *argv[])
{
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(FBOWIDTH, FBOHEIGHT);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    init();
    glutMainLoop();
    return 0;
}
