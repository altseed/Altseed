@echo off
rem amcr-glfw初期化用bat
rem 
rem git.exeにパスを切っておいてください。
rem TortoiseGitで同様の操作をするには、
rem Submodule update→Dev\amcr-glfw\でSwitch/checkout→Switch to でbranchを選択、maseterを選択

git.exe submodule init
git.exe submodule update
cd .\Dev\amcr-glfw
git.exe checkout master
pause
