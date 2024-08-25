import subprocess
import shlex

subprocess.call(shlex.split("""llvm-profdata merge
  Debug/libFileRevisorTests/coverage.profraw -sparse -o coverage.profdata"""))

subprocess.call(shlex.split("""llvm-cov report
  Debug/libFileRevisorTests/libFileRevisorTests -instr-profile=coverage.profdata"""))

subprocess.call(shlex.split("""llvm-cov show
  Debug/libFileRevisorTests/libFileRevisorTests -instr-profile=coverage.profdata -project-title=FileRevisor -format=html -output-dir=covhtml -show-instantiation-summary -show-line-counts-or-regions -Xdemangler c++filt"""))
