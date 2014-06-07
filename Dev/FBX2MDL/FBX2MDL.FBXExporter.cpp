
#include "FBX2MDL.FBXExporter.h"

namespace FBX2MDL
{
	void FBXExporter::WriteHeader()
	{
		m_writer->Push((uint8_t)'M');
		m_writer->Push((uint8_t)'D');
		m_writer->Push((uint8_t)'L');
		m_writer->Push((uint8_t) 0);

		m_writer->Push(1);
	}

	void FBXExporter::WriteAnimationSource(AnimationSource& source)
	{
		m_writer->Push(source.Name);
		m_writer->Push((int32_t) source.keyFrameAnimations.size());

		for (auto& anim : source.keyFrameAnimations)
		{
			m_writer->Push(anim.TargetName);
			m_writer->Push((int32_t)anim.KeyFrames.size());

			for (auto& keyframe : anim.KeyFrames)
			{
				m_writer->Push(keyframe.KeyValue);
				m_writer->Push(keyframe.LeftPosition);
				m_writer->Push(keyframe.RightPosition);
				m_writer->Push(keyframe.Interpolation);
			}
		}
	}

	void FBXExporter::WriteAnimationSources(std::vector<AnimationSource>& animationSources)
	{
		m_writer->Push((int32_t) animationSources.size());
		for (auto& animationSource : animationSources)
		{
			WriteAnimationSource(animationSource);
		}
	}

	void FBXExporter::WriteMaterial(const Material& material)
	{
		int32_t external = 0;
		m_writer->Push(external);
		m_writer->Push(material.DiffuseTexturePath);
		m_writer->Push(ace::ToAString("").c_str());
		m_writer->Push(ace::ToAString("").c_str());
	}

	void FBXExporter::WriteMaterials(const std::vector<Material>& materials)
	{
		int32_t count = (int32_t)materials.size();
		m_writer->Push(count);

		for (const auto& material : materials)
		{
			WriteMaterial(material);
		}
	}

	void FBXExporter::WriteMesh(const std::shared_ptr<Mesh>& mesh)
	{
		auto vcount = (int32_t)mesh->Vertexes.size();
		m_writer->Push(vcount);
		for (const auto& v : mesh->Vertexes)
		{
			m_writer->Push(v.Position);
			m_writer->Push(v.Normal);
			m_writer->Push(v.Binormal);
			m_writer->Push(v.UV);
			m_writer->Push(v.SubUV);
			m_writer->Push(v.Color.R);
			m_writer->Push(v.Color.G);
			m_writer->Push(v.Color.B);
			m_writer->Push(v.Color.A);
			
			for (const auto& w : v.Weight)
			{
				m_writer->Push(w);
			}
			
			for (const auto& w : v.WeightIndexOriginal)
			{
				m_writer->Push(w);
			}

			for (const auto& w : v.WeightIndexDivided)
			{
				m_writer->Push(w);
			}
		}

		auto fcount = (int32_t) mesh->Faces.size();
		m_writer->Push(fcount);

		for (const auto& f : mesh->Faces)
		{
			m_writer->Push(f.Index[0]);
			m_writer->Push(f.Index[1]);
			m_writer->Push(f.Index[2]);
		}
	}


	std::vector<std::shared_ptr<Mesh>> FBXExporter::GetMeshes(const std::shared_ptr<Node>& node)
	{
		std::vector<std::shared_ptr<Mesh>> dst;
		if (node->MeshParameter != nullptr)
		{
			dst.push_back(node->MeshParameter);
		}

		for (const auto& node_ : node->Children)
		{
			auto dst_ = GetMeshes(node_);
			dst.insert(dst.end(), dst_.begin(), dst_.end());
		}

		return dst;
	}

	std::vector<std::function<void()>> FBXExporter::GetWritingDeformerFuncs(const std::shared_ptr<Node>& node, int32_t parentIndex, int32_t& currentIndex)
	{
		std::vector<std::function<void()>> dst;

		auto func = [this, node, parentIndex]()-> void
		{
			m_writer->Push(node->Name);
			m_writer->Push(parentIndex);
			m_writer->Push((int32_t) node->RotationOrder);
			m_writer->Push(node->LclMatrix);
		};

		dst.push_back(func);

		auto current = currentIndex;
		for (const auto& node_ : node->Children)
		{
			currentIndex++;
			auto dst_ = GetWritingDeformerFuncs(node_, current, currentIndex);
			dst.insert(dst.end(), dst_.begin(), dst_.end());
		}

		return dst;
	}

	std::map<ace::astring, int32_t> FBXExporter::GetDeformerNameToIndexes(const std::shared_ptr<Node>& node, int32_t& currentIndex)
	{
		std::map<ace::astring, int32_t> dst;

		dst[node->Name] = currentIndex;

		auto current = currentIndex;
		for (const auto& node_ : node->Children)
		{
			currentIndex++;
			auto dst_ = GetDeformerNameToIndexes(node_, currentIndex);
			dst.insert(dst_.begin(), dst_.end());
		}

		return dst;
	}

