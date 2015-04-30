using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class File
    {
        swig.File_Imp file;

        internal File(swig.File_Imp swig)
		{
			file = swig;
		}

        public void AddRootDirectory(string path)
        {
            file.AddRootDirectory(path);
        }
        
        /*
        public void EnumerateFiles(string path)
        {
            file.EnumerateFiles(path);
        }

        public void EnumerateFiles(string path, string searchPattern)
        {
            file.EnumerateFiles(path, searchPattern);
        }

        public void EnumerateFiles(string path, string searchPattern, bool isRecursive)
        {
            file.EnumerateFiles(path, searchPattern, isRecursive);
        }
         * */

        public bool Exists(string path)
        {
            return file.Exists(path);
        }

        public void ClearRootDirectories()
        {
            file.ClearRootDirectories();
        }

        public void AddRootPackageWithPassword(string path, string password)
        {
            file.AddRootPackage(path, password);
        }

        public StaticFile CreateStaticFile(string path)
        {
            var file_ = file.CreateStaticFile_(path);
            if (file_ == null) return null;
            return GC.GenerateStaticFile(file_, GC.GenerationType.Create);
        }

		public StreamFile CreateStreamFile(string path)
		{
			var file_ = file.CreateStreamFile_(path);
			if (file_ == null) return null;
			return GC.GenerateStreamFile(file_, GC.GenerationType.Create);
		}
    }
}
