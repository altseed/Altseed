#!/usr/bin/env python
#coding: UTF-8

import os.path
import aceutils
import makeMarkdownUtils

def make_webpage():

    exclude_ext = [".txt", ".psd", ".BAK", ".pptx"]
    aceutils.cdToScript()
    aceutils.cd('../')
    
    aceutils.copytree('./Document', './Doc/', True)
    aceutils.cd('./Doc')

    img_dir = 'img/'
    ss_dir = 'img/ss/'

    files = aceutils.get_files('.')
    
    for file in files:
        if os.path.splitext(file)[1] in exclude_ext:
            aceutils.rm(file)
            continue

        if os.path.splitext(file)[1] != ".md":
            continue

        ls = []

        # 読み込み
        with open(file, mode='r', encoding='utf-8-sig') as f:
            ls = f.readlines()
    
        #includeの実装
        relPath = file
        ls = makeMarkdownUtils.include_basic_sample(ls,relPath,ss_dir,'all')
    
        # リンクを修正
        ls = [s.replace('.md', '.html') for s in ls]

        ls = [s.replace('\r\n', '\n') for s in ls]

        # 出力
        with open(file, mode='wb') as f:
            for l in ls:
                f.write(l.encode('utf-8'))

    aceutils.mkdir(img_dir)
    aceutils.mkdir(ss_dir)
  
    # SSのコピー
    for ssfile in makeMarkdownUtils.included_ss_paths:
        aceutils.copy(ssfile,ss_dir)

    aceutils.cd('../')

if __name__ == "__main__":
    make_webpage()

