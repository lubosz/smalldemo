gcc -o small src/small.c \
  --entry main \
  -nostartfiles \
  -nostdlib \
  -nodefaultlibs \
  -lc
  
strip -s small

clang -o small-clang src/small.c \
  -nostdlib \
  -lc
strip -s small-clang

gcc -o smalldemo src/nurbs.c \
  --entry main \
  -nostartfiles \
  -nostdlib \
  -nodefaultlibs \
  -lc \
  -lGL -lGLU -lglut
strip -s smalldemo

clang -o smalldemo-clang src/nurbs.c \
  -nostdlib \
  -lc \
  -lGL -lGLU -lglut
strip -s smalldemo

gcc -o smallgood src/nurbs.c \
  -lGL -lGLU -lglut
strip -s smallgood
  
gcc -o smallglut src/glut.c \
  -nodefaultlibs \
  -lc \
  -lglut
strip -s smallglut

gcc -o smallxcb src/xcb.c \
  -std=c99 \
  -lGL -lX11 -lX11-xcb -lxcb
  #-nodefaultlibs \
  #-lc \
strip -s smallxcb

gcc -o smallerxcb src/xcb-smaller.c \
  -std=c99 \
  -lGL -lX11 -lX11-xcb -lxcb
  #-nodefaultlibs \
  #-lc \
strip -s smallerxcb

gcc -o smallxcb-only src/xcb-only.c \
  -Os \
  -g0 \
  --entry main \
  -nodefaultlibs \
  -nostartfiles \
  -nostdlib \
  -lc \
  -lxcb
strip -s smallxcb-only

gcc -o smallsfml src/sfml.cpp \
  --entry main \
  -nodefaultlibs \
  -nostartfiles \
  -nostdlib \
  -lsfml-window -lstdc++ -lgcc_s -lc
strip -s smallsfml

gcc -o smallglx src/glx.c \
  -std=c99 \
  -Os \
  -g0 \
  --entry main \
  -nodefaultlibs \
  -nostartfiles \
  -nostdlib \
  -lc \
  -lX11 -lGL
strip -s smallglx

gcc -o smallglx-cpp src/glx.cpp \
  -lX11 -lGL -lstdc++
strip -s smallglx-cpp

ls -alS | grep small
