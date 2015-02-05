import os

NAME='emperor_mysql'
CFLAGS = os.popen('mysql_config --cflags').read().rstrip().split()
CFLAGS.append('-I' + os.popen('mysql_config --include').read().rstrip())
#LDFLAGS = os.popen('mysql_config --ldflags').read().rstrip().split()
LIBS = os.popen('mysql_config --libs').read().rstrip().split()

GCC_LIST = ['emperor_mysql']
