ifdef DEBUG
	FLAGS += -g
endif

.PHONY = mex clean

all: mex

mex:
	mex $(FLAGS) rips.cpp ../src/ripser/ripser.cpp

clean:
	rm -f rips.mexa64
