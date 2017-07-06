// Load3DS.cpp: implementation of the CLoad3DS class.
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
#include "Load3DS.h"
#include <QMessageBox>

//! 3DS load support multi object
//! but, each of them must have only one texture now
//!
//! 2014-11-14
//!

// Buffer
static int gBuffer[50000] = {0};

//////////////////////////////////////////////////////////////////////
/////	This constructor initializes the tChunk data
//////////////////////////////////////////////////////////////////////

CLoad3DS::CLoad3DS()
{
    m_FilePointer = NULL;
}


CLoad3DS::~CLoad3DS()
{
    m_FilePointer = NULL;
}

///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	IMport 3DS File
/////
///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CLoad3DS::Import3DS(CLoad3DS::t3DModel *pModel, char *strFileName)
{
    tChunk currentChunk = {0};
    //int i=0;

    // Open file
    #if defined WIN32
        fopen_s(&m_FilePointer,strFileName, "rb");
    #elif defined unix
        m_FilePointer = fopen(strFileName,"rb");
    #endif

    // make sure file has opened
    if(!m_FilePointer)
    {
        //! QMessageBox lead to construct run twice
        //sprintf(strMessage, "can not find: %s!\n", strFileName);
        qDebug("cannot find %s",strFileName);
        return false;
    }

    // read first chunk after read

    ReadChunk(&currentChunk);

    //
    if (currentChunk.ID != PRIMARY)
    {
        //QMessageBox::warning(NULL,"Load model error",strMessage,QMessageBox::Yes);
        qDebug("Load model: %s error",strFileName);
        return false;
    }

    // Load Object
    ProcessNextChunk(pModel, &currentChunk);

    // Computer normal
    ComputeNormals(pModel);

    // clean
    CleanUp();

    return true;
}

///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	clean memory
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::CleanUp()
{
    if (m_FilePointer) {
        fclose(m_FilePointer);					// Close the current file pointer
        m_FilePointer = NULL;
    }
}


