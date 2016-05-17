
CC = g++
CFLAGS = -g -std=c++11 -Wall
SRCDIR = src
BUILDDIR = build
TARGET = bin/runner

LIBGLUI = -L./lib -lglui
LIBGLUT   = -L/usr/X11R6/lib -lglut
CFLAGS += -I/usr/X11R6/include

SRCEXT = cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB    = -lXmu -lXext -lX11 -lXi -lm -lglut -lGLU -lGL $(LIBGLUT) $(LIBGLUI)
INC = -I include


(TARGET): $(OBJECTS)
	@echo " Linking..."
	mkdir -p bin
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)


#CPPFLAGS += -I./ -I./include


#GLUI_OBJS = lib/glui_add_controls.o lib/glui_string.o lib/glui.o lib/glui_bitmap_img_data.o lib/glui_bitmaps.o lib/glui_button.o lib/glui_edittext.o lib/glui_commandline.o lib/glui_checkbox.o lib/glui_node.o lib/glui_radio.o lib/glui_statictext.o lib/glui_panel.o lib/glui_separator.o lib/glui_spinner.o lib/glui_control.o lib/glui_column.o lib/glui_translation.o lib/glui_rotation.o lib/glui_mouse_iaction.o lib/glui_listbox.o lib/glui_rollout.o lib/glui_window.o lib/arcball.o lib/algebra3.o lib/quaternion.o lib/viewmodel.o lib/glui_treepanel.o lib/glui_tree.o lib/glui_textbox.o lib/glui_scrollbar.o lib/glui_list.o lib/glui_filebrowser.o

#GLUI_LIB = lib/libglui.a

#GLUI_TOOLS = bin/ppm2array

#GLUI_MAIN = bin/main



#.PHONY: all setup main tools clean depend dist

#all: setup $(GLUI_LIB) main
#all: main

#setup:
#	mkdir -p bin
#	mkdir -p lib

#main: $(GLUI_MAIN)

#tools: $(GLUI_TOOLS)

# bin/ppm2array: tools/ppm2array.cpp tools/ppm.cpp
# 	$(CXX) $(CPPFLAGS) -o $@ $^
#
# bin/%: code/%.cpp $(GLUI_LIB)
# 	clear
# 	$(CXX) -g $(CPPFLAGS) -o $@ $< $(CLASSES) $(LIBGLUI) $(LIBGLUT) $(LIBGL) $(LIBS)
#
# $(GLUI_LIB): $(GLUI_OBJS)
# 	ar -r $(GLUI_LIB) $(GLUI_OBJS)
#
# .cpp.o:
# 	$(CXX) $(CPPFLAGS) -c $<
#
# clean:
# 	rm -f *.o $(GLUI_LIB) $(GLUI_MAIN) $(GLUI_TOOLS)
# 	rm -fr doc/html
#
# depend:
# 	makedepend -Y./include `find -name "*.cpp"` `find -name "*.c"`
#
# DIST = glui-2.3.0
#
# dist: clean
# 	mkdir -p $(DIST)
# 	cp --parents \
# 		`find -type f -name "*.cpp"` \
# 		`find -type f -name "*.c"` \
# 		`find -type f -name "*.h"` \
# 		`find -type f -name "*.dev"` \
# 		`find -type f -name "*.dsp"` \
# 		`find -type f -name "*.dsw"` \
# 		`find -type f -name "*.vcproj"` \
# 		`find -type f -name "*.sln"` \
# 		`find -type f -name "*.txt"` \
# 		makefile \
# 		$(DIST)
# 	tar cv $(DIST) | gzip -9 - > $(DIST).tgz
# 	rm -Rf $(DIST)
