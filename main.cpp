#include <iostream>
#include "f4v_log.hpp"
#include "f4v_parse.hpp"

using namespace std;

int main() {
    f4v_info("hello, world!");
    F4vFileParser* f4vfile = new F4vFileParser("sample1_1500kbps.f4v");
    f4vfile->initialize();

    f4vfile->show_box();

    delete f4vfile;
    return 0;
}
