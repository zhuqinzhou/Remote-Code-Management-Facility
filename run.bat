:run.bat
:

start "" /d "2_Server" "Debug\2_Server.exe"
start "" /d "1_Server" "Debug\1_Server.exe"
start "" /d "2_client_WPF" "Debug\2_client_WPF.exe"
start "" /d "CppCli-WPF-App" "Debug\CppCli-WPF-App.exe"
start "" "README.pdf"