# SCL Validator Makefile
# Copyright 2025
#
# Cross-compile for ARM Linux or native x86 build

# Compiler settings
CC ?= gcc
AR ?= ar
CFLAGS = -Wall -Wextra -O2 -I./include -I./mxml
LDFLAGS = -lmxml -lpthread

# Cross-compilation settings for ARM
ifeq ($(TARGET),arm)
    CC = arm-linux-gnueabihf-gcc
    AR = arm-linux-gnueabihf-ar
    CFLAGS += -march=armv7-a -mfpu=neon -mfloat-abi=hard
endif

# Source files
SRCS = src/scl_validator.c \
       src/scl_parser.c \
       src/scl_checker.c \
       src/scl_validate.c

OBJS = $(SRCS:.c=.o)

# Output files
LIBRARY = lib/scl_validator.a
EXECUTABLE = bin/scl_validate

# Mini-XML library (download if not present)
MXML_DIR = mxml
MXML_LIB = $(MXML_DIR)/libmxml.a

# Default target
all: dirs mxml $(LIBRARY) $(EXECUTABLE)

# Create directories
dirs:
	@mkdir -p lib bin

# Build Mini-XML library
mxml:
	@if [ ! -d $(MXML_DIR) ]; then \
		echo "Downloading Mini-XML..."; \
		git clone https://github.com/michaelrsweet/mxml.git $(MXML_DIR) 2>/dev/null || \
		wget -q https://github.com/michaelrsweet/mxml/releases/download/v4.0.3/mxml-4.0.3.tar.gz && \
		tar xzf mxml-4.0.3.tar.gz && mv mxml-4.0.3 $(MXML_DIR) && rm mxml-4.0.3.tar.gz; \
	fi
	@if [ ! -f $(MXML_LIB) ]; then \
		echo "Building Mini-XML..."; \
		cd $(MXML_DIR) && ./configure --disable-shared --enable-static && $(MAKE); \
	fi

# Build objects
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build static library
$(LIBRARY): src/scl_validator.o src/scl_parser.o src/scl_checker.o
	$(AR) rcs $@ $^

# Build executable
$(EXECUTABLE): $(OBJS) $(MXML_LIB)
	$(CC) $(CFLAGS) -o $@ src/scl_validate.o $(LIBRARY) $(MXML_LIB) $(LDFLAGS)

# Test target
test: $(EXECUTABLE)
	@echo "Running validation tests..."
	./$(EXECUTABLE) test/sample.icd -v
	./$(EXECUTABLE) test/sample_errors.icd -v

# Install target
install: all
	@mkdir -p $(DESTDIR)/usr/local/bin
	@mkdir -p $(DESTDIR)/usr/local/lib
	@mkdir -p $(DESTDIR)/usr/local/include/scl_validator
	cp $(EXECUTABLE) $(DESTDIR)/usr/local/bin/
	cp $(LIBRARY) $(DESTDIR)/usr/local/lib/
	cp include/*.h $(DESTDIR)/usr/local/include/scl_validator/

# Clean target
clean:
	rm -f $(OBJS) $(LIBRARY) $(EXECUTABLE)
	rm -rf lib bin
	@if [ -d $(MXML_DIR) ]; then cd $(MXML_DIR) && $(MAKE) clean; fi

# Deep clean (including mxml)
distclean: clean
	rm -rf $(MXML_DIR)

# Cross-compile for ARM
arm:
	$(MAKE) TARGET=arm all

# Help
help:
	@echo "SCL Validator Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build library and executable"
	@echo "  test       - Run validation tests"
	@echo "  install    - Install to /usr/local"
	@echo "  clean      - Remove build artifacts"
	@echo "  distclean  - Remove all including dependencies"
	@echo "  arm        - Cross-compile for ARM Linux"
	@echo ""
	@echo "Variables:"
	@echo "  TARGET=arm - Cross-compile for ARM"
	@echo "  DESTDIR    - Installation prefix"

.PHONY: all dirs mxml test install clean distclean arm help