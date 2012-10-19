INCLUDEPATH += ../shared

HEADERS       = ../shared/Vector3F.h \
                ../shared/Matrix44F.h \
                ../shared/BaseCamera.h \
                ../shared/shapeDrawer.h \
                ../shared/Polytode.h \
                ../shared/Vertex.h \
                ../shared/Facet.h \
                ../shared/GeoElement.h \
                ../shared/Edge.h \
                ../shared/BaseMesh.h \
                ../shared/BaseBuffer.h \
                ../shared/KdTreeNode.h \
                ../shared/KdTree.h \
                ../shared/Primitive.h \
                ../shared/Triangle.h \
                ../shared/BoundingBox.h \
                ../shared/BuildKdTreeContext.h \
				../shared/PrimitiveArray.h \
                glwidget.h \
                window.h \
                SceneContainer.h \
                RandomMesh.h
SOURCES       = ../shared/Vector3F.cpp \
                ../shared/Matrix44F.cpp \
                ../shared/BaseCamera.cpp \
                ../shared/shapeDrawer.cpp \
                ../shared/Polytode.cpp \
                ../shared/Vertex.cpp \
                ../shared/Facet.cpp \
                ../shared/GeoElement.cpp \
                ../shared/Edge.cpp \
                ../shared/BaseMesh.cpp \
                ../shared/BaseBuffer.cpp \
                ../shared/KdTreeNode.cpp \
                ../shared/KdTree.cpp \
                ../shared/Primitive.cpp \
                ../shared/Triangle.cpp \
                ../shared/BoundingBox.cpp \
				../shared/BuildKdTreeContext.cpp \
				../shared/PrimitiveArray.cpp \
                glwidget.cpp \
                main.cpp \
                window.cpp \
                SceneContainer.cpp \
                RandomMesh.cpp
QT           += opengl
win32 {
CONFIG += console
}
