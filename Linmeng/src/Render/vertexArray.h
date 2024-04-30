#pragma once


class VertexArray
{
public:
    VertexArray(const float* verts, unsigned int vertsNum,
        const unsigned int* indices , unsigned int indicesNum );
    ~VertexArray();

    void SetActive();
    void SetUnActive();

    unsigned int GetNumOfIndices() const { return m_indicesNum;}
    unsigned int GetNumOfVerts() const { return m_vertexNum;}

private:
    /// @brief opengl handle
    unsigned int m_vaoID;
    /// @brief opengl handle
    unsigned int m_vertsBufID;
    /// @brief opengl handle
    unsigned int m_indicesBufID;

    /// @brief num of indices
    unsigned int m_indicesNum;
    /// @brief num of vertexs
    unsigned int m_vertexNum;


}
;
