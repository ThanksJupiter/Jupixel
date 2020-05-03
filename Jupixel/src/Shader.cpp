#include "jppch.h"

#include "Shader.h"

#include <fstream>

std::string read_file_as_string(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize((size_t)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		printf("Could not open file '{0}'", filepath);
	}

	return result;
}

GLuint compile_shader_program_from_text_files(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexSource = read_file_as_string(vertexShaderPath);
	std::string fragmentSource = read_file_as_string(fragmentShaderPath);

	GLuint program = glCreateProgram();
	int glShaderIDIndex = 0;

	GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
	glAttachShader(program, vertexShader);
	GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		printf("%s", infoLog.data());
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

GLuint compile_shader(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);

	const GLchar* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, 0);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(shader);

		printf("%s", infoLog.data());
	}

	return shader;
}
