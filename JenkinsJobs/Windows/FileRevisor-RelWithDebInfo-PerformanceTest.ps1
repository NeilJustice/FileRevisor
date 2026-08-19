$args = "delete-directory " +
"--target=X:\PerformanceTestingInputFiles\FileRevisor\PerformanceTest " +
"--parallel " +
"--quiet"

AcceliTune.exe build-then-measure-program-run-time `
   --working-dir="." `
   --program="FileRevisor" `
   --configuration="RelWithDebInfo" `
   --args="$args" `
   --performance-test-results-file="C:\Code\WindowsPerformanceTestResults\Performance\FileRevisor-Performance.txt" `
   --prerun-powershell-script="Scripts\Windows\GenerateMarketDataForPerformanceTest.ps1"
