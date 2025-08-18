CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

CUT_INC_DIR = $(INCLUDES_DIR)/palantir
CUT_SRC_DIR = $(SOURCES_DIR)/palantir

OBJS_DIR = objs

CONSOLE_DEST = $(CUT_SRC_DIR)/console_dest.o
CONSOLE_SRC = $(CUT_SRC_DIR)/console_source.o
FILE_DEST = $(CUT_SRC_DIR)/file_dest.o
FILE_SRC = $(CUT_SRC_DIR)/file_source.o
MESSENGER = $(CUT_SRC_DIR)/messenger.o
ROT13_ENCODER = $(CUT_SRC_DIR)/rot13_encoder.o
ATBASH_ENCODER = $(CUT_SRC_DIR)/atbash_encoder.o
UDP_DEST = $(CUT_SRC_DIR)/udp_dest.o
UDP_SRC = $(CUT_SRC_DIR)/udp_source.o
TCP_DEST = $(CUT_SRC_DIR)/tcp_dest.o
TCP_SRC = $(CUT_SRC_DIR)/tcp_source.o
VIGENERE_DEC = $(CUT_SRC_DIR)/vigenère_decryptor.o
VIGENERE_ENC = $(CUT_SRC_DIR)/vigenère_encryptor.o
SOCKET = $(CUT_SRC_DIR)/socket.o
ADDRESS = $(CUT_SRC_DIR)/address.o
COMP_ENCODER = $(CUT_SRC_DIR)/composite_encoder.o
CAESAR_ENCODER = $(CUT_SRC_DIR)/caesar_encoder.o



OBJS = $(CAESAR_DEC) $(CAESAR_ENC) $(COMP_ENCODER) \
       $(CONSOLE_DEST) $(CONSOLE_SRC) $(FILE_DEST) $(FILE_SRC) \
       $(MESSENGER) $(SOCKET) $(ADDRESS) $(CAESAR_ENCODER)\
       $(UDP_DEST) $(UDP_SRC) $(VIGENERE_DEC) $(VIGENERE_ENC) \
	   $(TCP_DEST) $(TCP_SRC) $(ROT13_ENCODER) $(ATBASH_ENCODER)


TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(ATBASH_DEC): $(CUT_SRC_DIR)/atbash_decryptor.cpp $(CUT_INC_DIR)/atbash_decryptor.hpp 
$(ATBASH_ENC): $(CUT_SRC_DIR)/atbash_encryptor.cpp $(CUT_INC_DIR)/atbash_encryptor.hpp
$(CAESAR_ENCODER): $(CUT_SRC_DIR)/caesar_encoder.cpp $(CUT_INC_DIR)/caesar_encoder.hpp $(CUT_INC_DIR)/encoder.hpp
$(CONSOLE_DEST): $(CUT_SRC_DIR)/console_dest.cpp $(CUT_INC_DIR)/console_dest.hpp $(CUT_INC_DIR)/destination.hpp
$(CONSOLE_SRC): $(CUT_SRC_DIR)/console_source.cpp $(CUT_INC_DIR)/console_source.hpp $(CUT_INC_DIR)/source.hpp
$(FILE_DEST): $(CUT_SRC_DIR)/file_dest.cpp $(CUT_INC_DIR)/file_dest.hpp $(CUT_INC_DIR)/destination.hpp
$(FILE_SRC): $(CUT_SRC_DIR)/file_source.cpp $(CUT_INC_DIR)/file_source.hpp $(CUT_INC_DIR)/source.hpp
$(MESSENGER): $(CUT_SRC_DIR)/messenger.cpp $(CUT_INC_DIR)/messenger.hpp $(CUT_INC_DIR)/source.hpp $(CUT_INC_DIR)/destination.hpp 
$(UDP_DEST): $(CUT_SRC_DIR)/udp_dest.cpp $(CUT_INC_DIR)/udp_dest.hpp $(CUT_INC_DIR)/destination.hpp
$(UDP_SRC): $(CUT_SRC_DIR)/udp_source.cpp $(CUT_INC_DIR)/udp_source.hpp $(CUT_INC_DIR)/source.hpp
$(TCP_DEST): $(CUT_SRC_DIR)/tcp_dest.cpp $(CUT_INC_DIR)/tcp_dest.hpp $(CUT_INC_DIR)/destination.hpp $(CUT_INC_DIR)/socket.hpp
$(TCP_SRC): $(CUT_SRC_DIR)/tcp_source.cpp $(CUT_INC_DIR)/tcp_source.hpp $(CUT_INC_DIR)/source.hpp $(CUT_INC_DIR)/socket.hpp
$(VIGENERE_DEC): $(CUT_SRC_DIR)/vigenère_decryptor.cpp $(CUT_INC_DIR)/vigenère_decryptor.hpp 
$(VIGENERE_ENC): $(CUT_SRC_DIR)/vigenère_encryptor.cpp $(CUT_INC_DIR)/vigenère_encryptor.hpp
$(SOCKET): $(CUT_INC_DIR)/socket.hpp
$(ADDRESS): $(CUT_INC_DIR)/address.hpp
$(ROT13_ENCODER): $(CUT_INC_DIR)/encoder.hpp $(CUT_INC_DIR)/rot13_encoder.hpp $(CUT_SRC_DIR)/rot13_encoder.cpp $(CUT_INC_DIR)/caesar_encoder.hpp
$(ATBASH_ENCODER): $(CUT_INC_DIR)/encoder.hpp $(CUT_INC_DIR)/atbash_encoder.hpp $(CUT_SRC_DIR)/atbash_encoder.cpp
$(COMP_ENCODER): $(CUT_INC_DIR)/encoder.hpp $(CUT_INC_DIR)/composite_encoder.hpp $(CUT_SRC_DIR)/composite_encoder.cpp

utest.o: utest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET) utest.o

