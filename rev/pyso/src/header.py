from numba.pycc import CC
from numba import jit, njit
import numpy as np

cc = CC('libvalidator')
# Uncomment the following line to print out the compilation steps
#cc.verbose = True

