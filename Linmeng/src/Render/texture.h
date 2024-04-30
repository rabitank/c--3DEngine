#pragma once
#include <string>


class Texture
{

public:
    Texture()   {};
    ~Texture()  {};

    bool Load(const std::string& filePath);
    void Unload();

    void SetActive();

    int GetWidth() const {return m_width;};
    int GetHeight() const {return m_height;};

private:
    unsigned int m_textureID    {0};
    int m_width                 {0};
    int m_height                {0};
};