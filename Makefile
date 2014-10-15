SRCDIR=src
BINDIR=bin
OBJDIR=$(SRCDIR)

BINS=main
OMAIN=main.o inputbuffer.o
OUTPUTS=$(BINS:%=$(BINDIR)/%)
OBJECTS=$(OMAIN:%=$(OBJDIR)/%)

LOCAL_INCLUDES=-I$(SRCDIR)
SDL_LIBS=-lSDL2
BULLET_INCLUDES=-I/usr/include/bullet
BULLET_LIBS=-lBulletDynamics -lBulletCollision -lLinearMath
GL_LIBS=-lGL -lGLU -lXxf86vm -lGLEW
ASSIMP_LIBS=-lassimp
LIBS=$(BULLET_LIBS) $(SDL_LIBS) $(GL_LIBS) $(ASSIMP_LIBS)
INCLUDES=$(LOCAL_INCUDES) $(BULLET_INCLUDES)

STANDARDS=-Wall -std=c++11
#BUILDTYPE=-O3
BUILDTYPE=-g
BUILDPARAM=$(STANDARDS) $(BUILDTYPE)

CFLAGS=$(BUILDPARAM) $(INCLUDES)
LDFLAGS=$(BUILDPARAM) $(LIBS)

all: $(OUTPUTS) | main bin

bin:
	mkdir bin

%: $(BINDIR)/%
	@echo -e '\e[33mSYMLINK \e[96m$@\e[m \e[33mTO \e[94m$^\e[m'
	ln -s $^

$(BINDIR)/main: $(OBJECTS)
	@echo -e '\e[33mLINKING \e[96m$@\e[m \e[33mFROM \e[94m$^\e[m'
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo -e '\e[33mCOMPILING \e[96m$@\e[m \e[33mFROM \e[94m$^\e[m'
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo -e '\e[33mCLEANING...\e[m'
	rm -f -v *.o *~ $(OUTPUTS) $(OBJECTS) main

