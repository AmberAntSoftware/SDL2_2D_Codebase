#ifndef PREUTILS_H_INCLUDED
#define PREUTILS_H_INCLUDED

#define MemoryDumpStrucure(StructureName_Type, LeafType)\
typedef struct StructureName_Type{\
    struct StructureName_Type *node;\
    struct LeafType *leaf;\
} StructureName_Type;

#define MemoryDumpStaticLocals(MemoryDumpStructureType, LeafFreeFunctionName_HasOnlyOneParameter, LeafType, InitFunctionName, FreeFunctionName, AddDataName, RemoveDataName)\
static MemoryDumpStructureType *DMP_X_X_dump = NULL;\
static MemoryDumpStructureType *DMP_X_X_first = NULL;\
\
static void InitFunctionName (){\
    /*if(DMP_X_X_dump==NULL){\
        DMP_X_X_dump = calloc(sizeof(MemoryDumpStructureType),1);\
        DMP_X_X_first = DMP_X_X_dump;\
    }*/\
}\
static void FreeFunctionName (){\
    if(DMP_X_X_dump!=NULL){\
        MemoryDumpStructureType *cur = DMP_X_X_first;\
        MemoryDumpStructureType *next;\
        while(cur!=NULL){\
            next = cur->node;\
            if(cur->leaf!=NULL){\
                LeafFreeFunctionName_HasOnlyOneParameter(cur->leaf);\
            }\
            free(cur);\
            cur = next;\
        }\
    }\
}\
static void AddDataName (LeafType *data){\
    if(DMP_X_X_first!=NULL){\
        MemoryDumpStructureType *cur = DMP_X_X_first;\
        MemoryDumpStructureType *next;\
        MemoryDumpStructureType *pre = cur;\
        int hold = 0;\
        while(cur!=NULL){\
            next = cur->node;\
            if(cur!=NULL&&cur->leaf==data){\
                \
                return;\
            }\
            cur = next;\
        }\
    }\
    MemoryDumpStructureType *container = calloc(sizeof(MemoryDumpStructureType),1);\
    if(container==NULL){\
        return;\
    }\
    container->node = NULL;\
    container->leaf = data;\
    if(DMP_X_X_first==NULL){\
        DMP_X_X_first = container;\
        DMP_X_X_dump = DMP_X_X_first;\
        return;\
    }\
    DMP_X_X_dump->node = container;\
    DMP_X_X_dump = DMP_X_X_dump->node;\
}\
static void RemoveDataName (LeafType *data){\
    if(DMP_X_X_first!=NULL){\
        MemoryDumpStructureType *cur = DMP_X_X_first;\
        MemoryDumpStructureType *next;\
        MemoryDumpStructureType *pre = cur;\
        while(cur!=NULL){\
            next = cur->node;\
            if(cur->leaf!=NULL && data==cur->leaf){\
                LeafFreeFunctionName_HasOnlyOneParameter(cur->leaf);\
                free(cur->leaf);\
                if(cur==DMP_X_X_first){\
                    DMP_X_X_first=next;\
                    if(next==NULL){\
                        DMP_X_X_first=NULL;\
                    }\
                }else{\
                    pre->node=next;\
                    if(pre==DMP_X_X_first){\
                        DMP_X_X_first->node=next;\
                    }\
                }\
                free(cur);\
                return;\
            }\
            if(pre!=cur){\
                pre=pre->node;\
            }\
            cur = next;\
        }\
    }\
}

#endif // PREUTILS_H_INCLUDED
