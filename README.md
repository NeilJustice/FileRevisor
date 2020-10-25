# FileRevisor 🗂️

[![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17) [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

FileRevisor is a C++ command line program for quickly deleting large directories in parallel, renaming files and directories, and replacing text in files that match a regular expression.

|Build Type|Build Status|
|----------|------------|
|Travis CI Linux (Clang 7.0.0 and GCC 7.4.0)|<a href="https://travis-ci.org/NeilJustice/FileRevisor"><img src="https://travis-ci.org/NeilJustice/FileRevisor.svg?branch=master"/></a>|
|AppVeyor Windows (Visual Studio 2019 x64)|<a href="https://ci.appveyor.com/project/NeilJustice/FileRevisor"><img src="https://ci.appveyor.com/api/projects/status/fl8f0jle01k04nwp?svg=true"/></a>|

[![codecov](https://codecov.io/gh/NeilJustice/FileRevisor/branch/master/graph/badge.svg)](https://codecov.io/gh/NeilJustice/FileRevisor) FileRevisor is rigorously unit tested with <a href="https://github.com/NeilJustice/ZenUnitAndMetalMock">ZenUnit and MetalMock</a>.

* [FileRevisor Command Line Usage](#filerevisor-command-line-usage)
* [FileRevisor Program Modes](#filerevisor-program-modes)
  * [delete-directory](#delete-directory)
  * [rename-files](#rename-files)
  * [rename-directories](#rename-directories)
  * [replace-text](#replace-text)
* [Linux Folder Deletion Performance: rm -rf > /dev/null vs. FileRevisor](#linux-folder-deletion-performance-rm-rf->-dev-null-vs-FileRevisor)
* [Windows Folder Deletion Performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor](#windows-folder-deletion-performance-cmd-vs-git-bash-vs-powershell-vs-filerevisor)
* [Linux Jenkins Jobs That Compile, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor](#linux-jenkins-jobs-that-compile-clang-tidy-addresssanitize-undefinedbehaviorsanitize-and-threadsanitize-filerevisor)
* [Windows Jenkins Jobs That Compile FileRevisor](#windows-jenkins-jobs-that-compile-filerevisor)
* [4 Commands To Build And Install FileRevisor On Linux](#4-steps-to-build-and-install-filerevisor-on-linux)
* [4 Commands To Build And Install FileRevisor On Windows](#4-steps-to-build-and-install-filerevisor-on-windows)

## FileRevisor Command Line Usage

```
FileRevisor v0.9.6
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

FileRevisor `delete-directory` mode quickly deletes a directory optionally in parallel to delete its contents even faster.

Shown in this screenshot is FileRevisor `delete-directory` deleting `directory1` in parallel:

![delete-directory](Screenshots/delete-directory.png)

### rename-files

FileRevisor `rename-files` mode renames files in either a specified directory only or with `--recurse` in a specified directory and its subdirectories.

Shown in this screenshot is FileRevisor `rename-files` renaming files with name `file` to new name `new_file_name` in the current directory:

![rename-files](Screenshots/rename-files.png)

### rename-directories

FileRevisor `rename-directories` mode renames directories in either a specified directory only or with `--recurse` in a specified directory and its subdirectories.

Shown in this screenshot is FileRevisor `rename-directories` renaming directories with name `directory` to new name `new_directory_name` in the current directory:

![rename-directories](Screenshots/rename-directories.png)

### replace-text

![replace-text](Screenshots/replace-text.png)

### Linux Folder Deletion Performance: rm -rf > /dev/null vs. FileRevisor

![Linux Folder Deletion Performance: rm -rf > /dev/null vs. FileRevisor](Screenshots/LinuxFileRevisorPerformanceGraph.png)

CPU and storage hardware that generated the above and below performance numbers: 32-core 64-thread AMD Threadripper 2990WX with a 512 GB Samsung 970 PRO NVMe drive.

### Windows Folder Deletion Performance: CMD vs. Git Bash vs. PowerShell vs. FileRevisor

![Windows Folder Deletion Performance: Remove-Item -Recurse vs. FileRevisor delete-directory](Screenshots/WindowsFileRevisorPerformanceGraph.png)

### Linux Jenkins Jobs That Compile, clang-tidy, AddressSanitize, UndefinedBehaviorSanitize, and ThreadSanitize FileRevisor

A Jenkins Blue Ocean build pipeline builds the following FileRevisor Jenkins jobs on Fedora 31:

![A Jenkins Blue Ocean build pipeline builds the following FileRevisor Jenkins jobs on Fedora 31 Linux](Screenshots/LinuxJenkinsJobsForFileRevisor.png)

### Windows Jenkins Jobs That Compile FileRevisor

A Jenkins Blue Ocean build pipeline builds the following FileRevisor Jenkins jobs on Windows 10:

![Jenkins Jobs That Compile FileRevisor On Windows](Screenshots/WindowsJenkinsJobsForFileRevisor.png)

## 4 Commands To Build And Install FileRevisor On Linux

```
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor && mkdir Release && cd Release
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Release
sudo cmake --build . --target install
```

The result from running the above four Linux commands is binary `filerevisor` installed to `/usr/local/bin/filerevisor`:

![Linux filerevisor binary in /usr/local/bin](Screenshots/LinuxFileRevisorBinaryInUsrLocalBin.png)

## 4 Commands To Build And Install FileRevisor On Windows

```
git clone https://github.com/NeilJustice/FileRevisor
cd FileRevisor
cmake . -G"Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX=C:\bin
cmake --build . --config Release --target install
```

The result from running the above four Windows commands is binary `FileRevisor.exe` installed to `C:\bin\FileRevisor.exe`:

![Windows FileRevisor.exe](Screenshots/WindowsFileRevisorDotExe.png)
