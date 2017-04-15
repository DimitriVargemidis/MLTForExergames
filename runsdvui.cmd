cd /d "A:\Users\christiaan\Source\Repos\exergames" &msbuild "BodyBasics-D2D.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform=Win32
exit %errorlevel% 