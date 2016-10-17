cmake_minimum_required(VERSION 3.6)
project(metric_view)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -I/usr/X11R6/include -I include  -lXmu -lXext -lX11 -lXi -lm -lGLU -lGL -L/usr/X11R6/lib -lglut -L./lib -lglui -lGLU")

set(SOURCE_FILES
    include/GL/GLUI/include/GL/glui.h
    include/GL/GLUI/glui.cpp
    include/GL/glui.h
    include/BaseEntity.h
    include/Canvas.h
    include/Color.h
    include/Colormap.h
    include/Container.h
    include/Controller.h
    include/CsvParser.h
    include/Entity.h
        include/EntityData.h
    include/interaction.h
    include/MetricRank.h
    include/Mouse.h
    include/Package.h
    include/Point.h
    include/ProjectionCanvas.h
    include/renderer.h
    include/SpectrographCanvas.h
    include/StreamgraphCanvas.h
    include/SunburstCanvas.h
    include/Treemap.h
    include/TreemapCanvas.h
    lib/libglui.a
    src/BaseEntity.cpp
    src/Canvas.cpp
    src/Colormap.cpp
    src/Container.cpp
    src/CsvParser.cpp
    src/Entity.cpp
        src/EntityData.cpp
    src/interaction.cpp
    src/main.cpp
    src/MetricRank.cpp
    src/Mouse.cpp
    src/Package.cpp
    src/ProjectionCanvas.cpp
    src/renderer.cpp
    src/SpectrographCanvas.cpp
    src/StreamgraphCanvas.cpp
    src/SunburstCanvas.cpp
    src/Treemap.cpp
    src/TreemapCanvas.cpp)

add_custom_target(metric_view ALL COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(metric_view_dummy ${SOURCE_FILES})