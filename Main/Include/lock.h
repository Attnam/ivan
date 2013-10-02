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

#ifndef __LOCK_H__
#define __LOCK_H__

template <class base, class lockbase>
class lockable : public base, public lockbase
{
 public:
  typedef typename lockbase::prototype prototype;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsOpenable(ccharacter*) const { return true; }
  virtual truth HasLock(ccharacter*) const { return true; }
  virtual truth IsLocked() const { return lockbase::Locked; }
  virtual void SetIsLocked(truth What) { lockbase::Locked = What; }
  virtual void Lock() { lockbase::Locked = true; }
  virtual int GetVirtualConfig() const { return base::GetConfig(); }
  virtual void SetVirtualConfig(int What, int F = 0) { base::SetConfig(What, F); }
  virtual const prototype* GetVirtualProtoType() const { return base::GetProtoType(); }
  virtual festring GetVirtualDescription(int Case) const { return base::GetDescription(Case); }
  virtual truth TryKey(item* K, character* C) { return lockbase::TryKey(K, C); }
 protected:
  virtual void PostConstruct();
};

template <class base, class lockbase>
inline void lockable<base, lockbase>::Save(outputfile& SaveFile) const
{
  base::Save(SaveFile);
  lockbase::Save(SaveFile);
}

template <class base, class lockbase>
inline void lockable<base, lockbase>::Load(inputfile& SaveFile)
{
  base::Load(SaveFile);
  lockbase::Load(SaveFile);
}

template <class base, class lockbase>
inline void lockable<base, lockbase>::PostConstruct()
{
  lockbase::PostConstruct();
  base::PostConstruct();
}

#endif
