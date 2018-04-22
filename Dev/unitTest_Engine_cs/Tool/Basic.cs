using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Tool
{
	class Basic : EngineTest
	{
		private asd.TextureObject2D obj;
		private sbyte[] inputText = new sbyte[128];
		private int[] inputInt = new int[] { 99 };
		private float[] color = new float[4] { 0, 0, 0, 0 };
		private int[] selection = new int[1];
		private string list;
		private asd.Texture2D texture;

		private int selectedCombo = 0;
		private string[] listArray = new string[4]
			{
				"Plain Propton",
				"Raster Road",
				"Octern Ocean",
				"Gigant Ground",
			};

		public Basic() : base(600)
		{
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

				if(asd.Engine.Tool.BeginCombo("Combo", listArray[selectedCombo]))
				{
					for (int i = 0; i < listArray.Length; i++)
					{
						if (asd.Engine.Tool.Selectable(listArray[i], selectedCombo == i))
						{
							selectedCombo = i;
							asd.Engine.Tool.SetItemDefaultFocus();
						}
					}
					
					asd.Engine.Tool.EndCombo();
				}

				//asd.Engine.Tool.Image(texture, new asd.Vector2DF(100, 100));
				
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
