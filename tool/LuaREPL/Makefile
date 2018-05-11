TARGET := lua

CCFLAG := -I./ -I../../../3rd/lua-5.3.3/src/
LDFLAG := -L../../../3rd/lua-5.3.3/lib_linux/
LDLIBS := -llua



SRC1 :=	 $(wildcard ./*.cpp) 
SRC2 :=  $(wildcard ./*.c) 


OBJS := $(SRC2:./%.c=obj/%.o)
OBJS += $(SRC1:./%.cpp=obj/%.o)

all:$(OBJS)
	@echo [M]Link...
	g++ $(OBJS) -o ../$(TARGET) $(LDFLAG) $(LDLIBS)


obj/%.o:%.cpp
	@echo [M]Compile CXX...
	@mkdir -p $(@D)
	g++ -c $< -o $@ $(CCFLAG)

obj/%.o:%.c
	@echo [M]Compile C...
	@mkdir -p $(@D)
	gcc -c $< -o $@ $(CCFLAG)

run:
	../$(TARGET)

clean:
	rm -rf bin
	rm -rf obj
	rm -rf out















