#include "../Header/CModel.h"

void CModel::Update(float time)
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Update(time, m_modelPath);
	}
}

unsigned int  CModel::GetNumVertices()
{
	unsigned int count = 0;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		count += m_meshes[i].m_numVertices;
	}

	return count;
}

unsigned int  CModel::GetNumFaces()
{
	unsigned int count = 0;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		count += m_meshes[i].m_numIndices;
	}

	return count / 3;
}

void CModel::AddTransform(MATRIX4 position)
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].AddTransform(position);
	}
}

#ifdef OPEN_GL
bool CModel::Initialize(ModelData data)
{
	m_modelPath = data.path;

	//Load mesh from file
	const aiScene* model = aiImportFile(data.path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!model)
	{
		std::cout << "Error:  Scene not found" << std::endl;
		return false;
	}

	std::vector < const aiMaterial* > aiTextures;

	//Resize the software container
	m_meshes.resize(model->mNumMeshes);

	float mul;

	mul = 1.0f;
	
	if (m_meshes.size() > 5)
	{
		mul = 30.0f;
	}

	for (int i = 0; i < m_meshes.size(); i++)
	{
		//Load Indices
		unsigned int numFaces = model->mMeshes[i]->mNumFaces;

		m_meshes[i].m_indices.resize(numFaces * 3);

		for (unsigned int j = 0; j < numFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				if (mul > 1.0f)
				{
					m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[2 - k];
				}
				else
				{
					m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[k];
				}
			}
		}

		//Load vertices
		unsigned int numVertices = model->mMeshes[i]->mNumVertices;
		aiVector3D* UVs = model->mMeshes[i]->mTextureCoords[0];
		m_meshes[i].m_vertices.resize(numVertices);

		for (unsigned int j = 0; j < numVertices; j++)
		{
			m_meshes[i].m_vertices[j] = Vertex{ VECTOR3{model->mMeshes[i]->mVertices[j].x  * mul, model->mMeshes[i]->mVertices[j].y * mul,
			model->mMeshes[i]->mVertices[j].z * mul}, VECTOR2{UVs[j].x, 1.0f - UVs[j].y},
			VECTOR3{ model->mMeshes[i]->mNormals[j].x,  model->mMeshes[i]->mNormals[j].y, model->mMeshes[i]->mNormals[j].z } };
		}

		std::string textureName;

		if (model->HasMaterials())
		{
			//Texture
			const aiMaterial* material = model->mMaterials[model->mMeshes[i]->mMaterialIndex];

			aiString filePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL);


			for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
			{
				if (data.path[i] == '/')
				{
					for (unsigned int j = 0; j <= i; j++)
					{
						textureName.append(&data.path[j], 1);
					}

					textureName += filePath.data;

					break;
				}
			}
		}

		//Cambio para texturas dds
		textureName.erase(textureName.length() - 1);
		textureName.erase(textureName.length() - 1);
		textureName.erase(textureName.length() - 1);

		textureName += "jpg";
		//--------------------------------------------------------------------------

		m_meshes[i].Initialize(textureName);
	}

	AddTransform(data.transform);

	return true;
};

void CModel::Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{

	for (int i = 0; i < m_meshes.size(); i++)
	{
		for (int j = 0; j < m_meshes[i].m_transforms.size(); j++)
		{
			if (m_modelPath == "../Anubis/Anubis.fbx")
			{
				if (otherCamera.m_viewMatrix == camera.m_viewMatrix)
				{
					m_meshes[i].m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), camera.m_position),
						glm::radians(-camera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-camera.m_pitch),
						VECTOR3(1.0f, 0.0f, 0.0f));
				}
				else
				{
					m_meshes[i].m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), otherCamera.m_position),
						glm::radians(-otherCamera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-otherCamera.m_pitch),
						VECTOR3(1.0f, 0.0f, 0.0f));
				}
			}

			shaderProgram.UpdateMesh(m_meshes[i], j);
		}
		m_meshes[i].Render(shaderProgram.m_id);

	}
}
#endif

