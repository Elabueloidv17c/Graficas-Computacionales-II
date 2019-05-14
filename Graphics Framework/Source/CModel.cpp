#include "../Header/CShaderProgram.h"
#include "../Header/CCamera.h"
#include "../Header/CModel.h"
#include "../Header/CMesh.h"

CModel::CModel()
{
	m_scale = 1.0f;
	m_isRotating = false;
}

void CModel::Update(float time)
{

}

void CModel::SetTransform(MATRIX4 transform, unsigned int index)
{
	if (index >= m_transforms.size())
	{
		std::cout << "Warning:  index out of range, transform set in index 0" << std::endl;
		m_transforms[0] = transform;
		return;
	}

	m_transforms[index] = transform;
}

void CModel::Rotate(float speed)
{
	for (int i = 0; i < m_transforms.size(); i++)
	{
		m_transforms[i] = glm::rotate(m_transforms[i], glm::radians(cos(speed)), VECTOR3(0.0f, 1.0f, 0.0f));
	}
}

void CModel::ChangeScale()
{
	if (m_scale >= 1.0f)
	{
		m_scale = 0.5f;
	}
	else
	{
		m_scale = 2.0f;
	}

	for (int i = 0; i < m_transforms.size(); i++)
	{
		m_transforms[i] = glm::scale(m_transforms[i], VECTOR3(m_scale));
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

void CModel::AddTransform(MATRIX4 transform)
{
	m_transforms.push_back(transform);
}

void CModel::Erase()
{
	m_transforms.clear();
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

	for (int i = 0; i < m_meshes.size(); i++)
	{
		//Load Indices
		unsigned int numFaces = model->mMeshes[i]->mNumFaces;

		m_meshes[i].m_indices.resize(numFaces * 3);

		for (unsigned int j = 0; j < numFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[k];
			}
		}

		//Load vertices
		unsigned int numVertices = model->mMeshes[i]->mNumVertices;
		aiVector3D* UVs = model->mMeshes[i]->mTextureCoords[0];
		m_meshes[i].m_vertices.resize(numVertices);

		for (unsigned int j = 0; j < numVertices; j++)
		{
			m_meshes[i].m_vertices[j].Position = VECTOR3{ model->mMeshes[i]->mVertices[j].x,
			model->mMeshes[i]->mVertices[j].y, model->mMeshes[i]->mVertices[j].z };

			if (model->mMeshes[i]->HasTextureCoords(0))
			{
				m_meshes[i].m_vertices[j].TexCoord = VECTOR2{ UVs[j].x, 1.0f - UVs[j].y };
			}

			if (model->mMeshes[i]->HasNormals())
			{
				m_meshes[i].m_vertices[j].Normals = VECTOR3{ model->mMeshes[i]->mNormals[j].x,
				model->mMeshes[i]->mNormals[j].y, model->mMeshes[i]->mNormals[j].z };
			}

			if (model->mMeshes[i]->HasTangentsAndBitangents())
			{
				m_meshes[i].m_vertices[j].Tangents = VECTOR3{ model->mMeshes[i]->mTangents[j].x,
				model->mMeshes[i]->mTangents[j].y, model->mMeshes[i]->mTangents[j].z };
			}
		}

		std::vector <std::string> textureNames;
		textureNames.resize(2);

		if (model->HasMaterials())
		{
			//Diffuse
			const aiMaterial* material = model->mMaterials[model->mMeshes[i]->mMaterialIndex];

			aiString filePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL);

			if (filePath.length)
			{
				for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
				{
					if (data.path[i] == '/')
					{
						for (unsigned int j = 0; j <= i; j++)
						{
							textureNames[0].append(&data.path[j], 1);
						}

						textureNames[0] += filePath.data;

						break;
					}
				}
			}

			filePath = "";
			material->GetTexture(aiTextureType_NORMALS, 0, &filePath, NULL, NULL, NULL, NULL);

			if (filePath.length)
			{
				for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
				{
					if (data.path[i] == '/')
					{
						for (unsigned int j = 0; j <= i; j++)
						{
							textureNames[1].append(&data.path[j], 1);
						}

						textureNames[1] += filePath.data;

						break;
					}
				}
			}
		}

		m_meshes[i].Initialize(textureNames);
	}

	AddTransform(data.transform);

	return true;
};

