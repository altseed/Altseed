@echo off
rem submodule更新bat
rem 
rem git.exeにパスを切っておいてください。
rem TortoiseGitで同様の操作をするには、
rem Submodule update(--initオプションをはずす)
cd Dev\amcr-glfw
git.exe pull "origin"
pause
