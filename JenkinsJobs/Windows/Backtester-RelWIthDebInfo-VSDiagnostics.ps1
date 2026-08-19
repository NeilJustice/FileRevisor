AcceliTune.exe build-then-collect-vsdiagnostics `
   --working-dir="." `
   --program=Backtester `
   --configuration=RelWithDebInfo `
   --prerun-powershell-script="Scripts\Windows\GenerateMarketDataForSequentialProfiling.ps1" `
   --args-file="JenkinsJobs\Windows\VSDiagnosticsArgs.txt" `
   --session-id=59 `
   --diagsession-output-file="D:\PerformanceProfilingResults\VSDiagnostics\Backtester-RelWithDebInfo-VSDiagnostics.diagsession"
