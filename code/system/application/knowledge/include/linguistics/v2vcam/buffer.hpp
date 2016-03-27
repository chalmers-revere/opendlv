#ifndef LINGUISTICS_V2VCAM_BUFFER_HPP
#define LINGUISTICS_V2VCAM_BUFFER_HPP

#include <memory>
#include <string>
#include <vector>

namespace opendlv {


/**
 * @brief For storing bytes in a buffer.
 */
class Buffer {
  public:
    /**
     * @brief For reading a buffer.
     */
    class Iterator {
      public:
        explicit Iterator(Buffer const *);
        Iterator(Iterator const &) = delete;
        Iterator &operator=(Iterator const &) = delete;
        virtual ~Iterator();
        bool ReadBoolean();
        unsigned char ReadByte();
        std::shared_ptr<std::vector<unsigned char>> ReadBytes();
        std::vector<unsigned char> ReadBytesReversed(int );
        double ReadDouble();
        float ReadFloat();
        int ReadInteger();
        int ReadInteger24();
        long ReadLong();
        short ReadShort();
        std::string ReadString();
        void Reset();
        void ItReversed();


      private:
        void CheckOverflow(int);
        bool HasBytesLeft(int);
   
        Buffer const *m_outer_buffer;
        int m_read_pos;


        bool m_itreversed = false;
    };



    Buffer();
    explicit Buffer(std::vector<unsigned char> const &);
    Buffer(Buffer const &) = delete;
    Buffer &operator=(Buffer const &) = delete;
    virtual ~Buffer();
    void AppendBoolean(bool);
    void AppendByte(unsigned char);
    void AppendBytes(std::vector<unsigned char> const &);
    void AppendBytesRaw(std::vector<unsigned char> const &);
    void AppendDouble(double);
    void AppendFloat(float);
    void AppendInteger(int);
    void AppendLong(long);
    void AppendShort(short);
    void AppendString(std::string const &);
    void AppendStringRaw(std::string const &);
    std::vector<unsigned char> const &GetBytes() const;
    std::shared_ptr<Buffer::Iterator> GetIterator() const;
    int GetSize() const;
    unsigned char ReadByte(int) const;
    void Reversed();


  private:
    void Append(std::vector<unsigned char> const &);

    std::vector<unsigned char> m_bytes;


    bool m_reversed = false;

};

}

#endif
