# 🗂️ FileRevisor 🗂️

[![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20) ![GitHub](https://img.shields.io/github/license/NeilJustice/FileRevisor) ![GitHub last commit](https://img.shields.io/github/last-commit/NeilJustice/FileRevisor)

FileRevisor is a cross-platform C++ command line program for quickly deleting large directories in parallel, renaming files and directories, and replacing text in files.

FileRevisor's key feature is its extreme performance when deleting large folders in parallel on Windows. On Linux, file deletions are quite a bit faster than on Windows, and so the parallel file deletion peformance boost which FileRevisor provides is much more modest on Linux relative to Windows.

|Build Type|Build Status|
|----------|------------|
|Travis CI Linux Clang 9.0.1 and Linux GCC 9.3.0 Debug and Release|[![Build Status](https://travis-ci.com/NeilJustice/FileRevisor.svg?branch=main)](https://travis-ci.com/NeilJustice/FileRevisor)|
|AppVeyor Windows Visual Studio 2019 x64 Debug and Release|<a href="https://ci.appveyor.com/project/NeilJustice/FileRevisor"><img src="https://ci.appveyor.com/api/projects/status/fl8f0jle01k04nwp?svg=true"/></a>|
|Code coverage for the Travis CI Linux GCC 9.3.0 Release build|[![codecov](https://codecov.io/gh/NeilJustice/FileRevisor/branch/master/graph/badge.svg)](https://codecov.io/gh/NeilJustice/FileRevisor)|

FileRevisor is rigorously unit tested with <a href="https://github.com/NeilJustice/ZenUnitAndMetalMock">ZenUnit and MetalMock</a>.

* [FileRevisor Command Line Usage](#filerevisor-command-line-usage)
* [FileRevisor Program Modes](#filerevisor-program-modes)
  * [delete-directory](#delete-directory)
  * [rename-files](#rename-files)
  * [rename-directories](#rename-directories)
  * [replace-text](#replace-text)
* [Linux Directory Deletion Performance: rm -rf > /dev/null vs. FileRevisor](#linux-directory-deletion-performance-rm-rf->-dev-null-vs-FileRevisor)
* [Windows Folder Deletion Performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor delete-directory --parallel](#windows-folder-deletion-performance-cmd-vs-git-bash-vs-powershell-vs-filerevisor-delete-directory---parallel)
* [FileRevisor Code Structure As It Appears In Visual Studio Code On Linux](#filerevisor-code-structure-as-it-appears-in-visual-studio-code-on-linux)
* [FileRevisor Code Structure As It Appears In Visual Studio 2019 On Windows](#filerevisor-code-structure-as-it-appears-in-visual-studio-2019-on-windows)
* [Linux Jenkins Jobs Which Build, Cppcheck, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor's C++ Code and SonarQube Scan FileRevisor's Python Code](#linux-jenkins-jobs-which-build-cppcheck-clang-tidy-addresssanitize-undefinedbehaviorsanitize-and-threadsanitize-filerevisors-c-code-and-sonarqube-scan-filerevisors-python-code)
* [Windows Jenkins Jobs Which Build And Cppcheck FileRevisor's C++ Code and SonarQube Scan FileRevisor's Python Code](#windows-jenkins-jobs-which-build-and-cppcheck-filerevisors-c-code-and-sonarqube-scan-filerevisors-python-code)
* [How To Build Binary filerevisor On Linux With Clang](#how-to-build-binary-filerevisor-on-linux-with-clang)
* [How To Build Executable FileRevisor.exe On Windows With Visual Studio 2019](#how-to-build-executable-filerevisorexe-on-windows-with-visual-studio-2019)
* [FileRevisor Roadmap](#filerevisor-roadmap)

## FileRevisor Command Line Usage

```
FileRevisor v0.10.0
Quickly deletes directories in parallel,
bulk renames files or directories,
or replaces text in files that matches a regular expression.
https://github.com/NeilJustice/FileRevisor

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
      [--preview]
      [--verbose]
   filerevisor rename-directories
      --target=<TargetDirectoryPath>
      --from=<DirectoryNameRegex>
      --to=<DirectoryNameRegex>
      [--recurse]
      [--preview]
      [--verbose]
   filerevisor replace-text
      --target=<TargetDirectoryPath>
      --from=<FileTextRegexPattern>
      --to=<ReplacementFileTextRegexPattern>
      [--recurse]
      [--preview]
      [--verbose]
```

## FileRevisor Program Modes

### delete-directory

```
filerevisor delete-directory
      --target=<TargetDirectoryPath>
      [--parallel]
      [--minimal]
```

FileRevisor `delete-directory` mode quickly deletes all files in and below `--target=<TargetDirectoryPath>`. For extreme directory deletion performance, especially on Windows, specify `--parallel` to delete subdirectories below `TargetDirectoryPath` using one thread per subdirectory.

Shown in this screenshot is `filerevisor delete-directory` deleting `directory1` in parallel:

![delete-directory](Screenshots/delete-directory.png)

### rename-files

```
filerevisor rename-files
      --target=<TargetDirectoryPath>
      --from=<FileNameRegex>
      --to=<FileNameRegex>
      [--recurse]
      [--preview]
      [--verbose]
```

FileRevisor `rename-files` renames files in a specified directory matching a specified regular expression. `--recurse` can be specified to rename files in and below a specified directory.

Shown in this screenshot is `filerevisor rename-files` renaming files with name `file` to new name `new_file_name` in the current directory:

![rename-files](Screenshots/rename-files.png)

### rename-directories

```
filerevisor rename-directories
      --target=<TargetDirectoryPath>
      --from=<DirectoryNameRegex>
      --to=<DirectoryNameRegex>
      [--recurse]
      [--preview]
      [--verbose]
```

FileRevisor `rename-directories` renames directories in a specified directory matching a specified regular expression. `--recurse` can be specified to rename directories in and below a specified directory.

Shown in this screenshot is `filerevisor rename-directories` renaming directories with name `directory` to new name `new_directory_name` in the current directory:

![rename-directories](Screenshots/rename-directories.png)

### replace-text

```
filerevisor replace-text
      --target=<TargetDirectoryPath>
      --from=<FileTextRegexPattern>
      --to=<ReplacementFileTextRegexPattern>
      [--recurse]
      [--preview]
      [--verbose]
```

FileRevisor `replace-text` replaces text in files in a specified directory matching a specified regular expression. `--recurse` can be specified to replace text in files in and below a specified directory.

Shown in this screenshot is `filerevisor replace-text` replacing text in files in the current directory containing text `apple` with replacement text `orange`:

![replace-text](Screenshots/replace-text.png)

### Linux Directory Deletion Performance: rm -rf > /dev/null vs. FileRevisor

![Linux Directory Deletion Performance: rm -rf > /dev/null vs. FileRevisor](Screenshots/LinuxFileRevisorPerformanceGraph.png)

### Windows Folder Deletion Performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor delete-directory --parallel

![Windows Folder Deletion Performance: Remove-Item -Recurse vs. FileRevisor delete-directory](Screenshots/WindowsFileRevisorPerformanceGraph.png)

As you can see from the above two graphs, the implementation of Linux file deletions is quite a bit faster than the implementation of Windows file deletions, with modest time savings on Linux and massive time savings on Windows available by way of FileRevisor parallel folder deletion.

The CPU and storage hardware which generated the above performance numbers is a 32-core 64-thread AMD Threadripper 2990WX with a 512 GB Samsung 970 PRO NVMe drive.

### FileRevisor Code Structure As It Appears In Visual Studio Code On Linux

Shown in this screenshot is function `FileRevisorArgsParser::ParseArgs` which uses the excellent single-header command line arguments parsing library [docopt.cpp](https://github.com/docopt/docopt.cpp) to parse FileRevisor's command line arguments:

![FileRevisor Code Structure As It Appears In Visual Studio Code On Linux](Screenshots/Linux/VisualStudioCode.png)

### FileRevisor Code Structure As It Appears In Visual Studio 2019 On Windows

Shown in this screenshot is function `DeleteDirectorySubProgram::Run` with the key if-statement highlighted for determining whether to delete a given directory's subdirectories in parallel or sequentially:

![FileRevisor Code Structure As It Appears In Visual Studio 2019 On Windows](Screenshots/Windows/VisualStudio.png)

### Linux Jenkins Jobs Which Build, Cppcheck, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor's C++ Code and SonarQube Scan FileRevisor's Python Code

A Jenkins Blue Ocean build pipeline builds the following FileRevisor Jenkins jobs on Fedora 33 with Clang 11.0.0 and GCC 10.2.1:

![Linux FileRevisor Jenkins Jobs](Screenshots/Linux/LinuxJenkinsJobs.png)

### Windows Jenkins Jobs Which Build And Cppcheck FileRevisor's C++ Code and SonarQube Scan FileRevisor's Python Code

A Jenkins Blue Ocean build pipeline builds the following FileRevisor Jenkins jobs on Windows 10 with Visual Studio 2019:

![Windows Jenkins Jobs](Screenshots/Windows/WindowsJenkinsJobs.png)

## How To Build Binary filerevisor On Linux With Clang

```bash
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor && mkdir Release && cd Release
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Release
sudo cmake --build . --target install
```

Result binary `/usr/local/bin/filerevisor`:

![Linux filerevisor binary in /usr/local/bin](Screenshots/LinuxFileRevisorBinaryInUsrLocalBin.png)

## How To Build Executable FileRevisor.exe On Windows With Visual Studio 2019

```powershell
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor
cmake . -G"Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX=C:\bin
cmake --build . --config Release --target install
```

Resulting executable `C:\bin\FileRevisor.exe`:

![Windows FileRevisor.exe](Screenshots/WindowsFileRevisorDotExe.png)

## FileRevisor Roadmap

|Future FileRevisor Feature|Implementation Status As Of 3/15/2021|
|--------------------------|------------------------------------|
|GitHub Actions build|Awaiting implementation|
|SonarCloud Python static analysis badge for FileRevisorPyUtils|Awaiting implementation|
|SonarCloud C++ static analysis badge|Awaiting implementation|
|Coverity C++ static analysis badge|Awaiting implementation|
|Automated acceptance testing|Awaiting implementation|
