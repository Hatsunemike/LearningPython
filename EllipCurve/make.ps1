$includePath = "./include"
$outputExe = "main.exe"
$sourceFiles = Get-ChildItem -Path . -Filter *.cpp

$gppPath = "D:/Dev-Cpp/MinGW64/bin/g++.exe"
$flags = @("-std=c++11", "-I", $includePath, "-o", $outputExe, "-g", "-fdiagnostics-color=always", "-Wall")

& $gppPath $sourceFiles.Name @flags