base.dir=./
compiler=gcc
cflags=-Wall
src=$(wildcard *.c)
obj.dir=obj
tree=$(addprefix $(obj.dir)/,cawax_analyst.o cawax_datareader.o cawax_datastructs.o cawax_maths.o cawax_time.o cawax_test.o)
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
	$(compiler) -o cawax $(tree) -lm 

# header dependency
$(tree) : $(headers)

$(obj.dir)/%.o : %.c 
	$(compiler) -c $(cflags) $< -o $@



 

