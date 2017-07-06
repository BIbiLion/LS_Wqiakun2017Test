// 3DSModel.cpp: implementation of the C3DSModel class.
//
// Date:2014-11-10
//
// Author:ZhangJG
//
//////////////////////////////////////////////////////////////////////
#include <QImage>
#include <QGLWidget>
#include "MeshModel.h"
#include <QCoreApplication>

#define VERTEX_DATA     0
#define NORMAL_DATA     1
#define TEXTURE_DATA    2
#define INDEX_DATA      3

int mWindowWidth  = 800;
int mWindowHeight = 600;

enum GLT_SHADER_ATTRIBUTE { GLT_ATTRIBUTE_VERTEX = 0, GLT_ATTRIBUTE_COLOR, GLT_ATTRIBUTE_NORMAL,
                            GLT_ATTRIBUTE_TEXTURE0, GLT_ATTRIBUTE_TEXTURE1, GLT_ATTRIBUTE_TEXTURE2,
                            GLT_ATTRIBUTE_TEXTURE3, GLT_ATTRIBUTE_LAST};

//GLT_SHADER_TEXTURE_MODULATE
// Just put the texture on the polygons, but multiply by the color (as a unifomr)
static const char *szTextureModulateVP ="uniform mat4 mvpMatrix;"
                                        "attribute vec4 vVertex;"
                                        "attribute vec2 vTexCoords;"
                                        "varying vec2 vTex;"
                                        "void main(void) "
                                        "{ vTex = vTexCoords;"
                                        " gl_Position = mvpMatrix * vVertex; "
                                        "}";

static const char *szTextureModulateFP =
#ifdef OPENGL_ES
                                        "precision mediump float;"
#endif
                                        "varying vec2 vTex;"
                                        "uniform sampler2D textureUnit0;"
                                        "uniform vec4 vColor;"
                                        "void main(void) "
                                        "{ gl_FragColor = vColor * texture2D(textureUnit0, vTex); "
                                        "}";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cModelObject::cModelObject()
{
    initializeOpenGLFunctions();
    //pIndices.clear();
}

cModelObject::~cModelObject()
{
    // Delete buffer objects
    glDeleteBuffers(4, bufferObjects);

#ifndef OPENGL_ES
    glDeleteVertexArrays(1, &vertexArrayBufferObject);
#endif
}

void cModelObject::GenerateVABO()
{
    #ifndef OPENGL_ES
    // Create the master vertex array object
    glGenVertexArrays(1, &vertexArrayBufferObject);
    glBindVertexArray(vertexArrayBufferObject);
    #endif

    // Create the buffer objects
    glGenBuffers(4, bufferObjects);

    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*VertPos->size(), (void *)VertPos->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*VertNorms->size(), (void *)VertNorms->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*VertTexts->size(), (void *)VertTexts->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*faceIndexes->size(), (void *)faceIndexes->data(), GL_STATIC_DRAW);

    // Done
    #ifndef OPENGL_ES
    glBindVertexArray(0);
    #endif
}

