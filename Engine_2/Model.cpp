#include "Model.h"
#include "stb_image.h"



void Model::loadModel(string const& path) {
    // Чтение файла с помощью Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    // Проверка на ошибки                                                       aiProcess_PreTransformVertices
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // если НЕ 0
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    // Получение пути к файлу
    directory = path.substr(0, path.find_last_of('/'));

    // Рекурсивная обработка корневого узла Assimp
    processNode(scene->mRootNode, root, scene);
    // delete scene;
    cout << "end";
}
void Model::processNode(aiNode* node, TreeNode& treeNode, const aiScene* scene) {
    // Обрабатываем каждый меш текущего узла
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // Узел содержит только индексы объектов в сцене.
        // Сцена же содержит все данные; узел - это лишь способ организации данных

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        Mesh tempMesh = processMesh(mesh, scene);
        tempMesh.name = mesh->mName.C_Str();
        // meshes.push_back(tempMesh);
        //  treeNode.addMesh(&tempMesh);
        treeNode.addMesh(tempMesh);
    }
    // После того, как мы обработали все меши (если таковые имелись), мы начинаем рекурсивно обрабатывать каждый из
    // дочерних узлов

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
    // Данные для заполнения
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // Цикл по всем вершинам меша
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;  // мы объявляем промежуточный вектор, т.к. Assimp использует свой собственный векторный
                           // класс, который не преобразуется напрямую в тип glm::vec3, поэтому сначала мы передаем
                           // данные в этот промежуточный вектор типа glm::vec3

        // Координаты
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Нормали
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Текстурные координаты
        if (mesh->mTextureCoords[0])  // если меш содержит текстурные координаты
        {
            glm::vec2 vec;

            // Вершина может содержать до 8 различных текстурных координат. Мы предполагаем, что мы не будем
            // использовать модели, в которых вершина может содержать несколько текстурных координат, поэтому мы всегда
            // берем первый набор (0)
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        } else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        // Касательный вектор
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;

        // Вектор бинормали
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;
        vertices.push_back(vertex);
    }
    // Теперь проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        // Получаем все индексы граней и сохраняем их в векторе indices
        for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    // Обрабатываем материалы
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Мы вводим соглашение об именах сэмплеров в шейдерах. Каждая диффузная текстура будет называться
    // 'texture_diffuseN', где N - порядковый номер от 1 до MAX_SAMPLER_NUMBER. Тоже самое относится и к другим
    // текстурам: диффузная - texture_diffuseN отражения - texture_specularN нормали - texture_normalN

    // 1. Диффузные карты
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. Карты отражения
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. Карты нормалей
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. Карты высот
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // Возвращаем меш-объект, созданный на основе полученных данных
    Mesh temp(vertices, indices, textures, render);
    return temp;
}
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Проверяем, не была ли текстура загружена ранее, и если - да, то пропускаем загрузку новой текстуры и
        // переходим к следующей итерации
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;  // текстура с тем же путем к файлу уже загружена, переходим к следующей (оптимизация)
                break;
            }
        }
        if (!skip) {  // если текстура еще не была загружена, то загружаем её
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(
                texture);  // сохраняем текстуру в массиве с уже загруженными текстурами, тем самым гарантируя, что у
                           // нас не появятся без необходимости дубликаты текстур
        }
    }
    return textures;
}
unsigned int Model::TextureFromFile(const char* path, const string& directory) {
    string filename = string(path);
    filename = directory + '/' + filename;
    
    return render->generateTexturesFromFile(filename, path);
}
