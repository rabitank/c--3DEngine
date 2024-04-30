#include "vertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray(const float* verts, unsigned int vertsNum,
        const unsigned int* indices , unsigned int indicesNum)
    :m_vertexNum(vertsNum),
    m_indicesNum(indicesNum)
{
    // Create & use VAO
    glGenVertexArrays(1,&m_vaoID);
    glBindVertexArray(m_vaoID);

    /// Gen VertexBUffer Data. GL_ARRAY_BUFFER:GEnum <--> VertexBuffer 
    /// TODO: update when vertex data format change;
    glGenBuffers(1,&m_vertsBufID);
    glBindBuffer(GL_ARRAY_BUFFER,m_vertsBufID);
    glBufferData(GL_ARRAY_BUFFER,vertsNum*8*sizeof(float),verts,GL_STATIC_DRAW);

    /// Gen IndeicesBUffer Data. GL_ELEMENT_ARRAY_BUFFER:GEnum <--> indicesBuffer 
    glGenBuffers(1,&m_indicesBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indicesBufID);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,indicesNum * sizeof(unsigned int) 
        ,indices,GL_STATIC_DRAW
    );

    //glBindBuffer(GL_ARRAY_BUFFER,0);

    /// Enable & Set Attribute
    /// TODO: figureout the VAO VBO VertexAttribut
    /// TODO: update attribut when changed;
    /// view: https://blog.csdn.net/weixin_37459951/article/details/96433508
    glEnableVertexArrayAttrib(m_vaoID,0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),0);

    glEnableVertexArrayAttrib(m_vaoID,1);
    glVertexAttribPointer(
        1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float))
    );
    
    glEnableVertexArrayAttrib(m_vaoID,2);
    glVertexAttribPointer(
        2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),
        reinterpret_cast<void*>(6 * sizeof(float))
    );



}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1,&m_indicesBufID);
    glDeleteBuffers(1,&m_vertsBufID);
    glDeleteVertexArrays(1,&m_vaoID);

}

void VertexArray::SetActive()
{
    glBindVertexArray(m_vaoID);
}

void VertexArray::SetUnActive()
{
    glBindVertexArray(0);
}


