#include <GL/glew.h>
#include "CustomObject.h"
#include <iostream> 
#include <fstream> 
///////////#define STB_IMAGE_IMPLEMENTATION
///////////#include "stb_image.h"
#include <iostream>

//unsigned int TextureFromFile(string const& path);
// bool readObjFile(string const& path, vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures,unsigned long int& position);
CustomObject::CustomObject(string const& path, glm::vec3 position, objectTypes objectType,TreeNode& root,Renderer* rend, glm::vec3 scale)
:render(rend){
	createNewObject(path, position, objectType, root, scale);
}
 void CustomObject::Draw(Shader shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
 void CustomObject::createNewObject(string const& path,glm::vec3 position, objectTypes objectType, TreeNode& root, glm::vec3 scale)
 {
	vector<Vertex> vertices;
	vector<Texture> textures;
	vector<unsigned int> indices;
	Vertex pos1, pos2, pos3,pos4, pos5, pos6, pos7, pos8;
	Texture baseTex;
	Texture baseTex2;
	bool finish;
	unsigned long int pos_file=0;
	//Mesh modelMesh;
	switch (objectType)
	{
	case TRIANGLE:
		//Vertex pos1,pos2,pos3;
		pos1.Position = scale*(glm::vec3(0.0,0.0,0.0) + position);
		pos2.Position =  scale*(glm::vec3(1.0,0.0,0.0) + position);
		pos3.Position =  scale*(glm::vec3(0.0,1.0,0.0) + position);

		pos1.TexCoord = glm::vec2(0.0, 0.0);
		pos2.TexCoord = glm::vec2(1.0, 0.0);
		pos3.TexCoord = glm::vec2(0.0, 1.0);

		pos1.Normal = glm::vec3(0.0,0.0,1.0);
		pos2.Normal = glm::vec3(0.0,0.0,1.0);
		pos3.Normal = glm::vec3(0.0,0.0,1.0);

		vertices.resize(3);
		vertices[0] = pos1; vertices[1] = pos2; vertices[2] = pos3;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);

		baseTex.id = TextureFromFile(path);
		baseTex.path = path;
		baseTex.type = "texture_diffuse";
		textures.push_back(baseTex);
		break;
	case PLANE:
		//Vertex pos1,pos2,pos3,pos4;
		//pos1.Position = scale* glm::vec3(0.0, 0.0, 0.0) + position;
		//pos2.Position =  scale*glm::vec3(1.0,0.0,0.0) + position;
		//pos3.Position = scale* glm::vec3(1.0,1.0,0.0) + position;
		//pos4.Position =  scale*glm::vec3(0.0,1.0,0.0) + position;

		pos1.Position = scale* glm::vec3(-0.5, -0.5, 0.0) + position;
		pos2.Position =  scale*glm::vec3(0.5,-0.5,0.0) + position;
		pos3.Position = scale* glm::vec3(0.5,0.5,0.0) + position;
		pos4.Position =  scale*glm::vec3(-0.5,0.5,0.0) + position;


		pos1.TexCoord = glm::vec2(0.0, 0.0);
		pos2.TexCoord = glm::vec2(0.0, 1.0);
		pos3.TexCoord = glm::vec2(1.0, 1.0);
		pos4.TexCoord = glm::vec2(1.0, 0.0);

		pos1.Normal = glm::vec3(0.0,0.0,1.0);
		pos2.Normal = glm::vec3(0.0,0.0,1.0);
		pos3.Normal = glm::vec3(0.0,0.0,1.0);
		pos4.Normal = glm::vec3(0.0,0.0,1.0);

		vertices.resize(4);
		vertices[0] = pos1; vertices[1] = pos2; vertices[2] = pos3; vertices[3] = pos4;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(2); indices.push_back(3); indices.push_back(0);
		baseTex.id = TextureFromFile(path);
		baseTex.path = path;
		baseTex.type = "texture_diffuse";
		textures.push_back(baseTex);

		break;
	case CUBE:

		pos1.Position = scale*( glm::vec3(0.0,0.0,0.0)+position);
		pos2.Position =  scale*(glm::vec3(1.0,0.0,0.0)+position);
		pos3.Position = scale*( glm::vec3(1.0,1.0,0.0)+position);
		pos4.Position = scale*( glm::vec3(0.0,1.0,0.0)+position);

		pos5.Position = glm::vec3(0.0,0.0,1.0)+position;
		pos6.Position = glm::vec3(1.0,0.0,1.0)+position;
		pos7.Position = glm::vec3(1.0,1.0,1.0)+position;
		pos8.Position = glm::vec3(0.0,1.0,1.0)+position;

		pos1.TexCoord = glm::vec2(0.0, 0.0);
		pos2.TexCoord = glm::vec2(0.0, 1.0);
		pos3.TexCoord = glm::vec2(1.0, 1.0);
		pos4.TexCoord = glm::vec2(1.0, 0.0);

		pos5.TexCoord = glm::vec2(0.0, 0.0);
		pos6.TexCoord = glm::vec2(0.0, 1.0);
		pos7.TexCoord = glm::vec2(1.0, 1.0);
		pos8.TexCoord = glm::vec2(1.0, 0.0);

		pos1.Normal = glm::vec3(0.0,0.0,1.0);
		pos2.Normal = glm::vec3(0.0,0.0,1.0);
		pos3.Normal = glm::vec3(0.0,0.0,1.0);
		pos4.Normal = glm::vec3(0.0,0.0,1.0);

		pos5.Normal = glm::vec3(0.0,0.0,-1.0);
		pos6.Normal = glm::vec3(0.0,0.0,-1.0);
		pos7.Normal = glm::vec3(0.0,0.0,-1.0);
		pos8.Normal = glm::vec3(0.0,0.0,-1.0);

		vertices.resize(8);
		vertices[0] = pos1; vertices[1] = pos2; vertices[2] = pos3; vertices[3] = pos4;
		vertices[4] = pos5; vertices[5] = pos6; vertices[6] = pos7; vertices[7] = pos8;
		indices.push_back(0); indices.push_back(1); indices.push_back(2); //перед
		indices.push_back(2); indices.push_back(3); indices.push_back(0);

		indices.push_back(4); indices.push_back(5); indices.push_back(6); //зад
		indices.push_back(6); indices.push_back(7); indices.push_back(4);

		indices.push_back(0); indices.push_back(4); indices.push_back(7); //лево
		indices.push_back(7); indices.push_back(3); indices.push_back(0);

		baseTex.id = TextureFromFile(path);
		baseTex.path = path;
		baseTex.type = "texture_diffuse";
		textures.push_back(baseTex);
		break;
	case SIMPLE_OBJ:
		
		
		finish = readObjFile(path);
		/*
		baseTex.id = TextureFromFile("models/x_base_color.png");
		baseTex.path = "models/x_base_color.png";
		baseTex.type = "texture_diffuse";
		textures.push_back(baseTex);

		baseTex2.id = TextureFromFile("models/y_base_color.png");
		baseTex2.path = "models/y_base_color.png";
		baseTex2.type = "texture_diffuse";
		textures.push_back(baseTex2);
		*/
		break;
	default:
		break;
	}

	if (vertices.size() > 0)
	{
		Mesh modelMesh(vertices, indices, textures, render);
		root.addMesh(modelMesh);
		meshes.push_back(modelMesh);
	}
	
 }
 long int max3(long int a, long int b, long int c)
 {
	if (a > b)
		 if (a > c)return a;
		 else return c;
	if (b > c) return b;
	return c;
 }
 void CustomObject::readMtlFile(string const& path, 
	  vector<Texture> &textures,string const& material_load, string const& name)
 {
	 string dir = path.substr(0, path.find_last_of('/'));
	 dir = dir + '/' + name;
	 std::fstream FileMtl;
	 FileMtl.open(dir, std::ios::in);
	 vector<std::string> materials;
	 vector<std::string> textures_path;
	 //провер€ем открылс€ ли файл 
	 if (FileMtl.is_open())
	 {
		 while (!FileMtl.eof())
		 {
			 std::string ReadLine;
			 char V1 = -1;
			 std::getline(FileMtl, ReadLine, '\n');
			 if (!ReadLine.empty())
			 {
				 V1 = ReadLine.c_str()[0];
			 }
			 switch (V1)
			 {
			 case 'n':
				 materials.push_back(ReadLine.substr(7));
				 break;
			 case 'm':
				 if (materials.size() - 1 > textures_path.size())
					 textures_path.push_back("Nothing_there");
				 textures_path.push_back(ReadLine.substr(7));
				 break;
			 default:
				 break;
			 }
		 }
		 string dir_tex = path.substr(0, path.find_last_of('/'));
		 for (int i =0;i<materials.size();i++)
		 {
			 if (materials[i] == material_load)
			 {
				 if (!(textures_path[i] == "Nothing_there"))
				 {
					 Texture baseTex;
				 baseTex.id = TextureFromFile(dir_tex + '/' + textures_path[i]);
				 baseTex.path = dir_tex + '/' + textures_path[i];
				 baseTex.type = "texture_diffuse";
				 textures.resize(1);
				 textures[0] = baseTex;
				 }
				 else
				 {
					 textures.resize(0);
				 }
				 
			 }
		 }
	 }
	 FileMtl.close();
 }
 bool CustomObject::readObjFile(string const& path)
{
    //объ€вл€ем переменную котора€ будет ссылкой на файл
	 vector<Vertex> vertices;
	 vector<unsigned int> indices;
	 vector<Texture> textures;
	// unsigned long int position_in_file;

    std::fstream FileObj;
	bool is_end=true;
	string mtlPath;
	vector<unsigned int> objects;
	vector<std::string>nameMaterials;


	struct TempInd
	{
		unsigned int temp_point;
		unsigned int temp_normal;
		unsigned int temp_texcoord;
	};

	vector<TempInd> index_load;
	vector<glm::vec3> vertics_load;
	vector<glm::vec3> normals_load;
	vector<glm::vec2> texcoord_load;
	//vector<unsigned int> index_load;

    FileObj.open(path, std::ios::in);
    //провер€ем открылс€ ли файл 
    if (FileObj.is_open())
    {
		unsigned int kolvo_o=0;
		char V1=-1, V2=-1;
		long int vSize = 0, vnSize = 0, vtSize = 0,vertexMax = 0;
		
        //обь€вл€ем переменную дл€ чтени€ строк текста из файла
        std::string ReadLine;
		
        //„итаем файл пока он не закончилс€
        while (!FileObj.eof() && is_end)
        {
				//получаем 1 строку из файла
			//str_counter++;
            std::getline(FileObj, ReadLine, '\n');
			if (!ReadLine.empty())
			{
				V1 = ReadLine.c_str()[0],
				V2 = ReadLine.c_str()[1];
			}
			
			//получаем 1 символ из строки и по нему определ€ем что будет читатьс€ в данный момент
            switch (V1)
            {
                //значит это координаты 
                case 'v':    
                    switch (V2)
                    {
                    case ' ':
						glm::vec3 pos;

						ReadLine[0] = ' ';
						sscanf_s(ReadLine.c_str(),"%f %f %f",
							&pos.x,
							&pos.y,
							&pos.z);
						vertics_load.push_back(pos);
						vSize++;
                        break;
                        //координаты нормали
                    case 'n':
							glm::vec3 norm;

							ReadLine[0] = ' ';ReadLine[1] = ' ';
							sscanf_s(ReadLine.c_str(),"%f %f %f",
							&norm.x,
							&norm.y,
							&norm.z);
							normals_load.push_back(norm);
                        //std::cout << "\n Normal \t" << ReadLine;
						vnSize++;
                        break;
                        //координаты текстуры
                    case 't':
							glm::vec2 coords;
						ReadLine[0] = ' ';ReadLine[1] = ' ';
						sscanf_s(ReadLine.c_str(),"%f %f",
							&coords.x,
							&coords.y);
							

						texcoord_load.push_back(coords);
                       // std::cout << "\n Textura \t" << ReadLine;
						vtSize++;
                        break;

                    default:
                        break;
                    }
                break;
				case 'm':
					 mtlPath=ReadLine.substr(7);//mtlib
					
				break;
				case 'u':
					nameMaterials.push_back(ReadLine.substr(7));
					 //cout << "\n usemtl: "<<ReadLine.substr(6)<<endl;
				//	 mtlPath=ReadLine.substr(6);//mtlib

				break;
				case 'o':
					objects.push_back(index_load.size());
				break;
				/*
				case 'u':
					cout << "\n usemtl:\t" << ReadLine;
				break;
				case 's':
					cout << "\n s: \t" << ReadLine;
				break;
				*/
				case 'f':
					TempInd first; TempInd second; TempInd third;
						ReadLine[0] = ' ';
							sscanf_s(ReadLine.c_str(),"%i/%i/%i %i/%i/%i %i/%i/%i",
							&first.temp_point,
							&first.temp_texcoord,
							&first.temp_normal,
							&second.temp_point,
							&second.temp_texcoord,
							&second.temp_normal,
							&third.temp_point,
							&third.temp_texcoord,
							&third.temp_normal);
							index_load.push_back(first); index_load.push_back(second); index_load.push_back(third);		
				break;

            default:
                break;
            }
        }
    }
	else
	std::cout << "Can't open file!!!";
	objects.push_back(index_load.size());//EOF

	vertices.resize(index_load.size());
	indices.resize(index_load.size());

	//vector<std::string> materials;
	vector<std::string> textures_path;

	unsigned int i=0;
	unsigned int ind=0;
	for (int j = 1; j < objects.size(); j++)
	{
		//for (int ind = 0; ind < objects[j]; ind++)
		//{
			vertices.resize(objects[j]);
			indices.resize(objects[j]);

			for (i; i < objects[j]; i++)
			{
				Vertex v;
				v.Position = vertics_load[index_load[i].temp_point - 1];
				v.Normal = normals_load[index_load[i].temp_normal - 1];
				v.TexCoord = texcoord_load[index_load[i].temp_texcoord - 1];
				vertices[i] = v;
				indices[i] = i;
				if (i > 4550)
				{
					std::cout << "fhfddh";
				}
					//	ind++;

			}
			i = objects[j]-1;
			

		//}
		readMtlFile(path, textures, nameMaterials[j-1], mtlPath);
		Mesh modelMesh(vertices, indices, textures,render);
		meshes.push_back(modelMesh);
	}
	

	FileObj.close();

	cout << "\nend";
	return is_end;
}

 unsigned int CustomObject::TextureFromFile(string const& path)/////////TO DO
{   
	 
    unsigned int textureID=-1;
    /*glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else*/
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
       // stbi_image_free(data);
    }

    return textureID;
}