///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read every model chunk
/////
///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextChunk(CLoad3DS::t3DModel *pModel, tChunk *pPreviousChunk)
{
    t3DObject newObject;    					// This is used to add to our object list
    tMaterialInfo newTexture = {{0}};           // This is used to add to our material list

    tChunk currentChunk = {0};					// The current chunk to load
    tChunk tempChunk = {0};						// A temp chunk for holding data

    // Below we check our chunk ID each time we read a new chunk.  Then, if
    // we want to extract the information from that chunk, we do so.
    // If we don't want a chunk, we just read past it.

    // Continue to read the sub chunks until we have reached the length.
    // After we read ANYTHING we add the bytes read to the chunk and then check
    // check against the length.
    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        // Read next Chunk
        ReadChunk(&currentChunk);

        // Check the chunk ID
        switch (currentChunk.ID)
        {
        case VERSION:							// This holds the version of the file

            // If the file was made in 3D Studio Max, this chunk has an int that
            // holds the file version.  Since there might be new additions to the 3DS file
            // format in 4.0, we give a warning to that problem.
            // However, if the file wasn't made by 3D Studio Max, we don't 100% what the
            // version length will be so we'll simply ignore the value

            // Read the file version and add the bytes read to our bytesRead variable
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

            // If the file version is over 3, give a warning that there could be a problem
            if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
                //MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
                QMessageBox::warning(NULL,"Load model error","This 3DS file is over version 3 so it may load incorrectly",QMessageBox::Yes);
            }
            break;

        case OBJECTINFO:						// This holds the version of the mesh
            {
            // This chunk holds the version of the mesh.  It is also the head of the MATERIAL
            // and OBJECT chunks.  From here on we start reading in the material and object info.

            // Read the next chunk
            ReadChunk(&tempChunk);

            // Get the version of the mesh
            tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

            // Increase the bytesRead by the bytes read from the last chunk
            currentChunk.bytesRead += tempChunk.bytesRead;

            // Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
            ProcessNextChunk(pModel, &currentChunk);
            break;
        }
        case MATERIAL:							// This holds the material information

            // This chunk is the header for the material info chunks

            // Increase the number of materials
            pModel->numOfMaterials++;

            // Add a empty texture structure to our texture list.
            // If you are unfamiliar with STL's "vector" class, all push_back()
            // does is add a new node onto the list.  I used the vector class
            // so I didn't need to write my own link list functions.
            pModel->vctMaterials.push_back(newTexture);

            // Proceed to the material loading function
            ProcessNextMaterialChunk(pModel, &currentChunk);
            break;

        case OBJECT:							// This holds the name of the object being read

            // This chunk is the header for the object info chunks.  It also
            // holds the name of the object.

            // Increase the object count
            pModel->numOfObjects++;

            // Add a new tObject node to our list of objects (like a link list)
            pModel->vctObjects.push_back(newObject);

            // Initialize the object and all it's data members
            memset(&(pModel->vctObjects[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

            // Get the name of the object and store it, then add the read bytes to our byte counter.
            currentChunk.bytesRead += GetString(pModel->vctObjects[pModel->numOfObjects - 1].strName);

            // Now proceed to read in the rest of the object information
            ProcessNextObjectChunk(pModel, &(pModel->vctObjects[pModel->numOfObjects - 1]), &currentChunk);
            break;

        case EDITKEYFRAME:

            // Because I wanted to make this a SIMPLE tutorial as possible, I did not include
            // the key frame information.  This chunk is the header for all the animation info.
            // In a later tutorial this will be the subject and explained thoroughly.

            //ProcessNextKeyFrameChunk(pModel, currentChunk);

            // Read past this chunk and add the bytes read to the byte counter
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;

        default:

            // If we didn't care about a chunk, then we get here.  We still need
            // to read past the unknown or ignored chunk and add the bytes read to the byte counter.
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;
        }

        // Add the bytes read from the last chunk to the previous chunk passed in.
        pPreviousChunk->bytesRead += currentChunk.bytesRead;
    }
}


///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	deal an object
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextObjectChunk(CLoad3DS::t3DModel *pModel, CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk)
{
    // The current chunk to work with
    tChunk currentChunk = {0};
    //vector<tMatREF*> vctMatIDS;

    // Continue to read these chunks until we read the end of this sub chunk
    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        // Read the next chunk
        ReadChunk(&currentChunk);

        // Check which chunk we just read
        switch (currentChunk.ID)
        {
        case OBJECT_MESH:					// This lets us know that we are reading a new object

            // We found a new object, so let's read in it's info using recursion
            ProcessNextObjectChunk(pModel, pObject, &currentChunk);
            break;

        case OBJECT_VERTICES:				// This is the objects vertices
            ReadVertices(pObject, &currentChunk);
            break;

        case OBJECT_FACES:					// This is the objects face information
            ReadVertexIndices(pObject, &currentChunk);
            break;

        case OBJECT_MATERIAL:				// This holds the material name that the object has

            // This chunk holds the name of the material that the object has assigned to it.
            // This could either be just a color or a texture map.  This chunk also holds
            // the faces that the texture is assigned to (In the case that there is multiple
            // textures assigned to one object, or it just has a texture on a part of the object.
            // Since most of my game objects just have the texture around the whole object, and
            // they aren't multitextured, I just want the material name.

            // We now will read the name of the material assigned to this object
            ReadObjectMaterial(pModel, pObject, &currentChunk);
            break;

        case OBJECT_UV:						// This holds the UV texture coordinates for the object

            // This chunk holds all of the UV coordinates for our object.  Let's read them in.
            ReadUVCoordinates(pObject, &currentChunk);
            break;

        case 0x4111: //TRI_VERTEXOPTIONS:
            // Read past the ignored or unknown chunks
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;
        default:
            // Read past the ignored or unknown chunks
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;
        }

        // Add the bytes read from the last chunk to the previous chunk passed in.
        pPreviousChunk->bytesRead += currentChunk.bytesRead;

    }
}

