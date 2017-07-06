#ifndef OBJLOADER_H
#define OBJLOADER_H

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

#include <map>
#include <vector>
#include <stdio.h>
#include <QVector2D>
#include <QVector3D>

struct cObjObject{
    GLuint nMaterialID;
    //GLchar strTexName[128];
    std::vector<QVector3D>  VertPos;            // Vertex position
    std::vector<QVector3D>  VertNormal;         // vertex normaltes
    std::vector<QVector2D>  VertTexture;        // vertex texture
    std::vector<unsigned short> vFaceIndices;   // Face Indices
};


struct cObjMaterial{
    GLuint nMaterialID;
    char strMatName[24];
    char strTexName[128];
    cObjMaterial(){
        nMaterialID = -1;
    }
};

struct cObjModel{
    std::vector<cObjObject> vObjectArray;
    std::vector<cObjMaterial>vMaterialArray;
};

class cLoadOBJ
{
public:
    cLoadOBJ();

    bool ImportModel(cObjModel *pModel, const char *strFileName);
    void ProcessVertexInfo(char chKey);
    void ComputeNormal(cObjObject *pObj);
    void ProcessFaceInfo(cObjModel *pModel);
    void ProcessFileInfo(cObjModel *pModel);
    //void GetMtlTexture(tObjModel* pModel, char *szMtlname, char *TexName);
    int  FinMtlID(cObjModel* pModel, char *szMtlname);
    void ProcessMtlFileInfo(cObjModel *pModel, char *MtlFileName);

    //////////////////////////////////////////////////////////////////////
    /// \brief m_ObjModel
    ///
    FILE *m_FilePointer;

    // Model file path
    char g_sMediaPath[255];

    //tObjModel m_ModelObj;
    int  m_nCurObjectCount;
    int  m_nCurMaterialCount;

    // Use for initialization
    std::vector<QVector3D>  m_VertPos;          // Vertex position
    std::vector<QVector3D>  m_VertNormal;       // vertex normaltes
    std::vector<QVector2D>  m_VertTexture;      // vertex texture

    // first original index, second new index
    std::map<unsigned short, unsigned short>m_objVertexMap;
};

#endif // OBJLOADER_H
