SHELL = /bin/sh

C++ = /usr/bin/g++
# Distribution libraries
LIBS =
# Libraries at home
#LIBS = -L../lib/iris -lvrmlapi
# Distribution #includes
#INCLUDES = \
	-I..
# Includes at home
#INCLUDES = \
#	-I.. \
#	-I../vrml/api

# Distribution output directory
OUTPUT_DIR = lib/iris
# Home output directory
#OUTPUT_DIR = ../lib/iris

# Target libraries in distribution
TARGET_LIBS = -L. -L$(OUTPUT_DIR) -lgleem /usr/lib32/libglut.a -lGL -lGLU -lX11 -lXmu -lXi -lm
# Target Libraries at home
#TARGET_LIBS = -L. -L$(OUTPUT_DIR) -lgleem -lvrmlapi /usr/lib32/libglut.a -lGL -lGLU -lX11 -lXmu -lXi -lm

# Use this for -o32 calling convention
#COMPILER_ABI = -32
# Use this for -n32 calling convention
COMPILER_ABI = -n32

# Distribution version
HOME_SWITCH =
# At home
#HOME_SWITCH = -DHAVE_VRML_API

# Debugging options
C++OPTS = -woff 1681 -Wl,-woff,15 -Wl,-woff,85 -Wl,-no_unresolved -Wl,-wall $(COMPILER_ABI) -g $(INCLUDES) $(HOME_SWITCH)

# Optimizing options (no IPA)
#C++OPTS = -woff 1681 -Wl,-woff,15 -Wl,-woff,85 -Wl,-no_unresolved -Wl,-wall $(COMPILER_ABI) -O3 $(INCLUDES) $(HOME_SWITCH)

GLEEM_SRCS = \
	BSphere.cpp			\
	ExaminerViewer.cpp		\
	HandleBoxManip.cpp		\
	_Linalg.cpp			\
	Line.cpp			\
	Manip.cpp			\
	ManipManager.cpp		\
	ManipPart.cpp			\
	ManipPartCube.cpp		\
	ManipPartGroup.cpp		\
	ManipPartHollowCubeFace.cpp	\
	ManipPartLineSeg.cpp		\
	ManipPartSquare.cpp		\
	ManipPartTransform.cpp		\
	ManipPartTriBased.cpp		\
	ManipPartTwoWayArrow.cpp	\
	MathUtil.cpp			\
	NormalCalc.cpp			\
	Plane.cpp			\
	PlaneUV.cpp			\
	RayTriangleIntersection.cpp	\
	RightTruncPyrMapping.cpp	\
	Translate1Manip.cpp		\
	Translate2Manip.cpp

GLEEM_OBJS = $(GLEEM_SRCS:.cpp=.o)
GLEEM = libgleem.so

TEST_TRANSLATE1_SRCS = \
	TestTranslate1.cpp
TEST_TRANSLATE1_OBJS = $(TEST_TRANSLATE1_SRCS:.cpp=.o)
TEST_TRANSLATE1 = testTranslate1
TEST_TRANSLATE1_LIBS = $(TARGET_LIBS)

TEST_TRANSLATE2_SRCS = \
	TestTranslate2.cpp
TEST_TRANSLATE2_OBJS = $(TEST_TRANSLATE2_SRCS:.cpp=.o)
TEST_TRANSLATE2 = testTranslate2
TEST_TRANSLATE2_LIBS = $(TARGET_LIBS)

TEST_HANDLEBOX_SRCS = \
	TestHandleBox.cpp
TEST_HANDLEBOX_OBJS = $(TEST_HANDLEBOX_SRCS:.cpp=.o)
TEST_HANDLEBOX = testHandleBox
TEST_HANDLEBOX_LIBS = $(TARGET_LIBS)

TEST_EXAMINERVIEWER_SRCS = \
	TestExaminerViewer.cpp
TEST_EXAMINERVIEWER_OBJS = $(TEST_EXAMINERVIEWER_SRCS:.cpp=.o)
TEST_EXAMINERVIEWER = testExaminerViewer
TEST_EXAMINERVIEWER_LIBS = $(TARGET_LIBS)

TEST_MULTIWIN_SRCS = \
	TestMultiWin.cpp
TEST_MULTIWIN_OBJS = $(TEST_MULTIWIN_SRCS:.cpp=.o)
TEST_MULTIWIN = testMultiWin
TEST_MULTIWIN_LIBS = $(TARGET_LIBS)

TARGETS = $(GLEEM) $(TEST_TRANSLATE1) $(TEST_TRANSLATE2) $(TEST_HANDLEBOX) $(TEST_EXAMINERVIEWER) $(TEST_MULTIWIN)

SRCS = \
	$(GLEEM_SRCS)		\
	$(TEST_TRANSLATE1_SRCS)	\
	$(TEST_TRANSLATE2_SRCS)	\
	$(TEST_HANDLEBOX_SRCS)  \
	$(TEST_EXAMINERVIEWER_SRCS)