void CModel::Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		for (int j = 0; j < m_transforms.size(); j++)
		{
			if (m_modelPath == "../Models/Anubis/Anubis.fbx")
			{
				if (otherCamera.m_viewMatrix == camera.m_viewMatrix)
				{
					m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), camera.m_position),
					glm::radians(-camera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-camera.m_pitch),
					VECTOR3(1.0f, 0.0f, 0.0f));
				}
				else
				{
					m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), otherCamera.m_position),
					glm::radians(-otherCamera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-otherCamera.m_pitch),
					VECTOR3(1.0f, 0.0f, 0.0f));
				}
			}

			shaderProgram.UpdateModel(m_transforms[j]);
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
				m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[2 - k];
			}
		}

		//Load vertices
		unsigned int numVertices = model->mMeshes[i]->mNumVertices;
		aiVector3D* UVs = model->mMeshes[i]->mTextureCoords[0];
		m_meshes[i].m_vertices.resize(numVertices);

		for (unsigned int j = 0; j < numVertices; j++)
		{
			m_meshes[i].m_vertices[j].Position = VECTOR3{model->mMeshes[i]->mVertices[j].x, 
			model->mMeshes[i]->mVertices[j].y, model->mMeshes[i]->mVertices[j].z };

			if (model->mMeshes[i]->HasTextureCoords(0))
			{
				m_meshes[i].m_vertices[j].TexCoord = VECTOR2{ UVs[j].x, 1.0f - UVs[j].y };
			}

			if (model->mMeshes[i]->HasNormals())
			{
				m_meshes[i].m_vertices[j].Normals = VECTOR3{ model->mMeshes[i]->mNormals[j].x,
				model->mMeshes[i]->mNormals[j].y, model->mMeshes[i]->mNormals[j].z };
			}

			if (model->mMeshes[i]->HasTangentsAndBitangents())
			{
				m_meshes[i].m_vertices[j].Tangents = VECTOR3{ model->mMeshes[i]->mTangents[j].x,
				model->mMeshes[i]->mTangents[j].y, model->mMeshes[i]->mTangents[j].z };
			}
		}

		std::vector <std::string> textureNames;
		textureNames.resize(2);

		if (model->HasMaterials())
		{
			//Diffuse
			const aiMaterial* material = model->mMaterials[model->mMeshes[i]->mMaterialIndex];

			aiString filePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL);
			
			if (filePath.length)
			{
				for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
				{
					if (data.path[i] == '/')
					{
						for (unsigned int j = 0; j <= i; j++)
						{
							textureNames[0].append(&data.path[j], 1);
						}

						textureNames[0] += filePath.data;

						break;
					}
				}
			}

			filePath = "";
			material->GetTexture(aiTextureType_NORMALS, 0, &filePath, NULL, NULL, NULL, NULL);

			if (filePath.length)
			{
				for (unsigned int i = strlen(data.path) - 1; i >= 0; i--)
				{
					if (data.path[i] == '/')
					{
						for (unsigned int j = 0; j <= i; j++)
						{
							textureNames[1].append(&data.path[j], 1);
						}

						textureNames[1] += filePath.data;

						break;
					}
				}
			}
		}

		m_meshes[i].Initialize(textureNames);
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
		deviceContext.SetMesh(m_meshes[i]);

		for (int j = 0; j < m_transforms.size(); j++)
		{
			if (m_modelPath == "../Models/Anubis/Anubis.fbx")
			{
				if (otherCamera.m_viewMatrix == camera.m_viewMatrix)
				{
					m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), camera.m_position),
					glm::radians(-camera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-camera.m_pitch), 
					VECTOR3(1.0f, 0.0f, 0.0f));
				}
				else
				{
					m_transforms[0] = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), otherCamera.m_position),
					glm::radians(-otherCamera.m_yaw + 90.0f), VECTOR3(0.0f, 1.0f, 0.0f)), glm::radians(-otherCamera.m_pitch),
						VECTOR3(1.0f, 0.0f, 0.0f));
				}
			}

			deviceContext.UpdateModelMatrix(shaderProgram.m_modelCB, *this, j);
		}

		deviceContext.Draw(m_meshes[i].m_numIndices);
	}
}
#endif
