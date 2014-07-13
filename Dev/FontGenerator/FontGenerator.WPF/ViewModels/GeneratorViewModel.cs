using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FontGenerator.Model;
using System.ComponentModel.DataAnnotations;
using System.Windows.Forms;
using System.IO;

namespace FontGenerator.WPF.ViewModels
{
	class GeneratorViewModel : INotifyPropertyChanged, IDataErrorInfo
	{
		public event PropertyChangedEventHandler PropertyChanged;

		#region Properties
		private GenerationConfig config;

		public GenerationConfig Config
		{
			get { return config; }
			set
			{
				config = value;
				PropertyChanged.Raise(this, () => FontPath);
				PropertyChanged.Raise(this, () => TextPath);
				PropertyChanged.Raise(this, () => ExportPath);
				PropertyChanged.Raise(this, () => SheetName);
				PropertyChanged.Raise(this, () => FontSize);
				PropertyChanged.Raise(this, () => TextureSize);
				PropertyChanged.Raise(this, () => Red);
				PropertyChanged.Raise(this, () => Green);
				PropertyChanged.Raise(this, () => Blue);
				PropertyChanged.Raise(this, () => Alpha);
				PropertyChanged.Raise(this, () => ColorString);
			}
		}


		[Required]
		public string FontPath
		{
			get { return config.FontPath; }
			set
			{
				if(!value.Equals(config.FontPath))
				{
					config.FontPath = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string TextPath
		{
			get { return config.TextPath; }
			set
			{
				if(!value.Equals(config.TextPath))
				{
					config.TextPath = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string ExportPath
		{
			get { return config.ExportPath; }
			set
			{
				if(!value.Equals(config.ExportPath))
				{
					config.ExportPath = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string SheetName
		{
			get { return config.SheetName; }
			set
			{
				if(!value.Equals(config.SheetName))
				{
					config.SheetName = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public int FontSize
		{
			get { return config.FontSize; }
			set
			{
				if(!value.Equals(config.FontSize))
				{
					config.FontSize = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public int TextureSize
		{
			get { return config.TextureSize; }
			set
			{
				if(!value.Equals(config.TextureSize))
				{
					config.TextureSize = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		public byte Red
		{
			get { return config.FontColor.Red; }
			set
			{
				if(!value.Equals(config.FontColor.Red))
				{
					config.FontColor.Red = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Green
		{
			get { return config.FontColor.Green; }
			set
			{
				if(!value.Equals(config.FontColor.Green))
				{
					config.FontColor.Green = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Blue
		{
			get { return config.FontColor.Blue; }
			set
			{
				if(!value.Equals(config.FontColor.Blue))
				{
					config.FontColor.Blue = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Alpha
		{
			get { return config.FontColor.Alpha; }
			set
			{
				if(!value.Equals(config.FontColor.Alpha))
				{
					config.FontColor.Alpha = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public string ColorString
		{
			get { return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", Alpha, Red, Green, Blue); }
		}
		#endregion

		public string Error
		{
			get { throw new NotImplementedException(); }
		}

		public string this[string columnName]
		{
			get
			{
				var errors = new List<ValidationResult>();
				var validation = Validator.TryValidateProperty(
					GetType().GetProperty(columnName).GetValue(this, null),
					new ValidationContext(this) { MemberName = columnName },
					errors);

				return validation ? null : errors.FirstOrDefault().ErrorMessage;
			}
		}

		public DelegateCommand OpenFontFileCommand { get; private set; }
		public DelegateCommand OpenTextFileCommand { get; private set; }
		public DelegateCommand OpenExportPathCommand { get; private set; }
		public DelegateCommand SaveConfigurationCommand { get; private set; }
		public DelegateCommand LoadConfigurationCommand { get; private set; }

		public GeneratorViewModel()
		{
			config = new GenerationConfig();
			FontPath = "C:\\Windows\\Fonts\\";
			Alpha = 255;
			FontSize = 14;
			TextureSize = 1024;
			OpenFontFileCommand = new DelegateCommand { CommandHandler = OpenFontFile };
			OpenTextFileCommand = new DelegateCommand { CommandHandler = OpenTextFile };
			OpenExportPathCommand = new DelegateCommand { CommandHandler = OpenExportPath };
			SaveConfigurationCommand = new DelegateCommand { CommandHandler = SaveConfiguration };
			LoadConfigurationCommand = new DelegateCommand { CommandHandler = LoadConfiguration };
		}

		private void LoadConfiguration(object obj)
		{
			var dialog = new OpenFileDialog();

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				Config = ConfigurationFile.Load(dialog.FileName);
			}
		}

		private void SaveConfiguration(object obj)
		{
			var dialog = new SaveFileDialog()
			{
				DefaultExt = "json",
				FileName = "FontSetting",
			};

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				ConfigurationFile.Save(config, dialog.FileName);
			}
		}

		private void OpenExportPath(object obj)
		{
			var dialog = new FolderBrowserDialog();

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				ExportPath = dialog.SelectedPath;
			}
		}

		private void OpenTextFile(object obj)
		{
			var dialog = new OpenFileDialog()
			{
				Filter = "テキスト ファイル(*.txt)|*.txt|すべてのファイル(*.*)|*.*",
			};

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				TextPath = dialog.FileName;
			}
		}

		private void OpenFontFile(object obj)
		{
			var dialog = new FontDialog();

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				var fontDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Fonts);
				//FontPath = Path.Combine(fontDirectory, );
			}
		}

		public async Task GenerateAsync()
		{
			await Generator.GenerateAsync(config);
		}
	}
}
