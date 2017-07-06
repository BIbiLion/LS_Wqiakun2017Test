#include <stdio.h>
#include <algorithm>
#include <QCoreApplication>

#include "LoadOBJ.h"

///////////////////////////////////////////////////////////////////////////////
//!
//! OBJ mode loader
//! 2014-11-15
//! Author:ZhangJG
//! Currently, we only support the file without normal
//! that is to say, the face type should has type like "f %d/%d %d/%d %d/%d"
//////////////////////////////////////////////////////////////////////////////

#define MAX_LINE   255

cLoadOBJ::cLoadOBJ()
{
    m_FilePointer = NULL;
    // get the app path
    memcpy(g_sMediaPath,QCoreApplication::applicationDirPath().toStdString().data(),255);
}

bool cLoadOBJ::ImportModel(cObjModel *pModel, const char *strFileName)
{
    // Total file name
    // Including the path
    char stmp[512] = {0};
    // match the path and the filename
    strcpy(stmp,g_sMediaPath);
    strcat(stmp,"/models/");
    strcat(stmp,strFileName);

    //m_FilePointer = fopen(stmp, "rb");
    // open the file
    #if defined WIN32
        fopen_s(&m_FilePointer,stmp, "rb");
    #elif defined unix
        m_FilePointer = fopen(stmp,"rb");
    #endif

    // ensure the point
    if(!m_FilePointer){
        qDebug("cannot find: %s!", strFileName);
        return false;
    }
//    else
//        qDebug("Read Model Success!");

    m_nCurObjectCount = 0;
    m_nCurMaterialCount = 0;

    //parse .obj file
    ProcessFileInfo(pModel);

    // open the closed file
    fclose(m_FilePointer);
    m_VertPos.clear();
    m_VertNormal.clear();
    m_VertTexture.clear();
    m_objVertexMap.clear();

    // Cmpute normal for all object
    for(unsigned int i=0; i< pModel->vObjectArray.size(); i++){
        ComputeNormal(&pModel->vObjectArray[i]);
    }

    return true;
}

void cLoadOBJ::ProcessFileInfo(cObjModel *pModel)
{
    // match the path and filename
    char stmp[MAX_LINE] = {0};
    strcpy(stmp,g_sMediaPath);
    strcat(stmp,"/models/");

    // Markbale
    char chKeyword = 0;
    char strBuff[MAX_LINE]	= {0};
    int numline = 0;
    while(EOF != fscanf(m_FilePointer,"%s",strBuff)){
        numline++;
        // get the first char of the object
        chKeyword = strBuff[0];
        switch (chKeyword) {
        // Material
        case 'm':       //read mtllib name
            if(0 == strcmp(strBuff,"mtllib")){
                // Read Material file name
                fscanf(m_FilePointer,"%s",strBuff);
                strcat(stmp,strBuff);
                ProcessMtlFileInfo(pModel,stmp);
            }
            // read remaining lines
            fgets(strBuff, MAX_LINE, m_FilePointer);
            break;
        // use maetrial, regard as an new object
        case 'u':
            if(0 == strcmp(strBuff,"usemtl")){
                cObjObject newObj;
                // read material name
                fscanf(m_FilePointer,"%s",strBuff);
                //GetMtlTexture(&m_ModelObj,strBuff,newObj.strTexName);
                //memcpy(newObj.strTexName,GetMtlTexture(&m_ModelObj,strBuff),128);
                newObj.nMaterialID = FinMtlID(pModel,strBuff);
                pModel->vObjectArray.push_back(newObj);

                //qDebug("Texture:%s",newObj.strTexName);
                //qDebug("id:%s,%d",strBuff,newObj.nMaterialID);
                ++m_nCurObjectCount;
            }
            fgets(strBuff, MAX_LINE, m_FilePointer);
            break;
        case 'v':   // read vertex information
            // position// texture
            ProcessVertexInfo(strBuff[1]);
            fgets(strBuff, MAX_LINE, m_FilePointer);
            break;
        case 'f':   // read face information
            // create an object without material
            if(0 == m_nCurObjectCount){
                cObjObject newObject;
                pModel->vObjectArray.push_back(newObject);
                ++m_nCurObjectCount;
            }
            ProcessFaceInfo(pModel);
            // innner loop have already readed!
            //fgets(strBuff, MAX_LINE, m_FilePointer);
            break;
        default:    // default read next line
            fgets(strBuff, MAX_LINE, m_FilePointer);
            break;
        }   // switch
    }   // while

//    for(unsigned int i=0; i<m_ModelObj.vObjectArray.size(); i++){
//        tobjObject *pCurObj = &pModel->vObjectArray[m_nCurObjectCount-1];
//        qDebug("VertexSize:%d, TextureSize:%d, FaceIndices:%d",
//               pCurObj->VertPos.size(),pCurObj->VertTexture.size(),
//               pCurObj->vFaceIndices.size());
//        }

}