	FBXExporter::FBXExporter()
	{
	}

	FBXExporter::~FBXExporter()
	{
	}


	std::shared_ptr<ace::BinaryWriter> FBXExporter::Export(std::shared_ptr<Scene> scene)
	{
		auto writer = std::make_shared<ace::BinaryWriter>();
		m_writer = writer;

		WriteHeader();

		// デフォーマー出力
		{
			int32_t currentIndex = 0;
			auto deformerFuncs = GetWritingDeformerFuncs(scene->Root, -1, currentIndex);
			m_writer->Push((int32_t)deformerFuncs.size());
			for (auto& func : deformerFuncs)
			{
				func();
			}
		}

		// デフォーマー情報取得
		std::map<ace::astring, int32_t> deformer_name2ind;
		{
			int32_t currentIndex = 0;
			deformer_name2ind = GetDeformerNameToIndexes(scene->Root, currentIndex);
		}

		// メッシュ編集
		auto meshes = GetMeshes(scene->Root);
		m_writer->Push((int32_t) meshes.size());
		for (auto& mesh : meshes)
		{
			// 上下方向に面ソート
			std::sort(mesh->Faces.begin(), mesh->Faces.end(), [mesh](const Face& f1, const Face& f2) -> float {
				return
					(mesh->Vertexes[f1.Index[0]].Position.Y +
					mesh->Vertexes[f1.Index[1]].Position.Y +
					mesh->Vertexes[f1.Index[2]].Position.Y) < 
					(mesh->Vertexes[f2.Index[0]].Position.Y +
					mesh->Vertexes[f2.Index[1]].Position.Y +
					mesh->Vertexes[f2.Index[2]].Position.Y);
			});

			// ボーン数による分割
			// 現在省略につきボーン数32以上は落ちる。

			// 材質ソート
			std::sort(mesh->Faces.begin(), mesh->Faces.end(), [](const Face& f1, const Face& f2) -> int32_t { return f1.MaterialIndex > f2.MaterialIndex; });

			// 分割済頂点
			int32_t divided = 1;
			m_writer->Push(divided);
			{
				// 頂点に情報書き込み
				for (auto i = 0; i < mesh->Vertexes.size(); i++)
				{
					auto& v = mesh->Vertexes[i];
					v.WeightIndexDivided[0] = v.WeightIndexOriginal[0];
					v.WeightIndexDivided[1] = v.WeightIndexOriginal[1];
					v.WeightIndexDivided[2] = v.WeightIndexOriginal[2];
					v.WeightIndexDivided[3] = v.WeightIndexOriginal[3];
				}

				// メッシュ情報出力
				WriteMesh(mesh);

				// 材質面出力
				{
					std::vector<MaterialOffset> materialFaces;
					int32_t materialIndex = -1;
					int32_t materialCount = 0;
					for (const auto& face : mesh->Faces)
					{
						if (materialIndex == face.MaterialIndex)
						{
							materialCount++;
						}
						else
						{
							if (materialCount > 0)
							{
								MaterialOffset mo;
								mo.MaterialIndex = materialIndex;
								mo.FaceOffset = materialCount;
								materialFaces.push_back(mo);
								materialCount = 0;
							}

							materialCount++;
							materialIndex = face.MaterialIndex;
						}
					}

					if (materialCount > 0)
					{
						MaterialOffset mo;
						mo.MaterialIndex = materialIndex;
						mo.FaceOffset = materialCount;
						materialFaces.push_back(mo);
					}

					m_writer->Push((int32_t) materialFaces.size());
					for (auto& mf : materialFaces)
					{
						m_writer->Push(mf.MaterialIndex);
						m_writer->Push(mf.FaceOffset);
					}
				}

				// ボーン情報出力
				m_writer->Push((int32_t) mesh->BoneConnectors.size());
				for (auto& bone : mesh->BoneConnectors)
				{
					auto id = deformer_name2ind[bone.Name];
					m_writer->Push((int32_t) id);
					m_writer->Push(bone.OffsetMatrix);
				}

			}

			// 材質出力
			WriteMaterials(mesh->Materials);
		}

		// アニメーション出力
		std::remove_if(
			scene->AnimationSources.begin(), 
			scene->AnimationSources.end(),
			[](const AnimationSource& source) {
			return source.keyFrameAnimations.size() == 0;
		});

		// アニメーションソース出力
		WriteAnimationSources(scene->AnimationSources);

		// アニメーションクリップ出力
		{
			m_writer->Push((int32_t) scene->AnimationSources.size());
			for (auto i = 0; i < scene->AnimationSources.size(); i++)
			{
				m_writer->Push(scene->AnimationSources[i].Name);
				m_writer->Push(i);
			}
		}
		m_writer.reset();
		return writer;
	}
}