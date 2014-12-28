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

        public void AddRootDirectories(string path)
        {
            file.AddRootDirectories(path);
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
            var staticFile = file.CreateStaticFile_(path);
            if (staticFile == null) return null;
            return GC.GenerateStaticFile(staticFile, GC.GenerationType.Create);
        }


    }
}
