SRCDIR=src
BINDIR=bin
OBJDIR=$(SRCDIR)

#TO ADD A NEW OUTPUT BINARY...
#  add its name to BINS
#  add a variable listing its objects (see OMAIN, MAIN_OBJECTS)
#  copy the $(BINDIR)/main target, replacing main with your output, and its
#    prereqs with your variable listing objects.
#NOTE: -std=c++11 is no longer a prerequisite, but we can reintroduce it
#    as soon as it is needed.
BINS=main

OMAIN=main.o scenenode.o sciurus/sciurus.o sciurus/window.o sciurus/keyboard.o
MAIN_OBJECTS=$(OMAIN:%=$(OBJDIR)/%)

OUTPUTS=$(BINS:%=$(BINDIR)/%)

LOCAL_INCLUDES=-I$(SRCDIR)
SDL_LIBS=-lSDL2
BULLET_INCLUDES=-I/usr/include/bullet
BULLET_LIBS=-lBulletDynamics -lBulletCollision -lLinearMath
GL_LIBS=-lGL -lGLU -lXxf86vm -lGLEW
ASSIMP_LIBS=-lassimp
LIBS=$(BULLET_LIBS) $(SDL_LIBS) $(GL_LIBS) $(ASSIMP_LIBS)
INCLUDES=$(LOCAL_INCLUDES) $(BULLET_INCLUDES)

STANDARDS=-Wall
#BUILDTYPE=-O3
BUILDTYPE=-g
BUILDPARAM=$(STANDARDS) $(BUILDTYPE)

CFLAGS=$(BUILDPARAM) $(INCLUDES)
LDFLAGS=$(BUILDPARAM) $(LIBS)
# BFLAGS is used when doing compilation and linking in a single step
BFLAGS=$(BUILDPARAM) $(INCLUDES) $(LIBS)

all: $(OUTPUTS) main

bin:
	mkdir bin

%: $(BINDIR)/%
	@echo -e '\e[33mSYMLINK \e[96m$@\e[m \e[33mTO \e[94m$^\e[m'
	ln -s $^

$(BINDIR)/main: $(MAIN_OBJECTS) | bin
	@echo -e '\e[33mLINKING \e[96m$@\e[m \e[33mFROM \e[94m$^\e[m'
	$(CXX) $^ $(LDFLAGS) -o $@
	
$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo -e '\e[33mCOMPILING \e[96m$@\e[m \e[33mFROM \e[94m$^\e[m'
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo -e '\e[33mCLEANING...\e[m'
	rm -f -v *~ $(MAIN_OBJECTS)

clobber:
	rm -f -v *~ $(OUTPUTS) main

