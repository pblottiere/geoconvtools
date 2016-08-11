#include <pdal/PointView.hpp>
#include <pdal/BufferReader.hpp>
#include <pdal/PointTable.hpp>
#include <pdal/Dimension.hpp>
#include <pdal/Options.hpp>
#include <pdal/StageFactory.hpp>

#include <glob.h>
#include <vector>
#include <fstream>
#include <stdlib.h>

using namespace pdal;

union STR_FLOAT {
    char c[4];
    float f;
};

struct Point
{
    double x;
    double y;
    double z;
    uint16_t i;
};

std::vector<std::string> glob(const std::string& pat){
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
            ret.push_back(string(glob_result.gl_pathv[i]));
        }
    globfree(&glob_result);
    return ret;
}

int fillView(pdal::PointViewPtr view, const char* filename,
    double x_offset, double y_offset, double z_offset, int idx)
{
    // open file
    std::ifstream binfile( filename );
    std::string str((std::istreambuf_iterator<char>(binfile)),
                     std::istreambuf_iterator<char>());

    std::cout << filename << std::endl;

    // type: float32
    // point data: X Y Z I
    // point size: 4 + 4 + 4 + 4
    for ( size_t j = 0; j < str.size(); j = j+16 )
    {
        STR_FLOAT x;
        x.c[0] = str[j+0];
        x.c[1] = str[j+1];
        x.c[2] = str[j+2];
        x.c[3] = str[j+3];

        STR_FLOAT y;
        y.c[0] = str[j+4+0];
        y.c[1] = str[j+4+1];
        y.c[2] = str[j+4+2];
        y.c[3] = str[j+4+3];

        STR_FLOAT z;
        z.c[0] = str[j+4+4+0];
        z.c[1] = str[j+4+4+1];
        z.c[2] = str[j+4+4+2];
        z.c[3] = str[j+4+4+3];

        STR_FLOAT i;
        i.c[0] = str[j+4+4+4+0];
        i.c[1] = str[j+4+4+4+1];
        i.c[2] = str[j+4+4+4+2];
        i.c[3] = str[j+4+4+4+3];

        Point p;
        p.x = x.f + x_offset;
        p.y = z.f + y_offset;
        p.z = y.f;
        p.i = -i.f*10;
        /*p.x = x.f + x_offset;
        p.y = y.f + y_offset;
        p.z = z.f + z_offset;*/

        view->setField(pdal::Dimension::Id::X, idx, p.x);
        view->setField(pdal::Dimension::Id::Y, idx, p.y);
        view->setField(pdal::Dimension::Id::Z, idx, p.z);
        view->setField(pdal::Dimension::Id::Intensity, idx, p.i);
        idx += 1;
    }

    return idx;
}


int main(int argc, char* argv[])
{
    // get parameters
    std::string regex = argv[1];
    double x_offset = strtod(argv[2], NULL);
    double y_offset = strtod(argv[3], NULL);
    double z_offset = strtod(argv[4], NULL);
    std::string las = argv[5];

    std::vector<std::string> binfiles = glob(regex);

    Options options;
    options.add("filename", las);

    PointTable table;
    table.layout()->registerDim(Dimension::Id::X);
    table.layout()->registerDim(Dimension::Id::Y);
    table.layout()->registerDim(Dimension::Id::Z);
    table.layout()->registerDim(Dimension::Id::Intensity);

    PointViewPtr view(new PointView(table));

    int idx = 0;
    for ( size_t j = 0; j < binfiles.size(); j++ )
        idx = fillView(view, binfiles[j].c_str(), x_offset, y_offset, z_offset,
                idx);

    BufferReader reader;
    reader.addView(view);

    StageFactory factory;

    Stage *writer = factory.createStage("writers.las");

    writer->setInput(reader);
    writer->setOptions(options);
    writer->prepare(table);
    writer->execute(table);
}