///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	deal with matrial, like name
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextMaterialChunk(CLoad3DS::t3DModel *pModel, CLoad3DS::tChunk *pPreviousChunk)
{
    // The current chunk to work with
    tChunk currentChunk = {0};

    // Continue to read these chunks until we read the end of this sub chunk
    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        // Read the next chunk
        ReadChunk(&currentChunk);

        // Check which chunk we just read in
        switch (currentChunk.ID)
        {
        case MATNAME:							// This chunk holds the name of the material

            // Here we read in the material name
            currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;

        case MATDIFFUSE:						// This holds the R G B color of our object
            ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk);
            break;

        case MATMAP:							// This is the header for the texture info

            // Proceed to read in the material information
            ProcessNextMaterialChunk(pModel, &currentChunk);
            break;

        case MATMAPFILE:						// This stores the file name of the material

            // Here we read in the material's file name
            currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;

        default:

            // Read past the ignored or unknown chunks
            currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
            break;
        }

        // Add the bytes read from the last chunk to the previous chunk passed in.
        pPreviousChunk->bytesRead += currentChunk.bytesRead;
    }
}

///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read the id and length of a chunk
/////
///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadChunk(CLoad3DS::tChunk *pChunk)
{
    // This reads the chunk ID which is 2 bytes.
    // The chunk ID is like OBJECT or MATERIAL.  It tells what data is
    // able to be read in within the chunks section.
    pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

    // Then, we read the length of the chunk which is 4 bytes.
    // This is how we know how much to read in, or read past.
    pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read strings
/////
///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int CLoad3DS::GetString(char *pBuffer)
{
    int index = 0;

    // Read 1 byte of data which is the first letter of the string
    fread(pBuffer, 1, 1, m_FilePointer);

    // Loop until we get NULL
    while (*(pBuffer + index++) != 0) {

        // Read in a character at a time until we hit NULL.
        fread(pBuffer + index, 1, 1, m_FilePointer);
    }

    // Return the string length, which is how many bytes we read in (including the NULL)
    return strlen(pBuffer) + 1;
}


///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read color data
/////
///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadColorChunk(CLoad3DS::tMaterialInfo *pMaterial, CLoad3DS::tChunk *pChunk)
{
    tChunk tempChunk = {0};

    // Read the color chunk info
    ReadChunk(&tempChunk);

    // Read in the R G B color (3 bytes - 0 through 255)
    tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

    // Add the bytes read to our chunk
    pChunk->bytesRead += tempChunk.bytesRead;
}


///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read vertex index
/////
///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertexIndices(CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk)
{
    unsigned short index = 0;					// This is used to read in the current face index

    // In order to read in the vertex indices for the object, we need to first
    // read in the number of them, then read them in.  Remember,
    // we only want 3 of the 4 values read in for each face.  The fourth is
    // a visibility flag for 3D Studio Max that doesn't mean anything to us.

    // Read in the number of faces that are in this object (int)
    pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

    // Alloc enough memory for the faces and initialize the structure
    pObject->pFaces = new tFace [pObject->numOfFaces];
    memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

    // Go through all of the faces in this object
    for(int i = 0; i < pObject->numOfFaces; i++)
    {
        // Next, we read in the A then B then C index for the face, but ignore the 4th value.
        // The fourth value is a visibility flag for 3D Studio Max, we don't care about this.
        for(int j = 0; j < 4; j++)
        {
            // Read the first vertice index for the current face
            pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

            if(j < 3)
            {
                // Store the index in our face structure.
                pObject->pFaces[i].vertIndex[j] = index;
                pObject->Indexes.push_back(index);
            }
        }
    }
}


///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read texture coordinate
/////
///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
    // In order to read in the UV indices for the object, we need to first
    // read in the amount there are, then read them in.

    // Read in the number of UV coordinates there are (int)
    pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

    // Allocate memory to hold the UV coordinates
    QVector2D *pTexVerts = new QVector2D[pObject->numTexVertex];

    // Read in the texture coodinates (an array 2 float)
    pPreviousChunk->bytesRead += fread(pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);


    int i = 0;
    while(i < pObject->numTexVertex){
        pObject->VertTexture.push_back(QVector2D(pTexVerts[i].x(),pTexVerts[i].y()));
        i++;
    }
    delete [] pTexVerts;
}


