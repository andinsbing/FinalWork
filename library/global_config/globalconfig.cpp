#include "globalconfig.h"
#include "../library/file/filesystem.h"

const QJsonObject Global::config = [] {
    FileSystem fs;
    return fs.readJson("config/global.json");
}();
