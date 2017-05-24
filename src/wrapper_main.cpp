/**
 * @file   wrapper_main.cpp
 * @date   20.03.2017
 * @author Klemens D. Morgenstern
 * @brief Main function of the wrapper tool
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 <pre>
    /  /|  (  )   |  |  /
   /| / |   \/    | /| /
  / |/  |   /\    |/ |/
 /  /   |  (  \   /  |
            )
 </pre>
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/process/pipe.hpp>
#include <boost/process/spawn.hpp>
#include <boost/process/io.hpp>
#include <boost/process/search_path.hpp>
#include <memory>
#include <iterator>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/fstream.hpp>
#include <mw/wrap/generator.hpp>
#include <mw/wrap/outline/gcc.hpp>

int main(int argc, char **argv)
{
    using namespace std;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;


//    fs::path bin;
    fs::path demangled;
    fs::path mangled;
    fs::path output;
    fs::path wrapper_out;
    fs::path tpl_f;

    std::string nm_cmd;
    std::vector<std::string> binary;

    std::string comp;

    bool indirect_call = false;
    auto at_option_parser = [](string const&s)->pair<string, string>
                            {
                                if ('@' == s[0])
                                    return std::make_pair(string("response-file"), s.substr(1));
                                else
                                    return pair<string, string>();
                            };

    po::options_description desc("Possible options");
    desc.add_options()
            ("help,H", "produce help message")
            ("output,o",        po::value<fs::path>(&output)->default_value("./mw-wrapper.cpp"),       "output file")
            ("compiler,C",      po::value<string>  (&comp),         "compiler [gcc, clang]")
            ("outline,X",       po::value<fs::path>(&mangled),      "outline")
            ("dem-outline,Y",   po::value<fs::path>(&demangled),    "demangled outline")
            ("wrapper-out,v",   po::value<fs::path>(&wrapper_out)->default_value("./wrapper-param"),  "output of the wrapper names for the linker")
            ("response-file",   po::value<string>(), "can be specified with '@name', too")
            ("config-file,E",   po::value<string>(), "config file")
            ("binary,B",        po::value<std::vector<std::string>>(&binary), "binary file to read the outline from")
            ("template,T",      po::value<fs::path>(&tpl_f), "Template file")
            ("nm,N",            po::value<std::string>(&nm_cmd)->default_value("nm"), "Custom nm command")
            ("indirect,I",      po::bool_switch(&indirect_call), "Indirect call, i.e. though g++/gcc command. I.e. add -Wl,")
            ;

    po::positional_options_description pos;
    pos.add("binary", -1);
    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(pos).
                  extra_parser(at_option_parser).run(), vm);

        po::notify(vm);

        if (vm.count("response-file"))
        {
             // Load the file and tokenize it
             ifstream ifs(vm["response-file"].as<string>().c_str());
             if (!ifs)
             {
                 cout << "Could not open the response file\n";
                 exit(1);
             }
             // Read the whole file into a string
             stringstream ss;
             ss << ifs.rdbuf();
             // Split the file content
             boost::char_separator<char> sep(" \n\r");
             std::string ResponsefileContents( ss.str() );
             boost::tokenizer<boost::char_separator<char> > tok(ResponsefileContents, sep);
             vector<string> args(tok.begin(), tok.end());
             // Parse the file and store the options
             po::store(po::command_line_parser(args).options(desc).run(), vm);
        }
        if (vm.count("config-file"))
        {
            ifstream ifs(vm["config-file"].as<string>());
            po::store(po::parse_config_file(ifs, desc), vm);
        }

    }
    catch (boost::program_options::error  & err)
    {
        if (vm.count("help"))
        {
           std::cout << desc << endl;
           return 0;
        }
        cerr << err.what() << endl;
        cerr << "\n-----------------------------------\n" << endl;
        cerr << desc << endl;
        return 1;
    }

    mw::wrap::outline::storage st;
    if ((comp == "gcc") || (comp == "clang") || comp.empty())
    {
        //load the files
        if (!mangled.empty() && !demangled.empty())
        {
            fs::ifstream ms{  mangled};
            fs::ifstream ds{demangled};
            st = mw::wrap::outline::parse_gcc(mangled.string(), ms, ds);
        }
        for (auto & b : binary)
        {
            boost::process::ipstream ms;
            boost::process::ipstream ds;

            auto nm_cmd_ = fs::exists(nm_cmd) ? fs::path(nm_cmd) : boost::process::search_path(nm_cmd);

            boost::process::spawn(nm_cmd_, b, "--no-demangle", boost::process::std_out > ms);
            boost::process::spawn(nm_cmd_, b,    "--demangle", boost::process::std_out > ds);
            auto st_in = mw::wrap::outline::parse_gcc(b, ms, ds);

            st.insert(st.end(), std::make_move_iterator(st_in.begin()), std::make_move_iterator(st_in.end()));
        }

        if (st.empty())
        {
            std::cerr << "Outline is empty" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Unknown compiler " << comp << std::endl;
        return 1;
    }

    mw::wrap::generator gen{st, indirect_call};

    fs::ofstream out{output};

    if (vm.count("template"))
    {
        fs::ifstream tpl{tpl_f};
        out << tpl.rdbuf() << std::endl;
    }
    out << gen.make();

    fs::ofstream wrp{wrapper_out};

    wrp << gen.wraps();

    return 0;
}

