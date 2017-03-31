CC = g++

clean:
	-rm -rf build

mkdir_build:
	-mkdir build

predictor:
	$(CC) -o build/predictor \
		src/predictor.cpp \
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
		src/liblinear/blas/dscal.c

learner:
	$(CC) -o build/learner \
		src/learner.cpp \
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
		src/liblinear/blas/dscal.c

compile: mkdir_build predictor learner

