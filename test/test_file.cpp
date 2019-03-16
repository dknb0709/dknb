#include <file/Directory.hpp>
#include <file/File.hpp>
#include <iostream>

int main() {
  {
    dknb::Directory dir1("d1");
    dknb::Directory dir2("d2");
    dknb::Directory dir3("d3");
    dknb::Directory dir4("d4");
    dknb::File file1("file1.txt");
    dknb::File file2("file2.txt");
    dknb::File file3("file3.txt");
    dknb::File file4("file4.txt");

    dir1.add(&dir2)
        .add(&dir3).add(&dir4);

    dir1.add(&file1);
    dir2.add(&file2);
    dir3.add(&file3);
    dir3.add(&file4);

    dir1.create();

    std::fstream ofs = file1.open(std::ios::out);
    ofs << "test1" << std::endl;
    ofs << "test2" << std::endl;
    ofs << "test3" << std::endl;
  }
}
