$args = "delete-directory " +
"--target=X:\PerformanceTestingInputFiles\FileRevisor\VTuneHotspots " +
"--quiet"

AcceliTune.exe build-then-collect-hotspots `
   --working-dir="." `
   --program="FileRevisor" `
   --configuration=RelWithDebInfo `
   --args="$args" `
   --prerun-powershell-script="Scripts\Windows\GenerateMarketDataForSequentialProfiling.ps1" `
   --results-folder="D:\PerformanceProfilingResults\VTune\FileRevisor_Hotspots" `
   --overwrite-results-folder
