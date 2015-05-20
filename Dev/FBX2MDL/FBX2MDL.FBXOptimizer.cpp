
#include "FBX2MDL.FBXOptimizer.h"

namespace FBX2MDL
{
	bool FBXOptimizer::HasEnabledBone(std::map<ace::astring, int32_t>& boneCounts, std::vector<std::shared_ptr<Node>>& children)
	{
		for (auto child : children)
		{
			if (HasEnabledBone(boneCounts, child->Children))
			{
				return true;
			}

			if (boneCounts.count(child->Name) > 0) return true;
		}

		return false;
	}

	void FBXOptimizer::RemoveDisabledBone(std::map<ace::astring, int32_t>& boneCounts, std::shared_ptr<Node>& bone)
	{
		std::vector<std::shared_ptr<Node>> children;

		for (size_t i = 0; i < bone->Children.size(); i++)
		{
			auto child = bone->Children[i];

			if (boneCounts.count(child->Name) > 0)
			{
				children.push_back(child);
			}
			else if (HasEnabledBone(boneCounts, child->Children))
			{
				children.push_back(child);
			}
		}

		bone->Children = children;

		for (size_t i = 0; i < bone->Children.size(); i++)
		{
			RemoveDisabledBone(boneCounts, bone->Children[i]);
		}
	}

	void FBXOptimizer::OptimizeArmature(std::shared_ptr<Scene> scene, std::shared_ptr<Node> armature, std::vector<std::shared_ptr<Node>> meshes)
	{
		std::map<ace::astring, int32_t> boneCounts;

		// 使用されているボーン名を取得
		for (auto& mesh : meshes)
		{
			for (auto v : mesh->MeshParameter->Vertexes)
			{
				for (int32_t i = 0; i < 4; i++)
				{
					if (v.BoneWeights[i] > 0)
					{
						auto name = mesh->MeshParameter->BoneConnectors[v.BoneIndexesOriginal[i]].Name;

						if (boneCounts.count(name) == 0)
						{
							boneCounts[name] = 0;
						}

						boneCounts[name]++;
					}
				}
			}
		}

		// ボーン名整理
		{
			std::map<ace::astring, int32_t> bc;
			for (auto it : boneCounts)
			{
				if (it.second > 0)
				{
					bc[it.first] = it.second;
				}
			}
			boneCounts = bc;
		}

		// ボーンコネクタを書き換え
		for (auto& mesh : meshes)
		{
			auto param = mesh->MeshParameter;
			std::map<int32_t, int32_t> boneIndexPair;

			int32_t oldInd = 0;
			int32_t newInd = 0;

			std::vector<BoneConnector> bc;
			for (auto& bc_ : param->BoneConnectors)
			{
				if (boneCounts.count(bc_.Name) > 0)
				{
					boneIndexPair[oldInd] = newInd;
					bc.push_back(bc_);
					newInd++;
				}
				oldInd++;
			}

			param->BoneConnectors = bc;

			// メッシュ内部走査
			for (size_t i = 0; i < param->Vertexes.size(); i++)
			{
				param->Vertexes[i].BoneIndexesOriginal[0] = boneIndexPair[param->Vertexes[i].BoneIndexesOriginal[0]];
				param->Vertexes[i].BoneIndexesOriginal[1] = boneIndexPair[param->Vertexes[i].BoneIndexesOriginal[1]];
				param->Vertexes[i].BoneIndexesOriginal[2] = boneIndexPair[param->Vertexes[i].BoneIndexesOriginal[2]];
				param->Vertexes[i].BoneIndexesOriginal[3] = boneIndexPair[param->Vertexes[i].BoneIndexesOriginal[3]];
			}
		}

		// ボーン削除
		RemoveDisabledBone(boneCounts, armature);
	}

	void FBXOptimizer::Optimize(std::shared_ptr<Scene> scene)
	{
		auto root = scene->Root;
		std::shared_ptr<Node> armature;
		std::vector<std::shared_ptr<Node>> meshes;

		for (auto child : root->Children)
		{
			if (child->Name == ace::ToAString("Armature"))
			{
				armature = child;
			}
			else if (child->MeshParameter != nullptr && child->MeshParameter->BoneConnectors.size() > 0)
			{
				meshes.push_back(child);
			}
		}

		if (meshes.size() > 0)
		{
			OptimizeArmature(scene, armature, meshes);
		}
	}
}

