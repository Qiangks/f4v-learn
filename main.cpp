#include "f4v_log.hpp"
#include "f4v_parse.hpp"
#include "f4v_type.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    int ret = ERROR_SUCCESS;

    if (argc < 2 || argc > 2) {
        f4v_trace(
            "\nUsage:\n"
            "          ./f4v [filename]\n"
            "for example:               \n"
            "           ./f4v 1.mp4\n");
        return -1;                
    }
    
    F4vFileParser* f4vfile = new F4vFileParser(argv[1]);
    if ((ret = f4vfile->initialize()) != ERROR_SUCCESS) {
        f4v_error("f4v initialize failed. ret=%d", ret);
        return ret;
    }

    if ((ret = f4vfile->show()) != ERROR_SUCCESS) {
        f4v_error("f4v show information failed. ret=%d", ret);
        return ret;
    }

    delete f4vfile;
    return ret;
}
