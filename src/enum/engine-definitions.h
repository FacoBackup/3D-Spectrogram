#ifndef ENGINEID_H
#define ENGINEID_H

#define SURFACE_CACHE_RES 2000

typedef unsigned long long EntityID;
#define MAX_LIGHTS 300
#define EMPTY_ENTITY 0
#define FILE_METADATA ".mjson"
#define FILE_SCENE "-scene"
#define FILE_MESH "-mesh"
#define FILE_TEXTURE "-texture"
#define METAL_FILE_EXTENSION ".metalasset"
#define FILE_MATERIAL "-material"
#define FILE_SVO "-svo"
#define FILE_NAME_SEPARATOR "##"

#define METRIC_START \
auto currentTime = Clock::now(); \
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch());\
const auto start = duration.count();

#define METRIC_END(M) \
currentTime = Clock::now();\
duration = std::chrono::duration_cast< std::chrono::milliseconds>(currentTime.time_since_epoch());\
std::cout << M << " " << duration.count() - start << "ms" << std::endl;

#endif //ENGINEID_H
