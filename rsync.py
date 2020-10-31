import os
import sys

'''
command: rsync -av src dst --delete
eg: sudo python rsync.py localadmin@esa08.egr.duke.edu:/9962309 /
'''

if (__name__ == "__main__"):
  cnt = len(sys.argv)
  assert(cnt == 3)
  _, src, dst = sys.argv
  os.system("rsync -av {} {} --delete".format(src, dst))
  print("rsync succeeds")