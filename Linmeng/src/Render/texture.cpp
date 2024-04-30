#include "texture.h"
#include "SDL.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texture::Load(const std::string& filePath)
{
    stbi_set_flip_vertically_on_load(1);//使之符合gl的左下坐标系.
    int channelnum;
	auto img = stbi_load(
        filePath.c_str(),
        &m_width, 
        &m_height, 
        &channelnum, 0);//未来我们会导出 width,height,bpp的
    if(img == nullptr)
    {
        SDL_Log("stb failed to load Image: %s" , filePath.c_str());
        return false;
    }
    // 生成纹理(数组)对象(句柄)
    glCreateTextures(GL_TEXTURE_2D, 1 , &m_textureID );

    GLenum internalformat = 0,dataformat =0;
    if (channelnum ==4) 
    {
        internalformat = GL_RGBA8;
        dataformat= GL_RGBA;
    }
    else if (channelnum==3)
    {
        internalformat = GL_RGB8;
        dataformat = GL_RGB;
    }
    else if (channelnum==2)
    {
        internalformat = GL_RG8;
        dataformat = GL_RG;
    }


    {
        //方案1
            // 创建纹理
            glTextureStorage2D(m_textureID, 1, internalformat, m_width, m_height); // levels:texture纹理层级数(mipmap级别,对于2D纹理必须为1) internalformat 图像 存储格式 //该API指定纹理存储格式

            // 填入数据
            glTextureSubImage2D(
                m_textureID,0,0,0,m_width,m_height,dataformat,
                GL_UNSIGNED_BYTE,img
            );
        // 方案2; -> Discard!! 
        // glTexImage2D(
        //     m_textureID,0,
        //     internalformat,m_width,m_height,0,
        //     dataformat,GL_UNSIGNED_BYTE,img);
        
    }

    /// 放大双线性过滤
    glTextureParameteri(
        m_textureID, 
        GL_TEXTURE_MIN_FILTER, 
        GL_LINEAR_MIPMAP_LINEAR);
    /// 缩小三线性过滤
    glTextureParameteri(
        m_textureID, 
        GL_TEXTURE_MAG_FILTER, 
        GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Parameteri:参数 WRAP_S:水平环绕,CLAMP:嵌入
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入


    // 要求生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    

    stbi_image_free(img);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::Unload()
{
    glDeleteTextures(1,&m_textureID);
}

void Texture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D,m_textureID);
}
