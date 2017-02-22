g++ -o preditctor \
       src/preditctor.cpp \
       src/Machine.h src/Machine.cpp \
       src/FeaturesSelection.h src/FeaturesSelection.cpp \
       src/Feats.h src/Feats.cpp \
       src/DicMap.h src/DicMap.cpp \
       src/SylMap.h src/SylMap.cpp \
       src/StrMap.h src/StrMap.cpp \
       src/configure.h \
       src/liblinear/linear.h src/liblinear/linear.cpp \
       src/liblinear/tron.h src/liblinear/tron.cpp \
       src/liblinear/blas/dnrm2.c \
       src/liblinear/blas/daxpy.c \
       src/liblinear/blas/blas.h \
       src/liblinear/blas/ddot.c \
       src/liblinear/blas/dscal.c \
