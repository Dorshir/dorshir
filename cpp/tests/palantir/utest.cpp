#include "mu_test.h"
#include "caesar_encoder.hpp"
#include "vigenère_encryptor.hpp"
#include "vigenère_decryptor.hpp"
#include "console_source.hpp"
#include "console_dest.hpp"
#include "messenger.hpp"
#include "udp_dest.hpp"
#include "udp_source.hpp"
#include "tcp_dest.hpp"
#include "tcp_source.hpp"
#include "rot13_encoder.hpp"
#include "atbash_encoder.hpp"
#include "composite_encoder.hpp"
#include "file_source.hpp"
#include "file_dest.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

BEGIN_TEST(rot13_encrypt_decrypt_test)
    palantir::Rot13Encoder encoder;

    std::string plain = "HELLO";
    std::string encrypted = encoder.encode(plain);
    ASSERT_EQUAL(encrypted, "URYYB");


    std::string decrypted = encoder.encode(encrypted);
    ASSERT_EQUAL(decrypted, "HELLO");
END_TEST

BEGIN_TEST(caesar_encrypt_decrypt_test)
    palantir::CaesarEncoder encryptor(3); // Shift by 3
    palantir::CaesarEncoder decryptor(-3); // Reverse shift by 3

    std::string plain = "HELLO";
    std::string encrypted = encryptor.encode(plain);
    std::string decrypted = decryptor.encode(encrypted);

    ASSERT_EQUAL(encrypted, "KHOOR");
    ASSERT_EQUAL(decrypted, "HELLO");
END_TEST

BEGIN_TEST(atbash_encrypt_decrypt_test)
    palantir::AtbashEncoder encoder;

    std::string plain = "HELLO";
    std::string encrypted = encoder.encode(plain);
    std::string decrypted = encoder.encode(encrypted);

    ASSERT_EQUAL(encrypted, "SVOOL");
    ASSERT_EQUAL(decrypted, "HELLO");
END_TEST

BEGIN_TEST(vigenere_encrypt_decrypt_test)
    palantir::VigenereEncryptor encryptor("KEY");
    palantir::VigenereDecryptor decryptor("KEY");

    std::string plain = "HELLO";
    std::string encrypted = encryptor.encode(plain);
    std::string decrypted = decryptor.encode(encrypted);

    ASSERT_EQUAL(encrypted, "RIJVS");
    ASSERT_EQUAL(decrypted, "HELLO");
END_TEST

BEGIN_TEST(messenger_test_caesar)
    std::istringstream input("Encrypt this message\nAnother message");
    std::ostringstream output;

    palantir::ConsoleSource source(input);
    palantir::ConsoleDest destination(output);
    palantir::CaesarEncoder encryptor(3); // Shift by 3

    palantir::Messenger messenger(source, destination, encryptor);
    messenger.execute();

    ASSERT_EQUAL(output.str(), "Hqfubsw wklv phvvdjh\nDqrwkhu phvvdjh\n");
END_TEST

BEGIN_TEST(messenger_test_atbash)
    std::istringstream input("Encrypt this message\nAnother message");
    std::ostringstream output;

    palantir::ConsoleSource source(input);
    palantir::ConsoleDest destination(output);
    palantir::AtbashEncoder encoder;

    palantir::Messenger messenger(source, destination, encoder);
    messenger.execute();

    ASSERT_EQUAL(output.str(), "Vmxibkg gsrh nvhhztv\nZmlgsvi nvhhztv\n");
END_TEST


BEGIN_TEST(messenger_test_vigenere)
{
    std::istringstream input("GEEKSFORGEEKS");
    std::ostringstream output;

    palantir::ConsoleSource source(input);
    palantir::ConsoleDest destination(output);

    palantir::VigenereEncryptor encryptor("AYUSH");

    palantir::Messenger messenger(source, destination, encryptor);

    messenger.execute();

    ASSERT_EQUAL(output.str(), "GCYCZFMLYLEIM\n");
}
END_TEST


BEGIN_TEST(udp_dest_source_test)
{
    int TEST_PORT = 5555;

    palantir::Address local_address("127.0.0.1", TEST_PORT);
    palantir::UdpSource source(local_address);
    
    palantir::UdpDest destination(local_address);

    std::string sent_message = "Hello via UDP!";
    destination.send_message(sent_message);

    std::string received_message = source.read_message();

    ASSERT_EQUAL(received_message, sent_message);
}
END_TEST


BEGIN_TEST(tcp_source_netcat_test)
{
    int TEST_PORT = 6666;

    palantir::Address local_addr("0.0.0.0", TEST_PORT);
    palantir::TcpSource server(local_addr);

    std::cout << "Server is ready on port " << TEST_PORT << std::endl;
    std::string received_message = server.read_message();
    std::cout << "Message received: " << received_message << std::endl;

    ASSERT_EQUAL(received_message, "Hello from netcat!");
}
END_TEST


BEGIN_TEST(tcp_dest_netcat_test)
{
    int TEST_PORT = 7777;

    palantir::Address server_addr("127.0.0.1", TEST_PORT);
    palantir::TcpDest client(server_addr);

    std::string sent_message = "Hello from TcpDest!";
    client.send_message(sent_message);

    std::cout << "Message sent to netcat server: " << sent_message << std::endl;
    ASSERT_PASS();
}
END_TEST


BEGIN_TEST(composite_encoder_test)
{
    palantir::Rot13Encoder rot13;
    palantir::AtbashEncoder atbash;

    palantir::CompositeEncoder composite;
    composite.add(rot13);
    composite.add(atbash);

    std::string plain = "HELLO";

    std::string encoded = composite.encode(plain);

    ASSERT_EQUAL(encoded, "FIBBY");

    std::string decoded = composite.encode(encoded);

    ASSERT_EQUAL(decoded, plain);
}
END_TEST


TEST_SUITE(Palantir Unit Tests)
    TEST(rot13_encrypt_decrypt_test)
    TEST(caesar_encrypt_decrypt_test)
    TEST(atbash_encrypt_decrypt_test)
    TEST(vigenere_encrypt_decrypt_test)
    TEST(messenger_test_caesar)
    TEST(messenger_test_atbash)
    TEST(messenger_test_vigenere)
    TEST(udp_dest_source_test) 
    IGNORE_TEST(tcp_source_netcat_test)
    IGNORE_TEST(tcp_dest_netcat_test)
    TEST(composite_encoder_test)
END_SUITE
