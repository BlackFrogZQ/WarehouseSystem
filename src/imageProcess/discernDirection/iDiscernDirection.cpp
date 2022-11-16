#include "iDiscernDirection.h"
#include "discernDirection/discernLzDirection.h"
#include "discernDirection/discernYcgDirection.h"

using namespace TIGER_ProcessTool;
TIGER_ProcessTool::IDiscernDirection *createDiscernPositioning(bool p_processYcg)
{
    IDiscernDirection *p = nullptr;
    if(p_processYcg)
    {
        p = new CYcgDiscern;
    }
    else
    {
        p = new CLzDiscern;
    }
    assert(p != nullptr);
    return p;
}