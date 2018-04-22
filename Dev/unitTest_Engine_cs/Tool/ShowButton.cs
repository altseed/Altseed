using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Tool
{
	class ShowButton : EngineTest
	{
		private asd.TextureObject2D obj;
		private sbyte[] inputText = new sbyte[128];
		private int[] inputInt = new int[] { 99 };
		private float[] color = new float[4] { 0, 0, 0, 0 };
		private int[] selection = new int[1];
		private string list;
		private asd.Texture2D texture;

		public ShowButton() : base(600)
		{
			var listArray = new string[4]
			{
				"Plain Propton",
				"Raster Road",
				"Octern Ocean",
				"Gigant Ground",
			};
			list = string.Join(";", listArray);
		}

		protected override void OnStart()
		{
			obj = new asd.TextureObject2D()
			{
				Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png"),
				Position = new asd.Vector2DF(10, 10),
				IsDrawn = false,
			};
			asd.Engine.AddObject2D(obj);

			asd.Engine.OpenTool();

			texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
		}

		protected override void OnUpdating()
		{
			asd.Engine.Tool.Begin("ShowButton");

			{
				asd.Engine.Tool.Text("It's text!");

				if (asd.Engine.Tool.Button("Push me!"))
				{
					obj.IsDrawn = true;
				}

				/*
				asd.Engine.Tool.BeginCombo("Combo!", "Select");

				{
					asd.Engine.Tool.Text("Plain Propton");
					asd.Engine.Tool.Text("Raster Road");
					asd.Engine.Tool.Text("Octern Ocean");
					asd.Engine.Tool.Text("Gigant Ground");
				}

				asd.Engine.Tool.EndCombo();
				//*/

				asd.Engine.Tool.Image(texture, new asd.Vector2DF(100, 100));
				
				asd.Engine.Tool.InputText("Enter text!", inputText, inputText.Length);
				asd.Engine.Tool.InputInt("Enter integer!", inputInt);
				asd.Engine.Tool.ColorEdit4("Color", color);
				asd.Engine.Tool.ListBox("List", selection, list);
			}

			asd.Engine.Tool.End();
		}

		protected override void OnFinish()
		{
			asd.Engine.CloseTool();
		}
	}
}