void cLoadOBJ::ProcessVertexInfo(char chKeyWord)
{
    // Vertex
    if('\0' == chKeyWord){
        float a,b,c;
        fscanf(m_FilePointer,"%f %f %f",&a, &b, &c);

        // push vertex position into vector
        m_VertPos.push_back(QVector3D(a,b,c));
    }
    else if('t' == chKeyWord){
        float a,b;
        fscanf(m_FilePointer,"%f %f",&a, &b);

        //push texture into vector
        m_VertTexture.push_back(QVector2D(a,b));
    }
}

void cLoadOBJ::ProcessFaceInfo(cObjModel *pModel)
{
    //char strBuff[MAX_LINE] = {0};
    unsigned int vIdx = 0, tIdx = 0;

    // No object now
    if(m_nCurObjectCount <= 0 ){
        return;
    }

    cObjObject *pCurObj = &pModel->vObjectArray[m_nCurObjectCount-1];

    //fscanf(m_FilePointer,"%s",strBuff);
    // only handle 1/2 type
    //if(3 == strlen(strBuff)){
    if(2 == fscanf(m_FilePointer,"%d/%d",&vIdx,&tIdx)){
        //sscanf(strBuff,"%d/%d",&vIdx,&tIdx);
        do{
            std::map<unsigned short, unsigned short>::iterator pFindPos
                    = m_objVertexMap.find(vIdx-1);

            // Cureent vertex has existed
            if(m_objVertexMap.end() != pFindPos){
                pCurObj->vFaceIndices.push_back(pFindPos->second);
            }
            else{
                pCurObj->VertPos.push_back(m_VertPos[vIdx-1]);
                pCurObj->VertTexture.push_back(m_VertTexture[tIdx-1]);
                pCurObj->vFaceIndices.push_back(pCurObj->VertPos.size()-1);

                m_objVertexMap.insert(std::pair<unsigned short, unsigned short>(vIdx-1,pCurObj->VertPos.size()-1));
            }

        }while(2 == fscanf(m_FilePointer,"%d/%d",&vIdx,&tIdx));
    }
}

int cLoadOBJ::FinMtlID(cObjModel *pModel, char *szMtlname)
{
    for(GLuint i=0; i< pModel->vMaterialArray.size();i++){
        if(0 == strcmp(pModel->vMaterialArray[i].strMatName,szMtlname)){
            return i;
        }
    }
    return -1;
}

