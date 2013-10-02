/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#ifndef __ILOOPS_H__
#define __ILOOPS_H__

#include "feloops.h"
#include "char.h"

struct combinebodypartpredicates : public combinepredicates<ccharacter, bodypart>
{
  typedef combinepredicates<ccharacter, bodypart> base;
  truth operator()(ccharacter* C, base::routine F, truth OrBit) const
  {
    return base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts(), OrBit);
  }
};

template <class param>
struct combinebodypartpredicateswithparam : public combinepredicateswithparam<ccharacter, bodypart, param>
{
  typedef combinepredicateswithparam<ccharacter, bodypart, param> base;
  truth operator()(ccharacter* C, typename base::routine F, param P, truth OrBit) const
  {
    return base::operator()(C, &character::GetBodyPart, F, P, C->GetBodyParts(), OrBit);
  }
};

struct combineequipmentpredicates : public combinepredicates<ccharacter, item>
{
  typedef combinepredicates<ccharacter, item> base;
  truth operator()(ccharacter* C, base::routine F, truth OrBit) const
  {
    return base::operator()(C, &character::GetEquipment, F, C->GetEquipments(), OrBit);
  }
};

template <class param>
struct combineequipmentpredicateswithparam : public combinepredicateswithparam<ccharacter, item, param>
{
  typedef combinepredicateswithparam<ccharacter, item, param> base;
  truth operator()(ccharacter* C, typename base::routine F, param P, truth OrBit) const
  {
    return base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments(), OrBit);
  }
};

struct doforbodyparts : public doforelements<ccharacter, bodypart>
{
  typedef doforelements<ccharacter, bodypart> base;
  void operator()(ccharacter* C, base::routine F) const
  {
    base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts());
  }
};

template <class param>
struct doforbodypartswithparam : public doforelementswithparam<ccharacter, bodypart, param>
{
  typedef doforelementswithparam<ccharacter, bodypart, param> base;
  void operator()(ccharacter* C, typename base::routine F, param P) const
  {
    base::operator()(C, &character::GetBodyPart, F, P, C->GetBodyParts());
  }
};

struct doforequipments : public doforelements<ccharacter, item>
{
  typedef doforelements<ccharacter, item> base;
  void operator()(ccharacter* C, base::routine F) const
  {
    base::operator()(C, &character::GetEquipment, F, C->GetEquipments());
  }
  void operator()(ccharacter* C, void (item::*F)() const) const
  {
    base::operator()(C, &character::GetEquipment, base::routine(F), C->GetEquipments());
  }
};

template <class param>
struct doforequipmentswithparam : public doforelementswithparam<ccharacter, item, param>
{
  typedef doforelementswithparam<ccharacter, item, param> base;
  void operator()(ccharacter* C, typename base::routine F, param P) const
  {
    base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments());
  }
  void operator()(ccharacter* C, void (item::*F)(param) const, param P) const
  {
    base::operator()(C, &character::GetEquipment, (typename base::routine)(F), P, C->GetEquipments());
  }
};

struct sumbodypartproperties : public sumproperties<ccharacter, bodypart>
{
  typedef sumproperties<ccharacter, bodypart> base;
  int operator()(ccharacter* C, base::routine F) const
  {
    return base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts());
  }
};

template <class param>
struct findequipment : public findelement<ccharacter, item, param>
{
  typedef findelement<ccharacter, item, param> base;
  item* operator()(ccharacter* C, typename base::routine F, param P) const
  {
    return base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments());
  }
};

#endif
