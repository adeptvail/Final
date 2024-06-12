#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//create vertex shader object and its reference :3
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach the source code to the object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//compile the source code into machine code bc opengl is a dumb ass bitch that is more retarded than me holy fuck how 
	//the fuck can it not do this but it has a
	//function so it should just  do this but it wont because it is a dumb ass bitch
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	//create fragment shader object and its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//attach fragment shader source code to the object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//compile fragment shader into source code
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	//create a shaderprogram object
	ID = glCreateProgram();
	//attach the fragment shader to the program
	glAttachShader(ID, vertexShader);
	//attach the vertex shader to the program
	glAttachShader(ID, fragmentShader);

	//link all of the saders together into the shader program
	glLinkProgram(ID);

	//delete the now obsolete shaders
	//dumb fucks
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

//Compiles errors for shader if there are any
void Shader::CompileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1028];
	if (type != "PROGRAM") {
		//gives hasCompiled compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "failed to compile the shader" << type << "\n@" << infoLog << "\n";
		}
	}
}
