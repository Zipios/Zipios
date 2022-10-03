

#include    <fstream>
#include    <iomanip>
#include    <iostream>

#include    <zlib.h>


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if(!in)
    {
        std::cerr << "error: could not access file \"" << argv[1] << "\".\n";
        return 1;
    }

    int result(crc32(0, nullptr, 0));
    for(;;)
    {
        char buf[64 * 1024];
        in.read(buf, sizeof(buf));
        if(in.gcount() == 0)
        {
            break;
        }
        result = crc32(result, reinterpret_cast<Bytef const *>(buf), in.gcount());
    }

    std::cout << std::hex << std::setw(8) << std::setfill('0') << result << "\n";

    return 0;
}

// vim: ts=4 sw=4 et
