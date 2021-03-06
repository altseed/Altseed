
#include "FBX2MDL.Base.h"

namespace FBX2MDL
{
	class FBXImporter
	{
		struct FbxWeight
		{
			float	Weight;
			int32_t	Index;

			FbxWeight()
				: Weight(0.0f)
				, Index(0x00)
			{
			}

			bool operator () (const FbxWeight& lhs, const FbxWeight& rhs)
			{
				return lhs.Weight > rhs.Weight;
			}
		};

		struct FbxVertexWeight
		{
			std::vector<FbxWeight> Data;
			int32_t Indexes[4];
			uint8_t Weights[4];

			FbxVertexWeight()
			{
				Indexes[0] = 0;
				Indexes[1] = 0;
				Indexes[2] = 0;
				Indexes[3] = 0;

				Weights[0] = 255;
				Weights[1] = 0;
				Weights[2] = 0;
				Weights[3] = 0;
			}
		};

		struct FbxFace
		{
			std::vector<asd::Model_IO::Vertex> Vertecies;
			int32_t MaterialIndex = -1;
		};

		struct VertexNormals
		{
			int32_t Count = 0;
			asd::Vector3DF Binormal;
			asd::Vector3DF Tangent;
		};

	private:
		/**
		@brief	接ベクトルを計算する。
		@param	v1	頂点1
		@param	v2	頂点2
		@param	v3	頂点3
		@param	binormal	従法線
		@param	tangent		接法線
		*/
		void CalcTangentSpace(const asd::Model_IO::Vertex& v1, const asd::Model_IO::Vertex& v2, const asd::Model_IO::Vertex& v3, asd::Vector3DF& binormal, asd::Vector3DF& tangent);

		asd::Vector3DF LoadPosition(FbxMesh* fbxMesh, int32_t ctrlPointIndex);
		asd::Vector3DF LoadNormal(FbxLayerElementNormal* normals, int32_t vertexID, int32_t ctrlPointIndex);
		asd::Vector2DF LoadUV(FbxMesh* fbxMesh, FbxLayerElementUV* uvs, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex);
		asd::Color LoadVertexColor(FbxMesh* fbxMesh, FbxLayerElementVertexColor* colors, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex);

		void LoadSkin(FbxMesh* fbxMesh, std::vector<BoneConnector>& boneConnectors, std::vector<FbxVertexWeight>& weights);
		void LoadMaterial(FbxMesh* fbxMesh, FbxLayerElementMaterial* materials, std::vector<Material>& dst);

		std::shared_ptr<Mesh> LoadMesh(FbxMesh* fbxMesh);

		std::shared_ptr<Node> LoadHierarchy(std::shared_ptr<Node> parent, FbxNode* fbxNode, FbxManager* fbxManager);

		asd::Matrix44 CalcMatrix(asd::RotationOrder order, float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz);

		void LoadAnimationSource(FbxAnimStack* fbxAnimStack, FbxNode* fbxRootNode, AnimationSource &animationSource);
		void LoadCurve(FbxNode* fbxNode, FbxAnimLayer* fbxAnimLayer, AnimationSource &animationSource);
		void AddConstant(asd::astring target, float value, AnimationSource &animationSource);
		void LoadCurve(asd::astring target, FbxAnimCurve* curve, AnimationSource &animationSource);
	public:

		std::shared_ptr<Scene> LoadScene(FbxScene* fbxScene, FbxManager* fbxManager);
	};
}