.SUFFIXES: .cpp

#default all: default_targets
default all: default_targets install

default_targets: $(TARGETS)

$(GLEEM): $(GLEEM_OBJS)
	rm -f $(GLEEM)
	$(C++) $(C++OPTS) -shared -o $(GLEEM) $(GLEEM_OBJS) $(LIBS)

$(TEST_TRANSLATE1) : $(TEST_TRANSLATE1_OBJS)
	$(C++) $(C++OPTS) -o $@ $(TEST_TRANSLATE1_OBJS) $(TEST_TRANSLATE1_LIBS)

$(TEST_TRANSLATE2) : $(TEST_TRANSLATE2_OBJS)
	$(C++) $(C++OPTS) -o $@ $(TEST_TRANSLATE2_OBJS) $(TEST_TRANSLATE2_LIBS)

$(TEST_HANDLEBOX) : $(TEST_HANDLEBOX_OBJS)
	$(C++) $(C++OPTS) -o $@ $(TEST_HANDLEBOX_OBJS) $(TEST_HANDLEBOX_LIBS)

$(TEST_EXAMINERVIEWER) : $(TEST_EXAMINERVIEWER_OBJS)
	$(C++) $(C++OPTS) -o $@ $(TEST_EXAMINERVIEWER_OBJS) $(TEST_EXAMINERVIEWER_LIBS)

$(TEST_MULTIWIN) : $(TEST_MULTIWIN_OBJS)
	$(C++) $(C++OPTS) -o $@ $(TEST_MULTIWIN_OBJS) $(TEST_MULTIWIN_LIBS)

install: $(TARGETS)
	if [ ! -d $(OUTPUT_DIR) ]; then mkdir -p $(OUTPUT_DIR); fi
	cp $(TARGETS) ${OUTPUT_DIR}

.cpp.o:
	$(C++) $(C++OPTS) -c $< -o $@

depend:
	makedepend -I/usr/include -I/usr/include/CC $(INCLUDES) -- $(SRCS)

clean:
	rm -rf *.o core $(TARGETS) ii_files so_locations

# DO NOT DELETE THIS LINE -- make depend depends on it.

