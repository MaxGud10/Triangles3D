#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "triag_core.hpp"

#ifdef TRIAG_HAS_GRAPHICS
#include "viewer.hpp"
#endif

int main(int argc, char** argv)
{
    try
    {
        bool use_opengl = false;
        const char* input_file = nullptr;

        for (int i = 1; i < argc; ++i)
        {
            std::string a = argv[i];
            if (a == "--opengl")
            {
                use_opengl = true;
            }
            else if (a == "--help" || a == "-h")
            {
                std::cout
                    << "Usage:\n"
                    << "  triag < input\n"
                    << "  triag [file]\n"
                    << "  triag --opengl < input\n"
                    << "  triag --opengl [file]\n";
                return 0;
            }
            else
            {
                input_file = argv[i];
            }
        }

        std::unique_ptr<std::ifstream> fin;
        std::istream* in = &std::cin;

        if (input_file)
        {
            fin = std::make_unique<std::ifstream>(input_file);
            if (!fin->is_open())
            {
                std::cerr << "Failed to open file: " << input_file << "\n";
                return 1;
            }
            in = fin.get();
        }

        auto shapes = triag_core::read_shapes(*in);
        auto ids_sorted = triag_core::compute_intersected_ids_sorted(shapes);

        if (!use_opengl)
        {
            for (auto id : ids_sorted)
                std::cout << id << "\n";
            return 0;
        }

#ifndef TRIAG_HAS_GRAPHICS
        std::cerr << "This build was compiled without GRAPHICS. Rebuild with -DGRAPHICS=ON.\n";
        return 1;
#else
        auto ids_set = triag_core::to_set(ids_sorted);
        return triag_viewer::run_opengl_viewer(shapes, ids_set);
#endif
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}