
@echo off
set TMPFILE=".\TmpMY.xml"
@rem åˆ†æžç»“æžœè·¯å¾„
set DestDir="C:/"
set AnalysisReport=%DestDir%"AnalysisReport.xml"
@rem è®¾ç½®msbuildè·¯å¾„
set Msbuild_path="C:\Program Files (x86)\MSBuild\12.0\Bin"
@rem ç›®æ ‡å·¥ç¨‹
set DestPro="./hotdance.sln"
@rem åˆ†æžæ—¥å¿—
set CodeAnalzeLog="./hotdanceCodeAnalze.log"
@rem xlsæ–‡ä»¶
set XLS="localcodeanalyze.xsl"

@echo ¿ªÊ¼´úÂë·ÖÎö¡£¡£
@echo ·ÖÎö½á¹ûÔÚ %AnalysisReport%

@if exist %TMPFILE% del /f %TMPFILE%
@if exist %AnalysisReport% del /f %AnalysisReport%
 
@rem è®¾ç½®visual stuido çŽ¯å¢ƒ
 call C:\"Program Files (x86)"\"Microsoft Visual Studio 12.0"\VC\bin\vcvars32.bat
 

 
%Msbuild_path%\MSBuild %DestPro% /p:Configuration=Debug /p:Platform=Win32 /p:RunCodeAnalysis=true >> %CodeAnalzeLog%
 

@rem æ”¶é›†åˆ†æžæŠ¥å‘Š
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
 