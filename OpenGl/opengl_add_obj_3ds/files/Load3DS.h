// Load3DS.h: interface for the CLoad3DS class.
//
//     Original Author:
//     沈阳蓝雨视景科技
//     http://www.bvrain.com
//     bvrain@163.com  chglei@163.com
//     13998383976
//
// Modified:ZhangJG
// Modified Date:2014-11-10
//
//////////////////////////////////////////////////////////////////////

#ifndef _LOAD_3DS_H
#define _LOAD_3DS_H

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

//STL
#include <vector>
#include <stdio.h>
using namespace std;

// THREE DIMENSIONAL VERTEX
#include <QVector>
#include <QVector2D>
#include <QVector3D>

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002				// This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				// This stored the texture info
#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME       0xA000				// This holds the material name
#define MATDIFFUSE    0xA020				// This holds the color of the object/material
#define MATMAP        0xA200				// This is a header for a new material
#define MATMAPFILE    0xA300				// This holds the file name of the texture

#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			// The objects vertices
#define OBJECT_FACES		0x4120			// The objects faces
#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
#define OBJECT_UV			0x4140			// The UV texture coordinates


// This class handles all of the loading code
class CLoad3DS
{
public:
    // Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
    struct tIndices {

        unsigned short a, b, c, bVisible;		// This will hold point1, 2, and 3 index's into the vertex array plus a visible flag
    };

    // This holds the chunk info
    struct tChunk
    {
        unsigned short int ID;					// The chunk's ID
        unsigned int length;					// The length of the chunk
        unsigned int bytesRead;					// The amount of bytes read within that chunk
    };

    // This is our face structure.  This is is used for indexing into the vertex
    // and texture coordinate arrays.  From this information we know which vertices
    // from our vertex array go to which face, along with the correct texture coordinates.
    struct tFace
    {
        int vertIndex[3];			// indicies for the verts that make up this triangle
        int coordIndex[3];			// indicies for the tex coords to texture this face
    };

    // This holds the information for a material.  It may be a texture map of a color.
    // Some of these are not used, but I left them because you will want to eventually
    // read in the UV tile ratio and the UV tile offset for some models.
    struct tMaterialInfo
    {
        char  strName[255];			// The texture name
        char  strFile[255];			// The texture file name (If this is set it's a texture map)
        GLbyte  color[3];			// The color of the object (R, G, B)
        int   texureId;				// the texture ID
        float uTile;				// u tiling of texture  (Currently not used)
        float vTile;				// v tiling of texture	(Currently not used)
        float uOffset;			    // u offset of texture	(Currently not used)
        float vOffset;				// v offset of texture	(Currently not used)
    } ;

    // This holds all the information for our model/scene.
    // You should eventually turn into a robust class that
    // has loading/drawing/querying functions like:
    // LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
    struct t3DObject
    {
    public:
        int  numOfVerts;			// The number of verts in the model
        int  numOfFaces;			// The number of faces in the model
        int  numTexVertex;			// The number of texture coordinates
        int  numOfMaterials;		// the number of material
        int  materialID;			// The texture ID to use, which is the index into our texture array
        bool bHasTexture;			// This is TRUE if there is a texture map for this object
        char strName[255];			// The name of the object
        std::vector<QVector3D>      VertPos;          // Vertex position
        std::vector<QVector3D>      VertNormal;       // vertex normaltes
        std::vector<QVector2D>      VertTexture;      // vertex texture
        std::vector<unsigned short> Indexes;          // vertex index
        tFace *pFaces;				// The faces information of the object
    };

    // This holds our model information.  This should also turn into a robust class.
    // We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
    struct t3DModel
    {
        int numOfObjects;					// The number of objects in the model
        int numOfMaterials;					// The number of materials for the model
        // rename
        QVector<tMaterialInfo> vctMaterials;	// The list of material information (Textures and colors)
        QVector<t3DObject> vctObjects;			// The object list for our model
    };

public:
    // constructor
    CLoad3DS();
    // desconstructor
    ~CLoad3DS();
    // load 3DS file
    bool Import3DS(t3DModel *pModel, char *strFileName);

private:
    // This reads in a string and saves it in the char array passed in
    int GetString(char *);

    // This reads the next chunk
    void ReadChunk(tChunk *);

    // This reads the next large chunk
    void ProcessNextChunk(t3DModel *pModel, tChunk *);

    // This reads the object chunks
    void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);

    // This reads the material chunks
    void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);

    // This reads the RGB value for the object's color
    void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);

    // This reads the objects vertices
    void ReadVertices(t3DObject *pObject, tChunk *);

    // This reads the objects face information
    void ReadVertexIndices(t3DObject *pObject, tChunk *);

    // This reads the texture coodinates of the object
    void ReadUVCoordinates(t3DObject *pObject, tChunk *);

    // This reads in the material name assigned to the object and sets the materialID
    void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);

    // This computes the vertex normals for the object (used for lighting)
    void ComputeNormals(t3DModel *pModel);

    // This frees memory and closes the file
    void CleanUp();

    // The file pointer
    FILE *m_FilePointer;
};


#endif // !defined(AFX_LOAD3DS_H__69E7E7AC_7A07_4479_9687_AC19CED0E3CF__INCLUDED_)
