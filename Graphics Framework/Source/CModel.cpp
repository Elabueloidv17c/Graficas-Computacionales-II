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

bool CModel::Initialize(ModelData data, CDevice& device)
{
	m_modelPath = data.path;

	//Load mesh from file
	const aiScene* model = aiImportFile(data.path, aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_Triangulate);

	if (!model)
	{
		std::cout << "Error:  Scene not found" << std::endl;
		return false;
	}

	//Resize the software container
	m_meshes.resize(model->mNumMeshes);

	for (int i = 0; i < m_meshes.size(); i++)
	{
		//Load Indices
		m_meshes[i].m_indices.resize(model->mMeshes[i]->mNumFaces * 3);

		for (unsigned int j = 0; j < model->mMeshes[i]->mNumFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				m_meshes[i].m_indices[(3 * j) + k] = model->mMeshes[i]->mFaces[j].mIndices[2 - k];
			}
		}

		//Load vertices
		m_meshes[i].m_vertices.resize(model->mMeshes[i]->mNumVertices);
		aiVector3D* UVs = model->mMeshes[i]->mTextureCoords[0];

		for (unsigned int j = 0; j < m_meshes[i].m_vertices.size(); j++)
		{
			m_meshes[i].m_vertices[j].Position = VECTOR3{model->mMeshes[i]->mVertices[j].x, 
			model->mMeshes[i]->mVertices[j].y, model->mMeshes[i]->mVertices[j].z };

			if (model->mMeshes[i]->HasTextureCoords(0))
			{
				m_meshes[i].m_vertices[j].TexCoord = VECTOR2{ UVs[j].x, UVs[j].y };
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

		//In this point, the program just supports 2 texture types, diffuse and normal map
		std::vector <std::string> textureNames;
		textureNames.resize(2);

		const aiMaterial* material = model->mMaterials[model->mMeshes[i]->mMaterialIndex];

		//Diffuse
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString filePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL);

			std::string texture;
			texture = filePath.data;
			int format = texture.find(".tga");

			if (format >= 0)
			{
				texture.erase(format);
				texture += ".png";
			}

			if (filePath.length)
			{
				for (int j = strlen(data.path) - 1; j >= 0; j--)
				{
					if (data.path[j] == '/')
					{
						for (unsigned int k = 0; k <= j; k++)
						{
							textureNames[0].append(&data.path[k], 1);
						}

						textureNames[0] += texture;

						break;
					}
				}
			}
		}

		//Normals
		if (material->GetTextureCount(aiTextureType_NORMALS))
		{
			aiString filePath;
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
		else
		{
			textureNames[1] = textureNames[0];
			int replace = textureNames[1].find("a.png");

			if (replace >= 0)
			{
				textureNames[1].erase(replace);
				textureNames[1] += "n.png";
			}
		}

		//Create Mesh
		m_meshes[i].Initialize(textureNames);

#ifdef DIRECT_X
		device.CreateBuffers(m_meshes[i]);
		device.CreateTexture(m_meshes[i]);
		m_meshes[i].m_vertices.clear();
		m_meshes[i].m_indices.clear();
#endif
	}

	AddTransform(data.transform);

	return true;
}

void CModel::Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{
#ifdef DIRECT_X
	deviceContext.UpdateViewMatrix(shaderProgram.m_viewCB, camera);
	deviceContext.UpdateProjectionMatrix(shaderProgram.m_projectionCB, camera);

	for (int i = 0; i < m_meshes.size(); i++)
	{
		deviceContext.SetMesh(m_meshes[i]);

		for (int j = 0; j < m_transforms.size(); j++)
		{
			deviceContext.UpdateModelMatrix(shaderProgram.m_modelCB, *this, j);
		}

		deviceContext.Draw(m_meshes[i].m_numIndices);
	}
#endif
#ifdef OPEN_GL
	for (int i = 0; i < m_meshes.size(); i++)
	{
		for (int j = 0; j < m_transforms.size(); j++)
		{
			shaderProgram.UpdateModel(m_transforms[j]);
		}

		m_meshes[i].Render(shaderProgram.m_id);
	}
#endif
}

void InvertString(std::string& str)
{
	int n = str.length();

	for (int i = 0; i < n / 2; i++)
	{
		std::swap(str[i], str[n - i - 1]);
	}
}
