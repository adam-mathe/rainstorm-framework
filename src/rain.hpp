#pragma once

#include "main.hpp"
#include "math.hpp"

#include <stdio.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.141592653589

namespace rain
{
    struct Mat {
        float m[16] = {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };
    };

    rain::Mat matMultiply(const rain::Mat& one, const rain::Mat& two) {
        rain::Mat res;

        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                for(int k = 0; k < 4; ++k) {
                    res.m[j + 4 * i] += (one.m[k + 4 * i] * two.m[j + 4 * k]);
                }
            }
        }

        return res;
    }

    rain::Mat identity() {
        rain::Mat mat;

        mat.m[0] = 1.0f;
        mat.m[5] = 1.0f;
        mat.m[10] = 1.0f;
        mat.m[15] = 1.0f;

        return mat;
    }

    void ortho(rain::Mat& mat, float l, float r, float t, float b, float n, float f) {
        rain::Mat scale = rain::identity();

        scale.m[0] = 1.0f / r;
        scale.m[5] = 1.0f / t;
        scale.m[10] = 2.0f / (f - n);

        rain::Mat trans = rain::identity();

        trans.m[11] = (n + f) / 2.0f;

        mat = rain::matMultiply(scale, trans);
    }

    void perspective(rain::Mat& mat, float l, float r, float t, float b, float n, float f) {
        
    }

    void translate(rain::Mat& mat, vector3 pos) {
        rain::Mat t = rain::identity();

        t.m[3]  = pos.x; 
        t.m[7]  = pos.y;
        t.m[11] = pos.z;

        mat = rain::matMultiply(mat, t);
    }

    void scale(rain::Mat& mat, vector3 scale) {
        rain::Mat s = rain::identity();

        s.m[0] = scale.x;
        s.m[5] = scale.y;
        s.m[10] = scale.z;

        mat = rain::matMultiply(mat, s);
    }

    void rotate(rain::Mat& mat, float angle) {
        rain::Mat r = rain::identity();

        float rads = angle * (PI / 180);

        r.m[0] =  std::cos(rads);
        r.m[1] =  std::sin(rads);
        r.m[4] = -std::sin(rads);
        r.m[5] =  std::cos(rads);

        mat = rain::matMultiply(mat, r);
    }

    void setConstant(unsigned int program, const char* location, rain::Mat& data)
    {
        unsigned int uniform_location = glGetUniformLocation(program, location);
        glUniformMatrix4fv(uniform_location, 1, GL_TRUE, data.m);
    }

    unsigned int shaderProgram(const char *vertex_shader_source, const char *fragment_shader_source) 
    {
        unsigned int program, vertex_shader, fragment_shader;

        //vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        int success;

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char error_message[1024];

            glGetShaderInfoLog(fragment_shader, sizeof(error_message), NULL, error_message);

            printf("FRAGMENT ERROR %s \n", error_message);
        }

        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char error_message[1024];

            glGetShaderInfoLog(vertex_shader, sizeof(error_message), NULL, error_message);

            printf("VERTEX ERROR %s \n", error_message);
        }

        program = glCreateProgram();

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success)
        {
            char error_message[1024];

            glGetProgramInfoLog(program, sizeof(error_message), NULL, error_message);

            printf("PROGRAM ERROR %s \n", error_message);
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);  

        return program;
    }
}