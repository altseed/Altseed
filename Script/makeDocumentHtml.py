#!/usr/bin/env python
#coding: UTF-8
"""
convert Document/*.md to DocumentHtml/*.html.
requirement: pandoc(http://johnmacfarlane.net/pandoc/)
"""
import os.path
import aceutils
import re

def include_basic_sample(ls,mode=''):
    #includeÇÃé¿ëï
    ls_included = []
    includePattern = r'\* include_basic_sample (.+)'
    includer = re.compile(includePattern)
    
    for s in ls:
        m = includer.search(s.replace('\r','').replace('\n',''))
        if m != None:
            sampleName = m.group(1)
            searchedTargetDir = '../Sample/BasicSample/'
            
            files = []
            for f in aceutils.get_files(searchedTargetDir):
                basename = os.path.basename(f)
                name = os.path.splitext(basename)[0]
                if name == sampleName:
                    files.append(f)
      
            searchedExt = '.cs'

            if mode == 'cs':
                searchedExt = '.cs'
            elif mode == 'cpp':
                searchedExt = '.cpp'

            targetPath = ''

            for f in files:
                ext = os.path.splitext(f)[1] 
                if ext == searchedExt:
                    targetPath = f
                    break
            
            searchedExt = '.cs'
            if targetPath == '':
                for f in files:
                    ext = os.path.splitext(f)[1]
                    if ext == searchedExt:
                        targetPath = f
                        break

        
            if os.path.exists(targetPath):
                ext = os.path.splitext(targetPath)[1]
          
                if ext=='.cpp':
                    ls_included.append('```cpp\n')
                elif ext=='.cs':
                    ls_included.append('```cs\n')
                else:
                    ls_included.append('```\n')
	    
                with open(targetPath, mode='r', encoding='utf-8') as f:
                    ls_included.extend(f.readlines())
                
                ls_included.append('\n```\n')
        else:
            ls_included.append(s)

    return ls_included


def make_document_html():
  exclude_ext = [".txt", ".psd", ".BAK"]

  template="""<!DOCTYPE html>
  <html>
  <head>
  <meta charset="UTF-8">
  <STYLE type="text/css">
  <!--
  @charset "utf-8";
  body
  {
  	line-height: 1.5em;
  }

  *::selection
  {
  	background-color: rgba(255, 210, 130, 0.5);
  /*	background-color: rgba(160, 220, 150, 0.5);*/
  }

  h1, h2, h3, h4
  {
  	font-family: "Meiryo UI";
  	display: block;
  }

  h1, h2, h3
  {
  	padding: 0.2em 0.3em;
  }

  h1
  {
  	font-size: 2em;
  	background-color: #ddeeee;
  	border-bottom: 1px solid #999999;
  	line-height: 1.2em;
  }
  h2
  {
  	font-size: 1.5em;
  	background-color: #eef3f3;
  	border-bottom: 1px solid #999999;
  	line-height: 1.2em;
  }
  h3{
  	font-size: 1.3em;
  	background-color: #eeeeee;
  }
  h4
  {
  	font-size: 1.1em;
  	margin-top: 0.3em;
  	margin-bottom: 0em;
  }

  p
  {
  	margin-bottom: 0.9em;
  }
  p code {
    border: 1px solid #ccc;
    border-radius: 3px;
    background-color: #f9f9f9;
    padding: 1px 5px;
    display: inline-block;
  }
  pre
  {
    margin-top: 0.1em;
    margin-left: 1em;
    border: 1px solid #ccc;
    background-color: #f9f9f9;
    padding: 3px 5px;
    display: block;
    font: 1em "Consolas";
  }

  pre code
  {
    font: 0.9em "Consolas";
  }

  code
  {
    color: #52595d;
    font: 0.95em "Consolas";
    margin: 0em 0.3em;
  }

  hr
  {
  	border: 0;
  	border-bottom: 2px dashed #acc;
  	background-color: #fff;
  }

  ul
  {
  	list-style-type: none;
  }

  li
  {
  	border-left: 2px solid #cdd;
  	margin-bottom: 2px;
  	padding-left: 0.5em;
  }

  em
  {
  	display: none;
  }

  table
  {
  	border-collapse: collapse;
  }

  td, th
  {
  	border: 1px solid #8bb;
  	padding: 2px 5px;
  	vertical-align: middle;
  }
  -->
  </STYLE>
  <title>$title$</title>
  </head>
  <body>
  $body$
  </body>
  </html>
  """

  aceutils.cdToScript()
  aceutils.cd('../')
  aceutils.copytree('./Document', './DocumentHtml', True)
  aceutils.cd('./DocumentHtml')

  files = aceutils.get_files('.')

  with open('template.html', mode='w',  encoding='utf-8') as f:
    f.write(template)

  for file in files:
    if os.path.splitext(file)[1] in exclude_ext:
      aceutils.rm(file)
      continue

    if os.path.splitext(file)[1] != ".md":
      continue

    ls = []

    # ì«Ç›çûÇ›
    with open(file, mode='r',  encoding='utf-8') as f:
      ls = f.readlines()
    
    #includeÇÃé¿ëï
    ls = include_basic_sample(ls,'')
    ls_included_bsample = []
    include_bsample_Pattern = r'\* include_basic_sample (.+)'
    include_bsample_r = re.compile(include_bsample_Pattern)

    # ÉäÉìÉNÇèCê≥
    ls = [s.replace('.md', '.html') for s in ls]
    
    # èoóÕ
    with open(file, mode='w',  encoding='utf-8') as f:
      f.writelines(ls)

    # pandocÇ…ÇÊÇÈèoóÕ
    aceutils.call('pandoc -f markdown_github -t html5 -s --template=template.html -o {0}.html {0}.md'.format(os.path.splitext(file)[0]))
    aceutils.rm(file)

  aceutils.rm('template.html')
  aceutils.cd('../')
  
if __name__ == "__main__":
  make_document_html()

