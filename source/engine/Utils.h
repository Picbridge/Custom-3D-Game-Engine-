#pragma once

class Utils
{
public:
	//@brief Extracts the class name from datatype
	//@return string class name
	template<typename DataType>
	static std::string GetClassName()
	{
		std::string typeName = typeid(DataType).name();
		std::string prefix = "class ";
		size_t prefixPosition = typeName.find(prefix);
		if (prefixPosition == 0) {
			return typeName.substr(prefix.length());
		}
		return typeName;
	}

	//@brief Utility function for checking shader compilation/linking errors.
	static void GetGLError()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::string errorString;
			switch (error)
			{
			case GL_INVALID_ENUM:
				errorString = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errorString = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errorString = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				errorString = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				errorString = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				errorString = "GL_OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			default:
				errorString = "UNKNOWN";
				break;
			}
			std::cout << "OpenGL Error: " << errorString << std::endl;
		}
	}
};
