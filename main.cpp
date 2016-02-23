#include <iostream>
#include "f4v_log.hpp"
#include "f4v_parse.hpp"
#include "f4v_type.hpp"

using namespace std;

int main() {
    int ret = ERROR_SUCCESS;
    
    F4vFileParser* f4vfile = new F4vFileParser("sample1_1500kbps.f4v");
    
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
