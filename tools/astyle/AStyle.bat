::   ת���������ʾCannot convert to multi-byte string...������  
::   ���������Ƕ�С������ Astyle , ��ǰ�汾 Artistic Style 2.03  
::   �ص�ַ http://sourceforge.net/projects/astyle/files/astyle/  
::�����÷�  
::������4���ո�Ļ�,�� -s2 �ĳ� -s4  
::�޸Ĳ�����ֱ�Ӳ����ɱ����ļ�(Ҳ�Ͳ���ɾ��)  
::--------------------------------------------------------------------  
@echo off  
title ����������������V1  
echo         �ļ�֧������c,h,cpp ,   ֧��4�ֹ�����ʽ  
echo 1˫��ִ��ת����ǰĿ¼����Ŀ¼���ļ�  
echo 2��ק����·���ĵ����ļ�  
echo 3��ק����·����Ŀ¼  
echo 4�Ҽ��ļ���Ŀ¼���͵�sendto  
echo.  

echo. 

cd /d "%~dp1"
set filename=%~nx1
set pats=%~dp1
set Asuffix="--style=ansi -f -X -U -Q -s4 -S -N -L -m0 -M40 --convert-tabs --suffix=.pre"
::�״�ʹ��,���޸������AsPath��·��!!!!!!!!!!!!!!!!!!!!!!!!!  
set AsPath="astyle.exe"  
::%1���Ǳ�ʾ������ĵ�һ�������� %~1��ʾɾ���������������
if /i "%~1"=="" goto :doubleClick   
IF EXIST "%~1\" GOTO :dir
if "%filename:~-4%"==".cpp" goto :single  
if "%filename:~-2%"==".c"   goto :single
if "%filename:~-3%"==".cc"   goto :single
if "%filename:~-2%"==".h"   goto :single  
cls  
color 0a  
ECHO %filename%  
ECHO ����Ч�ĺ�׺,��ǰ֧�ֵĺ�׺������c,cpp,h ,Ҫ֧�������������޸Ĳ���  
pause  
exit  
:single  
echo --------------------singleFile mode----------------------  
ECHO ת���ĵ����ļ�:%filename%  
%AsPath%  "%Asuffix%" "%filename%"  
::�Ͼ��еĲ��������޸�  

REM ɾ�����еı����ļ�  
REM del *.pre /s /q  
pause  
exit  
:dir  
echo ---------------------dir mode-----------------------------  
for /r "%~nx1" %%f in (*.cpp;*.c;*.cc;*.h) do %AsPath% "%Asuffix%" "%%f"  
REM ɾ�����еı����ļ�  
REM for /r "%~nx1" %%a in (*.pre) do del "%%a"  
pause  
exit  
:doubleClick  
echo -------------------doubleClick mode--------------------------  
for /r . %%f in (*.cpp;*.c;*.cc;*.h) do %AsPath% "%Asuffix%" "%%f"  
REM ɾ�����еı����ļ�  
REM del *.pre /s /q  
pause  
exit  
