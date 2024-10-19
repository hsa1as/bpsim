CC = g++
OPT = -O3
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)
OP_DIR = outputs
REF_DIR = val_outputs
# List all your .cc files here (source files, excluding header files)
SIM_SRC = main.cc bpu.cc 

# List corresponding compiled object files here (.o files)
SIM_OBJ = main.o bpu.o
 
#################################

# default rule

all: bpsim
	@echo "Done with make..."


# rule for making bpsim

bpsim: $(SIM_OBJ)
	$(CC) -o bpsim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH BPSIM-----------"


# generic rule for converting any .cc file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc


# type "make clean" to remove all .o files plus the bpsim binary

clean:
	rm -f *.o bpsim
	rm -rf ./$(OP_DIR)


# type "make clobber" to remove all .o files (leaves bpsim binary)

clobber:
	rm -f *.o


run: all
	-rm -rf ./$(OP_DIR)
	mkdir ./$(OP_DIR)
	./bpsim bimodal 6 gcc_trace.txt >| ./$(OP_DIR)/bimodal_gcc_val0.txt
	./bpsim bimodal 12 gcc_trace.txt >| ./$(OP_DIR)/bimodal_gcc_val1.txt
	./bpsim bimodal 4 jpeg_trace.txt >| ./$(OP_DIR)/bimodal_jpeg_val2.txt
	./bpsim gshare 9 3 gcc_trace.txt >| ./$(OP_DIR)/gshare_gcc_val0.txt
	./bpsim gshare 14 8 gcc_trace.txt >| ./$(OP_DIR)/gshare_gcc_val1.txt
	./bpsim gshare 11 5 jpeg_trace.txt >| ./$(OP_DIR)/gshare_jpeg_val2.txt

	
diff: run  
	-diff -iw ./$(OP_DIR)/bimodal_gcc_val0.txt ./$(REF_DIR)/bimodal_gcc_val0.txt || true;
	-diff -iw ./$(OP_DIR)/bimodal_gcc_val1.txt ./$(REF_DIR)/bimodal_gcc_val1.txt || true;
	-diff -iw ./$(OP_DIR)/bimodal_jpeg_val2.txt ./$(REF_DIR)/bimodal_jpeg_val2.txt || true;
	-diff -iw ./$(OP_DIR)/gshare_gcc_val0.txt ./$(REF_DIR)/gshare_gcc_val0.txt || true;
	-diff -iw ./$(OP_DIR)/gshare_gcc_val1.txt ./$(REF_DIR)/gshare_gcc_val1.txt || true;
	-diff -iw ./$(OP_DIR)/gshare_jpeg_val2.txt ./$(REF_DIR)/gshare_jpeg_val2.txt || true;






	
