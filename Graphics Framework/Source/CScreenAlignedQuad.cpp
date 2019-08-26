#include "..\Header\CScreenAlignedQuad.h"
#include "..\Header\CDeviceContext.h"
#include "..\Header\CRenderTarget.h"
#include "..\Header\CDevice.h"

bool CScreenAlignedQuad::Initialize(const char* path, CDevice& device)
{
	m_path = path;

	//Load mesh from file
	const aiScene* model = aiImportFile(path, aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_Triangulate);

	if (!model)
	{
		std::cout << "Error:  Screen Aligned Quad Not Found" << std::endl;
		return false;
	}
	
	//Load Indices
	m_indices.resize(model->mMeshes[0]->mNumFaces * 3);

	for (unsigned int j = 0; j < model->mMeshes[0]->mNumFaces; j++)
	{
		for (unsigned int k = 0; k < 3; k++)
		{
			m_indices[(3 * j) + k] = model->mMeshes[0]->mFaces[j].mIndices[2 - k];
		}
	}

	//Load vertices
	m_vertices.resize(model->mMeshes[0]->mNumVertices);
	aiVector3D* UVs = model->mMeshes[0]->mTextureCoords[0];

	for (unsigned int j = 0; j < m_vertices.size(); j++)
	{
		m_vertices[j].Position = VECTOR3{ model->mMeshes[0]->mVertices[j].x,
		-model->mMeshes[0]->mVertices[j].y, model->mMeshes[0]->mVertices[j].z };

		if (model->mMeshes[0]->HasTextureCoords(0))
		{
			m_vertices[j].TexCoord = VECTOR2{ UVs[j].x, UVs[j].y };
		}
	}

	//Create Mesh
	m_numIndices = m_indices.size();
	m_numVertices = m_vertices.size();

	m_vertexBuffer.Initialize(m_vertices.data(), m_vertices.size());
	m_indexBuffer.Initialize(m_indices.data(), m_indices.size());

#ifdef DIRECT_X
	device.CreateBuffers(*this);
	m_vertices.clear();
	m_indices.clear();
#endif

	return true;
}

void CScreenAlignedQuad::Render(CDeviceContext& deviceContext, std::vector <CRenderTarget*>& renderTargets)
{
	deviceContext.SetScreenAlignedQuad(*this, renderTargets);
	deviceContext.Draw(m_numIndices);
}

void CScreenAlignedQuad::SetBrightData(BrightData data)
{
	m_brightData.BloomThreshold = data.BloomThreshold;
	m_brightData.LevelOfDetail = data.LevelOfDetail;
}

void CScreenAlignedQuad::SetBlurHData(BlurHData data)
{
	m_blurHData.LevelOfDetail = data.LevelOfDetail;
	m_blurHData.ViewportDimensions = data.ViewportDimensions;
}

void CScreenAlignedQuad::SetBlurVData(BlurVData data)
{
	 m_blurVData.LevelOfDetail = data.LevelOfDetail;
	 m_blurVData.ViewportDimensions = data.ViewportDimensions;
}

void CScreenAlignedQuad::SetAddBrightData(AddBrightData data)
{
	m_addBrightData.LevelOfDetail = data.LevelOfDetail;
}

void CScreenAlignedQuad::SetToneMapData(ToneMapData data)
{
	m_toneMapData.BloomMultiplier = data.BloomMultiplier;
	m_toneMapData.Exposure = data.Exposure;
}

void CScreenAlignedQuad::Update(Size window)
{
	m_blurHData.ViewportDimensions = window;
	m_blurVData.ViewportDimensions = window;
}
