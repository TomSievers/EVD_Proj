#include <include/Controller.hpp>
#include <include/States.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char const *argv[])
{
    Controller con("../../Photos_pool_table/setup1_3.jpg");
    while(true)
    {
        con.run();
    }
    return 0;
}
