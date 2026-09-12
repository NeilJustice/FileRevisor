AcceliTune.exe build-then-collect-vsdiagnostics `
   --working-dir="."  `
   --solution=FileRevisor `
   --program=FileRevisor `
   --configuration=RelWithDebInfo `
   --args-file="JenkinsJobs\Windows\VSDiagnosticsArgs.txt" `
   --session-id=60 `
   --diagsession-output-file="D:\PerformanceProfilingResults\VSDiagnostics\FileRevisor-RelWithDebInfo-VSDiagnostics.diagsession" `
   --prerun-powershell-script="Scripts\Windows\GenerateMarketDataForSequentialProfiling.ps1" `