///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read vertex
/////
///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertices(CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk)
{
    // Like most chunks, before we read in the actual vertices, we need
    // to find out how many there are to read in.  Once we have that number
    // we then fread() them into our vertice array.

    // Read in the number of vertices (int)
    pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

    QVector3D *pVerts = new QVector3D [pObject->numOfVerts];
    // Allocate the memory for the verts and initialize the structure
    //pObject->pVerts = new QVector3D [pObject->numOfVerts];
    memset(pVerts, 0, sizeof(QVector3D) * pObject->numOfVerts);

    // Read in the array of vertices (an array of 3 floats)
    pPreviousChunk->bytesRead += fread(pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

    int i = 0;
    while(i < pObject->numOfVerts){
        QVector3D Vertpos = pVerts[i];

        // Now we should have all of the vertices read in.  Because 3D Studio Max
        // Models with the Z-Axis pointing up (strange and ugly I know!), we need
        // to flip the y values with the z values in our vertices.  That way it
        // will be normal, with Y pointing up.  If you prefer to work with Z pointing
        // up, then just delete this next loop.  Also, because we swap the Y and Z
        // we need to negate the Z to make it come out correctly.
        Vertpos.setY(pVerts[i].z());
        Vertpos.setZ(-pVerts[i].y());

        //pObject->VertPos.push_back(pObject->pVerts[i]);
        pObject->VertPos.push_back(Vertpos);
        i++;
    }
    delete []pVerts;
    //return;
}


///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	read material
/////
///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadObjectMaterial(CLoad3DS::t3DModel *pModel,CLoad3DS::t3DObject *pObject, CLoad3DS::tChunk *pPreviousChunk)
{
    char strMaterial[255] = {0};			// This is used to hold the objects material name
    bool bmaterror=true;
    Q_UNUSED(bmaterror);

    // *What is a material?*  - A material is either the color or the texture map of the object.
    // It can also hold other information like the brightness, shine, etc... Stuff we don't
    // really care about.  We just want the color, or the texture map file name really.

    // Here we read the material name that is assigned to the current object.
    // strMaterial should now have a string of the material name, like "Material #2" etc..
    pPreviousChunk->bytesRead += GetString(strMaterial);

    // Now that we have a material name, we need to go through all of the materials
    // and check the name against each material.  When we find a material in our material
    // list that matches this name we just read in, then we assign the materialID
    // of the object to that material index.  You will notice that we passed in the
    // model to this function.  This is because we need the number of textures.
    // Yes though, we could have just passed in the model and not the object too.

    // Go through all of the textures
    for(int i = 0; i < pModel->numOfMaterials; i++)
    {
        // If the material we just read in matches the current texture name
        if(strcmp(strMaterial, pModel->vctMaterials[i].strName) == 0)
        {
            // Set the material ID to the current index 'i' and stop checking
            pObject->materialID = i;
            // Now that we found the material, check if it's a texture map.
            // If the strFile has a string length of 1 and over it's a texture
            if(strlen(pModel->vctMaterials[i].strFile) > 0) {
                // Set the object's flag to say it has a texture map to bind.
                pObject->bHasTexture = true;
                //pMatref->bHasTexture=true;
            }
            bmaterror=false;
            break;
        }
        else
        {
            // Set the ID to -1 to show there is no material for this object
            pObject->materialID = -1;
            bmaterror=true;
        }
    }
//--------------------------------------------------------------
    // read face index
    pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
//--------------------------------------------------------------
}

///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	computer normal
/////
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ComputeNormals(CLoad3DS::t3DModel *pModel)
{
    QVector3D vVector1, vVector2, vNormal, vPoly[3];

    // If there are no objects, we can skip this part
    if(pModel->numOfObjects <= 0)
        return;

    // What are vertex normals?  And how are they different from other normals?
    // Well, if you find the normal to a triangle, you are finding a "Face Normal".
    // If you give OpenGL a face normal for lighting, it will make your object look
    // really flat and not very round.  If we find the normal for each vertex, it makes
    // the smooth lighting look.  This also covers up blocky looking objects and they appear
    // to have more polygons than they do.    Basically, what you do is first
    // calculate the face normals, then you take the average of all the normals around each
    // vertex.  It's just averaging.  That way you get a better approximation for that vertex.

    // Go through each of the objects to calculate their normals
    for(int index = 0; index < pModel->numOfObjects; index++)
    {
        // Get the current object
        t3DObject *pObject = &(pModel->vctObjects[index]);

        // Here we allocate all the memory we need to calculate the normals
        QVector3D *tmpNormals		= new QVector3D [pObject->numOfFaces];
        QVector3D *pTempNormals	= new QVector3D [pObject->numOfFaces];
        //pObject->pNormals		= new QVector3D [pObject->numOfVerts];
        QVector3D *pNormals = new QVector3D [pObject->numOfVerts];
        // Go though all of the faces of this object
        for(int i=0; i < pObject->numOfFaces; i++)
        {
            // To cut down LARGE code, we extract the 3 points of this face
            vPoly[0] = pObject->VertPos[pObject->pFaces[i].vertIndex[0]];
            vPoly[1] = pObject->VertPos[pObject->pFaces[i].vertIndex[1]];
            vPoly[2] = pObject->VertPos[pObject->pFaces[i].vertIndex[2]];

            // Now let's calculate the face normaland s (Get 2 vectors find the cross product of those 2)

            vVector1 = vPoly[0] - vPoly[2];		// Get the vector of the polygon (we just need 2 sides for the normal)
            vVector2 = vPoly[2] - vPoly[1];		// Get a second vector of the polygon

            vNormal  = QVector3D::crossProduct(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
            pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
            vNormal.normalize();                        // Normalize the cross product to give us the polygons normal

            tmpNormals[i] = vNormal;						// Assign the normal to the list of normals
        }

        //////////////// Now Get The Vertex Normals /////////////////

        QVector3D vSum (0.0, 0.0, 0.0);
        QVector3D vZero = vSum;
        int shared=0;

        for (int i = 0; i < pObject->numOfVerts; i++)			// Go through all of the vertices
        {
            for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
            {												// Check if the vertex is shared by another face
                if (pObject->pFaces[j].vertIndex[0] == i ||
                    pObject->pFaces[j].vertIndex[1] == i ||
                    pObject->pFaces[j].vertIndex[2] == i)
                {
                    vSum =vSum+pTempNormals[j];// AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
                    shared++;								// Increase the number of shared triangles
                }
            }

            // Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
            pNormals[i] = vSum/float(-shared);// .DivideVectorByScaler(vSum, float(-shared));

            // Normalize the normal for the final vertex normal
            //pObject->pNormals[i] = Normalize(pObject->pNormals[i]);
            //pObject->pNormals[i] = QVector3D(0,0,0);
            pNormals[i].normalize();
            pObject->VertNormal.push_back(pNormals[i]);

            vSum = vZero;									// Reset the sum
            shared = 0;										// Reset the shared
        }

        // Free our memory and start over on the next object
        delete [] pNormals;
        delete [] pTempNormals;
        delete [] tmpNormals;
    }
}
