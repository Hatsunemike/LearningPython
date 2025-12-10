#include "ElGamel.h"
#include <string>
#include <argparse/argparse.hpp>
#include <boost/filesystem.hpp>

namespace ag = argparse;
namespace fs = boost::filesystem;

using std::string;

void ensureFolderExists(const fs::path& p) {
    if(fs::exists(p)) return;
    fs::create_directory(p);
    return;
}

int main(int argc, char* argv[])
{
    ag::ArgumentParser program("elgamel");
    // InitParser(program);
    program.set_usage_max_line_width(80);
    program.set_usage_break_on_mutex();
    program.add_description("ElGamel-secp256k1 Ecryption and Decryption tool.");
    program.add_argument("-h", "--help")
        .help("Show this information.")
        .flag();

    // gen-key command
    ag::ArgumentParser genKeyParser("genkey");
    genKeyParser.add_description("Generate ElGamel-secp256k1 private and public key.");
    genKeyParser.add_argument("-o")
        .help("designate the key storage folder path.")
        .default_value("~/.elgamel");
    program.add_subparser(genKeyParser);
    
    // file-encrypt command
    ag::ArgumentParser fileEncParser("enc");
    fileEncParser.add_description("Encrypt a file");
    fileEncParser.add_argument("-p", "--plainfile")
        .help("path of plainfile to be encrypted.")
        .required();  
    fileEncParser.add_argument("-k", "--key")
        .help("path of pub-key")
        .default_value("~/.elgamel/elgamel_pub.key");
    fileEncParser.add_argument("-o")
        .help("path of encrypt result file")
        .default_value("a.enc");
    program.add_subparser(fileEncParser);
    
    // file-decrypt command
    ag::ArgumentParser fileDecParser("dec");
    fileDecParser.add_description("Decrypt a file");
    fileDecParser.add_argument("-c", "--cipherfile")
        .help("path of cipherfile to be decrypt")
        .required();
    fileDecParser.add_argument("-k", "--key")
        .help("path of pri-key")
        .default_value("~/.elgamel/elgamel_pri.key");
    fileDecParser.add_argument("-o")
        .help("path of decrypt result file")
        .default_value("a.dec");
    program.add_subparser(fileDecParser);

    // parse process
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "program error: " << e.what() << std::endl;
        return 1;
    }

    // command execution
    try {
        if (program.is_subcommand_used("genkey")) {
            auto& subargs = program.at<ag::ArgumentParser>("genkey");
            fs::path outputPath = subargs.get<string>("-o");
            ensureFolderExists(outputPath);

            fs::path skOutput = outputPath / "elgamel_pri.key";
            fs::path pkOutput = outputPath / "elgamel_pub.key";

            ElGamelGenerator generator;
            ElGamelSK sk;
            ElGamelPK pk;
            if (!generator.genKey(sk, pk)) {
                throw std::runtime_error("Failed to generate key pair");
            }

            // Save private key
            FILE* fsk = fopen(skOutput.c_str(), "wb");
            if (!fsk) {
                throw std::runtime_error("Cannot open private key file for writing: " + skOutput.string());
            }
            sk.writeTo(fsk);
            fclose(fsk);

            // Save public key
            FILE* fpk = fopen(pkOutput.c_str(), "wb");
            if (!fpk) {
                throw std::runtime_error("Cannot open public key file for writing: " + pkOutput.string());
            }
            pk.writeTo(fpk);
            fclose(fpk);
        }
        else if (program.is_subcommand_used("enc")) {
            auto& subargs = program.at<ag::ArgumentParser>("enc");
            fs::path plainFile = subargs.get<string>("--plainfile");
            fs::path pubKeyFile = subargs.get<string>("--key");
            fs::path outputFile = subargs.get<string>("-o");

            // Load public key
            FILE* fpk = fopen(pubKeyFile.c_str(), "rb");
            if (!fpk) {
                throw std::runtime_error("Cannot open public key file: " + pubKeyFile.string());
            }
            ElGamelPK pk;
            if (!ElGamel_File2PK(fpk, pk)) {
                fclose(fpk);
                throw std::runtime_error("Failed to load public key from file: " + pubKeyFile.string());
            }
            fclose(fpk);

            // Open input and output files
            FILE* fin = fopen(plainFile.c_str(), "rb");
            if (!fin) {
                throw std::runtime_error("Cannot open plain file: " + plainFile.string());
            }

            FILE* fout = fopen(outputFile.c_str(), "wb");
            if (!fout) {
                fclose(fin);
                throw std::runtime_error("Cannot create output file: " + outputFile.string());
            }

            // Encrypt
            ElGamelEncryptor encryptor(pk);
            if (!encryptor.EncryptFile(fin, fout)) {
                fclose(fin);
                fclose(fout);
                throw std::runtime_error("Encryption failed");
            }

            fclose(fin);
            fclose(fout);
        }
        else if (program.is_subcommand_used("dec")) {
            auto& subargs = program.at<ag::ArgumentParser>("dec");
            fs::path cipherFile = subargs.get<string>("--cipherfile");
            fs::path privKeyFile = subargs.get<string>("--key");
            fs::path outputFile = subargs.get<string>("-o");

            // Load private key
            FILE* fsk = fopen(privKeyFile.c_str(), "rb");
            if (!fsk) {
                throw std::runtime_error("Cannot open private key file: " + privKeyFile.string());
            }
            ElGamelSK sk;
            if (!ElGamel_File2SK(fsk, sk)) {
                fclose(fsk);
                throw std::runtime_error("Failed to load private key from file: " + privKeyFile.string());
            }
            fclose(fsk);

            // Open input and output files
            FILE* fin = fopen(cipherFile.c_str(), "rb");
            if (!fin) {
                throw std::runtime_error("Cannot open cipher file: " + cipherFile.string());
            }

            FILE* fout = fopen(outputFile.c_str(), "wb");
            if (!fout) {
                fclose(fin);
                throw std::runtime_error("Cannot create output file: " + outputFile.string());
            }

            // Decrypt
            ElGamelDecryptor decryptor(sk);
            if (!decryptor.DecryptFile(fin, fout)) {
                fclose(fin);
                fclose(fout);
                throw std::runtime_error("Decryption failed");
            }

            fclose(fin);
            fclose(fout);
        }
        else {
            std::cout << program;
        }
    } catch (std:: exception& e) {
        std::cerr << "Command Execution Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}