base.dir=./
compiler=gcc
cflags=-Wall
src=$(wildcard *.c)
obj.dir=obj
tree=$(addprefix $(obj.dir)/,cawax_analyst.o cawax_datareader.o cawax_datastructs.o cawax_maths.o cawax_time.o cawax_test.o cawax_debug.o)
headers=cawax.h
app=cawax
makelog=makelog

all : 
	make clean
	mkdir -p  $(obj.dir)
	make cawax 

clean : 
	@echo "Cleaning..."
	-rm -f $(tree) cawax
	-rmdir -p $(obj.dir)

cawax : $(tree)
	$(compiler) -o cawax $(tree) /usr/local/lib/liblog4c.a -lm 

# header dependency
$(tree) : $(headers)

# log4c gets installed to /usr/local by default which on some machines is not 
# on the search path of gcc, include it here.
$(obj.dir)/%.o : %.c 
	$(compiler) -c $(cflags) $< -o $@ 
