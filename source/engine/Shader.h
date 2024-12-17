#pragma once

class Shader
{
public:

	//@brief Load the shader from the file
	//@param vertexPath : Path to the vertex shader file
	//@param fragmentPath : Path to the fragment shader file
    //@param geometryPath : Path to the geometry shader file (optional)
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	//--------------------------------
	//Shader control
	//--------------------------------

	//@brief Use the shader
	void Use();
	//@brief Unuse the shader
	void Unuse();

    GLuint GetId() const { return m_id; }

	//@brief Returns the location of the attribute found in shader file
	//@param name : Attribute name to look up
	//@return GLint Location id of the attribute
	GLint GetAttributeLocation(const std::string& name);

	//@brief Clear the uniform cache
    void ClearUniformCache();

	//--------------------------------
	//Utility uniform functions
	//--------------------------------

	//@brief Returns the location of the uniform found in shader file
	//@param name : Uniform name to look up
    GLint GetUniformLocation(const std::string& name) 
    {
        auto it = m_uniformCache.find(name);
        if (it != m_uniformCache.end()) 
            return it->second;
        
        // If not cached, query the location from OpenGL
        GLint location = glGetUniformLocation(m_id, name.c_str());
        if (location == -1) 
            std::cerr << "Warning: Uniform " << name << " not found in shader." << std::endl;
        

        // Store the location in cache and return it
        m_uniformCache[name] = location;
        return location;
    }

	//@brief Assign value to uniform in shader
	//@param name : Uniform variable name to assign value 
	//@param value : Value to assign to the name of the uniform
    template <typename T>
    void SetUniform(const std::string& name, const T& value)
    {
        GLint location = GetUniformLocation(name);
        if (location == -1) return;  // Skip if uniform not found

        // Using constexpr to differentiate between types
        if constexpr (std::is_same_v<T, bool>) {
            glUniform1i(location, static_cast<int>(value));
        }
        else if constexpr (std::is_same_v<T, int>) {
            glUniform1i(location, value);
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            glUniform1i(location, value);
        }
        else if constexpr (std::is_same_v<T, float>) {
            glUniform1f(location, value);
        }
        else if constexpr (std::is_same_v<T, glm::vec2>) {
            glUniform2fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            glUniform3fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::vec4>) {
            glUniform4fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat2>) {
            glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat3>) {
            glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat4>) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }

        Utils::GetGLError();
    }

private:
	GLuint m_id;
    std::unordered_map<std::string, GLint> m_uniformCache;
};