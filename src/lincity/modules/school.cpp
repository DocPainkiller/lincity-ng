/* ---------------------------------------------------------------------- *
 * school.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * (c) Corey Keasling, 2004
 * ---------------------------------------------------------------------- */

#include "school.h"


// school place:
SchoolConstructionGroup schoolConstructionGroup(
    N_("Elementary School"),
     FALSE,                     /* need credit? */
     GROUP_SCHOOL,
     GROUP_SCHOOL_SIZE,
     GROUP_SCHOOL_COLOUR,
     GROUP_SCHOOL_COST_MUL,
     GROUP_SCHOOL_BUL_COST,
     GROUP_SCHOOL_FIREC,
     GROUP_SCHOOL_COST,
     GROUP_SCHOOL_TECH,
     GROUP_SCHOOL_RANGE
);

Construction *SchoolConstructionGroup::createConstruction(int x, int y) {
    return new School(x, y, this);
}

void School::update()
{
    if (commodityCount[STUFF_JOBS] >= JOBS_MAKE_TECH_SCHOOL
    &&  commodityCount[STUFF_GOODS] >= GOODS_MAKE_TECH_SCHOOL
    &&  commodityCount[STUFF_WASTE] + GOODS_MAKE_TECH_SCHOOL / 3 <= MAX_WASTE_AT_SCHOOL)
    {
        commodityCount[STUFF_JOBS] -= JOBS_MAKE_TECH_SCHOOL;
        commodityCount[STUFF_GOODS] -= GOODS_MAKE_TECH_SCHOOL;
        commodityCount[STUFF_WASTE] += GOODS_MAKE_TECH_SCHOOL / 3;
        ++working_days;
        tech_level += TECH_MADE_BY_SCHOOL;
        total_tech_made += TECH_MADE_BY_SCHOOL;
        if( !animate && (busy >= 20) && (real_time > anim) ) //start the swing
        {
            frameIt->frame = 1;
            frit->frame = 0;
            animate = true;
            anim = real_time + SCHOOL_ANIMATION_SPEED;
        }

    }
    if ((total_time % 100) == 0)
    {
        busy = working_days;
        working_days = 0;
    }
    school_cost += SCHOOL_RUNNING_COST;
    if (animate && (real_time > anim)) // do the swing
    {
        frameIt->frame = 1;
        anim = real_time + SCHOOL_ANIMATION_SPEED;
        if ( ++(frit->frame) >= (int)frit->resourceGroup->graphicsInfoVector.size())
        {

            anim = real_time + SCHOOL_ANIMATION_BREAK- 100 * busy; //set swing delay
            if ((real_time >= anim)) // restart
            {
                frit->frame = 1;
                frameIt->frame = 1;
                animate = true;
                anim = real_time + SCHOOL_ANIMATION_SPEED;
            }
            else //
            {
                frit->frame = -1;
                frameIt->frame = 0;
                animate = false;
            }

        }
    }
}

void School::report()
{
    int i = 0;
    mps_store_sd(i++, constructionGroup->name, ID);
    i++;
    mps_store_sfp(i++, N_("busy"), (float)busy);
    mps_store_sfp(i++, N_("Lessons learned"), total_tech_made * 100.0 / MAX_TECH_LEVEL);
    i++;
    list_commodities(&i);
}

/** @file lincity/modules/school.cpp */

