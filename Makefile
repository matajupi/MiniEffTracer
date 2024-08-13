CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3 -Iinc -Ibin
FLEX = flex
BISON = bison
LINT = cpplint

B = bin
S = src

SRCS = 	$(S)/main.cc $(S)/driver.cc $(S)/nodes.cc \
		# $(S)/tracer.cc $(S)/prims.cc $(S)/env.cc
SCAN 	= $(S)/scanner.ll
PARSE 	= $(S)/parser.yy
SCANCC 	= $(SCAN:$(S)/%.ll=$(B)/%.cc)
PARSECC = $(PARSE:$(S)/%.yy=$(B)/%.cc)
TARGET 	= $(B)/tracer

.PHONY: clean prebuild all
all: clean prebuild $(TARGET)

clean:
	rm -rf $(B)

prebuild:
	mkdir -p $(B)

$(TARGET): $(SRCS) $(PARSECC) $(SCANCC)
	# -@$(LINT) $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARSECC): $(PARSE)
	$(BISON) -o $@ $^ -v

$(SCANCC): $(SCAN)
	$(FLEX) -o $@ $^

