$includePath = "./include"
$BoostIncludePath = "$Env:BOOST_HOME/include/boost-1_89/"
$outputExe = "main.exe"
$sourceFiles = Get-ChildItem -Path . -Filter *.cpp

$gppPath = "D:/Dev-Cpp/MinGW64/bin/g++.exe"
$flags = @("-std=c++17", "-I", $includePath, "-I", $BoostIncludePath, "-o", $outputExe, "-g", "-fdiagnostics-color=always", "-Wall")

& $gppPath $sourceFiles.Name @flags