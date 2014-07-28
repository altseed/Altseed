
#pragma once

#include "../../../ace.Core.Base_Imp.h"

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "ace.Mesh.h"

namespace ace
{
	class Mesh_Imp
		: public Mesh
		, public ReferenceObject
	{
	public:
		struct Material
		{
			std::shared_ptr<Texture2D>	ColorTexture;
			std::shared_ptr<Texture2D>	NormalTexture;
			std::shared_ptr<Texture2D>	SpecularTexture;

			Material();
			~Material();
		};

		struct MaterialOffset
		{
			int32_t		MaterialIndex;
			int32_t		FaceOffset;
		};

		struct BoneConnector
		{
			int32_t		TargetIndex;
			Matrix44	BoneToMesh;
		};

		struct Vertex
		{
			Vector3DF	Position;
			Vector3DF	Normal;
			Vector3DF	Binormal;
			Vector2DF	UV1;
			Vector2DF	UV2;
			Color		VColor;
			int32_t		BoneWeights;
			int32_t		BoneIndexes;
			int32_t		BoneIndexesOriginal;
		};

		struct Face
		{
			int32_t		Index1;
			int32_t		Index2;
			int32_t		Index3;
			int32_t		MaterialIndex;
		};

		struct DividedMesh
		{
			std::shared_ptr<VertexBuffer_Imp>	VertexBufferPtr;
			std::shared_ptr<IndexBuffer_Imp>	IndexBufferPtr;
			std::vector<Vertex>					VertexBufferOnMM;
			std::vector<Face>					FaceBufferOnMM;
			std::vector<BoneConnector>			BoneConnectors;
			std::vector<MaterialOffset>			MaterialOffsets;
		};

	private:
		Graphics_Imp*	m_graphics;

		std::vector<DividedMesh>			m_dividedMeshes;
		std::vector<Vertex>					m_vertexBufferOnMM;
		std::vector<Face>					m_faceBufferOnMM;
		std::vector<BoneConnector>			m_boneConnectors;

		std::vector<Material>	m_materials;

		Mesh_Imp(Graphics* graphics);
		virtual ~Mesh_Imp();

	public:

#if !SWIG
		static Mesh_Imp* Create(Graphics* graphics);
#endif

		void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneWeights,
			int32_t boneIndexes) override;

		void AddFace(int32_t index1, int32_t index2, int32_t index3, int32_t materialIndex) override;

		void AddBoneConnector(int32_t targetIndex, const Matrix44& boneToMesh) override;

		/**
			@brief	直接内部形式を読み込む。
			@note
			基本的にファイル読み込み時専用である。FaceのMaterialIndexを設定する必要はない。
		*/
		void AddDividedMesh(const std::vector<Vertex>& vertexes, const std::vector<Face>& faces, const std::vector<BoneConnector>& boneConnectors, const std::vector<MaterialOffset>& materialOffsets);

		int32_t AddMaterial() override;
		void SendToGPUMemory() override;

		void SetColorTexture(int32_t materialIndex, Texture2D* texture) override;
		void SetNormalTexture(int32_t materialIndex, Texture2D* texture) override;
		void SetSpecularTexture(int32_t materialIndex, Texture2D* texture) override;

		Material* GetMaterial(int32_t materialIndex);


#if !SWIG
		std::vector<DividedMesh>& GetDvidedMeshes() { return m_dividedMeshes; }
#endif

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
};