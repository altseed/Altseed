using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Graphics
	{
		internal Graphics(swig.Graphics_Imp nativeInstance)
		{
			CoreInstance = nativeInstance;
		}

		/// <summary>
		/// テクスチャを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>テクスチャ</returns>
		public Texture2D CreateTexture2D(string path)
		{
			var texture = CoreInstance.CreateTexture2D_Imp(path);
			return GC.GenerateTexture2D(texture, GenerationType.Create);
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
			var texture = CoreInstance.CreateTexture2DAsRawData_Imp(path);
			return GC.GenerateTexture2D(texture, GenerationType.Create);
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
			var texture = CoreInstance.CreateEmptyTexture2D_Imp(width, height, (swig.TextureFormat)format);
			return GC.GenerateTexture2D(texture, GenerationType.Create);
		}

		/// <summary>
		/// 編集可能なテクスチャを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>テクスチャ</returns>
		/// <remarks>
		/// 読み込める画像形式はPNGのみ。
		/// </remarks>
		public Texture2D CreateEditableTexture2D(string path)
		{
			var texture = CoreInstance.CreateEditableTexture2D_Imp(path);
			return GC.GenerateTexture2D(texture, GenerationType.Create);
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
			var rt = CoreInstance.CreateRenderTexture2D_Imp(width,height, (swig.TextureFormat)format);
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
		/// シェーダー(2D)を生成する。
		/// </summary>
		/// <param name="shaderText">シェーダーのコード</param>
		/// <returns></returns>
		public Shader2D CreateShader2D(string shaderText)
		{
			var shader = CoreInstance.CreateShader2D_(shaderText);
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
			var material = CoreInstance.CreateMaterial2D_(shader.CoreInstance);
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
			var shader = CoreInstance.CreateShader3D_(shaderText);
			return GC.GenerateShader3D(shader, GenerationType.Create);
		}

		/// <summary>
		/// マテリアル(3D)を生成する。
		/// </summary>
		/// <param name="shader">シェーダー</param>
		/// <returns>マテリアル(3D)</returns>
		public Material3D CreateMaterial3D(Shader3D shader)
		{
			var material = CoreInstance.CreateMaterial3D_(shader.CoreInstance);
			return GC.GenerateMaterial3D(material, GenerationType.Create);
		}

		/// <summary>
		/// マテリアルプロパティブロックを生成する。
		/// </summary>
		/// <returns>マテリアルプロパティブロック</returns>
		public MaterialPropertyBlock CreateMaterialPropertyBlock()
		{
			return GC.GenerateMaterialPropertyBlock(CoreInstance.CreateMaterialPropertyBlock_(), GenerationType.Create);
		}

		/// <summary>
		/// メッシュを生成する。
		/// </summary>
		/// <returns>メッシュ</returns>
		public Mesh CreateMesh()
		{
			return GC.GenerateMesh(CoreInstance.CreateMesh_(), GenerationType.Create);
		}

		/// <summary>
		/// デフォーマーを生成する。
		/// </summary>
		/// <returns>デフォーマー</returns>
		public Deformer CreateDeformer()
		{
			return GC.GenerateDeformer(CoreInstance.CreateDeformer_(), GenerationType.Create);
		}

		/// <summary>
		/// モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>モデル</returns>
		public Model CreateModel(string path)
		{
			var model = CoreInstance.CreateModel_(path);
			if (model == null) return null;
			return GC.GenerateModel(model, GenerationType.Create);
		}

		/// <summary>
		/// 大量描画用モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>大量描画用モデル</returns>
		public MassModel CreateMassModelFromModelFile(string path)
		{
			var model = CoreInstance.CreateMassModelFromModelFile_(path);
			if (model == null) return null;
			return GC.GenerateMassModel(model, GenerationType.Create);
		}

		/// <summary>
		/// 大量描画用モデルを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>大量描画用モデル</returns>
		public MassModel CreateMassModel(string path)
		{
			var model = CoreInstance.CreateMassModel_(path);
			if (model == null) return null;
			return GC.GenerateMassModel(model, GenerationType.Create);
		}

		/// <summary>
		/// 地形を生成する。
		/// </summary>
		/// <returns>地形</returns>
		public Terrain3D CreateTerrain3D()
		{
			var terrain = CoreInstance.CreateTerrain3D_();
			if (terrain == null) return null;
			return GC.GenerateTerrain3D(terrain, GenerationType.Create);
		}

		/// <summary>
		/// エフェクトを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>エフェクト</returns>
		public Effect CreateEffect(string path)
		{
			var effect = CoreInstance.CreateEffect_(path);
			if (effect == null) return null;
			return GC.GenerateEffect(effect, GenerationType.Create);
		}

        /// <summary>
        /// フォントを生成する。
        /// </summary>
        /// <param name="path">パス</param>
        /// <returns>フォント</returns>
        public Font CreateFont(string path)
        {
            var font = CoreInstance.CreateFont_(path);
            if (font == null) return null;
            return GC.GenerateFont(font, GenerationType.Create);
        }

		/// <summary>
		/// 必要に応じて動的に生成されるフォントを生成する。
		/// </summary>
		/// <param name="font">フォント名/フォントパス</param>
		/// <param name="fontSize">フォントサイズ</param>
		/// <param name="color">フォントの色</param>
		/// <param name="outlineSize">外枠の太さ</param>
		/// <param name="outlineColor">外枠の色</param>
		/// <returns>フォント</returns>
		/// <remarks>
		/// 文字を表示する時に必要な文字の分だけフォントを生成するフォントクラスを生成する。
		/// fontには、フォント名、もしくはフォントファイルへのパスを指定する。
		/// 何もfontに文字を指定しない、もしくはnullだと標準フォントが使用される。
		/// 事前に専用のフォントファイルを用意する必要はないが、アプリケーションを実行する環境に指定したフォントが存在する必要がある。
		/// </remarks>
		public Font CreateDynamicFont(string font, int fontSize, Color color, int outlineSize, Color outlineColor)
		{
			var font_ = CoreInstance.CreateDynamicFont_(font, fontSize, color, outlineSize, outlineColor);
			if (font_ == null) return null;
			return GC.GenerateFont(font_, GenerationType.Create);
		}

		/// <summary>
		/// 画像パッケージを生成する。
		/// </summary>
		/// <param name="path">パス</param>
		/// <returns>画像パッケージ</returns>
		public ImagePackage CreateImagePackage(string path)
		{
			 var ip = CoreInstance.CreateImagePackage_(path);
            if (ip == null) return null;
            return GC.GenerateImagePackage(ip, GenerationType.Create);
		}

		/// <summary>
		/// メディアプレイヤーを生成する。
		/// </summary>
		/// <returns>メディアプレイヤー</returns>
		public MediaPlayer CreateMediaPlayer()
		{
			var ip = CoreInstance.CreateMediaPlayer_();
			if (ip == null) return null;
			return GC.GenerateMediaPlayer(ip, GenerationType.Create);
		}

		/// <summary>
		/// 1つのファイルからミップマップ付のキューブマップテクスチャを生成する。
		/// </summary>
		/// <param name="path">画像ファイルへの相対パス</param>
		/// <returns>キューブマップ</returns>
		/// <remarks>
		/// 読み込める画像形式はDDSのみ。
		/// </remarks>
		public CubemapTexture CreateCumemapTexture(string path)
		{
			var ip = CoreInstance.CreateCubemapTexture_(path);
			if (ip == null) return null;
			return GC.GenerateCubemapTexture(ip, GenerationType.Create);
		}

	/// <summary>
	/// 描画ランタイムの種類を取得する。
	/// </summary>
	public GraphicsDeviceType GraphicsDeviceType
		{
			get { return (GraphicsDeviceType)CoreInstance.GetGraphicsDeviceType(); }
		}
	}
}
