#include "shader.h"

#include <fstream>
#include <sstream>
#include "SDL.h"

bool Shader::Load(const std::string& vertSPath ,const std::string& fragSPath)
{
    if(
        CompileShader(
            vertSPath ,
            GL_VERTEX_SHADER ,
            m_vertshader)
        && 
        CompileShader(
            fragSPath ,
            GL_FRAGMENT_SHADER ,
            m_fragshader)
    )
    {
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram,m_vertshader);
        glAttachShader(m_ShaderProgram,m_fragshader);
        glLinkProgram(m_ShaderProgram);
    }


    if(!IsVaildProgram())
    {
        return false;
    }

    return true;

}

void Shader::Unload()
{
    glDeleteProgram(m_ShaderProgram);
    glDeleteShader(m_fragshader);
    glDeleteShader(m_vertshader);
}

void Shader::SetActive()
{
    glUseProgram(m_ShaderProgram);   
}

void Shader::SetMatrixUniform(const char* name,const Matrix4& matrix)
{
    auto pos  = glGetUniformLocation(m_ShaderProgram,name);
    
    if(pos == -1) {
        SDL_Log("shader %d didn't found uniform %s !",m_ShaderProgram ,name);
        return;
    }

    glUniformMatrix4fv(
        pos,
        1,
        GL_TRUE,  // transpose 转置. 我们用的行向量,矩阵右边.
        matrix.GetAsFloatPtr()
    );
}

void Shader::SetVector3Uniform(const char* name,const Vector3& vec3)
{
    auto pos  = glGetUniformLocation(m_ShaderProgram,name);
    if(pos == -1) {
        SDL_Log("shader %d didn't found uniform %s !",m_ShaderProgram ,name);
        return;
    }
    glUniform3fv(
        pos,
        1,
        vec3.GetAsFloatPtr()
    );
}

void Shader::SetfloatUniform(const char* name,float val)
{
    auto pos  = glGetUniformLocation(m_ShaderProgram,name);
    if(pos == -1) {
        SDL_Log("shader %d didn't found uniform %s !",m_ShaderProgram ,name);
        return;
    }
    glUniform1f(
        pos,
        val
    );
}

bool Shader::CompileShader(const std::string& filePath , 
        GLenum shaderType ,
        GLuint& outShader)
{
    std::ifstream file(filePath);
    if(file.is_open())
    {
        std::stringstream ss;
        ss<<file.rdbuf();
        std::string content = ss.str();
        const char* contentChar = content.c_str();

        outShader = glCreateShader(shaderType);
        glShaderSource(outShader, 1,&(contentChar),nullptr);
        glCompileShader(outShader);

        if(!IsCompiled(outShader))
        {
            SDL_Log("Failed to compile shader: %s",filePath.c_str());
            return false;
        }

    }
    else
    {
        SDL_Log("Failed to open shaderfile: %s",filePath.c_str());
        return false;
    }
    return true;
}

bool Shader::IsCompiled(GLuint shader)
{
    GLint states;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&states);

    if(states != GL_TRUE)
    {
        char buffer[512];
        memset(buffer,0,512);
        glGetShaderInfoLog(shader,511,nullptr,buffer);
        SDL_Log("GLSL Compile failed: %s",buffer);
        return false;
    }
    return true;

}

bool Shader::IsVaildProgram()
{
    GLint states;
    glGetProgramiv(m_ShaderProgram,GL_LINK_STATUS,&states);
    
    if(states != GL_TRUE)
    {
        char buffer[512];
        memset(buffer,0,512);
        glGetProgramInfoLog(m_ShaderProgram,511,nullptr,buffer);
        SDL_Log("GLSL Link failed: %s",buffer);
        return false;
    }

    return true;
}
