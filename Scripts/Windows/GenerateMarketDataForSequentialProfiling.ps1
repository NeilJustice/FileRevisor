param([string]$profilingType = "VTuneHotspots")

FileArb.exe create-binary-files `
   --target="X:\PerformanceTestingInputFiles\FileRevisor\$profilingType" `
   --directories=10 `
   --files=10000 `
   --bytes=4096 `
   --parallel `
   --quiet