void cModelObject::Draw()
{
#ifndef OPENGL_ES
    glBindVertexArray(vertexArrayBufferObject);
#else
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
#endif

    glDrawElements(GL_TRIANGLES, faceIndexes->size(), GL_UNSIGNED_SHORT, 0);

#ifndef OPENGL_ES
    // Unbind to anybody
    glBindVertexArray(0);
#else
    glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
#endif
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cMeshModel::cMeshModel(){
    initDefault();
}

cMeshModel::cMeshModel(QString strFileName)
{
    initDefault();

    // LOAD Model Mesh
    if(strFileName.endsWith(QString("3ds")))
        Load3DSFromFile(strFileName.toStdString().data());
    else if(strFileName.endsWith(QString("obj")))
        LoadOBJFromFile(strFileName.toStdString().data());
}

cMeshModel::~cMeshModel()
{

    for(int i = 0; i < m_3DModel.numOfObjects; i++){
        // Free the faces, normals, vertices, and texture coordinates.
        delete [] m_3DModel.vctObjects[i].pFaces;
        m_3DModel.vctObjects[i].VertPos.clear();
        m_3DModel.vctObjects[i].Indexes.clear();
        m_3DModel.vctObjects[i].VertNormal.clear();
        m_3DModel.vctObjects[i].VertTexture.clear();
    }

    for(unsigned int i=0; i< m_ObjModel.vObjectArray.size();i++){
        m_ObjModel.vObjectArray[i].VertPos.clear();
        m_ObjModel.vObjectArray[i].VertNormal.clear();
        m_ObjModel.vObjectArray[i].VertTexture.clear();
        m_ObjModel.vObjectArray[i].vFaceIndices.clear();
    }

}

//////////////////////////////////////////////////////////////////////
// default parameters setup
//////////////////////////////////////////////////////////////////////
void cMeshModel::initDefault()
{
    m_3DModel.numOfMaterials=0;
    m_3DModel.numOfObjects=0;
    m_3DModel.vctMaterials.clear();
    m_3DModel.vctObjects.clear();

    InitializeShader();

    // get the app path
    memcpy(g_sMediaPath,QCoreApplication::applicationDirPath().toStdString().data(),255);
}

void cMeshModel::InitializeShader()
{
    m_program = new QOpenGLShaderProgram;

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,szTextureModulateVP);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,szTextureModulateFP);

    // Bind the attribute to the specific location
    // Should be called before link
    m_program->bindAttributeLocation("vVertex",GLT_ATTRIBUTE_VERTEX);
    m_program->bindAttributeLocation("vTexCoords",GLT_ATTRIBUTE_TEXTURE0);

    m_program->link();

    // Color and Matrix
    iTransform = m_program->uniformLocation("mvpMatrix");
    iColor = m_program->uniformLocation("vColor");
    iInteger = m_program->uniformLocation("textureUnit0");
}

