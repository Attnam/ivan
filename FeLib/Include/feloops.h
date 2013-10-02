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

#ifndef __FELOOPS_H__
#define __FELOOPS_H__

#include "typedef.h"

template <class objecttype, class elementtype>
struct combinepredicates
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef truth (elementtype::*routine)() const;
  truth operator()(objecttype* O, extractor X, routine F, int A, truth OrBit) const
  {
    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E && !(E->*F)() == !OrBit)
	return OrBit;
    }

    return !OrBit;
  }
};

template <class objecttype, class elementtype, class param>
struct combinepredicateswithparam
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef truth (elementtype::*routine)(param) const;
  truth operator()(objecttype* O, extractor X, routine F, param P, int A, truth OrBit) const
  {
    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E && !(E->*F)(P) == !OrBit)
	return OrBit;
    }

    return !OrBit;
  }
};

template <class objecttype, class elementtype>
struct doforelements
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef void (elementtype::*routine)();
  void operator()(objecttype* O, extractor X, routine F, int A) const
  {
    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E)
	(E->*F)();
    }
  }
};

template <class objecttype, class elementtype, class param>
struct doforelementswithparam
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef void (elementtype::*routine)(param);
  void operator()(objecttype* O, extractor X,
			  routine F, param P, int A) const
  {
    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E)
	(E->*F)(P);
    }
  }
};

template <class objecttype, class elementtype>
struct sumproperties
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef int (elementtype::*routine)() const;
  int operator()(objecttype* O, extractor X, routine F, int A) const
  {
    int Sum = 0;

    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E)
	Sum += (E->*F)();
    }

    return Sum;
  }
};

template <class objecttype, class elementtype, class param>
struct findelement
{
  typedef elementtype* (objecttype::*extractor)(int) const;
  typedef truth (elementtype::*routine)(param) const;
  elementtype* operator()(objecttype* O, extractor X,
			   routine F, param P, int A) const
  {
    for(int c = 0; c < A; ++c)
    {
      elementtype* E = (O->*X)(c);

      if(E && (E->*F)(P))
	return E;
    }

    return 0;
  }
};

#endif
