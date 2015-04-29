#ifndef DYNGLOBAL_H
#define DYNGLOBAL_H
#define CUDA_DYNAMIC_WORLD_MAX_NUM_OBJECTS 32
#define CUDABROADPHASE_MAX_NUMOBJECTS 32
#define DRAW_BPH_PAIRS 0
#define DRAW_BVH_HASH 0
#define DRAW_NPH_CONTACT 0
#define COLLIDEJUST 0
#define PRINT_BOUND 1
#define PRINT_BVH_MAXLEVEL 1
#define DRAW_BVH_HIERARCHY 0
#define DISABLE_COLLISION_DETECTION 0
#define DISABLE_COLLISION 0
class DynGlobal
{
public:
    static bool CheckConvergence;
    static int MaxTOINumIterations;
    static int MaxContactNumIterations;
};
#endif        //  #ifndef BVHGLOBAL_H

