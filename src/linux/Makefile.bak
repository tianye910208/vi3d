TARGET := test

CCFLAG := -I../ -I../../lib/lua-5.3.3/src/
LDFLAG := -L../../lib/lua-5.3.3/lib_linux/
LDLIBS := -lEGL -lGLESv2 -lX11 -llua



SRC1 :=	 $(wildcard ../linux/*.cpp) \
	 $(wildcard ../*.cpp) \

SRC2 :=  $(wildcard ../linux/*.c) \
	 $(wildcard ../*.c) \


OBJS := $(SRC2:../%.c=obj/%.o)
OBJS += $(SRC1:../%.cpp=obj/%.o)

all:$(OBJS)
	@echo [M]Link...
	@mkdir -p bin
	g++ $(OBJS) -o bin/$(TARGET) $(LDFLAG) $(LDLIBS)


obj/%.o:../%.cpp
	@echo [M]Compile CXX...
	@mkdir -p $(@D)
	g++ -c $< -o $@ $(CCFLAG)

obj/%.o:../%.c
	@echo [M]Compile C...
	@mkdir -p $(@D)
	gcc -c $< -o $@ $(CCFLAG)

run:
	./bin/$(TARGET)

clean:
	rm -rf bin
	rm -rf obj
	rm -rf out















