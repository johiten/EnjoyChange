#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T14:41:41
#
#-------------------------------------------------

QT       += core gui

#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EnjoyChange
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ecengine.cpp \
    training.cpp \
    signalpanel.cpp \
    brainpowerpanel.cpp \
    deviceinfopanel.cpp

HEADERS  += mainwindow.h \
    include/edk.h \
    include/edkErrorCode.h \
    include/EmoStateDLL.h \
    include/glut.h \
    include/spppinclude/advmath-impl.h \
    include/spppinclude/advmath.h \
    include/spppinclude/avltree-impl.h \
    include/spppinclude/avltree.h \
    include/spppinclude/bfgs-impl.h \
    include/spppinclude/bfgs.h \
    include/spppinclude/binaryheap-impl.h \
    include/spppinclude/binaryheap.h \
    include/spppinclude/bstree-impl.h \
    include/spppinclude/bstree.h \
    include/spppinclude/bwt-impl.h \
    include/spppinclude/bwt.h \
    include/spppinclude/ccholesky-impl.h \
    include/spppinclude/ccholesky.h \
    include/spppinclude/cevd-impl.h \
    include/spppinclude/cevd.h \
    include/spppinclude/cholesky-impl.h \
    include/spppinclude/cholesky.h \
    include/spppinclude/classicalpse-impl.h \
    include/spppinclude/classicalpse.h \
    include/spppinclude/conjgrad-impl.h \
    include/spppinclude/conjgrad.h \
    include/spppinclude/constants.h \
    include/spppinclude/convolution-impl.h \
    include/spppinclude/convolution.h \
    include/spppinclude/convolution_usefftw-impl.h \
    include/spppinclude/convolution_usefftw.h \
    include/spppinclude/correlation-impl.h \
    include/spppinclude/correlation.h \
    include/spppinclude/correlation_usefftw-impl.h \
    include/spppinclude/correlation_usefftw.h \
    include/spppinclude/cqrd-impl.h \
    include/spppinclude/cqrd.h \
    include/spppinclude/csvd-impl.h \
    include/spppinclude/csvd.h \
    include/spppinclude/cwt-impl.h \
    include/spppinclude/cwt.h \
    include/spppinclude/cwt_usefftw-impl.h \
    include/spppinclude/cwt_usefftw.h \
    include/spppinclude/dfd-impl.h \
    include/spppinclude/dfd.h \
    include/spppinclude/dgt-impl.h \
    include/spppinclude/dgt.h \
    include/spppinclude/dgt_usefftw-impl.h \
    include/spppinclude/dgt_usefftw.h \
    include/spppinclude/dwt-impl.h \
    include/spppinclude/dwt.h \
    include/spppinclude/eigenanalysispse-impl.h \
    include/spppinclude/eigenanalysispse.h \
    include/spppinclude/evd-impl.h \
    include/spppinclude/evd.h \
    include/spppinclude/fft-impl.h \
    include/spppinclude/fft.h \
    include/spppinclude/fftmr-impl.h \
    include/spppinclude/fftmr.h \
    include/spppinclude/fftpf-impl.h \
    include/spppinclude/fftpf.h \
    include/spppinclude/fftw-impl.h \
    include/spppinclude/fftw.h \
    include/spppinclude/fftw3.h \
    include/spppinclude/filtercoefs-impl.h \
    include/spppinclude/filtercoefs.h \
    include/spppinclude/fir-impl.h \
    include/spppinclude/fir.h \
    include/spppinclude/fitcurves.h \
    include/spppinclude/hashtable-impl.h \
    include/spppinclude/hashtable.h \
    include/spppinclude/huffmancode-impl.h \
    include/spppinclude/huffmancode.h \
    include/spppinclude/iir-impl.h \
    include/spppinclude/iir.h \
    include/spppinclude/integral-impl.h \
    include/spppinclude/integral.h \
    include/spppinclude/integrand.h \
    include/spppinclude/interpolation.h \
    include/spppinclude/inverse-impl.h \
    include/spppinclude/inverse.h \
    include/spppinclude/kalman-impl.h \
    include/spppinclude/kalman.h \
    include/spppinclude/levinson-impl.h \
    include/spppinclude/levinson.h \
    include/spppinclude/linequs1-impl.h \
    include/spppinclude/linequs1.h \
    include/spppinclude/linequs2-impl.h \
    include/spppinclude/linequs2.h \
    include/spppinclude/linequs3-impl.h \
    include/spppinclude/linequs3.h \
    include/spppinclude/linesearch-impl.h \
    include/spppinclude/linesearch.h \
    include/spppinclude/lms-impl.h \
    include/spppinclude/lms.h \
    include/spppinclude/lsfitting-impl.h \
    include/spppinclude/lsfitting.h \
    include/spppinclude/lud-impl.h \
    include/spppinclude/lud.h \
    include/spppinclude/matrix-impl.h \
    include/spppinclude/matrix.h \
    include/spppinclude/matrixallocate-impl.h \
    include/spppinclude/matrixallocate.h \
    include/spppinclude/matrixmath-impl.h \
    include/spppinclude/matrixmath.h \
    include/spppinclude/newtoninterp-impl.h \
    include/spppinclude/newtoninterp.h \
    include/spppinclude/nleroot-impl.h \
    include/spppinclude/nleroot.h \
    include/spppinclude/nleroots-impl.h \
    include/spppinclude/nleroots.h \
    include/spppinclude/nlfunc.h \
    include/spppinclude/nlfuncs.h \
    include/spppinclude/objfunc.h \
    include/spppinclude/parametricpse-impl.h \
    include/spppinclude/parametricpse.h \
    include/spppinclude/pseudoinverse-impl.h \
    include/spppinclude/pseudoinverse.h \
    include/spppinclude/qrd-impl.h \
    include/spppinclude/qrd.h \
    include/spppinclude/queue-impl.h \
    include/spppinclude/queue.h \
    include/spppinclude/random-impl.h \
    include/spppinclude/random.h \
    include/spppinclude/rls-impl.h \
    include/spppinclude/rls.h \
    include/spppinclude/sort-impl.h \
    include/spppinclude/sort.h \
    include/spppinclude/spline3interp-impl.h \
    include/spppinclude/spline3interp.h \
    include/spppinclude/stack-impl.h \
    include/spppinclude/stack.h \
    include/spppinclude/statistics-impl.h \
    include/spppinclude/statistics.h \
    include/spppinclude/steepdesc-impl.h \
    include/spppinclude/steepdesc.h \
    include/spppinclude/student.h \
    include/spppinclude/svd-impl.h \
    include/spppinclude/svd.h \
    include/spppinclude/timing-impl.h \
    include/spppinclude/timing.h \
    include/spppinclude/toeplitz-impl.h \
    include/spppinclude/toeplitz.h \
    include/spppinclude/usingdeclare.h \
    include/spppinclude/utilities-impl.h \
    include/spppinclude/utilities.h \
    include/spppinclude/vector-impl.h \
    include/spppinclude/vector.h \
    include/spppinclude/vectormath-impl.h \
    include/spppinclude/vectormath.h \
    include/spppinclude/wft-impl.h \
    include/spppinclude/wft.h \
    include/spppinclude/wft_usefftw-impl.h \
    include/spppinclude/wft_usefftw.h \
    include/spppinclude/wiener-impl.h \
    include/spppinclude/wiener.h \
    include/spppinclude/window-impl.h \
    include/spppinclude/window.h \
    include/spppinclude/wvd-impl.h \
    include/spppinclude/wvd.h \
    ecengine.h \
    training.h \
    signalpanel.h \
    brainpowerpanel.h \
    deviceinfopanel.h

FORMS    += mainwindow.ui \
    training.ui \
    signalpanel.ui \
    brainpowerpanel.ui \
    deviceinfopanel.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ledk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ledk

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ledk_utils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ledk_utils

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lglut32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lglut32

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

DISTFILES += \
    lib/edk.lib \
    lib/edk_utils.lib \
    lib/glut32.lib \
    lib/edk.dll \
    lib/edk_utils.dll \
    .gitignore.swp \
    .swp \
    .gitattributes \
    .gitignore \
    README.md
INCLUDEPATH += $$PWD/include/spppinclude