// Draw Model
void cMeshModel::Render(QMatrix4x4 matrix)
{
    glPushMatrix();
    m_program->bind();
    QVector4D color(0.8f, 0.8f, 0.8f, 0.6f);

    //qDebug("::::::::::%d",m_currentObj.size());
    foreach (cModelObject* m_obj, m_currentObj) {
            if(m_obj->bHasTexture)
            {
                glEnable(GL_TEXTURE_2D);
                //glColor3ub(255, 255, 255);
                //glBindTexture(GL_TEXTURE_2D, ui_TextureID[6]);
                glBindTexture(GL_TEXTURE_2D, ui_TextureID[m_obj->materialID]);//g_Texture[pObject->materialID]);
            }
            else
            {
                //continue;
                glDisable(GL_TEXTURE_2D);
                glColor3ub(255, 255, 255);
            }
            m_program->setUniformValue(iTransform,matrix);
            m_program->setUniformValue(iColor,color);
            m_program->setUniformValue(iInteger,0);
            m_obj->Draw();
    }
    m_program->release();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

bool cMeshModel::CreateTexture(GLuint textureArray[], char *strFileName, int textureID)
{
    QImage tex,buf;
    // load image
    if(buf.load(strFileName)){
        tex = QGLWidget::convertToGLFormat(buf);

        glGenTextures(1, &textureArray[textureID]);
        glBindTexture(GL_TEXTURE_2D,textureArray[textureID]);

        // iimage must be power of 2
        glTexImage2D(GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        return true;
    }
    return false;
}

// Load model file
bool cMeshModel::Load3DSFromFile(const char *sfilename)
{
    char stmp[512] = {0};
    //math the path and filename
    strcpy(stmp,g_sMediaPath);
    strcat(stmp,"/models/");
    strcat(stmp,sfilename);

    //local variable
    CLoad3DS cLoad3ds;
    // load 3DS Model
    if(cLoad3ds.Import3DS(&m_3DModel, stmp)==false){
        // failed return directly
        exit(1);
        return false;
    }

    ////////////////////////////////////////////////////////////////////
    // Assign the address and other parameters to the class for VABO
    for(int i=0; i< m_3DModel.numOfObjects; i++){
        cModelObject *obj = new cModelObject;
        CLoad3DS::t3DObject *pObject = &m_3DModel.vctObjects[i];

        // Material
        obj->materialID  = pObject->materialID;
        obj->bHasTexture = pObject->bHasTexture;

        obj->VertPos      = &pObject->VertPos;
        obj->VertNorms    = &pObject->VertNormal;
        obj->VertTexts    = &pObject->VertTexture;
        obj->faceIndexes  = &pObject->Indexes;
        m_currentObj.push_back(obj);

        // Create VA\BO
        m_currentObj[i]->GenerateVABO();
    }

    //g_texture2D = new cTexture2D*[m_3DModel.numOfMaterials];
    g_Texture = new GLuint[m_3DModel.numOfMaterials];
    ui_TextureID = new GLuint[m_3DModel.numOfMaterials];
    ///////////////////////////////////////////////////////////////////
    // static index
    static int ntextID=0;

    // success read texture
    for(int i = 0; i < m_3DModel.numOfMaterials; i++)
    {
        memset(stmp,0,512);
        // Check to see if there is a file name to load in this material
        if(strlen(m_3DModel.vctMaterials[i].strFile) > 0)
        {
            strcpy(stmp,g_sMediaPath);
            strcat(stmp,"/models/");
            strcat(stmp,m_3DModel.vctMaterials[i].strFile);
            // setup matrial
            if(!CreateTexture(&g_Texture[ntextID],stmp, 0))
                qDebug("Texture:%s failed",stmp);
            //m_3DModel.vctMaterials[i].texureId = g_Texture[ntextID];
            ui_TextureID[i] = g_Texture[ntextID];

            ntextID++;

        }
        else{
            m_3DModel.vctMaterials[i].texureId = 0;
            ui_TextureID[i] = 0;
        }
    }
    return true;
}

bool cMeshModel::LoadOBJFromFile(const char *sfilename)
{
    // local variable
    cLoadOBJ cLoadObj;
    // load model
    if(cLoadObj.ImportModel(&m_ObjModel, sfilename)==false){
        // failed return directly
        exit(1);
        return false;
    }

    ////////////////////////////////////////////////////////////////////
    // Assign the address and other parameters to the class for VABO
    for(unsigned int i=0; i< m_ObjModel.vObjectArray.size(); i++){
        cModelObject *obj = new cModelObject;
        cObjObject *pObject = &m_ObjModel.vObjectArray[i];

        // Material
        obj->materialID   = pObject->nMaterialID;
        //int hh = strlen(m_ObjModel.vMaterialArray[pObject->nMaterialID].strTexName);
        obj->bHasTexture  = strlen(m_ObjModel.vMaterialArray[pObject->nMaterialID].strTexName) > 1;

        obj->VertPos      = &pObject->VertPos;
        obj->VertNorms    = &pObject->VertNormal;
        obj->VertTexts    = &pObject->VertTexture;
        obj->faceIndexes  = &pObject->vFaceIndices;
        m_currentObj.push_back(obj);

        //qDebug("%d,v=%d,t=%d",i,obj->VertPos->size(),obj->VertTexts->size());
        // Create VA\BO
        m_currentObj[i]->GenerateVABO();
    }

    //g_texture2D = new cTexture2D*[m_3DModel.numOfMaterials];
    g_Texture = new GLuint[m_ObjModel.vMaterialArray.size()];
    ui_TextureID = new GLuint[m_ObjModel.vMaterialArray.size()];

    //static index
    static int ntextID=0;
    char stmp[512] = {0};

    // success, then read material
    for(unsigned int i = 0; i < m_ObjModel.vMaterialArray.size(); i++)
    {
        memset(stmp,0,512);
        // Check to see if there is a file name to load in this material
        if(strlen(m_ObjModel.vMaterialArray[i].strTexName) > 0)
        {
            strcpy(stmp,g_sMediaPath);
            strcat(stmp,"/models/");
            strcat(stmp,m_ObjModel.vMaterialArray[i].strTexName);

            // PUt texture name into string
            QString strName(m_ObjModel.vMaterialArray[i].strTexName);
            //qDebug("---:%s",strName.toStdString().data());
            //const char *ch=str.toStdString().data();
            //int iii=0;
            std::map<QString, unsigned int>::iterator pFindPos= m_MapTexture.find(strName);
            if(pFindPos != m_MapTexture.end()){
                ui_TextureID[i] = g_Texture[pFindPos->second];
            }
            else{
                // setup matarial index
                if(!CreateTexture(&g_Texture[ntextID],stmp, 0))
                    qDebug("Texture:%s, failed",stmp);
                //m_3DModel.vctMaterials[i].texureId = g_Texture[ntextID];
                ui_TextureID[i] = g_Texture[ntextID];

                //qDebug("Texture:%s",stmp);

                // Save Current Texture
                m_MapTexture.insert(std::pair<QString,unsigned int>(strName,ntextID));

                ntextID++;
            }
        }
        else{
            //m_3DModel.vctMaterials[i].texureId = 0;
            ui_TextureID[i] = 0;
        }
    }

    return true;
}
