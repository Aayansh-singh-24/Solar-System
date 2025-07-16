#include"model.h"



Mesh Process_Mesh(aiMesh* mesh)
{
	Mesh Processed_mesh;

	//Vertex and Normal Configuration:-
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//Position of vertices
		vertex.Position = glm::vec3
		(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		if (mesh->HasNormals())
		{
			vertex.Normal = glm::vec3
			(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}

		//Texture
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = glm::vec2
			(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		// Print UVs for debugging
		if ((vertex.TexCoords.x || vertex.TexCoords.y) > 1)
		{
			std::cout << "Error in UV-Mapping" << std::endl;
			break;
		}
		else
		{
			std::cout << "Vertex " << i << " UV: ("
				<< vertex.TexCoords.x << ", "
				<< vertex.TexCoords.y << ")" << std::endl;
		}



		Processed_mesh.vertices.push_back(vertex);
	}

	//Indices-Configuration:-
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace Face = mesh->mFaces[i];
		for (int j = 0; j < Face.mNumIndices; j++)
		{
			Processed_mesh.indices.push_back(Face.mIndices[j]);
		}
	}

	//Buffers-SetUp::

	//VAO:-
	glGenVertexArrays(1, &Processed_mesh.VAO);
	glBindVertexArray(Processed_mesh.VAO);

	//VBO:-
	glGenBuffers(1, &Processed_mesh.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Processed_mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, Processed_mesh.vertices.size() * sizeof(Vertex), &Processed_mesh.vertices[0], GL_STATIC_DRAW);

	//EBO:-
	glGenBuffers(1, &Processed_mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Processed_mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Processed_mesh.indices.size() * sizeof(unsigned int), &Processed_mesh.indices[0], GL_STATIC_DRAW);


	// Attributes:- Send Data to the vertex shader::

	// For position--
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	//For Normal--
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	//For Texture--
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0); // for unbinding VAO 

	return Processed_mesh;
}


std::vector<Mesh> LoadModel(std::string path)
{
	std::vector<Mesh> meshes;

	//Assimp Impoter-Setup:--
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate |aiProcess_FlipUVs | aiProcess_GenNormals);

	//Check scene is Sucessfully load or not::
	if (!scene || (scene->mFlags && AI_SCENE_FLAGS_NON_VERBOSE_FORMAT) || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return meshes;
	}

	//Recusively call and process all nodes:--
	std::function<void(aiNode*)> processNode = [&](aiNode* node) {
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(Process_Mesh(mesh));
		}
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i]);
		}
	};

	processNode(scene->mRootNode);
	return meshes;
}
