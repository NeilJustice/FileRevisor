import sys
sys.path.append('.')
from FileRevisorPyUtils import Python

Python.run_all_with_coverage(testsProjectName='FileRevisorPyUtilsTests', omitPattern='/usr/*')