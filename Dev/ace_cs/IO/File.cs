using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace.IO
{
    public class File
    {
        private swig.File file;

        internal File(swig.File swig)
		{
			file = swig;
		}

        public void AddRootDirectories(string path)
        {
            file.SetRootDirectories(path);
        }

        public void AddRootDirectories(string path, string path2)
        {
            file.SetRootDirectories(path, path2);
        }

        public void AddRootDirectories(string path, string path2, string path3)
        {
            file.SetRootDirectories(path, path2, path3);
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

        }

        public void AddRootPackage(string path)
        {

        }

        public void AddRootPackageWithPassword(string path, string password)
        {

        }

        //CreateStaticFileを追加しなければいけない。


    }
}