BSphere.o: /usr/include/assert.h ../gleem/BSphere.h ../gleem/Namespace.h
BSphere.o: ../gleem/Util.h ../gleem/Linalg.h ../gleem/_Linalg.h
BSphere.o: /usr/include/CC/iostream.h /usr/include/memory.h
BSphere.o: /usr/include/stddef.h /usr/include/sgidefs.h
BSphere.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
BSphere.o: ../gleem/MathUtil.h /usr/include/math.h /usr/include/standards.h
ExaminerViewer.o: /usr/include/math.h /usr/include/sgidefs.h
ExaminerViewer.o: /usr/include/standards.h /usr/include/stdlib.h
ExaminerViewer.o: /usr/include/CC/iostream.h /usr/include/memory.h
ExaminerViewer.o: /usr/include/stddef.h /usr/include/assert.h
ExaminerViewer.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
ExaminerViewer.o: /usr/include/GL/glut.h ../gleem/ExaminerViewer.h
ExaminerViewer.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
ExaminerViewer.o: ../gleem/Namespace.h ../gleem/Util.h
ExaminerViewer.o: ../gleem/CameraParameters.h ../gleem/Linalg.h
ExaminerViewer.o: ../gleem/_Linalg.h ../gleem/BSphere.h
ExaminerViewer.o: ../gleem/BasicHashtable.h /usr/include/CC/hashtable.h
ExaminerViewer.o: /usr/include/CC/stl_hashtable.h
ExaminerViewer.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
ExaminerViewer.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
ExaminerViewer.o: /usr/include/string.h /usr/include/limits.h
ExaminerViewer.o: /usr/include/CC/new.h /usr/include/CC/stl_iterator.h
ExaminerViewer.o: /usr/include/CC/stl_alloc.h /usr/include/mutex.h
ExaminerViewer.o: /usr/include/time.h /usr/include/CC/stl_construct.h
ExaminerViewer.o: /usr/include/CC/stl_tempbuf.h /usr/include/CC/stl_algo.h
ExaminerViewer.o: /usr/include/CC/stl_heap.h
ExaminerViewer.o: /usr/include/CC/stl_uninitialized.h
ExaminerViewer.o: /usr/include/CC/stl_function.h /usr/include/CC/stl_vector.h
ExaminerViewer.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
ExaminerViewer.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
ExaminerViewer.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
ExaminerViewer.o: /usr/include/CC/stl_raw_storage_iter.h
ExaminerViewer.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
ExaminerViewer.o: /usr/include/CC/alloc.h /usr/include/CC/vector.h
ExaminerViewer.o: ../gleem/MathUtil.h
HandleBoxManip.o: /usr/include/math.h /usr/include/sgidefs.h
HandleBoxManip.o: /usr/include/standards.h ../gleem/HandleBoxManip.h
HandleBoxManip.o: ../gleem/Namespace.h ../gleem/Util.h ../gleem/Manip.h
HandleBoxManip.o: /usr/include/CC/vector.h /usr/include/CC/algobase.h
HandleBoxManip.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
HandleBoxManip.o: /usr/include/CC/function.h /usr/include/stddef.h
HandleBoxManip.o: /usr/include/CC/iostream.h /usr/include/memory.h
HandleBoxManip.o: /usr/include/CC/stl_raw_storage_iter.h
HandleBoxManip.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
HandleBoxManip.o: ../gleem/HitPoint.h ../gleem/Linalg.h ../gleem/_Linalg.h
HandleBoxManip.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
HandleBoxManip.o: ../gleem/ManipPart.h ../gleem/Plane.h ../gleem/PlaneUV.h
HandleBoxManip.o: ../gleem/ManipPartTransform.h ../gleem/ManipPartGroup.h
HandleBoxManip.o: ../gleem/ManipPartHollowCubeFace.h
HandleBoxManip.o: ../gleem/ManipPartTriBased.h ../gleem/ManipPartCube.h
HandleBoxManip.o: ../gleem/ManipPartSquare.h
_Linalg.o: /usr/include/assert.h /usr/include/math.h /usr/include/sgidefs.h
_Linalg.o: /usr/include/standards.h ../gleem/_Linalg.h
_Linalg.o: /usr/include/CC/iostream.h /usr/include/memory.h
_Linalg.o: /usr/include/stddef.h /usr/include/CC/bool.h
_Linalg.o: /usr/include/CC/stl_config.h
Line.o: /usr/include/CC/iostream.h /usr/include/memory.h
Line.o: /usr/include/stddef.h /usr/include/sgidefs.h ../gleem/Line.h
Line.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
Line.o: ../gleem/Namespace.h ../gleem/Util.h ../gleem/Linalg.h
Line.o: ../gleem/_Linalg.h
Manip.o: /usr/include/GL/gl.h /usr/include/GL/glu.h /usr/include/GL/glut.h
Manip.o: ../gleem/Manip.h /usr/include/CC/vector.h /usr/include/CC/algobase.h
Manip.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
Manip.o: /usr/include/CC/function.h /usr/include/stddef.h
Manip.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
Manip.o: /usr/include/memory.h /usr/include/CC/stl_raw_storage_iter.h
Manip.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
Manip.o: ../gleem/Namespace.h ../gleem/Util.h ../gleem/HitPoint.h
Manip.o: ../gleem/Linalg.h ../gleem/_Linalg.h /usr/include/CC/bool.h
Manip.o: /usr/include/CC/stl_config.h ../gleem/ManipPart.h
Manip.o: ../gleem/ManipManager.h ../gleem/ScreenToRayMapping.h
Manip.o: ../gleem/CameraParameters.h ../gleem/BasicHashtable.h
Manip.o: /usr/include/CC/hashtable.h /usr/include/CC/stl_hashtable.h
Manip.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
Manip.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
Manip.o: /usr/include/string.h /usr/include/standards.h /usr/include/limits.h
Manip.o: /usr/include/stdlib.h /usr/include/CC/new.h
Manip.o: /usr/include/CC/stl_iterator.h /usr/include/CC/stl_alloc.h
Manip.o: /usr/include/assert.h /usr/include/mutex.h /usr/include/time.h
Manip.o: /usr/include/CC/stl_construct.h /usr/include/CC/stl_tempbuf.h
Manip.o: /usr/include/CC/stl_algo.h /usr/include/CC/stl_heap.h
Manip.o: /usr/include/CC/stl_uninitialized.h /usr/include/CC/stl_function.h
Manip.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
Manip.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
ManipManager.o: /usr/include/GL/gl.h /usr/include/GL/glut.h
ManipManager.o: /usr/include/GL/glu.h /usr/include/CC/iostream.h
ManipManager.o: /usr/include/memory.h /usr/include/stddef.h
ManipManager.o: /usr/include/sgidefs.h /usr/include/CC/algo.h
ManipManager.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
ManipManager.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
ManipManager.o: /usr/include/CC/stl_raw_storage_iter.h
ManipManager.o: /usr/include/CC/tempbuf.h /usr/include/limits.h
ManipManager.o: /usr/include/standards.h /usr/include/stdlib.h
ManipManager.o: /usr/include/CC/stl_algo.h /usr/include/CC/stl_heap.h
ManipManager.o: /usr/include/CC/stl_numeric.h ../gleem/ManipManager.h
ManipManager.o: /usr/include/CC/vector.h /usr/include/CC/alloc.h
ManipManager.o: /usr/include/CC/stl_vector.h ../gleem/Namespace.h
ManipManager.o: ../gleem/Util.h ../gleem/ScreenToRayMapping.h
ManipManager.o: ../gleem/CameraParameters.h ../gleem/Linalg.h
ManipManager.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
ManipManager.o: /usr/include/CC/stl_config.h ../gleem/BasicHashtable.h
ManipManager.o: /usr/include/CC/hashtable.h /usr/include/CC/stl_hashtable.h
ManipManager.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
ManipManager.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
ManipManager.o: /usr/include/string.h /usr/include/CC/new.h
ManipManager.o: /usr/include/CC/stl_iterator.h /usr/include/CC/stl_alloc.h
ManipManager.o: /usr/include/assert.h /usr/include/mutex.h
ManipManager.o: /usr/include/time.h /usr/include/CC/stl_construct.h
ManipManager.o: /usr/include/CC/stl_tempbuf.h
ManipManager.o: /usr/include/CC/stl_uninitialized.h
ManipManager.o: /usr/include/CC/stl_function.h /usr/include/CC/stl_hash_fun.h
ManipManager.o: ../gleem/RightTruncPyrMapping.h ../gleem/Manip.h
ManipManager.o: ../gleem/HitPoint.h ../gleem/ManipPart.h
ManipPart.o: ../gleem/ManipPart.h /usr/include/CC/bool.h
ManipPart.o: /usr/include/CC/stl_config.h /usr/include/CC/vector.h
ManipPart.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
ManipPart.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
ManipPart.o: /usr/include/stddef.h /usr/include/sgidefs.h
ManipPart.o: /usr/include/CC/iostream.h /usr/include/memory.h
ManipPart.o: /usr/include/CC/stl_raw_storage_iter.h /usr/include/CC/alloc.h
ManipPart.o: /usr/include/CC/stl_vector.h ../gleem/Namespace.h
ManipPart.o: ../gleem/Util.h ../gleem/HitPoint.h ../gleem/Linalg.h
ManipPart.o: ../gleem/_Linalg.h
ManipPartCube.o: ../gleem/ManipPartCube.h ../gleem/Namespace.h
ManipPartCube.o: ../gleem/Util.h ../gleem/ManipPartTriBased.h
ManipPartCube.o: /usr/include/CC/vector.h /usr/include/CC/algobase.h
ManipPartCube.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
ManipPartCube.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartCube.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartCube.o: /usr/include/memory.h /usr/include/CC/stl_raw_storage_iter.h
ManipPartCube.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
ManipPartCube.o: ../gleem/Manip.h ../gleem/HitPoint.h ../gleem/Linalg.h
ManipPartCube.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
ManipPartCube.o: /usr/include/CC/stl_config.h ../gleem/ManipPart.h
ManipPartCube.o: ../gleem/NormalCalc.h
ManipPartGroup.o: /usr/include/CC/algo.h /usr/include/CC/algobase.h
ManipPartGroup.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
ManipPartGroup.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartGroup.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartGroup.o: /usr/include/memory.h
ManipPartGroup.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartGroup.o: /usr/include/CC/tempbuf.h /usr/include/limits.h
ManipPartGroup.o: /usr/include/standards.h /usr/include/stdlib.h
ManipPartGroup.o: /usr/include/CC/stl_algo.h /usr/include/CC/stl_heap.h
ManipPartGroup.o: /usr/include/CC/stl_numeric.h ../gleem/ManipPartGroup.h
ManipPartGroup.o: /usr/include/CC/vector.h /usr/include/CC/alloc.h
ManipPartGroup.o: /usr/include/CC/stl_vector.h ../gleem/Namespace.h
ManipPartGroup.o: ../gleem/Util.h ../gleem/ManipPart.h /usr/include/CC/bool.h
ManipPartGroup.o: /usr/include/CC/stl_config.h ../gleem/HitPoint.h
ManipPartGroup.o: ../gleem/Linalg.h ../gleem/_Linalg.h
ManipPartHollowCubeFace.o: ../gleem/ManipPartHollowCubeFace.h
ManipPartHollowCubeFace.o: ../gleem/Namespace.h ../gleem/Util.h
ManipPartHollowCubeFace.o: ../gleem/ManipPartTriBased.h
ManipPartHollowCubeFace.o: /usr/include/CC/vector.h
ManipPartHollowCubeFace.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
ManipPartHollowCubeFace.o: /usr/include/CC/iterator.h
ManipPartHollowCubeFace.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartHollowCubeFace.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartHollowCubeFace.o: /usr/include/memory.h
ManipPartHollowCubeFace.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartHollowCubeFace.o: /usr/include/CC/alloc.h
ManipPartHollowCubeFace.o: /usr/include/CC/stl_vector.h ../gleem/Manip.h
ManipPartHollowCubeFace.o: ../gleem/HitPoint.h ../gleem/Linalg.h
ManipPartHollowCubeFace.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
ManipPartHollowCubeFace.o: /usr/include/CC/stl_config.h ../gleem/ManipPart.h
ManipPartHollowCubeFace.o: ../gleem/NormalCalc.h
ManipPartSquare.o: ../gleem/ManipPartSquare.h ../gleem/Namespace.h
ManipPartSquare.o: ../gleem/Util.h ../gleem/ManipPartTriBased.h
ManipPartSquare.o: /usr/include/CC/vector.h /usr/include/CC/algobase.h
ManipPartSquare.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
ManipPartSquare.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartSquare.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartSquare.o: /usr/include/memory.h
ManipPartSquare.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartSquare.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
ManipPartSquare.o: ../gleem/Manip.h ../gleem/HitPoint.h ../gleem/Linalg.h
ManipPartSquare.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
ManipPartSquare.o: /usr/include/CC/stl_config.h ../gleem/ManipPart.h
ManipPartSquare.o: ../gleem/NormalCalc.h
ManipPartTransform.o: ../gleem/ManipPartTransform.h ../gleem/Namespace.h
ManipPartTransform.o: ../gleem/Util.h ../gleem/ManipPartGroup.h
ManipPartTransform.o: /usr/include/CC/vector.h /usr/include/CC/algobase.h
ManipPartTransform.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
ManipPartTransform.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartTransform.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartTransform.o: /usr/include/memory.h
ManipPartTransform.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartTransform.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
ManipPartTransform.o: ../gleem/ManipPart.h /usr/include/CC/bool.h
ManipPartTransform.o: /usr/include/CC/stl_config.h ../gleem/HitPoint.h
ManipPartTransform.o: ../gleem/Linalg.h ../gleem/_Linalg.h
ManipPartTriBased.o: /usr/include/assert.h /usr/include/GL/gl.h
ManipPartTriBased.o: ../gleem/ManipPartTriBased.h /usr/include/CC/vector.h
ManipPartTriBased.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
ManipPartTriBased.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
ManipPartTriBased.o: /usr/include/stddef.h /usr/include/sgidefs.h
ManipPartTriBased.o: /usr/include/CC/iostream.h /usr/include/memory.h
ManipPartTriBased.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartTriBased.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
ManipPartTriBased.o: ../gleem/Namespace.h ../gleem/Util.h ../gleem/Manip.h
ManipPartTriBased.o: ../gleem/HitPoint.h ../gleem/Linalg.h ../gleem/_Linalg.h
ManipPartTriBased.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
ManipPartTriBased.o: ../gleem/ManipPart.h ../gleem/RayTriangleIntersection.h
ManipPartTwoWayArrow.o: ../gleem/ManipPartTwoWayArrow.h ../gleem/Namespace.h
ManipPartTwoWayArrow.o: ../gleem/Util.h ../gleem/ManipPartTriBased.h
ManipPartTwoWayArrow.o: /usr/include/CC/vector.h /usr/include/CC/algobase.h
ManipPartTwoWayArrow.o: /usr/include/CC/pair.h /usr/include/CC/iterator.h
ManipPartTwoWayArrow.o: /usr/include/CC/function.h /usr/include/stddef.h
ManipPartTwoWayArrow.o: /usr/include/sgidefs.h /usr/include/CC/iostream.h
ManipPartTwoWayArrow.o: /usr/include/memory.h
ManipPartTwoWayArrow.o: /usr/include/CC/stl_raw_storage_iter.h
ManipPartTwoWayArrow.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
ManipPartTwoWayArrow.o: ../gleem/Manip.h ../gleem/HitPoint.h
ManipPartTwoWayArrow.o: ../gleem/Linalg.h ../gleem/_Linalg.h
ManipPartTwoWayArrow.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
ManipPartTwoWayArrow.o: ../gleem/ManipPart.h ../gleem/NormalCalc.h
MathUtil.o: ../gleem/MathUtil.h /usr/include/CC/bool.h
MathUtil.o: /usr/include/CC/stl_config.h /usr/include/math.h
MathUtil.o: /usr/include/sgidefs.h /usr/include/standards.h
MathUtil.o: ../gleem/Namespace.h ../gleem/Util.h ../gleem/Linalg.h
MathUtil.o: ../gleem/_Linalg.h /usr/include/CC/iostream.h
MathUtil.o: /usr/include/memory.h /usr/include/stddef.h
NormalCalc.o: /usr/include/CC/iostream.h /usr/include/memory.h
NormalCalc.o: /usr/include/stddef.h /usr/include/sgidefs.h
NormalCalc.o: ../gleem/NormalCalc.h /usr/include/CC/bool.h
NormalCalc.o: /usr/include/CC/stl_config.h ../gleem/Namespace.h
NormalCalc.o: ../gleem/Util.h ../gleem/Linalg.h ../gleem/_Linalg.h
Plane.o: ../gleem/Plane.h ../gleem/Namespace.h ../gleem/Util.h
Plane.o: ../gleem/Linalg.h ../gleem/_Linalg.h /usr/include/CC/iostream.h
Plane.o: /usr/include/memory.h /usr/include/stddef.h /usr/include/sgidefs.h
Plane.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
PlaneUV.o: /usr/include/math.h /usr/include/sgidefs.h
PlaneUV.o: /usr/include/standards.h ../gleem/PlaneUV.h ../gleem/Namespace.h
PlaneUV.o: ../gleem/Util.h ../gleem/Linalg.h ../gleem/_Linalg.h
PlaneUV.o: /usr/include/CC/iostream.h /usr/include/memory.h
PlaneUV.o: /usr/include/stddef.h /usr/include/CC/bool.h
PlaneUV.o: /usr/include/CC/stl_config.h ../gleem/MathUtil.h
RayTriangleIntersection.o: /usr/include/assert.h /usr/include/stdio.h
RayTriangleIntersection.o: /usr/include/standards.h /usr/include/sgidefs.h
RayTriangleIntersection.o: /usr/include/math.h
RayTriangleIntersection.o: ../gleem/RayTriangleIntersection.h
RayTriangleIntersection.o: ../gleem/Namespace.h ../gleem/Util.h
RayTriangleIntersection.o: ../gleem/Linalg.h ../gleem/_Linalg.h
RayTriangleIntersection.o: /usr/include/CC/iostream.h /usr/include/memory.h
RayTriangleIntersection.o: /usr/include/stddef.h /usr/include/CC/bool.h
RayTriangleIntersection.o: /usr/include/CC/stl_config.h
RightTruncPyrMapping.o: /usr/include/math.h /usr/include/sgidefs.h
RightTruncPyrMapping.o: /usr/include/standards.h /usr/include/CC/iostream.h
RightTruncPyrMapping.o: /usr/include/memory.h /usr/include/stddef.h
RightTruncPyrMapping.o: ../gleem/RightTruncPyrMapping.h ../gleem/Namespace.h
RightTruncPyrMapping.o: ../gleem/Util.h ../gleem/ScreenToRayMapping.h
RightTruncPyrMapping.o: ../gleem/CameraParameters.h ../gleem/Linalg.h
RightTruncPyrMapping.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
RightTruncPyrMapping.o: /usr/include/CC/stl_config.h
Translate1Manip.o: ../gleem/Translate1Manip.h ../gleem/Namespace.h
Translate1Manip.o: ../gleem/Util.h ../gleem/Manip.h /usr/include/CC/vector.h
Translate1Manip.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
Translate1Manip.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
Translate1Manip.o: /usr/include/stddef.h /usr/include/sgidefs.h
Translate1Manip.o: /usr/include/CC/iostream.h /usr/include/memory.h
Translate1Manip.o: /usr/include/CC/stl_raw_storage_iter.h
Translate1Manip.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
Translate1Manip.o: ../gleem/HitPoint.h ../gleem/Linalg.h ../gleem/_Linalg.h
Translate1Manip.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
Translate1Manip.o: ../gleem/ManipPart.h ../gleem/Line.h
Translate1Manip.o: ../gleem/ManipPartTwoWayArrow.h
Translate1Manip.o: ../gleem/ManipPartTriBased.h ../gleem/MathUtil.h
Translate1Manip.o: /usr/include/math.h /usr/include/standards.h
Translate2Manip.o: ../gleem/Translate2Manip.h ../gleem/Namespace.h
Translate2Manip.o: ../gleem/Util.h ../gleem/Manip.h /usr/include/CC/vector.h
Translate2Manip.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
Translate2Manip.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
Translate2Manip.o: /usr/include/stddef.h /usr/include/sgidefs.h
Translate2Manip.o: /usr/include/CC/iostream.h /usr/include/memory.h
Translate2Manip.o: /usr/include/CC/stl_raw_storage_iter.h
Translate2Manip.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
Translate2Manip.o: ../gleem/HitPoint.h ../gleem/Linalg.h ../gleem/_Linalg.h
Translate2Manip.o: /usr/include/CC/bool.h /usr/include/CC/stl_config.h
Translate2Manip.o: ../gleem/ManipPart.h ../gleem/Plane.h
Translate2Manip.o: ../gleem/ManipPartTwoWayArrow.h
Translate2Manip.o: ../gleem/ManipPartTriBased.h ../gleem/ManipPartTransform.h
Translate2Manip.o: ../gleem/ManipPartGroup.h ../gleem/MathUtil.h
Translate2Manip.o: /usr/include/math.h /usr/include/standards.h
TestTranslate1.o: /usr/include/math.h /usr/include/sgidefs.h
TestTranslate1.o: /usr/include/standards.h /usr/include/GL/gl.h
TestTranslate1.o: /usr/include/GL/glu.h /usr/include/GL/glut.h
TestTranslate1.o: ../gleem/ManipManager.h /usr/include/CC/vector.h
TestTranslate1.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
TestTranslate1.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
TestTranslate1.o: /usr/include/stddef.h /usr/include/CC/iostream.h
TestTranslate1.o: /usr/include/memory.h
TestTranslate1.o: /usr/include/CC/stl_raw_storage_iter.h
TestTranslate1.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
TestTranslate1.o: ../gleem/Namespace.h ../gleem/Util.h
TestTranslate1.o: ../gleem/ScreenToRayMapping.h ../gleem/CameraParameters.h
TestTranslate1.o: ../gleem/Linalg.h ../gleem/_Linalg.h /usr/include/CC/bool.h
TestTranslate1.o: /usr/include/CC/stl_config.h ../gleem/BasicHashtable.h
TestTranslate1.o: /usr/include/CC/hashtable.h /usr/include/CC/stl_hashtable.h
TestTranslate1.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
TestTranslate1.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
TestTranslate1.o: /usr/include/string.h /usr/include/limits.h
TestTranslate1.o: /usr/include/stdlib.h /usr/include/CC/new.h
TestTranslate1.o: /usr/include/CC/stl_iterator.h /usr/include/CC/stl_alloc.h
TestTranslate1.o: /usr/include/assert.h /usr/include/mutex.h
TestTranslate1.o: /usr/include/time.h /usr/include/CC/stl_construct.h
TestTranslate1.o: /usr/include/CC/stl_tempbuf.h /usr/include/CC/stl_algo.h
TestTranslate1.o: /usr/include/CC/stl_heap.h
TestTranslate1.o: /usr/include/CC/stl_uninitialized.h
TestTranslate1.o: /usr/include/CC/stl_function.h
TestTranslate1.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
TestTranslate1.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
TestTranslate1.o: ../gleem/Translate1Manip.h ../gleem/Manip.h
TestTranslate1.o: ../gleem/HitPoint.h ../gleem/ManipPart.h ../gleem/Line.h
TestTranslate2.o: /usr/include/math.h /usr/include/sgidefs.h
TestTranslate2.o: /usr/include/standards.h /usr/include/GL/gl.h
TestTranslate2.o: /usr/include/GL/glu.h /usr/include/GL/glut.h
TestTranslate2.o: ../gleem/ManipManager.h /usr/include/CC/vector.h
TestTranslate2.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
TestTranslate2.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
TestTranslate2.o: /usr/include/stddef.h /usr/include/CC/iostream.h
TestTranslate2.o: /usr/include/memory.h
TestTranslate2.o: /usr/include/CC/stl_raw_storage_iter.h
TestTranslate2.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
TestTranslate2.o: ../gleem/Namespace.h ../gleem/Util.h
TestTranslate2.o: ../gleem/ScreenToRayMapping.h ../gleem/CameraParameters.h
TestTranslate2.o: ../gleem/Linalg.h ../gleem/_Linalg.h /usr/include/CC/bool.h
TestTranslate2.o: /usr/include/CC/stl_config.h ../gleem/BasicHashtable.h
TestTranslate2.o: /usr/include/CC/hashtable.h /usr/include/CC/stl_hashtable.h
TestTranslate2.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
TestTranslate2.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
TestTranslate2.o: /usr/include/string.h /usr/include/limits.h
TestTranslate2.o: /usr/include/stdlib.h /usr/include/CC/new.h
TestTranslate2.o: /usr/include/CC/stl_iterator.h /usr/include/CC/stl_alloc.h
TestTranslate2.o: /usr/include/assert.h /usr/include/mutex.h
TestTranslate2.o: /usr/include/time.h /usr/include/CC/stl_construct.h
TestTranslate2.o: /usr/include/CC/stl_tempbuf.h /usr/include/CC/stl_algo.h
TestTranslate2.o: /usr/include/CC/stl_heap.h
TestTranslate2.o: /usr/include/CC/stl_uninitialized.h
TestTranslate2.o: /usr/include/CC/stl_function.h
TestTranslate2.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
TestTranslate2.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
TestTranslate2.o: ../gleem/Translate2Manip.h ../gleem/Manip.h
TestTranslate2.o: ../gleem/HitPoint.h ../gleem/ManipPart.h ../gleem/Plane.h
TestHandleBox.o: /usr/include/math.h /usr/include/sgidefs.h
TestHandleBox.o: /usr/include/standards.h /usr/include/GL/gl.h
TestHandleBox.o: /usr/include/GL/glu.h /usr/include/GL/glut.h
TestHandleBox.o: ../gleem/ManipManager.h /usr/include/CC/vector.h
TestHandleBox.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
TestHandleBox.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
TestHandleBox.o: /usr/include/stddef.h /usr/include/CC/iostream.h
TestHandleBox.o: /usr/include/memory.h /usr/include/CC/stl_raw_storage_iter.h
TestHandleBox.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
TestHandleBox.o: ../gleem/Namespace.h ../gleem/Util.h
TestHandleBox.o: ../gleem/ScreenToRayMapping.h ../gleem/CameraParameters.h
TestHandleBox.o: ../gleem/Linalg.h ../gleem/_Linalg.h /usr/include/CC/bool.h
TestHandleBox.o: /usr/include/CC/stl_config.h ../gleem/BasicHashtable.h
TestHandleBox.o: /usr/include/CC/hashtable.h /usr/include/CC/stl_hashtable.h
TestHandleBox.o: /usr/include/CC/stl_algobase.h /usr/include/CC/stl_relops.h
TestHandleBox.o: /usr/include/CC/stl_pair.h /usr/include/CC/type_traits.h
TestHandleBox.o: /usr/include/string.h /usr/include/limits.h
TestHandleBox.o: /usr/include/stdlib.h /usr/include/CC/new.h
TestHandleBox.o: /usr/include/CC/stl_iterator.h /usr/include/CC/stl_alloc.h
TestHandleBox.o: /usr/include/assert.h /usr/include/mutex.h
TestHandleBox.o: /usr/include/time.h /usr/include/CC/stl_construct.h
TestHandleBox.o: /usr/include/CC/stl_tempbuf.h /usr/include/CC/stl_algo.h
TestHandleBox.o: /usr/include/CC/stl_heap.h
TestHandleBox.o: /usr/include/CC/stl_uninitialized.h
TestHandleBox.o: /usr/include/CC/stl_function.h
TestHandleBox.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
TestHandleBox.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
TestHandleBox.o: ../gleem/HandleBoxManip.h ../gleem/Manip.h
TestHandleBox.o: ../gleem/HitPoint.h ../gleem/ManipPart.h ../gleem/Plane.h
TestHandleBox.o: ../gleem/PlaneUV.h
TestExaminerViewer.o: /usr/include/math.h /usr/include/sgidefs.h
TestExaminerViewer.o: /usr/include/standards.h /usr/include/GL/gl.h
TestExaminerViewer.o: /usr/include/GL/glu.h /usr/include/GL/glut.h
TestExaminerViewer.o: ../gleem/ManipManager.h /usr/include/CC/vector.h
TestExaminerViewer.o: /usr/include/CC/algobase.h /usr/include/CC/pair.h
TestExaminerViewer.o: /usr/include/CC/iterator.h /usr/include/CC/function.h
TestExaminerViewer.o: /usr/include/stddef.h /usr/include/CC/iostream.h
TestExaminerViewer.o: /usr/include/memory.h
TestExaminerViewer.o: /usr/include/CC/stl_raw_storage_iter.h
TestExaminerViewer.o: /usr/include/CC/alloc.h /usr/include/CC/stl_vector.h
TestExaminerViewer.o: ../gleem/Namespace.h ../gleem/Util.h
TestExaminerViewer.o: ../gleem/ScreenToRayMapping.h
TestExaminerViewer.o: ../gleem/CameraParameters.h ../gleem/Linalg.h
TestExaminerViewer.o: ../gleem/_Linalg.h /usr/include/CC/bool.h
TestExaminerViewer.o: /usr/include/CC/stl_config.h ../gleem/BasicHashtable.h
TestExaminerViewer.o: /usr/include/CC/hashtable.h
TestExaminerViewer.o: /usr/include/CC/stl_hashtable.h
TestExaminerViewer.o: /usr/include/CC/stl_algobase.h
TestExaminerViewer.o: /usr/include/CC/stl_relops.h /usr/include/CC/stl_pair.h
TestExaminerViewer.o: /usr/include/CC/type_traits.h /usr/include/string.h
TestExaminerViewer.o: /usr/include/limits.h /usr/include/stdlib.h
TestExaminerViewer.o: /usr/include/CC/new.h /usr/include/CC/stl_iterator.h
TestExaminerViewer.o: /usr/include/CC/stl_alloc.h /usr/include/assert.h
TestExaminerViewer.o: /usr/include/mutex.h /usr/include/time.h
TestExaminerViewer.o: /usr/include/CC/stl_construct.h
TestExaminerViewer.o: /usr/include/CC/stl_tempbuf.h
TestExaminerViewer.o: /usr/include/CC/stl_algo.h /usr/include/CC/stl_heap.h
TestExaminerViewer.o: /usr/include/CC/stl_uninitialized.h
TestExaminerViewer.o: /usr/include/CC/stl_function.h
TestExaminerViewer.o: /usr/include/CC/stl_hash_fun.h /usr/include/CC/algo.h
TestExaminerViewer.o: /usr/include/CC/tempbuf.h /usr/include/CC/stl_numeric.h
TestExaminerViewer.o: ../gleem/HandleBoxManip.h ../gleem/Manip.h
TestExaminerViewer.o: ../gleem/HitPoint.h ../gleem/ManipPart.h
TestExaminerViewer.o: ../gleem/Plane.h ../gleem/PlaneUV.h ../gleem/MathUtil.h
TestExaminerViewer.o: ../gleem/ExaminerViewer.h ../gleem/BSphere.h
