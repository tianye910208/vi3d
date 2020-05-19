TARGET := vi3d

CC     := gcc
CCFLAG := -I../ -I../../3rd/lua-5.3.3/src/ -I../../3rd/stb/
LDFLAG := -L../../3rd/lua-5.3.3/lib_mingw/
LDLIBS := -lgdi32 -llua



SRCS := $(wildcard ../*.c) \
        $(wildcard ../mingw/*.c)

OBJS := $(SRCS:../%.c=obj/%.o)
DEPS := $(OBJS:%.o=%.d)



ifeq ($(EGL),1)
    CCFLAG += -DVI3D_GLES_EGL -I../../3rd/angle/include
    LDFLAG += -L../../3rd/angle/lib/x64/
    LDLIBS += -lEGL -lGLESv2 
else
	CCFLAG += -I../../3rd/glew-2.1.0/include
    LDFLAG += -L../../3rd/glew-2.1.0/lib/mingw/
    LDLIBS += -lopengl32 -lglew32.dll
endif


all: $(DEPS) $(OBJS)
	@echo [M]Link $(TARGET)
	@mkdir -p bin
	@$(CC) $(OBJS) -o bin/$(TARGET) $(LDFLAG) $(LDLIBS)

obj/%.o:../%.c
	@echo [M]Compile $@
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(CCFLAG) 

obj/%.d:../%.c
	@echo [M]Genrule $@
	@mkdir -p $(@D)
	@$(CC) -MM -MD $< -o $@ $(CCFLAG)

run:
	./bin/$(TARGET)

clean:
	rm -rf bin
	rm -rf obj




-include $(DEPS)









