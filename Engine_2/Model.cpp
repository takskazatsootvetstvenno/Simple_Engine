#include "Model.h"
#include "stb_image.h"



void Model::loadModel(string const& path) {
    // ������ ����� � ������� Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    // �������� �� ������                                                       aiProcess_PreTransformVertices
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // ���� �� 0
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    // ��������� ���� � �����
    directory = path.substr(0, path.find_last_of('/'));

    // ����������� ��������� ��������� ���� Assimp
    processNode(scene->mRootNode, root, scene);
    // delete scene;
    cout << "end";
}
void Model::processNode(aiNode* node, TreeNode& treeNode, const aiScene* scene) {
    // ������������ ������ ��� �������� ����
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // ���� �������� ������ ������� �������� � �����.
        // ����� �� �������� ��� ������; ���� - ��� ���� ������ ����������� ������

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        Mesh tempMesh = processMesh(mesh, scene);
        tempMesh.name = mesh->mName.C_Str();
        // meshes.push_back(tempMesh);
        //  treeNode.addMesh(&tempMesh);
        treeNode.addMesh(tempMesh);
    }
    // ����� ����, ��� �� ���������� ��� ���� (���� ������� �������), �� �������� ���������� ������������ ������ ��
    // �������� �����

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        TreeNode* child = new TreeNode(model_shaders, render);
        treeNode.addChild(child);
        // node->mChildren[i]->mTransformation;
        child->localMatrix[0][0] = node->mChildren[i]->mTransformation.a1;
        child->localMatrix[0][1] = node->mChildren[i]->mTransformation.b1;
        child->localMatrix[0][2] = node->mChildren[i]->mTransformation.c1;
        child->localMatrix[0][3] = node->mChildren[i]->mTransformation.d1;

        child->localMatrix[1][0] = node->mChildren[i]->mTransformation.a2;
        child->localMatrix[1][1] = node->mChildren[i]->mTransformation.b2;
        child->localMatrix[1][2] = node->mChildren[i]->mTransformation.c2;
        child->localMatrix[1][3] = node->mChildren[i]->mTransformation.d2;

        child->localMatrix[2][0] = node->mChildren[i]->mTransformation.a3;
        child->localMatrix[2][1] = node->mChildren[i]->mTransformation.b3;
        child->localMatrix[2][2] = node->mChildren[i]->mTransformation.c3;
        child->localMatrix[2][3] = node->mChildren[i]->mTransformation.d3;

        child->localMatrix[3][0] = node->mChildren[i]->mTransformation.a4;
        child->localMatrix[3][1] = node->mChildren[i]->mTransformation.b4;
        child->localMatrix[3][2] = node->mChildren[i]->mTransformation.c4;
        child->localMatrix[3][3] = node->mChildren[i]->mTransformation.d4;

        child->name = node->mChildren[i]->mName.C_Str();
        processNode(node->mChildren[i], *child, scene);
    }
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // ������ ��� ����������
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // ���� �� ���� �������� ����
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;  // �� ��������� ������������� ������, �.�. Assimp ���������� ���� ����������� ���������
                           // �����, ������� �� ������������� �������� � ��� glm::vec3, ������� ������� �� ��������
                           // ������ � ���� ������������� ������ ���� glm::vec3

        // ����������
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // �������
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // ���������� ����������
        if (mesh->mTextureCoords[0])  // ���� ��� �������� ���������� ����������
        {
            glm::vec2 vec;

            // ������� ����� ��������� �� 8 ��������� ���������� ���������. �� ������������, ��� �� �� �����
            // ������������ ������, � ������� ������� ����� ��������� ��������� ���������� ���������, ������� �� ������
            // ����� ������ ����� (0)
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        } else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        // ����������� ������
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;

        // ������ ���������
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;
        vertices.push_back(vertex);
    }
    // ������ ���������� �� ������ ����� ���� (����� - ��� ����������� ����) � ��������� ��������������� ������� ������
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        // �������� ��� ������� ������ � ��������� �� � ������� indices
        for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    // ������������ ���������
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // �� ������ ���������� �� ������ ��������� � ��������. ������ ��������� �������� ����� ����������
    // 'texture_diffuseN', ��� N - ���������� ����� �� 1 �� MAX_SAMPLER_NUMBER. ���� ����� ��������� � � ������
    // ���������: ��������� - texture_diffuseN ��������� - texture_specularN ������� - texture_normalN

    // 1. ��������� �����
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. ����� ���������
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. ����� ��������
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. ����� �����
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // ���������� ���-������, ��������� �� ������ ���������� ������
    Mesh temp(vertices, indices, textures, render);
    return temp;
}
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // ���������, �� ���� �� �������� ��������� �����, � ���� - ��, �� ���������� �������� ����� �������� �
        // ��������� � ��������� ��������
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;  // �������� � ��� �� ����� � ����� ��� ���������, ��������� � ��������� (�����������)
                break;
            }
        }
        if (!skip) {  // ���� �������� ��� �� ���� ���������, �� ��������� �
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(
                texture);  // ��������� �������� � ������� � ��� ������������ ����������, ��� ����� ����������, ��� �
                           // ��� �� �������� ��� ������������� ��������� �������
        }
    }
    return textures;
}
unsigned int Model::TextureFromFile(const char* path, const string& directory) {
    string filename = string(path);
    filename = directory + '/' + filename;
    
    return render->generateTexturesFromFile(filename, path);
}