#ifdef DIRECT_X
bool CModel::Initialize(ModelData data, CDevice& device)
{
	m_modelPath = data.path;

	//Load mesh from file
	const aiScene* model = aiImportFile(data.path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!model)
	{
		std::cout << "Error:  Scene not found" << std::endl;
		return false;
	}

	//Resize the software container
	m_meshes.resize(model->mNumMeshes);

	float mul;

	if (m_meshes.size() > 5)
	{
		mul = 30.0f;
	}
	else
	{
		mul = 1.0f;
	}

	std::vector < const aiMaterial* > aiTextures;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		//Load Indices
		unsigned int numFaces = model->mMeshes[i]->mNumFaces;

		m_meshes[i].m_indices.resize(numFaces * 3);

		for (unsigned int j = 0; j < numFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				if (mul > 1.0f && m_meshes.size() != 2)
				{
					m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[k];
				}
				else
				{
					m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[2 - k];
				}
			}
		}

		//Load vertices
		unsigned int numVertices = model->mMeshes[i]->mNumVertices;
		aiVector3D* UVs = model->mMeshes[i]->mTextureCoords[0];
		m_meshes[i].m_vertices.resize(numVertices);

		for (unsigned int j = 0; j < numVertices; j++)
		{
			m_meshes[i].m_vertices[j] = Vertex{ VECTOR3{model->mMeshes[i]->mVertices[j].x * mul, model->mMeshes[i]->mVertices[j].y * mul,
			model->mMeshes[i]->mVertices[j].z* mul}, VECTOR2{UVs[j].x, 1.0f - UVs[j].y}, 
			VECTOR3{ model->mMeshes[i]->mNormals[j].x,  model->mMeshes[i]->mNormals[j].y, model->mMeshes[i]->mNormals[j].z } };
		}

		std::string texturePath;

		//Texture
		const aiMaterial* material = model->mMaterials[model->mMeshes[i]->mMaterialIndex];

		aiString filePath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL);

		if (filePath.length > 1)
		{
			for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
			{
				if (data.path[i] == '/')
				{
					for (unsigned int j = 0; j <= i; j++)
					{
						texturePath.append(&data.path[j], 1);
					}

					texturePath += filePath.data;

					break;
				}
			}
		}

		m_meshes[i].Initialize(texturePath);
		device.CreateBuffers(m_meshes[i]);
		device.CreateTexture(m_meshes[i]);
		m_meshes[i].m_vertices.clear();
		m_meshes[i].m_indices.clear();
	}

	AddTransform(data.transform);

	return true;
}

void CModel::Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{
	deviceContext.UpdateViewMatrix(shaderProgram.m_viewCB, camera);
	deviceContext.UpdateProjectionMatrix(shaderProgram.m_projectionCB, camera);

	for (int i = 0; i < m_meshes.size(); i++)
	{
		deviceContext.UpdateMeshColor(shaderProgram.m_colorCB, m_meshes[i]);
		deviceContext.SetMesh(m_meshes[i]);

		for (int j = 0; j < m_meshes[i].m_transforms.size(); j++)
		{
			if (m_modelPath == "../Anubis/Anubis.fbx")
			{
				if (otherCamera.m_viewMatrix == camera.m_viewMatrix)
				{
					m_meshes[i].m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), camera.m_position),
					glm::radians(-camera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-camera.m_pitch), 
					VECTOR3(1.0f, 0.0f, 0.0f));
				}
				else
				{
					m_meshes[i].m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), otherCamera.m_position),
					glm::radians(-otherCamera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-otherCamera.m_pitch),
						VECTOR3(1.0f, 0.0f, 0.0f));
				}
			}

			deviceContext.UpdateWorldMatrix(shaderProgram.m_worldCB, m_meshes[i], j);
		}

		deviceContext.Draw(m_meshes[i].m_numIndices);
	}
}
#endif