void cLoadOBJ::ProcessMtlFileInfo(cObjModel *pModel, char *MtlFileName)
{
    FILE *pMTLFilePointer = NULL;
    //pMTLFilePointer = fopen(MtlFileName, "rb");

    // Open file
    #if defined WIN32
        fopen_s(&pMTLFilePointer,MtlFileName, "rb");
    #elif defined unix
        pMTLFilePointer = fopen(MtlFileName,"rb");
    #endif

    if(!pMTLFilePointer){
        qDebug("Con not find: %s!", MtlFileName);
        return;
    }
//    else
//        qDebug("Read Material Success!");

    char chKeyword = 0;
    char strBuff[MAX_LINE] = {0};

    // read data
    while(EOF != fscanf(pMTLFilePointer,"%s",strBuff)){;
        chKeyword = strBuff[0];
        switch (chKeyword) {
        case 'n':
            if(0 == strcmp(strBuff,"newmtl")){
                fscanf(pMTLFilePointer,"%s",strBuff);

                // New Material
                cObjMaterial newMaterial;
                newMaterial.nMaterialID = m_nCurObjectCount;
                // Clear the data space
                memset(newMaterial.strMatName,0,128);
                memcpy(newMaterial.strMatName,strBuff,strlen(strBuff)*sizeof(char));
                newMaterial.strMatName[strlen(strBuff) + 1] = '\0';
                pModel->vMaterialArray.push_back(newMaterial);
                ++m_nCurMaterialCount;
            }   // if
            fgets(strBuff,MAX_LINE,pMTLFilePointer);
            break;

        // mainly texture
        case 'm':
            if(m_nCurMaterialCount > 0 && 'a' == strBuff[1] && 'p' == strBuff[2]){
                chKeyword = strBuff[5];
                if('d' == chKeyword || 's' == chKeyword){
                    char chEd = 0;
                    do{
                        fscanf(pMTLFilePointer,"%s",strBuff);
                        chEd = fgetc(pMTLFilePointer);
                    }while(!strchr(strBuff,'.') && '\r' != chEd && '\n' != chEd);
                    // Store the texture name
                    //memset(pModel->vMaterialArray[m_nCurMaterialCount-1].strTexName,0,128);
                    memcpy(pModel->vMaterialArray[m_nCurMaterialCount-1].strTexName,strBuff,
                           strlen(strBuff)*sizeof(char));
                    pModel->vMaterialArray[m_nCurMaterialCount-1].strTexName[strlen(strBuff)+1] = '\0';
                    //qDebug("Texture:%s",strBuff);
                } //'k'
            }// m_nCurrent
            fgets(strBuff,MAX_LINE,pMTLFilePointer);
            break;
        default:
            fgets(strBuff,MAX_LINE,pMTLFilePointer);
            break;
        }
    }
    // Close FILE
    fclose(pMTLFilePointer);
}

// compute obj normal
void cLoadOBJ::ComputeNormal(cObjObject *pObj)
{
    if(pObj->VertPos.empty() || pObj->vFaceIndices.empty())
        return;

    QVector3D vVector1, vVector2, vPoly[3];

    // acllocate memory
    QVector3D *pTempNormals = new QVector3D[pObj->vFaceIndices.size() / 3];
    // assign 0 for all
    memset(pTempNormals,0,sizeof(QVector3D) * pObj->vFaceIndices.size() / 3);

    //qDebug("size:%d",pObj->vFaceIndices.size());
    // iterate all faces
    for(unsigned int i=0; i< pObj->vFaceIndices.size(); i+=3){
        vPoly[0] = pObj->VertPos[pObj->vFaceIndices[i]];
        vPoly[1] = pObj->VertPos[pObj->vFaceIndices[i + 1]];
        vPoly[2] = pObj->VertPos[pObj->vFaceIndices[i + 2]];

        // computer face normal
        vVector1 = vPoly[0] - vPoly[2];
        vVector2 = vPoly[2] - vPoly[1];
        pTempNormals[i / 3] = QVector3D::crossProduct(vVector1,vVector2);
    }

    // computer vertex normal
    QVector3D vSum(0,0,0);
    int shared = 0;

    // iterate all vertex
    for(unsigned int i=0; i<pObj->VertPos.size(); i++){
        shared = 0;
        vSum.setX(0); vSum.setY(0); vSum.setZ(0);

        for(unsigned int j=0; j< pObj->vFaceIndices.size(); j+=3){
            if(pObj->vFaceIndices[j  ] == i ||
               pObj->vFaceIndices[j+1] == i ||
               pObj->vFaceIndices[j+2] == i){
                vSum += pTempNormals[j / 3];
                shared++;
            } // if
        }   //for indices

        QVector3D normal;
        if(shared)
            normal = (vSum / GLfloat(-shared));
        normal.normalize();
        pObj->VertNormal.push_back(normal);

    }// for pos

    delete [] pTempNormals;
}
