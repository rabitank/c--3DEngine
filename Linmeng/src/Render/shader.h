#pragma once

#include "glad/glad.h"
#include <string>
#include "Umath.h"


/**
 * @brief       
 * @attention: about shader glsl write  
 * view https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/05%20Shaders/
 * about layout location view https://bbs.huaweicloud.com/blogs/313360#
 * 
 */

class Shader
{
public:
    Shader(){};
    ~Shader(){};

    bool Load(const std::string& vertSPath ,const std::string& fragSPath );
    void Unload();

    void SetActive();

    void SetMatrixUniform(const char* name,const Matrix4& matrix );
    void SetVector3Uniform(const char* name,const Vector3& vec3 );
    void SetfloatUniform(const char* name,float val );
private:

    bool CompileShader(
        const std::string& filePath , 
        GLenum shaderType ,
        GLuint& outShader );

    /// @brief Query Shader Compile state
    bool IsCompiled(GLuint shader);

    /// @brief Query final Shader Link State
    bool IsVaildProgram();


private: 

    GLuint m_vertshader   {0}  ;
    GLuint m_fragshader   {0}  ;
    GLuint m_ShaderProgram{0}  ;

};

