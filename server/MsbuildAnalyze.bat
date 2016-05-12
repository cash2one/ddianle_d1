
@echo off
set TMPFILE=".\TmpMY.xml"
@rem 分析结果路径
set DestDir="C:/"
set AnalysisReport=%DestDir%"AnalysisReport.xml"
@rem 设置msbuild路径
set Msbuild_path="C:\Program Files (x86)\MSBuild\12.0\Bin"
@rem 目标工程
set DestPro="./hotdance.sln"
@rem 分析日志
set CodeAnalzeLog="./hotdanceCodeAnalze.log"
@rem xls文件
set XLS="localcodeanalyze.xsl"

@echo ��ʼ�����������
@echo ��������� %AnalysisReport%

@if exist %TMPFILE% del /f %TMPFILE%
@if exist %AnalysisReport% del /f %AnalysisReport%
 
@rem 设置visual stuido 环境
 call C:\"Program Files (x86)"\"Microsoft Visual Studio 12.0"\VC\bin\vcvars32.bat
 

 
%Msbuild_path%\MSBuild %DestPro% /p:Configuration=Debug /p:Platform=Win32 /p:RunCodeAnalysis=true >> %CodeAnalzeLog%
 

@rem 收集分析报告
for /r ./ %%i in (*codeanalysis.xml) do (
type %%i >> %TMPFILE%
)

for /r ./ %%i in (*codeanalysis.all.xml) do (
type %%i >> %TMPFILE%
)

if exist ./%XLS% (goto HaveXLS) else (goto NOXLS)

:HaveXLS
del %DestDir%%XLS%
xcopy %XLS% %DestDir%
type %TMPFILE%|sed -e "s#<[?].*[?]>##g"|sed -e "s#<DEFECTS>##g"|sed -e "s#<DEFECTS/>##g"|sed -e "s#</DEFECTS>##g"|sed -e "1i<DEFECTS>"|sed -e "1i<?xml-stylesheet type=\"text/xsl\" href=\"%XLS:"=%\"?>"|sed -e "1i<?xml version='1.0' encoding='UTF-8'?>"|sed -e "$a</DEFECTS>" >> %AnalysisReport%

goto end

:NOXLS
type %TMPFILE%|sed -e "s#<[?].*[?]>##g"|sed -e "s#<DEFECTS>##g"|sed -e "s#<DEFECTS/>##g"|sed -e "s#</DEFECTS>##g"|sed -e "1i<DEFECTS>"|sed -e "1i<?xml version='1.0' encoding='UTF-8'?>"|sed -e "$a</DEFECTS>" >> %AnalysisReport%
goto end

:end
del /f %TMPFILE%
exit
 