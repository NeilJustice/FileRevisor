# 🗂️ FileRevisor 🗂️

[![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20) ![GitHub](https://img.shields.io/github/license/NeilJustice/FileRevisor) ![GitHub last commit](https://img.shields.io/github/last-commit/NeilJustice/FileRevisor)

FileRevisor is a cross-platform C++ command line program for quickly deleting large directories in parallel, quickly renaming files or directories, or quickly replacing text in files.

FileRevisor's signature feature is its extreme performance when deleting large folders in parallel on Windows. On Linux, directory deletions are quite a bit faster than on Windows, and so the parallel directory deletion peformance boost which FileRevisor provides is more modest on Linux relative to Windows. See below for Linux vs. Windows performance graphs.

|Build Type|Build Status|
|----------|------------|
|GitHub Actions Debug and Release build - ubuntu-latest Clang 10.0.0 / GCC 10.2.0|[![FileRevisor](https://github.com/NeilJustice/FileRevisor/actions/workflows/build.yml/badge.svg)](https://github.com/NeilJustice/FileRevisor/actions/workflows/build.yml)|
|AppVeyor Debug and Release build - Visual Studio 2019 x64|<a href="https://ci.appveyor.com/project/NeilJustice/FileRevisor"><img src="https://ci.appveyor.com/api/projects/status/fl8f0jle01k04nwp?svg=true"/></a>|
|Codecov.io code coverage for FileRevisor's C++ and FileRevisorDevOpsPython|[![codecov](https://codecov.io/gh/NeilJustice/FileRevisor/branch/master/graph/badge.svg?token=KH1wxoYSdS)](https://codecov.io/gh/NeilJustice/FileRevisor)|
|SonarCloud scan of FileRevisorDevOpsPython - the Python code which builds and tests FileRevisor|[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=NeilJustice_FileRevisor&metric=alert_status)](https://sonarcloud.io/dashboard?id=NeilJustice_FileRevisor)|

FileRevisor is rigorously unit tested with <a href="https://github.com/NeilJustice/ZenUnitAndMetalMock">ZenUnit and MetalMock</a>.

* [FileRevisor command line usage](#filerevisor-command-line-usage)
* [FileRevisor program modes](#filerevisor-program-modes)
  * [delete-directory](#delete-directory)
  * [rename-files](#rename-files)
  * [rename-directories](#rename-directories)
  * [replace-text](#replace-text)
* [Linux directory deletion performance: rm -rf > /dev/null vs. FileRevisor](#linux-directory-deletion-performance-rm-rf->-dev-null-vs-FileRevisor)
* [Windows folder deletion performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor delete-directory --parallel](#windows-folder-deletion-performance-cmd-vs-git-bash-vs-powershell-vs-filerevisor-delete-directory---parallel)
* [FileRevisor code structure as it appears in Visual Studio Code on Linux](#filerevisor-code-structure-as-it-appears-in-visual-studio-code-on-linux)
* [FileRevisor code structure as it appears in Visual Studio 2019 on Windows](#filerevisor-code-structure-as-it-appears-in-visual-studio-2019-on-windows)
* [Linux Jenkins jobs which build, cppcheck, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor's C++ code and mypy-flake8-pylint-SonarQube FileRevisor's CI/CD Python code](#linux-jenkins-jobs-which-build-cppcheck-clang-tidy-addresssanitize-undefinedbehaviorsanitize-and-threadsanitize-filerevisors-c-code-and-mypy-flake8-pylint-sonarqube-scan-filerevisors-cicd-python-code)
* [Windows Jenkins jobs which build and cppcheck FileRevisor's C++ code and mypy-flake8-pylint-SonarQube FileRevisor's CI/CD Python code](#windows-jenkins-jobs-which-build-and-cppcheck-filerevisors-c-code-and-mypy-flake8-pylint-sonarqube-scan-filerevisors-cicd-python-code)
* [4 steps to build binary filerevisor from source on Linux with Clang](#4-steps-to-build-binary-filerevisor-from-source-on-linux-with-clang)
* [4 steps to build executable FileRevisor.exe from source on Windows with Visual Studio 2019](#4-steps-to-build-executable-filerevisorexe-from-source-on-windows-with-visual-studio-2019)

## FileRevisor command line usage

```prolog
FileRevisor v0.13.0
Quickly deletes directories, renames files, renames directories, or replaces text in files.

Usage:
   filerevisor delete-directory
      --target=<TargetDirectoryPath>
      [--parallel]
      [--minimal]
   filerevisor rename-files
      --target=<TargetDirectoryPath>
      --from=<FileNameRegex>
      --to=<FileNameRegex>
      [--recurse]
      [--dryrun]
      [--verbose]
   filerevisor rename-directories
      --target=<TargetDirectoryPath>
      --from=<DirectoryNameRegex>
      --to=<DirectoryNameRegex>
      [--recurse]
      [--dryrun]
      [--verbose]
   filerevisor replace-text
      --target=<TargetDirectoryPath>
      --from=<FileTextRegexPattern>
      --to=<ReplacementFileTextRegexPattern>
      [--recurse]
      [--dryrun]
      [--verbose]
```

## FileRevisor program modes

### delete-directory

```prolog
filerevisor delete-directory
      --target=<TargetDirectoryPath>
      [--parallel]
      [--minimal]
```

FileRevisor `delete-directory` mode quickly deletes all files in and below `--target=<TargetDirectoryPath>`. For extreme directory deletion performance, especially on Windows, specify `--parallel` to delete subdirectories below `TargetDirectoryPath` using one thread per subdirectory.

Shown in this screenshot is `filerevisor delete-directory` deleting `directory1` in parallel:

![delete-directory](Screenshots/delete-directory.png)

### rename-files

```prolog
filerevisor rename-files
      --target=<TargetDirectoryPath>
      --from=<FileNameRegex>
      --to=<FileNameRegex>
      [--recurse]
      [--dryrun]
      [--verbose]
```

FileRevisor `rename-files` renames files in a specified directory matching a specified regular expression. `--recurse` can be specified to rename files in and below a specified directory.

Shown in this screenshot is `filerevisor rename-files` renaming files with name `file` to new name `new_file_name` in the current directory:

![rename-files](Screenshots/rename-files.png)

### rename-directories

```prolog
filerevisor rename-directories
      --target=<TargetDirectoryPath>
      --from=<DirectoryNameRegex>
      --to=<DirectoryNameRegex>
      [--recurse]
      [--dryrun]
      [--verbose]
```

FileRevisor `rename-directories` renames directories in a specified directory matching a specified regular expression. `--recurse` can be specified to rename directories in and below a specified directory.

Shown in this screenshot is `filerevisor rename-directories` renaming directories with name `directory` to new name `new_directory_name` in the current directory:

![rename-directories](Screenshots/rename-directories.png)

### replace-text

```prolog
filerevisor replace-text
      --target=<TargetDirectoryPath>
      --from=<FileTextRegexPattern>
      --to=<ReplacementFileTextRegexPattern>
      [--recurse]
      [--dryrun]
      [--verbose]
```

FileRevisor `replace-text` replaces text in files in a specified directory matching a specified regular expression. `--recurse` can be specified to replace text in files in and below a specified directory.

Shown in this screenshot is `filerevisor replace-text` replacing text in files in the current directory containing text `apple` with replacement text `orange`:

![replace-text](Screenshots/replace-text.png)

### Linux directory deletion performance: rm -rf > /dev/null vs. FileRevisor

![Linux Directory Deletion Performance: rm -rf > /dev/null vs. FileRevisor](Screenshots/LinuxFileRevisorPerformanceGraph.png)

### Windows folder deletion performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor delete-directory --parallel

![Windows Folder Deletion Performance: Remove-Item -Recurse vs. FileRevisor delete-directory](Screenshots/WindowsFileRevisorPerformanceGraph.png)

As you can see from the above two graphs, the implementation of Linux file deletions is quite a bit faster than the implementation of Windows file deletions, with modest time savings on Linux and massive time savings on Windows available by way of FileRevisor parallel folder deletion.

The CPU and storage hardware which generated the above performance numbers is a 32-core 64-thread AMD Threadripper 2990WX with a 512 GB Samsung 970 PRO NVMe drive.

### FileRevisor code structure as it appears in Visual Studio Code on Linux

Shown in this screenshot is function `FileRevisorArgsParser::ParseArgs` which uses the excellent single-header command line arguments parsing library [docopt.cpp](https://github.com/docopt/docopt.cpp) to parse FileRevisor's command line arguments:

![FileRevisor Code Structure As It Appears In Visual Studio Code On Linux](Screenshots/Linux/VisualStudioCode.png)

### FileRevisor code structure as it appears in Visual Studio 2019 on Windows

Shown in this screenshot is function `DeleteDirectorySubProgram::Run` with the key if-statement highlighted for determining whether to delete a given directory's subdirectories in parallel or sequentially:

![FileRevisor Code Structure As It Appears In Visual Studio 2019 On Windows](Screenshots/Windows/VisualStudio.png)

### Linux Jenkins jobs which build, Cppcheck, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor's C++ code and Mypy-Flake8-Pylint-SonarQube scan FileRevisor's CI/CD Python code

![Linux FileRevisor Jenkins Jobs](Screenshots/Linux/LinuxJenkinsJobs.png)

### Windows Jenkins jobs which build and Cppcheck FileRevisor's C++ code and Mypy-Flake8-Pylint-SonarQube scan FileRevisor's CI/CD Python code

![Windows Jenkins Jobs](Screenshots/Windows/WindowsJenkinsJobs.png)

## 4 steps to build binary filerevisor from source on Linux with Clang

```bash
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor && mkdir Release && cd Release
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Release
sudo cmake --build . --target install
```

Result binary `/usr/local/bin/filerevisor`:

![Linux filerevisor binary in /usr/local/bin](Screenshots/LinuxFileRevisorBinaryInUsrLocalBin.png)

## 4 steps to build executable FileRevisor.exe from source on Windows with Visual Studio 2019

```powershell
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor
cmake . -G"Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX=C:\bin
cmake --build . --config Release --target install
```

Resulting executable `C:\bin\FileRevisor.exe`:

![Windows FileRevisor.exe](Screenshots/WindowsFileRevisorDotExe.png)
