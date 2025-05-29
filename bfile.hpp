#pragma once
#include<filesystem>
#include<stdexcept>
#include<fstream>
#include<cstdint>
#include<cstring>
#include<string>
#include<memory>
#include<limits>
#include<bit>
#include"binary.hpp"
namespace cppp{
    struct file_error : std::exception{};
    struct bad_fstream : file_error{};
    struct operation_failed : file_error{};
    class BinaryFile{
        using traits = std::char_traits<char>;
        mutable std::fstream fs;
        public:
            //Make tellg() return value opaque, as it should be.
            class pos_buf{
                friend class BinaryFile;
                std::fstream::pos_type v;
                pos_buf(std::fstream::pos_type v) : v(v){}
            };
            BinaryFile(std::filesystem::path path,std::ios::openmode mode) : fs(path,mode){}
            pos_buf tell() const{
                return fs.tellg();
            }
            void seek(pos_buf pos){
                fs.seekg(pos.v);
            }
            void seek(std::int64_t pos,std::ios::seekdir where=std::ios::beg){
                fs.seekg(pos,where);
            }
            std::size_t read(buffer buf){
                fs.read(reinterpret_cast<char*>(buf.data()),buf.size());
                if(fs.eof()){
                    fs.clear(fs.rdstate()&~fs.failbit);
                }
                errchk();
                return fs.gcount();
            }
            void errchk() const{
                if(fs.bad()){
                    throw bad_fstream();
                }else if(fs.fail()){
                    throw operation_failed();
                }
            }
            bool fread(buffer buf){
                return read(buf)==buf.size();
            }
            bool mread(buffer buf){
                std::unique_ptr<std::byte[]> dup{new std::byte[buf.size()]};
                bool state;
                if((state=fread({dup.get(),buf.size()}))){
                    std::memcpy(buf.data(),dup.get(),buf.size());
                }
                return state;
            }
            template<std::size_t l>
            bool mread(staticbuffer<l> buf){
                std::array<std::byte,l> dup;
                bool state;
                if((state=fread(dup))){
                    std::memcpy(buf.data(),dup.data(),l);
                }
                return state;
            }
            bool eof() const{
                return fs.eof();
            }
            std::byte readbr(){
                return std::bit_cast<std::byte>(traits::to_char_type(fs.get()));
            }
            std::uint8_t readb(){
                return std::bit_cast<std::uint8_t>(traits::to_char_type(fs.get()));
            }
            std::uint16_t readwle(){
                return static_cast<std::uint16_t>(readb())|(static_cast<std::uint16_t>(readb())<<8);
            }
            std::uint32_t readdle(){
                return static_cast<std::uint32_t>(readb())
                     |(static_cast<std::uint32_t>(readb())<<8)
                     |(static_cast<std::uint32_t>(readb())<<16)
                     |(static_cast<std::uint32_t>(readb())<<24);
            }
            std::uint64_t readqle(){
                return static_cast<std::uint64_t>(readb())
                     |(static_cast<std::uint64_t>(readb())<<8)
                     |(static_cast<std::uint64_t>(readb())<<16)
                     |(static_cast<std::uint64_t>(readb())<<24)
                     |(static_cast<std::uint64_t>(readb())<<32)
                     |(static_cast<std::uint64_t>(readb())<<40)
                     |(static_cast<std::uint64_t>(readb())<<48)
                     |(static_cast<std::uint64_t>(readb())<<56);
            }
            void write(frozenbuffer buf){
                fs.write(reinterpret_cast<const char*>(buf.data()),buf.size());
            }
            void writeb(std::byte b){
                fs.put(std::bit_cast<char>(b));
            }
            void writeb(std::uint8_t v){
                fs.put(std::bit_cast<char>(v));
            }
            void writewle(std::uint16_t v){
                writeb(static_cast<std::uint8_t>(v));
                writeb(static_cast<std::uint8_t>(v>>8));
            }
            void writedle(std::uint32_t v){
                writeb(static_cast<std::uint8_t>(v));
                writeb(static_cast<std::uint8_t>(v>>8));
                writeb(static_cast<std::uint8_t>(v>>16));
                writeb(static_cast<std::uint8_t>(v>>24));
            }
            void writeqle(std::uint64_t v){
                writeb(static_cast<std::uint8_t>(v));
                writeb(static_cast<std::uint8_t>(v>>8));
                writeb(static_cast<std::uint8_t>(v>>16));
                writeb(static_cast<std::uint8_t>(v>>24));
                writeb(static_cast<std::uint8_t>(v>>32));
                writeb(static_cast<std::uint8_t>(v>>40));
                writeb(static_cast<std::uint8_t>(v>>48));
                writeb(static_cast<std::uint8_t>(v>>56));
            }
    };
}
