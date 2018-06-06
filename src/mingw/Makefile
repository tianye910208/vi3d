TARGET := test

CC     := gcc
CCFLAG := -I../ -I../../3rd/GLES-Mali-v3.0.1-x64/include -I../../3rd/lua-5.3.3/src/ -I../../3rd/stb/
LDFLAG := -L../../3rd/GLES-Mali-v3.0.1-x64/ -L../../3rd/lua-5.3.3/lib_mingw/
LDLIBS := -lgdi32 -lEGL -lGLESv2 -llua



SRCS := $(wildcard ../*.c) \
        $(wildcard ../mingw/*.c)

OBJS := $(SRCS:../%.c=obj/%.o)
DEPS := $(OBJS:%.o=%.d)

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













