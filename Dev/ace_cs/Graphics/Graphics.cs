using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Graphics
	{
		swig.Graphics_Imp graphics;

		internal Graphics(swig.Graphics_Imp swig)
		{
			graphics = swig;
		}

		/// <summary>
		/// テクスチャを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>テクスチャ</returns>
		public Texture2D CreateTexture2D(string path)
		{
			var texture = graphics.CreateTexture2D_Imp(path);
			return GC.GenerateTexture2D(texture, GC.GenerationType.Create);
		}

		/// <summary>
		/// テクスチャを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>テクスチャ</returns>
		/// <remarks>
		/// 画素がRGB空間にマッピングされている時に使用する。(ノーマルマップ等)
		/// </remarks>
		public Texture2D CreateTexture2DAsRawData(string path)
		{
			var texture = graphics.CreateTexture2DAsRawData_Imp(path);
			return GC.GenerateTexture2D(texture, GC.GenerationType.Create);
		}

		/// <summary>
		/// 空のテクスチャを生成する。
		/// </summary>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="format">フォーマット</param>
		/// <returns>テクスチャ</returns>
		public Texture2D CreateEmptyTexture2D(int width, int height, TextureFormat format)
		{
			var texture = graphics.CreateEmptyTexture2D_Imp(width, height, (swig.TextureFormat)format);
			return GC.GenerateTexture2D(texture, GC.GenerationType.Create);
		}

		/// <summary>
		/// 描画先として指定可能なテクスチャを生成する。
		/// </summary>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="format">フォーマット</param>
		/// <returns>テクスチャ</returns>
		public RenderTexture2D CreateRenderTexture2D(int width, int height, TextureFormat format)
		{
			var rt = graphics.CreateRenderTexture2D_Imp(width,height, (swig.TextureFormat)format);
			var p = rt.GetPtr();

			var existing = GC.Texture2Ds.GetObject(p);
			if (existing != null)
			{
				return (RenderTexture2D)existing;
			}

			var ret = new RenderTexture2D(rt);
			GC.Texture2Ds.AddObject(p, ret);
			return ret;
		}

		/// <summary>
		/// 6枚の画像ファイルからキューブマップテクスチャを生成する。
		/// </summary>
		/// <param name="front">前方向の画像ファイルへの相対パス</param>
		/// <param name="left">左方向の画像ファイルへの相対パス</param>
		/// <param name="back">後ろ方向の画像ファイルへの相対パス</param>
		/// <param name="right">右方向の画像ファイルへの相対パス</param>
		/// <param name="top">上方向の画像ファイルへの相対パス</param>
		/// <param name="bottom">下方向の画像ファイルへの相対パス</param>
		/// <returns>キューブマップ</returns>
		public CubemapTexture CreateCubemapTextureFrom6ImageFiles(string front, string left, string back, string right, string top, string bottom)
		{
			return GC.GenerateCubemapTexture(graphics.CreateCubemapTextureFrom6ImageFiles_(front, left, back, right, top, bottom), GC.GenerationType.Create);
		}

		/// <summary>
		/// 複数の画像ファイルからミップマップ付のキューブマップテクスチャを生成する。
		/// </summary>
		/// <param name="path">ファイルの名称の先頭</param>
		/// <param name="mipmapCount">ミップマップ数</param>
		/// <returns>キューブマップ</returns>
		public CubemapTexture CreateCubemapTextureFromMipmapImageFiles(string path, int mipmapCount)
		{
			return GC.GenerateCubemapTexture(graphics.CreateCubemapTextureFromMipmapImageFiles_(path, mipmapCount), GC.GenerationType.Create);
		}

		/// <summary>
		/// 1つのファイルからミップマップ付のキューブマップテクスチャを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>キューブマップ</returns>
		public CubemapTexture CreateCubemapTextureFromSingleImageFile(string path)
		{
			return GC.GenerateCubemapTexture(graphics.CreateCubemapTextureFromSingleImageFile_(path), GC.GenerationType.Create);
		}

		/// <summary>
		/// シェーダー(2D)を生成する。
		/// </summary>
		/// <param name="shaderText">シェーダーのコード</param>
		/// <returns></returns>
		public Shader2D CreateShader2D(string shaderText)
		{
			var shader = graphics.CreateShader2D_Imp(shaderText);
			if (shader == null) return null;

			var p = shader.GetPtr();

			var existing = GC.Shader2Ds.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Shader2D(shader);
			GC.Shader2Ds.AddObject(p, ret);
			return ret;
		}

		/// <summary>
		/// マテリアル(2D)を生成する。
		/// </summary>
		/// <param name="shader">シェーダー</param>
		/// <returns>マテリアル(2D)</returns>
		public Material2D CreateMaterial2D(Shader2D shader)
		{
			var material = graphics.CreateMaterial2D_(shader.SwigObject);
			var p = material.GetPtr();

			var existing = GC.Material2Ds.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Material2D(material);
			GC.Material2Ds.AddObject(p, ret);
			return ret;
		}

		/// <summary>
		/// シェーダー(3D)を生成する。
		/// </summary>
		/// <param name="shaderText">シェーダーのコード</param>
		/// <returns></returns>
		public Shader3D CreateShader3D(string shaderText)
		{
			var shader = graphics.CreateShader3D_(shaderText);
			return GC.GenerateShader3D(shader, GC.GenerationType.Create);
		}

		/// <summary>
		/// マテリアル(3D)を生成する。
		/// </summary>
		/// <param name="shader">シェーダー</param>
		/// <returns>マテリアル(3D)</returns>
		public Material3D CreateMaterial3D(Shader3D shader)
		{
			var material = graphics.CreateMaterial3D_(shader.SwigObject);
			return GC.GenerateMaterial3D(material, GC.GenerationType.Create);
		}

		/// <summary>
		/// マテリアルプロパティブロックを生成する。
		/// </summary>
		/// <returns>マテリアルプロパティブロック</returns>
		public MaterialPropertyBlock CreateMaterialPropertyBlock()
		{
			return GC.GenerateMaterialPropertyBlock(graphics.CreateMaterialPropertyBlock_(), GC.GenerationType.Create);
		}

		/// <summary>
		/// メッシュを生成する。
		/// </summary>
		/// <returns>メッシュ</returns>
		public Mesh CreateMesh()
		{
			return GC.GenerateMesh(graphics.CreateMesh_(), GC.GenerationType.Create);
		}

		/// <summary>
		/// デフォーマーを生成する。
		/// </summary>
		/// <returns>デフォーマー</returns>
		public Deformer CreateDeformer()
		{
			return GC.GenerateDeformer(graphics.CreateDeformer_(), GC.GenerationType.Create);
		}

		/// <summary>
		/// モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>モデル</returns>
		public Model CreateModel(string path)
		{
			var model = graphics.CreateModel_(path);
			if (model == null) return null;
			return GC.GenerateModel(model, GC.GenerationType.Create);
		}

		/// <summary>
		/// 大量描画用モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>大量描画用モデル</returns>
		public MassModel CreateMassModelFromModelFile(string path)
		{
			var model = graphics.CreateMassModelFromModelFile_(path);
			if (model == null) return null;
			return GC.GenerateMassModel(model, GC.GenerationType.Create);
		}

		/// <summary>
		/// 大量描画用モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>大量描画用モデル</returns>
		public MassModel CreateMassModel(string path)
		{
			var model = graphics.CreateMassModel_(path);
			if (model == null) return null;
			return GC.GenerateMassModel(model, GC.GenerationType.Create);
		}

		/// <summary>
		/// 地形を生成する。
		/// </summary>
		/// <returns>地形</returns>
		public Terrain3D CreateTerrain3D()
		{
			var terrain = graphics.CreateTerrain3D_();
			if (terrain == null) return null;
			return GC.GenerateTerrain3D(terrain, GC.GenerationType.Create);
		}

		/// <summary>
		/// エフェクトを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>エフェクト</returns>
		public Effect CreateEffect(string path)
		{
			var effect = graphics.CreateEffect_(path);
			if (effect == null) return null;
			return GC.GenerateEffect(effect, GC.GenerationType.Create);
		}

        /// <summary>
        /// フォントを生成する。
        /// </summary>
        /// <param name="path">パス</param>
        /// <returns>フォント</returns>
        public Font CreateFont(string path)
        {
            var font = graphics.CreateFont_(path);
            if (font == null) return null;
            return GC.GenerateFont(font, GC.GenerationType.Create);
        }

        /// <summary>
        /// 二次元チップを生成する。
        /// </summary>
        /// <returns>二次元チップ</returns>
        public Chip2D CreateChip2D()
        {
            var chip = graphics.CreateChip2D_();
            if (chip == null) return null;
            return GC.GenerateChip2D(chip, GC.GenerationType.Create);
        }

		/// <summary>
		/// 画像パッケージを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>画像パッケージ</returns>
		public ImagePackage CreateImagePackage(string path)
		{
			 var ip = graphics.CreateImagePackage_(path);
            if (ip == null) return null;
            return GC.GenerateImagePackage(ip, GC.GenerationType.Create);
		}

		/// <summary>
		/// 1フレーム間に実行された描画命令の回数を取得する。
		/// </summary>
		/// <remarks>
		/// 現在、エフェクトの描画回数はカウントされない。
		/// </remarks>
		int DrawCallCount
		{
			get { return graphics.GetDrawCallCount(); }
		}

		/// <summary>
		/// 現在使用済みのVRAM容量を取得する。
		/// </summary>
		/// <remarks>
		/// 値は推測値である。
		/// </remarks>
		public int UsedVRAMSize
		{
			get
			{
				return graphics.GetUsedVRAMSize();
			}
		}

		/// <summary>
		/// 描画ランタイムの種類を取得する。
		/// </summary>
		public GraphicsDeviceType GraphicsDeviceType
		{
			get { return (GraphicsDeviceType)graphics.GetGraphicsDeviceType(); }
		}
	}
}
