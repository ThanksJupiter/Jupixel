#pragma once

#include <string>

#include <glad/glad.h>

std::string read_file_as_string(const std::string& filepath);
GLuint compile_shader_program_from_text_files(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
GLuint compile_shader(GLenum type, const std::string& source);
